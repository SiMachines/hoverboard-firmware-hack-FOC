/**
  * This file is part of the hoverboard-firmware-hack project.
  *
  * Copyright (C) 2020-2021 Emanuel FERU <aerdronix@gmail.com>
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// Includes
#include <stdio.h>
#include <stdlib.h> // for abs()
#include <string.h>
#include <math.h>   // for fmod()
#include "stm32f1xx_hal.h"
#include "defines.h"
#include "setup.h"
#include "config.h"
#include "eeprom.h"
#include "util.h"
#include "BLDC_controller.h"
#include "rtwtypes.h"
#include "comms.h"

#if defined(DEBUG_I2C_LCD) || defined(SUPPORT_LCD)
#include "hd44780.h"
#endif

/* =========================== Variable Definitions =========================== */

// Timebase: buzzerTimer ticks at 16 ticks per millisecond (see main.c)
#ifndef TICKS_PER_MS
#define TICKS_PER_MS 16U
#endif
#define T_MS(ms) ((uint32_t)((ms) * TICKS_PER_MS))

//------------------------------------------------------------------------
// Global variables set externally
//------------------------------------------------------------------------
extern volatile adc_buf_t adc_buffer;
extern I2C_HandleTypeDef hi2c2;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

extern int16_t batVoltage;
extern uint8_t backwardDrive;
extern uint8_t buzzerCount;             // global variable for the buzzer counts. can be 1, 2, 3, 4, 5, 6, 7...
extern volatile uint32_t buzzerTimer;   // global timer variable for buzzer timing
static volatile uint8_t dcLinkProtection = false;
static volatile boolean_T overcurrent_fault_flag = false;

#if defined(ANALOG_BUTTON)
volatile uint8_t analogButtonPressed = 0U;
static uint8_t analogButtonLatched = 0U;

#if ANALOG_BUTTON_PRESSED_MIN > 4095U
  #error "ANALOG_BUTTON_PRESSED_MIN must be <= 4095"
#endif
#if ANALOG_BUTTON_RELEASE_MAX > 4095U
  #error "ANALOG_BUTTON_RELEASE_MAX must be <= 4095"
#endif
#if ANALOG_BUTTON_RELEASE_MAX >= ANALOG_BUTTON_PRESSED_MIN
  #error "ANALOG_BUTTON_RELEASE_MAX must be less than ANALOG_BUTTON_PRESSED_MIN"
#endif

static void AnalogButton_ProcessSample(uint16_t sample) {
  if (!analogButtonLatched) {
    if (sample >= ANALOG_BUTTON_PRESSED_MIN) {
      analogButtonLatched  = 1U;
      analogButtonPressed  = 1U;
    }
  } else {
    if (sample <= ANALOG_BUTTON_RELEASE_MAX) {
      analogButtonLatched  = 0U;
      analogButtonPressed  = 0U;
    }
  }
}

void AnalogButton_Init(void) {
  analogButtonPressed = 0U;
  analogButtonLatched = 0U;
}
#endif

#if defined(DC_LINK_WATCHDOG_ENABLE)
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;

static void DcLinkWatchdog_ArmRise(void) {
  ADC_AnalogWDGConfTypeDef config;
  config.WatchdogMode = ADC_ANALOGWATCHDOG_SINGLE_REG;
  config.HighThreshold = BAT_HIGH;
  config.LowThreshold  = HARD_18V_COUNTS-200;   // around 15v
  config.Channel       = DCLINK_ADC_CHANNEL;
  config.ITMode        = ENABLE;
  HAL_ADC_AnalogWDGConfig(&hadc3, &config);
}

static void DcLinkWatchdog_HandleWatchdog(void) {
  __HAL_ADC_CLEAR_FLAG(&hadc3, ADC_FLAG_AWD);
  //uint16_t sample = adc_buffer.adc3.value.batt1;
      dcLinkProtection = true;
      LEFT_TIM->BDTR &= ~TIM_BDTR_MOE;
      RIGHT_TIM->BDTR &= ~TIM_BDTR_MOE;
    }

void DcLinkWatchdog_Init(void) {
  dcLinkProtection = false;
  __HAL_ADC_CLEAR_FLAG(&hadc3, ADC_FLAG_AWD);
  DcLinkWatchdog_ArmRise();
}



void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef *hadc) {
  if ((hadc != NULL) && (hadc == &hadc3 || hadc->Instance == hadc3.Instance)) {
    DcLinkWatchdog_HandleWatchdog();
    return;
  }
  // ADC1/ADC2 analog watchdog: immediate motor disable
  // ADC1 -> right DC sensor (PC1 / ADC_CHANNEL_11)
  // ADC2 -> left  DC sensor (PC0 / ADC_CHANNEL_10)
  // Clear AWD flag and disable motors via MOE bits, then set global enable=0 so
   
  if ((hadc != NULL) && (hadc == &hadc1 || hadc->Instance == hadc1.Instance)) {
    __HAL_ADC_CLEAR_FLAG(&hadc1, ADC_FLAG_AWD);
  LEFT_TIM->BDTR &= ~TIM_BDTR_MOE;
  RIGHT_TIM->BDTR &= ~TIM_BDTR_MOE;
  overcurrent_fault_flag = true;
    return;
  }

  if ((hadc != NULL) && (hadc == &hadc2 || hadc->Instance == hadc2.Instance)) {
    __HAL_ADC_CLEAR_FLAG(&hadc2, ADC_FLAG_AWD);
  LEFT_TIM->BDTR &= ~TIM_BDTR_MOE;
  RIGHT_TIM->BDTR &= ~TIM_BDTR_MOE;
  overcurrent_fault_flag = true;
    return;
  }
}
#endif
boolean_T overcurrent_fault(void) {
  return overcurrent_fault_flag;
}
boolean_T  DLVPA(void) {
  return dcLinkProtection;
}
// Shared accessor used by the rest of the firmware regardless of button mode.
uint8_t powerButtonPressed(void) {
#if defined(ANALOG_BUTTON)
  AnalogButton_ProcessSample(adc_buffer.adc3.value.button);
  return analogButtonPressed;
#else
  return HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN);
#endif
}

#if defined(ENCODER_X)
SensorState encoder_x = {0}; // Initialize all members to 0/false
TIM_HandleTypeDef encoder_x_handle;
#endif

#if defined(ENCODER_Y)
SensorState encoder_y = {0}; // Initialize all members to 0/false
TIM_HandleTypeDef encoder_y_handle;
#endif


inline boolean_T encoder_alignment_faulted(void) {
#if defined(ENCODER_X) && defined(ENCODER_Y)
  return (encoder_x.align_fault != 0U) || (encoder_y.align_fault != 0U);
#elif defined(ENCODER_X)
  return (encoder_x.align_fault != 0U);
#elif defined(ENCODER_Y)
  return (encoder_y.align_fault != 0U);
#else 
return 0;
#endif
}

static inline boolean_T encoder_alignment_active(void) {
#if defined(ENCODER_X) && defined(ENCODER_Y)
  return (encoder_x.align_state != 0U) || (encoder_y.align_state != 0U);
#elif defined(ENCODER_X)
  return (encoder_x.align_state != 0U);
#elif  defined(ENCODER_Y)
  return (encoder_y.align_state != 0U);
#else 
return 0;
#endif
}


extern uint8_t buzzerFreq;              // global variable for the buzzer pitch. can be 1, 2, 3, 4, 5, 6, 7...
extern uint8_t buzzerPattern;           // global variable for the buzzer pattern. can be 1, 2, 3, 4, 5, 6, 7...

extern uint8_t enable;                  // global variable for motor enable

extern uint8_t nunchuk_data[6];
extern volatile uint32_t timeoutCntGen; // global counter for general timeout counter
extern volatile uint8_t  timeoutFlgGen; // global flag for general timeout counter
extern volatile uint32_t main_loop_counter;

#if defined(CONTROL_PPM_LEFT) || defined(CONTROL_PPM_RIGHT)
extern volatile uint16_t ppm_captured_value[PPM_NUM_CHANNELS+1];
#endif

#if defined(RC_PWM_LEFT) || defined(RC_PWM_RIGHT)
extern volatile int16_t pwm_captured_ch1_value;
extern volatile int16_t pwm_captured_ch2_value;
#endif

#if defined(SW_PWM_RIGHT)||(SW_PWM_LEFT)
extern volatile int16_t pwm_captured_ch1_value;
extern volatile int16_t pwm_captured_ch2_value;
#endif

#if defined(HW_PWM)
extern volatile int16_t pwm_captured_ch2_value;
#endif

//------------------------------------------------------------------------
// Global variables set here in util.c
//------------------------------------------------------------------------
// Matlab defines - from auto-code generation
//---------------
RT_MODEL rtM_Left_;                     /* Real-time model */
RT_MODEL rtM_Right_;                    /* Real-time model */
RT_MODEL *const rtM_Left  = &rtM_Left_;
RT_MODEL *const rtM_Right = &rtM_Right_;

extern P rtP_Left;                      /* Block parameters (auto storage) */
DW       rtDW_Left;                     /* Observable states */
ExtU     rtU_Left;                      /* External inputs */
ExtY     rtY_Left;                      /* External outputs */

P        rtP_Right;                     /* Block parameters (auto storage) */
DW       rtDW_Right;                    /* Observable states */
ExtU     rtU_Right;                     /* External inputs */
ExtY     rtY_Right;                     /* External outputs */
//---------------

uint8_t  inIdx      = 0;
uint8_t  inIdx_prev = 0;
#if defined(PRI_INPUT1) && defined(PRI_INPUT2) && defined(AUX_INPUT1) && defined(AUX_INPUT2)
InputStruct input1[INPUTS_NR] = { {0, 0, 0, PRI_INPUT1}, {0, 0, 0, AUX_INPUT1} };
InputStruct input2[INPUTS_NR] = { {0, 0, 0, PRI_INPUT2}, {0, 0, 0, AUX_INPUT2} };
#else
InputStruct input1[INPUTS_NR] = { {0, 0, 0, PRI_INPUT1} };
InputStruct input2[INPUTS_NR] = { {0, 0, 0, PRI_INPUT2} };
#endif
 
int16_t  speedAvg;                      // average measured speed
int16_t  speedAvgAbs;                   // average measured speed in absolute
uint8_t  timeoutFlgADC    = 0;          // Timeout Flag for ADC Protection:    0 = OK, 1 = Problem detected (line disconnected or wrong ADC data)
uint8_t  timeoutFlgSerial = 0;          // Timeout Flag for Rx Serial command: 0 = OK, 1 = Problem detected (line disconnected or wrong Rx data)

uint8_t  ctrlModReqRaw = CTRL_MOD_REQ;
uint8_t  ctrlModReq    = CTRL_MOD_REQ;  // Final control mode request 

#if defined(DEBUG_I2C_LCD) || defined(SUPPORT_LCD)
LCD_PCF8574_HandleTypeDef lcd;
#endif

#ifdef VARIANT_TRANSPOTTER
float    setDistance;
uint16_t VirtAddVarTab[NB_OF_VAR] = {1337};       // Virtual address defined by the user: 0xFFFF value is prohibited
static   uint16_t saveValue       = 0;
static   uint8_t  saveValue_valid = 0;
#elif !defined(VARIANT_HOVERBOARD) && !defined(VARIANT_TRANSPOTTER)
uint16_t VirtAddVarTab[NB_OF_VAR] = {1000, 1001, 1002, 1003, 1004, 1005, 1006, 1007, 1008, 1009,
                                     1010, 1011, 1012, 1013, 1014, 1015, 1016, 1017, 1018};
#else
uint16_t VirtAddVarTab[NB_OF_VAR] = {1000};       // Dummy virtual address to avoid warnings
#endif


//------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------
static int16_t INPUT_MAX;             // [-] Input target maximum limitation
static int16_t INPUT_MIN;             // [-] Input target minimum limitation


#if !defined(VARIANT_HOVERBOARD) && !defined(VARIANT_TRANSPOTTER)
  static uint8_t  cur_spd_valid  = 0;
  static uint8_t  inp_cal_valid  = 0;
#endif

#if defined(CONTROL_ADC)
static uint16_t timeoutCntADC = ADC_PROTECT_TIMEOUT;  // Timeout counter for ADC Protection
#endif

#if defined(DEBUG_SERIAL_USART2) || defined(CONTROL_SERIAL_USART2) || defined(SIDEBOARD_SERIAL_USART2)
static uint8_t  rx_buffer_L[SERIAL_BUFFER_SIZE];      // USART Rx DMA circular buffer
static uint32_t rx_buffer_L_len = ARRAY_LEN(rx_buffer_L);
#endif
#if defined(CONTROL_SERIAL_USART2) || defined(SIDEBOARD_SERIAL_USART2)
static uint16_t timeoutCntSerial_L = SERIAL_TIMEOUT;  // Timeout counter for Rx Serial command
static uint8_t  timeoutFlgSerial_L = 0;               // Timeout Flag for Rx Serial command: 0 = OK, 1 = Problem detected (line disconnected or wrong Rx data)
#endif
#if defined(SIDEBOARD_SERIAL_USART2)
SerialSideboard Sideboard_L;
SerialSideboard Sideboard_L_raw;
static uint32_t Sideboard_L_len = sizeof(Sideboard_L);
#endif

#if defined(DEBUG_SERIAL_USART3) || defined(CONTROL_SERIAL_USART3) || defined(SIDEBOARD_SERIAL_USART3)
static uint8_t  rx_buffer_R[SERIAL_BUFFER_SIZE];      // USART Rx DMA circular buffer
static uint32_t rx_buffer_R_len = ARRAY_LEN(rx_buffer_R);
#endif
#if defined(CONTROL_SERIAL_USART3) || defined(SIDEBOARD_SERIAL_USART3)
static uint16_t timeoutCntSerial_R = SERIAL_TIMEOUT;  // Timeout counter for Rx Serial command
static uint8_t  timeoutFlgSerial_R = 0;               // Timeout Flag for Rx Serial command: 0 = OK, 1 = Problem detected (line disconnected or wrong Rx data)
#endif
#if defined(SIDEBOARD_SERIAL_USART3)
SerialSideboard Sideboard_R;
SerialSideboard Sideboard_R_raw;
static uint32_t Sideboard_R_len = sizeof(Sideboard_R);
#endif

#if defined(CONTROL_SERIAL_USART2)
static SerialCommand commandL;
static SerialCommand commandL_raw;
static uint32_t commandL_len = sizeof(commandL);
  #ifdef CONTROL_IBUS
  static uint16_t ibusL_captured_value[IBUS_NUM_CHANNELS];
  #endif
#endif

#if defined(CONTROL_SERIAL_USART3)
static SerialCommand commandR;
static SerialCommand commandR_raw;
static uint32_t commandR_len = sizeof(commandR);
  #ifdef CONTROL_IBUS
  static uint16_t ibusR_captured_value[IBUS_NUM_CHANNELS];
  #endif
#endif

#if defined(SUPPORT_BUTTONS) || defined(SUPPORT_BUTTONS_LEFT) || defined(SUPPORT_BUTTONS_RIGHT)
static uint8_t button1;                 // Blue
static uint8_t button2;                 // Green
#endif

#ifdef VARIANT_HOVERCAR
static uint8_t brakePressed;
#endif

#if defined(CRUISE_CONTROL_SUPPORT) || (defined(STANDSTILL_HOLD_ENABLE) && (CTRL_TYP_SEL == FOC_CTRL) && (CTRL_MOD_REQ != SPD_MODE))
static uint8_t cruiseCtrlAcv = 0;
static uint8_t standstillAcv = 0;
#endif

/* =========================== Retargeting printf =========================== */
/* retarget the C library printf function to the USART */
#if defined(DEBUG_SERIAL_USART2) || defined(DEBUG_SERIAL_USART3)
  #ifdef __GNUC__
    #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
  #else
    #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
  #endif
  PUTCHAR_PROTOTYPE {
    #if defined(DEBUG_SERIAL_USART2)
      HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 1000);
    #elif defined(DEBUG_SERIAL_USART3)
      HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 1000);
    #endif
    return ch;
  }
  
  #ifdef __GNUC__
    int _write(int file, char *data, int len) {
      int i;
      for (i = 0; i < len; i++) { __io_putchar( *data++ );}
      return len;
    }
  #endif
#endif

 
/* =========================== Initialization Functions =========================== */

#if defined(RC_PWM_LEFT) || defined(RC_PWM_RIGHT) ||defined(SW_PWM_LEFT) || defined(SW_PWM_RIGHT) || defined(HW_PWM)
void PWM_Init(void);
#endif


void BLDC_Init(void) {
  BLDC_SetPwmResolution((uint16_t)LEFT_TIM->ARR);
  /* Set BLDC controller parameters */ 
  #if defined(ENCODER_X) || defined(ENCODER_Y)
  rtP_Left.b_angleMeasEna       = 1;            // Motor angle input: 0 = estimated angle, 1 = measured angle (e.g. if encoder is available)
  #else
  rtP_Left.b_angleMeasEna       = 0;            // Motor angle input: 0 = estimated angle, 1 = measured angle (e.g. if encoder is available)
  #endif
  rtP_Left.z_selPhaCurMeasABC   = 0;            // Left motor measured current phases {Blue, Yellow} = {iB, iC} -> do NOT change
  rtP_Left.z_ctrlTypSel         = CTRL_TYP_SEL;
  rtP_Left.b_diagEna            = DIAG_ENA;
  rtP_Left.i_max                = (I_MOT_MAX * A2BIT_CONV) << 4;        // fixdt(1,16,4)
  rtP_Left.n_max                = N_MOT_MAX << 4;                       // fixdt(1,16,4)
  rtP_Left.b_fieldWeakEna       = FIELD_WEAK_ENA; 
  rtP_Left.id_fieldWeakMax      = (FIELD_WEAK_MAX * A2BIT_CONV) << 4;   // fixdt(1,16,4)
  rtP_Left.a_phaAdvMax          = PHASE_ADV_MAX << 4;                   // fixdt(1,16,4)
  rtP_Left.r_fieldWeakHi        = FIELD_WEAK_HI << 4;                   // fixdt(1,16,4)
  rtP_Left.r_fieldWeakLo        = FIELD_WEAK_LO << 4;                   // fixdt(1,16,4)
  rtP_Left.n_polePairs          = N_POLE_PAIRS;                        // fixdt(1,16,4)
  //rtP_Left.cf_idKi              = DI;                              // fixdt(1,16,4) ufix16_En16
  //rtP_Left.cf_idKp              = DP;                              // fixdt(1,16,4) ufix16_En12
  //rtP_Left.cf_iqKi              = DQ;                              // fixdt(1,16,4) ufix16_En16
  //rtP_Left.cf_iqKp              = DQ;                              // fixdt(1,16,4) ufix16_En12

  rtP_Right                     = rtP_Left;     // Copy the Left motor parameters to the Right motor parameters
  rtP_Right.z_selPhaCurMeasABC  = 1;            // Right motor measured current phases {Green, Blue} = {iA, iB} -> do NOT change

  /* Pack LEFT motor data into RTM */
  rtM_Left->defaultParam        = &rtP_Left;
  rtM_Left->dwork               = &rtDW_Left;
  rtM_Left->inputs              = &rtU_Left;
  rtM_Left->outputs             = &rtY_Left;

  /* Pack RIGHT motor data into RTM */
  rtM_Right->defaultParam       = &rtP_Right;
  rtM_Right->dwork              = &rtDW_Right;
  rtM_Right->inputs             = &rtU_Right;
  rtM_Right->outputs            = &rtY_Right;

  /* Initialize BLDC controllers */
  BLDC_controller_initialize(rtM_Left);
  BLDC_controller_initialize(rtM_Right);
}
#if !defined(SW_PWM_RIGHT) && !defined(SW_PWM_LEFT) && !defined(HW_PWM)
void Input_Lim_Init(void) {     // Input Limitations - ! Do NOT touch !
  if (rtP_Left.b_fieldWeakEna || rtP_Right.b_fieldWeakEna) {
    INPUT_MAX = MAX( 1000, FIELD_WEAK_HI);
    INPUT_MIN = MIN(-1000,-FIELD_WEAK_HI);
  } else {
    INPUT_MAX =  1000;
    INPUT_MIN = -1000;
  }
}
#else
void Input_Lim_Init(void) {     // Input Limitations - ! Do NOT touch !
  if (rtP_Left.b_fieldWeakEna || rtP_Right.b_fieldWeakEna) {
    INPUT_MAX = MAX( 16000, FIELD_WEAK_HI);
    INPUT_MIN = MIN(-16000,-FIELD_WEAK_HI);
  } else {
    INPUT_MAX =  16000;
    INPUT_MIN = -16000;
  }
}
#endif

void Input_Init(void) {
  #if defined(CONTROL_PPM_LEFT) || defined(CONTROL_PPM_RIGHT)
    PPM_Init();
  #endif

 #if defined(RC_PWM_LEFT) || defined(RC_PWM_RIGHT) ||defined(SW_PWM_LEFT) || defined(SW_PWM_RIGHT) || defined(HW_PWM)
    PWM_Init();
  #endif

  #if defined(DEBUG_SERIAL_USART2) || defined(CONTROL_SERIAL_USART2) || defined(FEEDBACK_SERIAL_USART2) || defined(SIDEBOARD_SERIAL_USART2)
    UART2_Init();
  #endif
  #if defined(DEBUG_SERIAL_USART3) || defined(CONTROL_SERIAL_USART3) || defined(FEEDBACK_SERIAL_USART3) || defined(SIDEBOARD_SERIAL_USART3)
    UART3_Init();
  #endif
  #if defined(DEBUG_SERIAL_USART2) || defined(CONTROL_SERIAL_USART2) || defined(SIDEBOARD_SERIAL_USART2)
    HAL_UART_Receive_DMA(&huart2, (uint8_t *)rx_buffer_L, sizeof(rx_buffer_L));
    UART_DisableRxErrors(&huart2);
  #endif
  #if defined(DEBUG_SERIAL_USART3) || defined(CONTROL_SERIAL_USART3) || defined(SIDEBOARD_SERIAL_USART3)
    HAL_UART_Receive_DMA(&huart3, (uint8_t *)rx_buffer_R, sizeof(rx_buffer_R));
    UART_DisableRxErrors(&huart3);
  #endif

  #if !defined(VARIANT_HOVERBOARD) && !defined(VARIANT_TRANSPOTTER)
    uint16_t writeCheck, readVal;
    HAL_FLASH_Unlock();
    EE_Init();            /* EEPROM Init */
    EE_ReadVariable(VirtAddVarTab[0], &writeCheck);
    if (writeCheck == FLASH_WRITE_KEY) {
      #if defined(DEBUG_SERIAL_USART2) || defined(DEBUG_SERIAL_USART3)
        printf("Using the configuration from EEprom\r\n");
      #endif

      EE_ReadVariable(VirtAddVarTab[1] , &readVal); rtP_Left.i_max = rtP_Right.i_max = (int16_t)readVal;
      EE_ReadVariable(VirtAddVarTab[2] , &readVal); rtP_Left.n_max = rtP_Right.n_max = (int16_t)readVal;
      for (uint8_t i=0; i<INPUTS_NR; i++) {
        EE_ReadVariable(VirtAddVarTab[ 3+8*i] , &readVal); input1[i].typ = (uint8_t)readVal;
        EE_ReadVariable(VirtAddVarTab[ 4+8*i] , &readVal); input1[i].min = (int16_t)readVal;
        EE_ReadVariable(VirtAddVarTab[ 5+8*i] , &readVal); input1[i].mid = (int16_t)readVal;
        EE_ReadVariable(VirtAddVarTab[ 6+8*i] , &readVal); input1[i].max = (int16_t)readVal;
        EE_ReadVariable(VirtAddVarTab[ 7+8*i] , &readVal); input2[i].typ = (uint8_t)readVal;
        EE_ReadVariable(VirtAddVarTab[ 8+8*i] , &readVal); input2[i].min = (int16_t)readVal;
        EE_ReadVariable(VirtAddVarTab[ 9+8*i] , &readVal); input2[i].mid = (int16_t)readVal;
        EE_ReadVariable(VirtAddVarTab[10+8*i] , &readVal); input2[i].max = (int16_t)readVal;
      
        printf("Limits Input1: TYP:%i MIN:%i MID:%i MAX:%i\r\nLimits Input2: TYP:%i MIN:%i MID:%i MAX:%i\r\n",
          input1[i].typ, input1[i].min, input1[i].mid, input1[i].max,
          input2[i].typ, input2[i].min, input2[i].mid, input2[i].max);
      }
    } else {
      #if defined(DEBUG_SERIAL_USART2) || defined(DEBUG_SERIAL_USART3)
        printf("Using the configuration from config.h\r\n");
      #endif

      for (uint8_t i=0; i<INPUTS_NR; i++) {
        if (input1[i].typDef == 3) {  // If Input type defined is 3 (auto), identify the input type based on the values from config.h
          input1[i].typ = checkInputType(input1[i].min, input1[i].mid, input1[i].max);
        } else {
          input1[i].typ = input1[i].typDef;
        }
        if (input2[i].typDef == 3) {
          input2[i].typ = checkInputType(input2[i].min, input2[i].mid, input2[i].max);
        } else {
          input2[i].typ = input2[i].typDef;
        }
        printf("Limits Input1: TYP:%i MIN:%i MID:%i MAX:%i\r\nLimits Input2: TYP:%i MIN:%i MID:%i MAX:%i\r\n",
          input1[i].typ, input1[i].min, input1[i].mid, input1[i].max,
          input2[i].typ, input2[i].min, input2[i].mid, input2[i].max);
      }
    }
    HAL_FLASH_Lock();
  #endif

  #ifdef VARIANT_TRANSPOTTER
    enable = 1;

    HAL_FLASH_Unlock();
    EE_Init();            /* EEPROM Init */
    EE_ReadVariable(VirtAddVarTab[0], &saveValue);
    HAL_FLASH_Lock();

    setDistance = saveValue / 1000.0;
    if (setDistance < 0.2) {
      setDistance = 1.0;
    }
  #endif

  #if defined(DEBUG_I2C_LCD) || defined(SUPPORT_LCD)
    I2C_Init();
    HAL_Delay(50);
    lcd.pcf8574.PCF_I2C_ADDRESS = 0x27;
    lcd.pcf8574.PCF_I2C_TIMEOUT = 5;
    lcd.pcf8574.i2c             = hi2c2;
    lcd.NUMBER_OF_LINES         = NUMBER_OF_LINES_2;
    lcd.type                    = TYPE0;

    if(LCD_Init(&lcd)!=LCD_OK) {
        // error occured
        //TODO while(1);
    }

    LCD_ClearDisplay(&lcd);
    HAL_Delay(5);
    LCD_SetLocation(&lcd, 0, 0);
    #ifdef VARIANT_TRANSPOTTER
      LCD_WriteString(&lcd, "TranspOtter V2.1");
    #else
      LCD_WriteString(&lcd, "Hover V2.0");
    #endif
    LCD_SetLocation(&lcd,  0, 1); LCD_WriteString(&lcd, "Initializing...");
  #endif

  #if defined(VARIANT_TRANSPOTTER) && defined(SUPPORT_LCD)
    LCD_ClearDisplay(&lcd);
    HAL_Delay(5);
    LCD_SetLocation(&lcd,  0, 1); LCD_WriteString(&lcd, "Bat:");
    LCD_SetLocation(&lcd,  8, 1); LCD_WriteString(&lcd, "V");
    LCD_SetLocation(&lcd, 15, 1); LCD_WriteString(&lcd, "A");
    LCD_SetLocation(&lcd,  0, 0); LCD_WriteString(&lcd, "Len:");
    LCD_SetLocation(&lcd,  8, 0); LCD_WriteString(&lcd, "m(");
    LCD_SetLocation(&lcd, 14, 0); LCD_WriteString(&lcd, "m)");
  #endif
}

/**
  * @brief  Disable Rx Errors detection interrupts on UART peripheral (since we do not want DMA to be stopped)
  *         The incorrect data will be filtered based on the START_FRAME and checksum.
  * @param  huart: UART handle.
  * @retval None
  */
#if defined(DEBUG_SERIAL_USART2) || defined(CONTROL_SERIAL_USART2) || defined(SIDEBOARD_SERIAL_USART2) || \
    defined(DEBUG_SERIAL_USART3) || defined(CONTROL_SERIAL_USART3) || defined(SIDEBOARD_SERIAL_USART3)
void UART_DisableRxErrors(UART_HandleTypeDef *huart)
{  
  CLEAR_BIT(huart->Instance->CR1, USART_CR1_PEIE);    /* Disable PE (Parity Error) interrupts */  
  CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);     /* Disable EIE (Frame error, noise error, overrun error) interrupts */
}
#endif

/* =========================== Encoder Functions =========================== */
#if defined (ENCODER_X)

void Encoder_X_Init(void) {
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_TIM4_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Channel A
    GPIO_InitStruct.Pin = ENCODER_X_CHA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(ENCODER_X_CHA_PORT, &GPIO_InitStruct);

    // Channel B
    GPIO_InitStruct.Pin = ENCODER_X_CHB_PIN;
    HAL_GPIO_Init(ENCODER_X_CHB_PORT, &GPIO_InitStruct);

  encoder_x_handle.Init.Period = ENCODER_X_CPR - 1;
  encoder_x_handle.Init.Prescaler = 0;
  encoder_x_handle.Init.ClockDivision = 0;
  encoder_x_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
  encoder_x_handle.Init.RepetitionCounter = 0;
  encoder_x_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    TIM_Encoder_InitTypeDef encoder_config;

    encoder_config.EncoderMode = TIM_ENCODERMODE_TI12;

    encoder_config.IC1Polarity = TIM_ICPOLARITY_RISING;
    encoder_config.IC1Selection = TIM_ICSELECTION_DIRECTTI;
    encoder_config.IC1Prescaler = TIM_ICPSC_DIV1;
    encoder_config.IC1Filter = 0;

    encoder_config.IC2Polarity = TIM_ICPOLARITY_RISING;
    encoder_config.IC2Selection = TIM_ICSELECTION_DIRECTTI;
    encoder_config.IC2Prescaler = TIM_ICPSC_DIV1;
    encoder_config.IC2Filter = 0;

  encoder_x_handle.Instance = ENCODER_X_TIMER;

  if (HAL_TIM_Encoder_Init(&encoder_x_handle, &encoder_config) != HAL_OK) {
    encoder_x.ini = false;
        return;
    }
   if (HAL_TIM_Encoder_Start(&encoder_x_handle, TIM_CHANNEL_ALL) != HAL_OK) {
    encoder_x.ini = false;
        return;
    }
    
    // Initialize encoder state
  encoder_x.ini = true;
  encoder_x.ali = false;
  encoder_x.align_fault = false;
  encoder_x.offset = 0;
  encoder_x.direction = 1;
  encoder_x.aligned_count = 0;
  encoder_x.ENCODER_COUNT = 0;
  encoder_x.full_rotations = 0;
    
    // Initialize alignment state variables
  encoder_x.align_state = 0;
  encoder_x.align_ini_pos = 0;
}
void Encoder_X_Align_Start(void) {
  if (encoder_x.align_state != 0) {
        return; 
    }

  if (encoder_x.align_fault) {
    return;
  }
    
    enable = 1;
    rtP_Left.b_diagEna = 0; // Disable diagnostics during alignment
    rtP_Right.b_diagEna = 0; // Disable diagnostics during alignment
  encoder_x.ali = false;
  encoder_x.align_state = 1; // Start alignment sequence
  encoder_x.align_timer = 0;
  encoder_x.align_start_time = buzzerTimer;
  encoder_x.align_ini_pos = encoder_x_handle.Instance->CNT;
  encoder_x.align_total_ini_pos = get_x_TotalCount();
    // Initialize simulation variables
  // Calculate count increment for 5.256 electrical rotations in 3000ms (5 + 92°/360°)
  // This compensates for the electrical offset to naturally reach 0° electrical  
  // 5.256 electrical rotations = 5.256/N_POLE_PAIRS mechanical rotations = 0.3504 mechanical rotation
  // 0.3504 mech rotation = (0.3504 * ENCODER_X_CPR) counts over 3000ms
  // Timebase: buzzerTimer runs at 16 ticks/ms -> total ticks = 3000 * 16 = 48000
  // Increment per tick = (0.3504 * ENCODER_Y_CPR) / 48000
  // Store as per-tick * 1000 for precision: (0.3504 * ENCODER_Y_CPR * 1000) / 48000
  // Using integer math: (3504 * ENCODER_Y_CPR * 1000) / (10000 * 48000) = (3504 * ENCODER_Y_CPR) / 480000
     encoder_x.count_increment_x1000 = (int32_t)((((int64_t)280) * ENCODER_X_CPR) / 48000);
    encoder_x.align_inpTgt = 0; // Start with 0 power, will ramp up
}

// Non-blocking encoder alignment with mechanical angle simulation - call from main loop
void Encoder_X_Align(void) {
    uint32_t current_time = buzzerTimer;
    uint32_t elapsed_ticks = current_time - encoder_x.align_start_time;
    count_x_update();
    

    const uint32_t RAMP_MS = T_MS(125);
    const uint32_t MOVE_MS = T_MS(1500);
    
    switch (encoder_x.align_state) {
        case 1: // Rotation phase - spin to find position
            handle_x_rotation_phase(elapsed_ticks, RAMP_MS, MOVE_MS, current_time);
            break;
            
        case 2: // High power phase - apply extra torque
            handle_x_high_power_phase(elapsed_ticks, RAMP_MS, current_time);
            break;
            
        case 3: // Move back and finish
            handle_x_move_back_phase(elapsed_ticks, RAMP_MS, MOVE_MS, current_time);
            break;
            
        default:
            encoder_x.align_state = 0;
            break;
    }
}


 void handle_x_rotation_phase(uint32_t elapsed_ticks, uint32_t ramp_ms, uint32_t move_ms, uint32_t current_time) {
    // Power control: ramp up, full speed, then ramp down
    if (elapsed_ticks < ramp_ms) {
        // Ramp up to full power
        encoder_x.align_inpTgt = (ALIGNMENT_X_POWER * elapsed_ticks) / ramp_ms;
    } else if (elapsed_ticks < (move_ms-ramp_ms)) {
        // Full power rotation
        encoder_x.align_inpTgt = ALIGNMENT_X_POWER;
    } else if (elapsed_ticks < move_ms) {
        // Ramp down to stop
        uint32_t decel_ticks = elapsed_ticks - (move_ms-ramp_ms);
        encoder_x.align_inpTgt = ALIGNMENT_X_POWER - (ALIGNMENT_X_POWER * decel_ticks) / ramp_ms;
    }

    // Update emulated encoder position during movement
    if (elapsed_ticks < move_ms) {
        int32_t total_increment = ((int64_t)encoder_x.count_increment_x1000 * elapsed_ticks) / 1000;
    encoder_x.emulated_mech_count = encoder_x.align_ini_pos + total_increment;
    encoder_x.emulated_mech_count = normalize_x_encoder_count(encoder_x.emulated_mech_count);
    } else {
        // Rotation complete - snap to 0° electrical position
        int32_t counts_per_elec_cycle = ENCODER_X_CPR / N_POLE_PAIRS;
         encoder_x.offset = (ENCODER_X_CPR / N_POLE_PAIRS/4); //90 degrees offset

        int32_t current_elec_cycle = encoder_x.emulated_mech_count / counts_per_elec_cycle;
        encoder_x.emulated_mech_count = current_elec_cycle * counts_per_elec_cycle;
        encoder_x.emulated_mech_count = normalize_x_encoder_count(encoder_x.emulated_mech_count);
        encoder_x.align_state = 2;
        encoder_x.align_start_time = current_time;
    }
}

 void handle_x_high_power_phase(uint32_t elapsed_ticks, uint32_t ramp_ms, uint32_t current_time) {
    if (elapsed_ticks < ramp_ms) {
        // Ramp to double power
        encoder_x.align_inpTgt = (2*ALIGNMENT_X_POWER * elapsed_ticks) / ramp_ms;
    } else if (elapsed_ticks < T_MS(1000)) {
        // Hold at double power
        encoder_x.align_inpTgt = ALIGNMENT_X_POWER * 2;
    } else {
       
        __HAL_TIM_SET_COUNTER(&encoder_x_handle, encoder_x.emulated_mech_count+encoder_x.offset); //+90 degree offset align to Q axis
        encoder_x.align_zero_pos = encoder_x.emulated_mech_count;
        encoder_x.align_total_mid_pos = get_x_TotalCount();
        encoder_x.align_start_time = current_time;
        encoder_x.align_state = 3;
    }
}

 void handle_x_move_back_phase(uint32_t elapsed_ticks, uint32_t ramp_ms, uint32_t move_ms, uint32_t current_time) {
    // Stage A: Ramp down from high power to normal power
    if (elapsed_ticks < ramp_ms) {
        uint32_t decel_ticks = (move_ms-ramp_ms) - elapsed_ticks;
        encoder_x.align_inpTgt = ALIGNMENT_X_POWER + (ALIGNMENT_X_POWER * decel_ticks) / ramp_ms;
    }
    // Stage B: Move emulated position back toward start
    else if (elapsed_ticks < move_ms) {
        int32_t delta = encoder_x.align_zero_pos - encoder_x.align_ini_pos ;
    
    // Find shortest path (handle encoder wrap-around)
       if (delta > (ENCODER_X_CPR / 2)) delta -= ENCODER_X_CPR;
       if (delta < -(ENCODER_X_CPR / 2)) delta += ENCODER_X_CPR;
    // Interpolate position
        int32_t moved = (delta * (elapsed_ticks - ramp_ms)) / (move_ms - ramp_ms);
        encoder_x.emulated_mech_count = encoder_x.align_zero_pos - moved;
        encoder_x.emulated_mech_count = normalize_x_encoder_count(encoder_x.emulated_mech_count);

        encoder_x.align_inpTgt = ALIGNMENT_X_POWER;
    }
    // Stage C: Final ramp down to zero and finish
    else if (elapsed_ticks >= move_ms) {
        uint32_t final_ramp_time = elapsed_ticks - move_ms;
        if (final_ramp_time < ramp_ms) {
            encoder_x.align_inpTgt = ALIGNMENT_X_POWER * (ramp_ms - final_ramp_time) / ramp_ms;
        } else {
            encoder_x.align_inpTgt = 0;
            finalize_x_alignment();
        }
    }
}
void count_x_update() {
    encoder_x.ENCODER_COUNT = encoder_x_handle.Instance->CNT;
    int32_t count = encoder_x.ENCODER_COUNT - encoder_x.count_prev;
    // if overflow happened track it as full rotation
    if(abs(count) > (ENCODER_X_CPR/2) ) encoder_x.full_rotations += ( count > 0 ) ? -1 : 1; 
    encoder_x.count_prev = encoder_x.ENCODER_COUNT;
}

int32_t get_x_TotalCount(void){
return encoder_x.full_rotations * ENCODER_X_CPR + encoder_x.count_prev;
}
  int32_t normalize_x_encoder_count(int32_t count) {
    count %= ENCODER_X_CPR;
    if (count < 0) count += ENCODER_X_CPR;
    return count;
}

 void finalize_x_alignment(void) {
         // Calculate direction
       int32_t MIN_MOVMENT = (ENCODER_X_CPR / 15) * 1;
        encoder_x.align_total_end_pos = get_x_TotalCount();
       int32_t movement =  abs(encoder_x.align_total_mid_pos - encoder_x.align_total_ini_pos);
       
       if (abs(movement) < MIN_MOVMENT){
        encoder_x.align_fault = true;
        encoder_x.ali = false;
        encoder_x.align_state = 0;
       } else if(encoder_x.align_total_mid_pos < encoder_x.align_total_end_pos) {
         encoder_x.direction = 0;
        encoder_x.align_fault = false;
        encoder_x.ali = true;
        rtP_Right.b_diagEna = DIAG_ENA;
        encoder_x.align_state = 0;
       }else{
        encoder_x.direction = 1;
        encoder_x.align_fault = false;
        encoder_x.ali = true;
        rtP_Right.b_diagEna = DIAG_ENA;
        encoder_x.align_state = 0;
       }    
}
#endif // ENCODER_X

#if defined (ENCODER_Y)
void Encoder_Y_Init(void) {
    __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_TIM3_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Channel A
    GPIO_InitStruct.Pin = ENCODER_Y_CHA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(ENCODER_Y_CHA_PORT, &GPIO_InitStruct);

    // Channel B
    GPIO_InitStruct.Pin = ENCODER_Y_CHB_PIN;
    HAL_GPIO_Init(ENCODER_Y_CHB_PORT, &GPIO_InitStruct);

  encoder_y_handle.Init.Period = ENCODER_Y_CPR - 1;
  encoder_y_handle.Init.Prescaler = 0;
  encoder_y_handle.Init.ClockDivision = 0;
  encoder_y_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
  encoder_y_handle.Init.RepetitionCounter = 0;
  encoder_y_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    TIM_Encoder_InitTypeDef encoder_config;

    encoder_config.EncoderMode = TIM_ENCODERMODE_TI12;

    encoder_config.IC1Polarity = TIM_ICPOLARITY_RISING;
    encoder_config.IC1Selection = TIM_ICSELECTION_DIRECTTI;
    encoder_config.IC1Prescaler = TIM_ICPSC_DIV1;
    encoder_config.IC1Filter = 0;

    encoder_config.IC2Polarity = TIM_ICPOLARITY_RISING;
    encoder_config.IC2Selection = TIM_ICSELECTION_DIRECTTI;
    encoder_config.IC2Prescaler = TIM_ICPSC_DIV1;
    encoder_config.IC2Filter = 0;

  encoder_y_handle.Instance = ENCODER_Y_TIMER;

  if (HAL_TIM_Encoder_Init(&encoder_y_handle, &encoder_config) != HAL_OK) {
    encoder_y.ini = false;
        return;
    }
   if (HAL_TIM_Encoder_Start(&encoder_y_handle, TIM_CHANNEL_ALL) != HAL_OK) {
    encoder_y.ini = false;
        return;
    }
    
    // Initialize encoder state
  encoder_y.ini = true;
  encoder_y.ali = false;
  encoder_y.align_fault = false;
  encoder_y.offset = 0;
  encoder_y.direction = 1;
  encoder_y.aligned_count = 0;
  encoder_y.ENCODER_COUNT = 0;
  encoder_y.full_rotations = 0;
    // Initialize alignment state variables
  encoder_y.align_state = 0;
  encoder_y.align_ini_pos = 0;
}




void Encoder_Y_Align_Start(void) {
  if (encoder_y.align_state != 0) {
        return; 
    }

  if (encoder_y.align_fault) {
    return;
  }
    
    enable = 1;
  rtP_Left.b_diagEna = 0; // Disable diagnostics during alignment
  rtP_Right.b_diagEna = 0; // Disable diagnostics during alignment
  encoder_y.ali = false;
  encoder_y.align_state = 1; // Start alignment sequence
  encoder_y.align_timer = 0;
  encoder_y.align_start_time = buzzerTimer;
  encoder_y.align_ini_pos = encoder_y_handle.Instance->CNT;
  encoder_y.align_total_ini_pos = get_y_TotalCount();
    // Initialize simulation variables
  // Calculate count increment for 5.256 electrical rotations in 3000ms (5 + 92°/360°)
  // This compensates for the electrical offset to naturally reach 0° electrical  
  // 5.256 electrical rotations = 5.256/15 mechanical rotations = 0.3504 mechanical rotation
  // 0.3504 mech rotation = (0.3504 * ENCODER_Y_CPR) counts over 3000ms
  // Timebase: buzzerTimer runs at 16 ticks/ms -> total ticks = 3000 * 16 = 48000
  // Increment per tick = (0.3504 * ENCODER_Y_CPR) / 48000
  // Store as per-tick * 1000 for precision: (0.3504 * ENCODER_Y_CPR * 1000) / 48000
  // Using integer math: (3504 * ENCODER_Y_CPR * 1000) / (10000 * 48000) = (3504 * ENCODER_Y_CPR) / 480000
  encoder_y.count_increment_x1000 = (int32_t)((((int64_t)280) * ENCODER_Y_CPR) / 48000);
    encoder_y.align_inpTgt = 0; // Start with 0 power, will ramp up
}

// Non-blocking encoder alignment with mechanical angle simulation - call from main loop
void Encoder_Y_Align(void) {
    uint32_t current_time = buzzerTimer;
    uint32_t elapsed_ticks = current_time - encoder_y.align_start_time;
  // Keep encoder total/count tracking up-to-date (same behavior as Encoder_X_Align)
  count_y_update();
    
    const uint32_t RAMP_MS = T_MS(125);
    const uint32_t MOVE_MS = T_MS(1500);
    
    switch (encoder_y.align_state) {
        case 1: // Rotation phase - spin to find position
            handle_y_rotation_phase(elapsed_ticks, RAMP_MS, MOVE_MS, current_time);
            break;
            
        case 2: // High power phase - apply extra torque
            handle_y_high_power_phase(elapsed_ticks, RAMP_MS, current_time);
            break;
            
        case 3: // Move back and finish
            handle_y_move_back_phase(elapsed_ticks, RAMP_MS, MOVE_MS, current_time);
            break;
            
        default:
            encoder_y.align_state = 0;
            break;
    }
}

 void handle_y_rotation_phase(uint32_t elapsed_ticks, uint32_t ramp_ms, uint32_t move_ms, uint32_t current_time) {
    // Power control: ramp up, full speed, then ramp down
    if (elapsed_ticks < ramp_ms) {
        // Ramp up to full power
        encoder_y.align_inpTgt = (ALIGNMENT_Y_POWER * elapsed_ticks) / ramp_ms;
    } else if (elapsed_ticks < (move_ms-ramp_ms)) {
        // Full power rotation
        encoder_y.align_inpTgt = ALIGNMENT_Y_POWER;
    } else if (elapsed_ticks < move_ms) {
        // Ramp down to stop
        uint32_t decel_ticks = elapsed_ticks - (move_ms-ramp_ms);
        encoder_y.align_inpTgt = ALIGNMENT_Y_POWER - (ALIGNMENT_Y_POWER * decel_ticks) / ramp_ms;
    }

    // Update emulated encoder position during movement
    if (elapsed_ticks < move_ms) {
        int32_t total_increment = ((int64_t)encoder_y.count_increment_x1000 * elapsed_ticks) / 1000;
    encoder_y.emulated_mech_count = encoder_y.align_ini_pos + total_increment;
    encoder_y.emulated_mech_count = normalize_y_encoder_count(encoder_y.emulated_mech_count);
    } else {
        // Rotation complete - snap to 0° electrical position
        int32_t counts_per_elec_cycle = ENCODER_Y_CPR / N_POLE_PAIRS;
        encoder_y.offset = (ENCODER_Y_CPR / N_POLE_PAIRS/4);  // 90° electrical
    
        int32_t current_elec_cycle = encoder_y.emulated_mech_count / counts_per_elec_cycle;
        encoder_y.emulated_mech_count = current_elec_cycle * counts_per_elec_cycle;
        encoder_y.emulated_mech_count = normalize_y_encoder_count(encoder_y.emulated_mech_count);

        encoder_y.align_state = 2;
        encoder_y.align_start_time = current_time;
    }
}

 void handle_y_high_power_phase(uint32_t elapsed_ticks, uint32_t ramp_ms, uint32_t current_time) {
    if (elapsed_ticks < ramp_ms) {
        // Ramp to double power
        encoder_y.align_inpTgt = (2*ALIGNMENT_Y_POWER * elapsed_ticks) / ramp_ms;
    } else if (elapsed_ticks < T_MS(1000)) {
        // Hold at double power
        encoder_y.align_inpTgt = ALIGNMENT_Y_POWER * 2;
  } else {
    // Record final position and move to next phase
        __HAL_TIM_SET_COUNTER(&encoder_y_handle, encoder_y.emulated_mech_count + encoder_y.offset);
        encoder_y.align_zero_pos = encoder_y.emulated_mech_count;
        encoder_y.align_total_mid_pos = get_y_TotalCount();
        encoder_y.align_start_time = current_time;
        encoder_y.align_state = 3;
    }
}

 void handle_y_move_back_phase(uint32_t elapsed_ticks, uint32_t ramp_ms, uint32_t move_ms, uint32_t current_time) {
    // Stage A: Ramp down from high power to normal power
    if (elapsed_ticks < ramp_ms) {
        uint32_t decel_ticks = (move_ms-ramp_ms) - elapsed_ticks;
        encoder_y.align_inpTgt = ALIGNMENT_Y_POWER + (ALIGNMENT_Y_POWER * decel_ticks) / ramp_ms;
    }
    // Stage B: Move emulated position back toward start
    else if (elapsed_ticks < move_ms) {
        int32_t delta = encoder_y.align_zero_pos - encoder_y.align_ini_pos ;
    
    // Find shortest path (handle encoder wrap-around)
       if (delta > (ENCODER_Y_CPR / 2)) delta -= ENCODER_Y_CPR;
       if (delta < -(ENCODER_Y_CPR / 2)) delta += ENCODER_Y_CPR;
    // Interpolate position
        int32_t moved = (delta * (elapsed_ticks - ramp_ms)) / (move_ms - ramp_ms);
        encoder_y.emulated_mech_count = encoder_y.align_zero_pos - moved;
        encoder_y.emulated_mech_count = normalize_y_encoder_count(encoder_y.emulated_mech_count);

        encoder_y.align_inpTgt = ALIGNMENT_Y_POWER;
    }
    // Stage C: Final ramp down to zero and finish
    else if (elapsed_ticks >= move_ms) {
        uint32_t final_ramp_time = elapsed_ticks - move_ms;
        if (final_ramp_time < ramp_ms) {
            encoder_y.align_inpTgt = ALIGNMENT_Y_POWER * (ramp_ms - final_ramp_time) / ramp_ms;
        } else {
            encoder_y.align_inpTgt = 0;
            finalize_y_alignment();
        }
    }
}
void count_y_update() {
    encoder_y.ENCODER_COUNT = encoder_y_handle.Instance->CNT;
    int32_t count = encoder_y.ENCODER_COUNT - encoder_y.count_prev;
    // if overflow happened track it as full rotation
    if(abs(count) > (ENCODER_Y_CPR/2) ) encoder_y.full_rotations += ( count > 0 ) ? -1 : 1; 
    encoder_y.count_prev = encoder_y.ENCODER_COUNT;
}
int32_t get_y_TotalCount(void){
return encoder_y.full_rotations * ENCODER_Y_CPR + encoder_y.count_prev;
}
  int32_t normalize_y_encoder_count(int32_t count) {
    count %= ENCODER_Y_CPR;
    if (count < 0) count += ENCODER_Y_CPR;
    return count;
}

 void finalize_y_alignment(void) {
    // Calculate offset
       int32_t MIN_MOVMENT = (ENCODER_Y_CPR / 15) * 1;
        encoder_y.align_total_end_pos = get_y_TotalCount();
       int32_t movement =  abs(encoder_y.align_total_mid_pos - encoder_y.align_total_ini_pos);
       
       if (abs(movement) < MIN_MOVMENT){
        encoder_y.align_fault = true;
        encoder_y.ali = false;
        encoder_y.align_state = 0;
       } else if(encoder_y.align_total_mid_pos < encoder_y.align_total_end_pos) {
         encoder_y.direction = 0;
        encoder_y.align_fault = false;
        encoder_y.ali = true;
        rtP_Left.b_diagEna = DIAG_ENA;
        encoder_y.align_state = 0;
       }else{
        encoder_y.direction = 1;
        encoder_y.align_fault = false;
        encoder_y.ali = true;
        rtP_Left.b_diagEna = DIAG_ENA;
        encoder_y.align_state = 0;
       }    
}

#endif // ENCODER_Y
/* =========================== General Functions =========================== */

void poweronMelody(void) {
    buzzerCount = 0;  // prevent interraction with beep counter
    for (int i = 8; i >= 0; i--) {
      buzzerFreq = (uint8_t)i;
      HAL_Delay(100);
    }
    buzzerFreq = 0;
}

void beepCount(uint8_t cnt, uint8_t freq, uint8_t pattern) {
    buzzerCount   = cnt;
    buzzerFreq    = freq;
    buzzerPattern = pattern;
}

void beepLong(uint8_t freq) {
    buzzerCount = 0;  // prevent interraction with beep counter
    buzzerFreq = freq;
    HAL_Delay(500);
    buzzerFreq = 0;
}

void beepShort(uint8_t freq) {
    buzzerCount = 0;  // prevent interraction with beep counter
    buzzerFreq = freq;
    HAL_Delay(100);
    buzzerFreq = 0;
}

void beepShortMany(uint8_t cnt, int8_t dir) {
    if (dir >= 0) {   // increasing tone
      for(uint8_t i = 2*cnt; i >= 2; i=i-2) {
        beepShort(i + 3);
      }
    } else {          // decreasing tone
      for(uint8_t i = 2; i <= 2*cnt; i=i+2) {
        beepShort(i + 3);
      }
    }
}

/* ------------------ BASEPRI helper implementation ------------------ */
/**
 * Set BASEPRI threshold to mask interrupts whose numeric priority is
 * greater or equal to 'prio'. The 'prio' value is the numeric
 * priority passed to HAL_NVIC_SetPriority (0 = highest priority).
 *
 * Returns the previous BASEPRI raw value (call basepri_restore() with it
 * to restore the previous mask).
 */
uint32_t basepri_set_threshold(uint32_t prio)
{
  uint32_t prev = __get_BASEPRI();
  /* Priority field is stored left-aligned in an 8-bit field; only the
   * top __NVIC_PRIO_BITS are implemented. Shift 'prio' into the MSBs.
   */
  const uint32_t shift = 8U - (uint32_t)__NVIC_PRIO_BITS;
  uint32_t basepri_val = (prio << shift) & 0xFFU;

  /* Write BASEPRI then ensure the write takes effect before continuing. */
  __set_BASEPRI(basepri_val);
  __DSB(); __ISB();
  return prev;
}

/**
 * Restore prior BASEPRI value returned by basepri_set_threshold().
 */
void basepri_restore(uint32_t previous_basepri)
{
  __set_BASEPRI(previous_basepri);
  __DSB(); __ISB();
}

/* Usage example:
 * uint32_t old = basepri_set_threshold(2); // allow priorities 0 and 1, block 2+
 * // critical work here (keep short)
 * basepri_restore(old);
 *
 * Notes:
 * - 'prio' is the numeric priority (0..(2^__NVIC_PRIO_BITS -1)).
 * - If you call HAL_NVIC_SetPriority(IRQn, preempt, sub) the effective
 *   numeric priority encoded into the NVIC depends on the priority
 *   grouping; simplest approach is to pick values consistent with the
 *   preempt field you use when calling HAL_NVIC_SetPriority.
 * - Avoid long blocking sections; use BASEPRI to prevent lower-priority
 *   interrupts from preempting time-critical code while still allowing
 *   higher-priority handlers to run.
 */


void calcAvgSpeed(void) {
    // Calculate measured average speed. The minus sign (-) is because motors spin in opposite directions
    speedAvg = 0;
    #if defined(MOTOR_LEFT_ENA)
      #if defined(INVERT_L_DIRECTION)
        speedAvg -= rtY_Left.n_mot;
      #else
        speedAvg += rtY_Left.n_mot;
      #endif
    #endif
    #if defined(MOTOR_RIGHT_ENA)
      #if defined(INVERT_R_DIRECTION)
        speedAvg += rtY_Right.n_mot;
      #else
        speedAvg -= rtY_Right.n_mot;
      #endif

      // Average only if both motors are enabled
      #if defined(MOTOR_LEFT_ENA)
        speedAvg /= 2;
      #endif  
    #endif

    // Handle the case when SPEED_COEFFICIENT sign is negative (which is when most significant bit is 1)
    if (SPEED_COEFFICIENT & (1 << 16)) {
      speedAvg    = -speedAvg;
    } 
    speedAvgAbs   = abs(speedAvg);
}

 /*
 * Auto-calibration of the ADC Limits
 * This function finds the Minimum, Maximum, and Middle for the ADC input
 * Procedure:
 * - press the power button for more than 5 sec and release after the beep sound
 * - move the potentiometers freely to the min and max limits repeatedly
 * - release potentiometers to the resting postion
 * - press the power button to confirm or wait for the 20 sec timeout
 * The Values will be saved to flash. Values are persistent if you flash with platformio. To erase them, make a full chip erase.
 */
void adcCalibLim(void) {
#ifdef AUTO_CALIBRATION_ENA
  calcAvgSpeed();
  if (speedAvgAbs > 5) {    // do not enter this mode if motors are spinning
    return;
  }

#if !defined(VARIANT_HOVERBOARD) && !defined(VARIANT_TRANSPOTTER)

  #if defined(DEBUG_SERIAL_USART2) || defined(DEBUG_SERIAL_USART3)
  printf("Input calibration started...\r\n");
  #endif

  readInputRaw();
  // Inititalization: MIN = a high value, MAX = a low value
  int32_t  input1_fixdt = input1[inIdx].raw << 16;
  int32_t  input2_fixdt = input2[inIdx].raw << 16;
  int16_t  INPUT1_MIN_temp = MAX_int16_T;
  int16_t  INPUT1_MID_temp = 0;
  int16_t  INPUT1_MAX_temp = MIN_int16_T;
  int16_t  INPUT2_MIN_temp = MAX_int16_T;
  int16_t  INPUT2_MID_temp = 0;
  int16_t  INPUT2_MAX_temp = MIN_int16_T;
  int16_t  input_margin    = 0;
  uint16_t input_cal_timeout = 0;
  
  #ifdef CONTROL_ADC
  if (inIdx == CONTROL_ADC) {
    input_margin = ADC_MARGIN;
  }
  #endif

  // Extract MIN, MAX and MID from ADC while the power button is not pressed
  while (!powerButtonPressed() && input_cal_timeout++ < 4000) {   // 20 sec timeout
    readInputRaw();
    filtLowPass32(input1[inIdx].raw, FILTER, &input1_fixdt);
    filtLowPass32(input2[inIdx].raw, FILTER, &input2_fixdt);
    
    INPUT1_MID_temp = (int16_t)(input1_fixdt >> 16);// CLAMP(input1_fixdt >> 16, INPUT1_MIN, INPUT1_MAX);   // convert fixed-point to integer
    INPUT2_MID_temp = (int16_t)(input2_fixdt >> 16);// CLAMP(input2_fixdt >> 16, INPUT2_MIN, INPUT2_MAX);
    INPUT1_MIN_temp = MIN(INPUT1_MIN_temp, INPUT1_MID_temp);
    INPUT1_MAX_temp = MAX(INPUT1_MAX_temp, INPUT1_MID_temp);
    INPUT2_MIN_temp = MIN(INPUT2_MIN_temp, INPUT2_MID_temp);
    INPUT2_MAX_temp = MAX(INPUT2_MAX_temp, INPUT2_MID_temp);
    HAL_Delay(5);
  }

  #if defined(DEBUG_SERIAL_USART2) || defined(DEBUG_SERIAL_USART3)
  printf("Input1 is ");
  #endif
  uint8_t input1TypTemp = checkInputType(INPUT1_MIN_temp, INPUT1_MID_temp, INPUT1_MAX_temp);
  if (input1TypTemp == input1[inIdx].typDef || input1[inIdx].typDef == 3) {  // Accept calibration only if the type is correct OR type was set to 3 (auto)
    #if defined(DEBUG_SERIAL_USART2) || defined(DEBUG_SERIAL_USART3)
    printf("..OK\r\n");
    #endif
  } else {
    input1TypTemp = 0; // Disable input
    #if defined(DEBUG_SERIAL_USART2) || defined(DEBUG_SERIAL_USART3)
    printf("..NOK\r\n");
    #endif
  }

  #if defined(DEBUG_SERIAL_USART2) || defined(DEBUG_SERIAL_USART3)
  printf("Input2 is ");
  #endif
  uint8_t input2TypTemp = checkInputType(INPUT2_MIN_temp, INPUT2_MID_temp, INPUT2_MAX_temp);
  if (input2TypTemp == input2[inIdx].typDef || input2[inIdx].typDef == 3) {  // Accept calibration only if the type is correct OR type was set to 3 (auto)
    #if defined(DEBUG_SERIAL_USART2) || defined(DEBUG_SERIAL_USART3)
    printf("..OK\r\n");
    #endif
  } else {
    input2TypTemp = 0; // Disable input
    #if defined(DEBUG_SERIAL_USART2) || defined(DEBUG_SERIAL_USART3)
    printf("..NOK\r\n");
    #endif
  }


  // At least one of the inputs is not ignored
  if (input1TypTemp != 0 || input2TypTemp != 0){
    input1[inIdx].typ = input1TypTemp;
    input1[inIdx].min = INPUT1_MIN_temp + input_margin;
    input1[inIdx].mid = INPUT1_MID_temp;
    input1[inIdx].max = INPUT1_MAX_temp - input_margin;

    input2[inIdx].typ = input2TypTemp;
    input2[inIdx].min = INPUT2_MIN_temp + input_margin;
    input2[inIdx].mid = INPUT2_MID_temp;
    input2[inIdx].max = INPUT2_MAX_temp - input_margin;

    inp_cal_valid = 1;    // Mark calibration to be saved in Flash at shutdown
    #if defined(DEBUG_SERIAL_USART2) || defined(DEBUG_SERIAL_USART3)
    printf("Limits Input1: TYP:%i MIN:%i MID:%i MAX:%i\r\nLimits Input2: TYP:%i MIN:%i MID:%i MAX:%i\r\n",
            input1[inIdx].typ, input1[inIdx].min, input1[inIdx].mid, input1[inIdx].max,
            input2[inIdx].typ, input2[inIdx].min, input2[inIdx].mid, input2[inIdx].max);
    #endif
  }else{
    #if defined(DEBUG_SERIAL_USART2) || defined(DEBUG_SERIAL_USART3)
    printf("Both inputs cannot be ignored, calibration rejected.\r\n");
    #endif
  }

#endif
#endif  // AUTO_CALIBRATION_ENA
}
 /*
 * Update Maximum Motor Current Limit (via ADC3) and Maximum Speed Limit (via ADC3)
 * Procedure:
 * - press the power button for more than 5 sec and immediatelly after the beep sound press one more time shortly
 * - move and hold the pots to a desired limit position for Current and Speed
 * - press the power button to confirm or wait for the 10 sec timeout
 */
void updateCurSpdLim(void) {
  calcAvgSpeed();
  if (speedAvgAbs > 5) {    // do not enter this mode if motors are spinning
    return;
  }

#if !defined(VARIANT_HOVERBOARD) && !defined(VARIANT_TRANSPOTTER)

  #if defined(DEBUG_SERIAL_USART2) || defined(DEBUG_SERIAL_USART3)
  printf("Torque and Speed limits update started...\r\n");
  #endif

  int32_t  input1_fixdt = input1[inIdx].raw << 16;
  int32_t  input2_fixdt = input2[inIdx].raw << 16;
  uint16_t cur_factor;    // fixdt(0,16,16)
  uint16_t spd_factor;    // fixdt(0,16,16)
  uint16_t cur_spd_timeout = 0;
  cur_spd_valid = 0;

  // Wait for the power button press
  while (!powerButtonPressed() && cur_spd_timeout++ < 2000) {  // 10 sec timeout
    readInputRaw();
    filtLowPass32(input1[inIdx].raw, FILTER, &input1_fixdt);
    filtLowPass32(input2[inIdx].raw, FILTER, &input2_fixdt);
    HAL_Delay(5);
  }
  // Calculate scaling factors
  cur_factor = CLAMP((input1_fixdt - (input1[inIdx].min << 16)) / (input1[inIdx].max - input1[inIdx].min), 6553, 65535);    // ADC3, MIN_cur(10%) = 1.5 A 
  spd_factor = CLAMP((input2_fixdt - (input2[inIdx].min << 16)) / (input2[inIdx].max - input2[inIdx].min), 3276, 65535);    // ADC3, MIN_spd(5%)  = 50 rpm
      
  if (input1[inIdx].typ != 0){
    // Update current limit
    rtP_Left.i_max = rtP_Right.i_max  = (int16_t)((I_MOT_MAX * A2BIT_CONV * cur_factor) >> 12);    // fixdt(0,16,16) to fixdt(1,16,4)
    cur_spd_valid   = 1;  // Mark update to be saved in Flash at shutdown
  }

  if (input2[inIdx].typ != 0){
    // Update speed limit
    rtP_Left.n_max = rtP_Right.n_max  = (int16_t)((N_MOT_MAX * spd_factor) >> 12);                 // fixdt(0,16,16) to fixdt(1,16,4)
    cur_spd_valid  += 2;  // Mark update to be saved in Flash at shutdown
  }

  #if defined(DEBUG_SERIAL_USART2) || defined(DEBUG_SERIAL_USART3)
  // cur_spd_valid: 0 = No limit changed, 1 = Current limit changed, 2 = Speed limit changed, 3 = Both limits changed
  printf("Limits (%i)\r\nCurrent: fixdt:%li factor%i i_max:%i \r\nSpeed: fixdt:%li factor:%i n_max:%i\r\n",
          cur_spd_valid, input1_fixdt, cur_factor, rtP_Left.i_max, input2_fixdt, spd_factor, rtP_Left.n_max);
  #endif

#endif
}

 /*
 * Standstill Hold Function
 * This function uses Cruise Control to provide an anti-roll functionality at standstill.
 * Only available and makes sense for FOC VOLTAGE or FOC TORQUE mode.
 * 
 * Input:  none
 * Output: standstillAcv
 */
void standstillHold(void) {
  #if defined(STANDSTILL_HOLD_ENABLE) && (CTRL_TYP_SEL == FOC_CTRL) && (CTRL_MOD_REQ != SPD_MODE)
    if (!rtP_Left.b_cruiseCtrlEna) {                                  // If Stanstill in NOT Active -> try Activation
      if (((input1[inIdx].cmd > 50 || input2[inIdx].cmd < -50) && speedAvgAbs < 30) // Check if Brake is pressed AND measured speed is small
          || (input2[inIdx].cmd < 20 && speedAvgAbs < 5)) {           // OR Throttle is small AND measured speed is very small
        rtP_Left.n_cruiseMotTgt   = 0;
        rtP_Right.n_cruiseMotTgt  = 0;
        rtP_Left.b_cruiseCtrlEna  = 1;
        rtP_Right.b_cruiseCtrlEna = 1;
        standstillAcv = 1;
      } 
    }
    else {                                                            // If Stanstill is Active -> try Deactivation
      if (input1[inIdx].cmd < 20 && input2[inIdx].cmd > 50 && !cruiseCtrlAcv) { // Check if Brake is released AND Throttle is pressed AND no Cruise Control
        rtP_Left.b_cruiseCtrlEna  = 0;
        rtP_Right.b_cruiseCtrlEna = 0;
        standstillAcv = 0;
      }
    }
  #endif
}

 /*
 * Electric Brake Function
 * In case of TORQUE mode, this function replaces the motor "freewheel" with a constant braking when the input torque request is 0.
 * This is useful when a small amount of motor braking is desired instead of "freewheel".
 * 
 * Input: speedBlend = fixdt(0,16,15), reverseDir = {0, 1}
 * Output: input2.cmd (Throtle) with brake component included
 */
void electricBrake(uint16_t speedBlend, uint8_t reverseDir) {
  #if defined(ELECTRIC_BRAKE_ENABLE) && (CTRL_TYP_SEL == FOC_CTRL) && (CTRL_MOD_REQ == TRQ_MODE)
    int16_t brakeVal;

    // Make sure the Brake pedal is opposite to the direction of motion AND it goes to 0 as we reach standstill (to avoid Reverse driving) 
    if (speedAvg > 0) {
      brakeVal = (int16_t)((-ELECTRIC_BRAKE_MAX * speedBlend) >> 15);
    } else {
      brakeVal = (int16_t)(( ELECTRIC_BRAKE_MAX * speedBlend) >> 15);
    }

    // Check if direction is reversed
    if (reverseDir) {
      brakeVal = -brakeVal;
    }

    // Calculate the new input2.cmd with brake component included
    if (input2[inIdx].cmd >= 0 && input2[inIdx].cmd < ELECTRIC_BRAKE_THRES) {
      input2[inIdx].cmd = MAX(brakeVal, ((ELECTRIC_BRAKE_THRES - input2[inIdx].cmd) * brakeVal) / ELECTRIC_BRAKE_THRES);
    } else if (input2[inIdx].cmd >= -ELECTRIC_BRAKE_THRES && input2[inIdx].cmd < 0) {
      input2[inIdx].cmd = MIN(brakeVal, ((ELECTRIC_BRAKE_THRES + input2[inIdx].cmd) * brakeVal) / ELECTRIC_BRAKE_THRES);
    } else if (input2[inIdx].cmd >= ELECTRIC_BRAKE_THRES) {
      input2[inIdx].cmd = MAX(brakeVal, ((input2[inIdx].cmd - ELECTRIC_BRAKE_THRES) * INPUT_MAX) / (INPUT_MAX - ELECTRIC_BRAKE_THRES));
    } else {  // when (input2.cmd < -ELECTRIC_BRAKE_THRES)
      input2[inIdx].cmd = MIN(brakeVal, ((input2[inIdx].cmd + ELECTRIC_BRAKE_THRES) * INPUT_MIN) / (INPUT_MIN + ELECTRIC_BRAKE_THRES));
    }
  #endif
}

 /*
 * Cruise Control Function
 * This function activates/deactivates cruise control.
 * 
 * Input: button (as a pulse)
 * Output: cruiseCtrlAcv
 */
void cruiseControl(uint8_t button) {
  #ifdef CRUISE_CONTROL_SUPPORT
    if (button && !rtP_Left.b_cruiseCtrlEna) {                          // Cruise control activated
      rtP_Left.n_cruiseMotTgt   = rtY_Left.n_mot;
      rtP_Right.n_cruiseMotTgt  = rtY_Right.n_mot;
      rtP_Left.b_cruiseCtrlEna  = 1;
      rtP_Right.b_cruiseCtrlEna = 1;
      cruiseCtrlAcv = 1;
      beepShortMany(2, 1);                                              // 200 ms beep delay. Acts as a debounce also.
    } else if (button && rtP_Left.b_cruiseCtrlEna && !standstillAcv) {  // Cruise control deactivated if no Standstill Hold is active
      rtP_Left.b_cruiseCtrlEna  = 0;
      rtP_Right.b_cruiseCtrlEna = 0;
      cruiseCtrlAcv = 0;
      beepShortMany(2, -1);
    }
  #endif
}

 /*
 * Check Input Type
 * This function identifies the input type: 0: Disabled, 1: Normal Pot, 2: Middle Resting Pot
 */
int checkInputType(int16_t min, int16_t mid, int16_t max){

  int type = 0;  
  #ifdef CONTROL_ADC
  int16_t threshold = 400;      // Threshold to define if values are too close
  #else
  int16_t threshold = 200;
  #endif

  if ((min / threshold) == (max / threshold) || (mid / threshold) == (max / threshold) || min > max || mid > max) {
    type = 0;
    #if defined(DEBUG_SERIAL_USART2) || defined(DEBUG_SERIAL_USART3)
    printf("ignored");                // (MIN and MAX) OR (MID and MAX) are close, disable input
    #endif
  } else {
    if ((min / threshold) == (mid / threshold)){
      type = 1;
      #if defined(DEBUG_SERIAL_USART2) || defined(DEBUG_SERIAL_USART3)
      printf("a normal pot");        // MIN and MID are close, it's a normal pot
      #endif
    } else {
      type = 2;
      #if defined(DEBUG_SERIAL_USART2) || defined(DEBUG_SERIAL_USART3)
      printf("a mid-resting pot");   // it's a mid resting pot
      #endif
    }

    #ifdef CONTROL_ADC
    if ((min + ADC_MARGIN - ADC_PROTECT_THRESH) > 0 && (max - ADC_MARGIN + ADC_PROTECT_THRESH) < 4095) {
      #if defined(DEBUG_SERIAL_USART2) || defined(DEBUG_SERIAL_USART3)
      printf(" AND protected");
      #endif
      beepLong(2); // Indicate protection by a beep
    }
    #endif
  }

  return type;
}



/* =========================== Input Functions =========================== */

 /*
 * Calculate Input Command
 * This function realizes dead-band around 0 and scales the input between [out_min, out_max]
 */
void calcInputCmd(InputStruct *in, int16_t out_min, int16_t out_max) {
  switch (in->typ){
    case 1: // Input is a normal pot
      in->cmd = CLAMP(MAP(in->raw, in->min, in->max, 0, out_max), 0, out_max);
      break;
    case 2: // Input is a mid resting pot
      if( in->raw > in->mid - in->dband && in->raw < in->mid + in->dband ) {
        in->cmd = 0;
      } else if(in->raw > in->mid) {
        in->cmd = CLAMP(MAP(in->raw, in->mid + in->dband, in->max, 0, out_max), 0, out_max);
      } else {
        in->cmd = CLAMP(MAP(in->raw, in->mid - in->dband, in->min, 0, out_min), out_min, 0);
      }
      break;
    default: // Input is ignored
      in->cmd = 0;
      break;
  }
}

 /*
 * Function to read the Input Raw values from various input devices
 */
void readInputRaw(void) {
  uint8_t idx = (inIdx < INPUTS_NR) ? inIdx : 0; /* safe index to avoid out-of-bounds when INPUTS_NR==1 */
  #ifdef CONTROL_ADC
  if (inIdx == CONTROL_ADC) {
    #ifdef ADC_ALTERNATE_CONNECT
  input1[idx].raw = adc_buffer.adc3.value.l_rx2;
  input2[idx].raw = adc_buffer.adc3.value.l_tx2;
    #else
  input1[idx].raw = adc_buffer.adc3.value.l_tx2;
  input2[idx].raw = adc_buffer.adc3.value.l_rx2;
    #endif
  }
  #endif

    #if defined(CONTROL_NUNCHUK) || defined(SUPPORT_NUNCHUK)
    if (Nunchuk_Read() == NUNCHUK_CONNECTED) {
      if (inIdx == CONTROL_NUNCHUK) {
        input1[idx].raw = (nunchuk_data[0] - 127) * 8; // X axis 0-255
        input2[idx].raw = (nunchuk_data[1] - 128) * 8; // Y axis 0-255
      }
      #ifdef SUPPORT_BUTTONS
        button1 = (uint8_t)nunchuk_data[5] & 1;
        button2 = (uint8_t)(nunchuk_data[5] >> 1) & 1;
      #endif
    }
    #endif

    #if defined(CONTROL_SERIAL_USART2)
    if (inIdx == CONTROL_SERIAL_USART2) {
      #ifdef CONTROL_IBUS
        for (uint8_t i = 0; i < (IBUS_NUM_CHANNELS * 2); i+=2) {
          ibusL_captured_value[(i/2)] = CLAMP(commandL.channels[i] + (commandL.channels[i+1] << 8) - 1000, 0, INPUT_MAX); // 1000-2000 -> 0-1000
        }
        input1[idx].raw = (ibusL_captured_value[0] - 500) * 2;
        input2[idx].raw = (ibusL_captured_value[1] - 500) * 2; 
      #else
        input1[idx].raw = commandL.steer;
        input2[idx].raw = commandL.speed;
      #endif
    }
    #endif
    #if defined(CONTROL_SERIAL_USART3)
    if (inIdx == CONTROL_SERIAL_USART3) {
      #ifdef CONTROL_IBUS
        for (uint8_t i = 0; i < (IBUS_NUM_CHANNELS * 2); i+=2) {
          ibusR_captured_value[(i/2)] = CLAMP(commandR.channels[i] + (commandR.channels[i+1] << 8) - 1000, 0, INPUT_MAX); // 1000-2000 -> 0-1000
        }
        if (inIdx < INPUTS_NR) {
          input1[inIdx].raw = (ibusR_captured_value[0] - 500) * 2;
          input2[inIdx].raw = (ibusR_captured_value[1] - 500) * 2; 
        }
      #else
        if (inIdx < INPUTS_NR) {
          input1[inIdx].raw = commandR.steer;
          input2[inIdx].raw = commandR.speed;
        }
      #endif
    }
    #endif

    #if defined(SIDEBOARD_SERIAL_USART2)
    if (inIdx == SIDEBOARD_SERIAL_USART2) {
      input1[idx].raw = Sideboard_L.cmd1;
      input2[idx].raw = Sideboard_L.cmd2;
    }
    #endif
    #if defined(SIDEBOARD_SERIAL_USART3)
    if (inIdx == SIDEBOARD_SERIAL_USART3) {
      input1[idx].raw = Sideboard_R.cmd1;
      input2[idx].raw = Sideboard_R.cmd2;
    }
    #endif

    #if defined(CONTROL_PPM_LEFT)
    if (inIdx == CONTROL_PPM_LEFT) {
      input1[idx].raw = (ppm_captured_value[0] - 500) * 2;
      input2[idx].raw = (ppm_captured_value[1] - 500) * 2;
    }
    #endif
    #if defined(CONTROL_PPM_RIGHT)
    if (inIdx == CONTROL_PPM_RIGHT) {
      input1[idx].raw = (ppm_captured_value[0] - 500) * 2;
      input2[idx].raw = (ppm_captured_value[1] - 500) * 2;
    }
    #endif
    #if (defined(CONTROL_PPM_LEFT) || defined(CONTROL_PPM_RIGHT)) && defined(SUPPORT_BUTTONS)
      button1 = ppm_captured_value[5] > 500;
      button2 = 0;
    #endif

    #if defined(RC_PWM_LEFT)
    if (inIdx == RC_PWM_LEFT) {
      input1[idx].raw = (pwm_captured_ch1_value - 500) * 2;
      input2[idx].raw = (pwm_captured_ch2_value - 500) * 2;
    }
    #endif
    #if defined(RC_PWM_RIGHT)
    if (inIdx == RC_PWM_RIGHT) {
      input1[idx].raw = (pwm_captured_ch1_value - 500) * 2;
      input2[idx].raw = (pwm_captured_ch2_value - 500) * 2;
    }
    #endif
    #if defined(SW_PWM_RIGHT)
    if (inIdx == SW_PWM_RIGHT) {
      input1[idx].raw = pwm_captured_ch1_value;
      input2[idx].raw = pwm_captured_ch2_value;
    }
    #endif
    #if defined(SW_PWM_LEFT)
    if (inIdx == SW_PWM_LEFT) {
      input1[idx].raw = pwm_captured_ch1_value;
      input2[idx].raw = pwm_captured_ch2_value;
    }
    #endif
#if defined(HW_PWM)
    if (inIdx == HW_PWM) {
      input2[idx].raw = pwm_captured_ch2_value;                       
    }
    #endif
    #ifdef VARIANT_TRANSPOTTER
    #ifdef GAMETRAK_CONNECTION_NORMAL
  input1[idx].cmd = adc_buffer.adc12.value.l_rx2;
  input2[idx].cmd = adc_buffer.adc12.value.l_tx2;
    #endif
    #ifdef GAMETRAK_CONNECTION_ALTERNATE
  input1[idx].cmd = adc_buffer.adc12.value.l_tx2;
  input2[idx].cmd = adc_buffer.adc12.value.l_rx2;
    #endif
    #endif
}

 /*
 * Function to handle the ADC, UART and General timeout (Nunchuk, PPM, PWM)
 */
void handleTimeout(void) {
  uint8_t idx = (inIdx < INPUTS_NR) ? inIdx : 0; /* safe index to avoid out-of-bounds when INPUTS_NR==1 */
    #ifdef CONTROL_ADC
    if (inIdx == CONTROL_ADC) {
      // If input1 or Input2 is either below MIN - Threshold or above MAX + Threshold, ADC protection timeout
    if (IN_RANGE(input1[idx].raw, input1[idx].min - ADC_PROTECT_THRESH, input1[idx].max + ADC_PROTECT_THRESH) &&
      IN_RANGE(input2[idx].raw, input2[idx].min - ADC_PROTECT_THRESH, input2[idx].max + ADC_PROTECT_THRESH)) {
          timeoutFlgADC = 0;                            // Reset the timeout flag
          timeoutCntADC = 0;                            // Reset the timeout counter
      } else {
        if (timeoutCntADC++ >= ADC_PROTECT_TIMEOUT) {   // Timeout qualification
          timeoutFlgADC = 1;                            // Timeout detected
          timeoutCntADC = ADC_PROTECT_TIMEOUT;          // Limit timout counter value
        }
      }
    }
    #endif

    #if defined(CONTROL_SERIAL_USART2) || defined(SIDEBOARD_SERIAL_USART2)
      if (timeoutCntSerial_L++ >= SERIAL_TIMEOUT) {     // Timeout qualification
        timeoutFlgSerial_L = 1;                         // Timeout detected
        timeoutCntSerial_L = SERIAL_TIMEOUT;            // Limit timout counter value
        #if defined(DUAL_INPUTS) && ((defined(CONTROL_SERIAL_USART2) && CONTROL_SERIAL_USART2 == 1) || (defined(SIDEBOARD_SERIAL_USART2) && SIDEBOARD_SERIAL_USART2 == 1))
          inIdx = 0;                                    // Switch to Primary input in case of Timeout on Auxiliary input
        #endif
      } else {                                          // No Timeout
        #if defined(DUAL_INPUTS) && defined(SIDEBOARD_SERIAL_USART2)
          if (Sideboard_L.sensors & SWA_SET) {          // If SWA is set, switch to Sideboard control
            inIdx = SIDEBOARD_SERIAL_USART2;
          } else {
            inIdx = !SIDEBOARD_SERIAL_USART2;
          }
        #elif defined(DUAL_INPUTS) && (defined(CONTROL_SERIAL_USART2) && CONTROL_SERIAL_USART2 == 1)
          inIdx = 1;                                    // Switch to Auxiliary input in case of NO Timeout on Auxiliary input
        #endif
      }
      #if (defined(CONTROL_SERIAL_USART2) && CONTROL_SERIAL_USART2 == 0) || (defined(SIDEBOARD_SERIAL_USART2) && SIDEBOARD_SERIAL_USART2 == 0 && !defined(VARIANT_HOVERBOARD))
        timeoutFlgSerial = timeoutFlgSerial_L;          // Report Timeout only on the Primary Input
      #endif
    #endif

    #if defined(CONTROL_SERIAL_USART3) || defined(SIDEBOARD_SERIAL_USART3)
      if (timeoutCntSerial_R++ >= SERIAL_TIMEOUT) {     // Timeout qualification
        timeoutFlgSerial_R = 1;                         // Timeout detected
        timeoutCntSerial_R = SERIAL_TIMEOUT;            // Limit timout counter value
        #if defined(DUAL_INPUTS) && ((defined(CONTROL_SERIAL_USART3) && CONTROL_SERIAL_USART3 == 1) || (defined(SIDEBOARD_SERIAL_USART3) && SIDEBOARD_SERIAL_USART3 == 1))
          inIdx = 0;                                    // Switch to Primary input in case of Timeout on Auxiliary input
        #endif
      } else {                                          // No Timeout
        #if defined(DUAL_INPUTS) && defined(SIDEBOARD_SERIAL_USART3)
          if (Sideboard_R.sensors & SWA_SET) {          // If SWA is set, switch to Sideboard control
            inIdx = SIDEBOARD_SERIAL_USART3;
          } else {
            inIdx = !SIDEBOARD_SERIAL_USART3;
          }
        #elif defined(DUAL_INPUTS) && (defined(CONTROL_SERIAL_USART3) && CONTROL_SERIAL_USART3 == 1)
          inIdx = 1;                                    // Switch to Auxiliary input in case of NO Timeout on Auxiliary input
        #endif
      }
      #if (defined(CONTROL_SERIAL_USART3) && CONTROL_SERIAL_USART3 == 0) || (defined(SIDEBOARD_SERIAL_USART3) && SIDEBOARD_SERIAL_USART3 == 0 && !defined(VARIANT_HOVERBOARD))
        timeoutFlgSerial = timeoutFlgSerial_R;          // Report Timeout only on the Primary Input
      #endif
    #endif

    #if defined(SIDEBOARD_SERIAL_USART2) && defined(SIDEBOARD_SERIAL_USART3)
      timeoutFlgSerial = timeoutFlgSerial_L || timeoutFlgSerial_R;
    #endif

    #if defined(CONTROL_NUNCHUK) || defined(SUPPORT_NUNCHUK) || defined(VARIANT_TRANSPOTTER) || \
        defined(CONTROL_PPM_LEFT) || defined(CONTROL_PPM_RIGHT) || defined(RC_PWM_LEFT) || defined(RC_PWM_RIGHT)|| defined(SW_PWM_RIGHT) || defined(SW_PWMLEFT)
      if (timeoutCntGen++ >= TIMEOUT) {                 // Timeout qualification
        #if defined(CONTROL_NUNCHUK) || defined(SUPPORT_NUNCHUK) || defined(VARIANT_TRANSPOTTER) || \
            (defined(CONTROL_PPM_LEFT) && CONTROL_PPM_LEFT == 0) || (defined(CONTROL_PPM_RIGHT) && CONTROL_PPM_RIGHT == 0) || \
            (defined(RC_PWM_LEFT) && RC_PWM_LEFT == 0) || (defined(RC_PWM_RIGHT) && RC_PWM_RIGHT == 0)||\
            (defined(SW_PWMLEFT) && SW_PWMLEFT == 0) || (defined(SW_PWM_RIGHT) && SW_PWM_RIGHT == 0)
          timeoutFlgGen = 1;                            // Report Timeout only on the Primary Input
          timeoutCntGen = TIMEOUT;
        #endif
        #if defined(DUAL_INPUTS) && ((defined(CONTROL_PPM_LEFT)  && CONTROL_PPM_LEFT == 1) || (defined(CONTROL_PPM_RIGHT) && CONTROL_PPM_RIGHT == 1) || \
                                     (defined(RC_PWM_LEFT)  && RC_PWM_LEFT == 1) || (defined(RC_PWM_RIGHT) && RC_PWM_RIGHT == 1))
          inIdx = 0;                                    // Switch to Primary input in case of Timeout on Auxiliary input
        #endif
      } else {
        #if defined(DUAL_INPUTS) && ((defined(CONTROL_PPM_LEFT)  && CONTROL_PPM_LEFT == 1) || (defined(CONTROL_PPM_RIGHT) && CONTROL_PPM_RIGHT == 1) || \
                                     (defined(RC_PWM_LEFT)  && RC_PWM_LEFT == 1) || (defined(RC_PWM_RIGHT) && RC_PWM_RIGHT == 1))
          inIdx = 1;                                    // Switch to Auxiliary input in case of NO Timeout on Auxiliary input
        #endif
      }
    #endif
#if defined(ENCODER_X) || defined(ENCODER_Y)
  // In case of timeout bring the system to a Safe State
  if ((timeoutFlgADC || timeoutFlgSerial || timeoutFlgGen || DLVPA() || overcurrent_fault()) && (!encoder_alignment_active())) {
      ctrlModReq  = OPEN_MODE;                                          // Request OPEN_MODE. This will bring the motor power to 0 in a controlled way
  input1[idx].cmd  = 0;
  input2[idx].cmd  = 0;
    } else {
      ctrlModReq  = ctrlModReqRaw;                                      // Follow the Mode request
    }
 
    // Beep in case of Input index change
    if (inIdx && !inIdx_prev) {                                         // rising edge
      beepShort(8);
    } else if (!inIdx && inIdx_prev) {                                  // falling edge
      beepShort(18);
    }
#else
    // In case of timeout bring the system to a Safe State
    if (timeoutFlgADC || timeoutFlgSerial || timeoutFlgGen) {
      ctrlModReq  = OPEN_MODE;                                          // Request OPEN_MODE. This will bring the motor power to 0 in a controlled way
  input1[idx].cmd  = 0;
  input2[idx].cmd  = 0;
    } else {
      ctrlModReq  = ctrlModReqRaw;                                      // Follow the Mode request
    }

    // Beep in case of Input index change
    if (inIdx && !inIdx_prev) {                                         // rising edge
      beepShort(8);
    } else if (!inIdx && inIdx_prev) {                                  // falling edge
      beepShort(18);
    }
    #endif
}

 /*
 * Function to calculate the command to the motors. This function also manages:
 * - timeout detection
 * - MIN/MAX limitations and deadband
 */
void readCommand(void) {
    readInputRaw();

    #if !defined(VARIANT_HOVERBOARD) && !defined(VARIANT_TRANSPOTTER)
      calcInputCmd(&input1[inIdx], INPUT_MIN, INPUT_MAX);
      #if !defined(VARIANT_SKATEBOARD)
        calcInputCmd(&input2[inIdx], INPUT_MIN, INPUT_MAX);
      #else
        calcInputCmd(&input2[inIdx], INPUT_BRK, INPUT_MAX);
      #endif
    #endif

    handleTimeout();

    #ifdef VARIANT_HOVERCAR
    if (inIdx == CONTROL_ADC) {
      brakePressed = (uint8_t)(input1[inIdx].cmd > 50);
    }
    else {
      brakePressed = (uint8_t)(input2[inIdx].cmd < -50);
    }
    #endif

    #if defined(SUPPORT_BUTTONS_LEFT) || defined(SUPPORT_BUTTONS_RIGHT)
      button1 = !HAL_GPIO_ReadPin(BUTTON1_PORT, BUTTON1_PIN);
      button2 = !HAL_GPIO_ReadPin(BUTTON2_PORT, BUTTON2_PIN);
    #endif

    #if defined(CRUISE_CONTROL_SUPPORT) && (defined(SUPPORT_BUTTONS) || defined(SUPPORT_BUTTONS_LEFT) || defined(SUPPORT_BUTTONS_RIGHT))
        cruiseControl(button1);                                           // Cruise control activation/deactivation
    #endif
}


/*
 * Check for new data received on USART2 with DMA: refactored function from https://github.com/MaJerle/stm32-usart-uart-dma-rx-tx
 * - this function is called for every USART IDLE line detection, in the USART interrupt handler
 */
void usart2_rx_check(void)
{
  #if defined(DEBUG_SERIAL_USART2) || defined(CONTROL_SERIAL_USART2) || defined(SIDEBOARD_SERIAL_USART2)  
  static uint32_t old_pos;
  uint32_t pos;
  pos = rx_buffer_L_len - __HAL_DMA_GET_COUNTER(huart2.hdmarx);         // Calculate current position in buffer
  #endif

  #if defined(DEBUG_SERIAL_USART2)
  if (pos != old_pos) {                                                 // Check change in received data
    if (pos > old_pos) {                                                // "Linear" buffer mode: check if current position is over previous one
      usart_process_debug(&rx_buffer_L[old_pos], pos - old_pos);        // Process data
    } else {                                                            // "Overflow" buffer mode
      usart_process_debug(&rx_buffer_L[old_pos], rx_buffer_L_len - old_pos);        // Process data
      usart_process_debug(&rx_buffer_L[0], pos);        // Process data
    }
  }
  #endif // DEBUG_SERIAL_USART2

  #ifdef CONTROL_SERIAL_USART2
  uint8_t *ptr;	
  if (pos != old_pos) {                                                 // Check change in received data
    ptr = (uint8_t *)&commandL_raw;                                     // Initialize the pointer with command_raw address
    if (pos > old_pos && (pos - old_pos) == commandL_len) {             // "Linear" buffer mode: check if current position is over previous one AND data length equals expected length
      memcpy(ptr, &rx_buffer_L[old_pos], commandL_len);                 // Copy data. This is possible only if command_raw is contiguous! (meaning all the structure members have the same size)
      usart_process_command(&commandL_raw, &commandL, 2);               // Process data
    } else if ((rx_buffer_L_len - old_pos + pos) == commandL_len) {     // "Overflow" buffer mode: check if data length equals expected length
      memcpy(ptr, &rx_buffer_L[old_pos], rx_buffer_L_len - old_pos);    // First copy data from the end of buffer
      if (pos > 0) {                                                    // Check and continue with beginning of buffer
        ptr += rx_buffer_L_len - old_pos;                               // Move to correct position in command_raw
        memcpy(ptr, &rx_buffer_L[0], pos);                              // Copy remaining data
      }
      usart_process_command(&commandL_raw, &commandL, 2);               // Process data
    }
  }
  #endif // CONTROL_SERIAL_USART2

  #ifdef SIDEBOARD_SERIAL_USART2
  uint8_t *ptr;	
  if (pos != old_pos) {                                                 // Check change in received data
    ptr = (uint8_t *)&Sideboard_L_raw;                                  // Initialize the pointer with Sideboard_raw address
    if (pos > old_pos && (pos - old_pos) == Sideboard_L_len) {          // "Linear" buffer mode: check if current position is over previous one AND data length equals expected length
      memcpy(ptr, &rx_buffer_L[old_pos], Sideboard_L_len);              // Copy data. This is possible only if Sideboard_raw is contiguous! (meaning all the structure members have the same size)
      usart_process_sideboard(&Sideboard_L_raw, &Sideboard_L, 2);       // Process data
    } else if ((rx_buffer_L_len - old_pos + pos) == Sideboard_L_len) {  // "Overflow" buffer mode: check if data length equals expected length
      memcpy(ptr, &rx_buffer_L[old_pos], rx_buffer_L_len - old_pos);    // First copy data from the end of buffer
      if (pos > 0) {                                                    // Check and continue with beginning of buffer
        ptr += rx_buffer_L_len - old_pos;                               // Move to correct position in Sideboard_raw
        memcpy(ptr, &rx_buffer_L[0], pos);                              // Copy remaining data
      }
      usart_process_sideboard(&Sideboard_L_raw, &Sideboard_L, 2);       // Process data
    }
  }
  #endif // SIDEBOARD_SERIAL_USART2

  #if defined(DEBUG_SERIAL_USART2) || defined(CONTROL_SERIAL_USART2) || defined(SIDEBOARD_SERIAL_USART2)
  old_pos = pos;                                                        // Update old position
  if (old_pos == rx_buffer_L_len) {                                     // Check and manually update if we reached end of buffer
    old_pos = 0;
  }
	#endif
}


/*
 * Check for new data received on USART3 with DMA: refactored function from https://github.com/MaJerle/stm32-usart-uart-dma-rx-tx
 * - this function is called for every USART IDLE line detection, in the USART interrupt handler
 */
void usart3_rx_check(void)
{
  #if defined(DEBUG_SERIAL_USART3) || defined(CONTROL_SERIAL_USART3) || defined(SIDEBOARD_SERIAL_USART3)
  static uint32_t old_pos;
  uint32_t pos;  
  pos = rx_buffer_R_len - __HAL_DMA_GET_COUNTER(huart3.hdmarx);         // Calculate current position in buffer
  #endif

  #if defined(DEBUG_SERIAL_USART3)
  if (pos != old_pos) {                                                 // Check change in received data
    if (pos > old_pos) {                                                // "Linear" buffer mode: check if current position is over previous one
      usart_process_debug(&rx_buffer_R[old_pos], pos - old_pos);        // Process data
    } else {                                                            // "Overflow" buffer mode
      usart_process_debug(&rx_buffer_R[old_pos], rx_buffer_R_len - old_pos);        // Process data
      usart_process_debug(&rx_buffer_R[old_pos], pos);                           // Process data
    }
  }
  #endif // DEBUG_SERIAL_USART3

  #ifdef CONTROL_SERIAL_USART3
  uint8_t *ptr;
  if (pos != old_pos) {                                                 // Check change in received data
    ptr = (uint8_t *)&commandR_raw;                                     // Initialize the pointer with command_raw address
    if (pos > old_pos && (pos - old_pos) == commandR_len) {             // "Linear" buffer mode: check if current position is over previous one AND data length equals expected length
      memcpy(ptr, &rx_buffer_R[old_pos], commandR_len);                 // Copy data. This is possible only if command_raw is contiguous! (meaning all the structure members have the same size)
      usart_process_command(&commandR_raw, &commandR, 3);               // Process data
    } else if ((rx_buffer_R_len - old_pos + pos) == commandR_len) {     // "Overflow" buffer mode: check if data length equals expected length
      memcpy(ptr, &rx_buffer_R[old_pos], rx_buffer_R_len - old_pos);    // First copy data from the end of buffer
      if (pos > 0) {                                                    // Check and continue with beginning of buffer
        ptr += rx_buffer_R_len - old_pos;                               // Move to correct position in command_raw
        memcpy(ptr, &rx_buffer_R[0], pos);                              // Copy remaining data
      }
      usart_process_command(&commandR_raw, &commandR, 3);               // Process data
    }
  }
  #endif // CONTROL_SERIAL_USART3

  #ifdef SIDEBOARD_SERIAL_USART3
  uint8_t *ptr;
  if (pos != old_pos) {                                                 // Check change in received data
    ptr = (uint8_t *)&Sideboard_R_raw;                                  // Initialize the pointer with Sideboard_raw address
    if (pos > old_pos && (pos - old_pos) == Sideboard_R_len) {          // "Linear" buffer mode: check if current position is over previous one AND data length equals expected length
      memcpy(ptr, &rx_buffer_R[old_pos], Sideboard_R_len);              // Copy data. This is possible only if Sideboard_raw is contiguous! (meaning all the structure members have the same size)
      usart_process_sideboard(&Sideboard_R_raw, &Sideboard_R, 3);       // Process data
    } else if ((rx_buffer_R_len - old_pos + pos) == Sideboard_R_len) {  // "Overflow" buffer mode: check if data length equals expected length
      memcpy(ptr, &rx_buffer_R[old_pos], rx_buffer_R_len - old_pos);    // First copy data from the end of buffer
      if (pos > 0) {                                                    // Check and continue with beginning of buffer
        ptr += rx_buffer_R_len - old_pos;                               // Move to correct position in Sideboard_raw
        memcpy(ptr, &rx_buffer_R[0], pos);                              // Copy remaining data
      }
      usart_process_sideboard(&Sideboard_R_raw, &Sideboard_R, 3);       // Process data
    }
  }
  #endif // SIDEBOARD_SERIAL_USART3

  #if defined(DEBUG_SERIAL_USART3) || defined(CONTROL_SERIAL_USART3) || defined(SIDEBOARD_SERIAL_USART3)
  old_pos = pos;                                                        // Update old position
  if (old_pos == rx_buffer_R_len) {                                     // Check and manually update if we reached end of buffer
    old_pos = 0;
  }
  #endif
}

/*
 * Process Rx debug user command input
 */
#if defined(DEBUG_SERIAL_USART2) || defined(DEBUG_SERIAL_USART3)
void usart_process_debug(uint8_t *userCommand, uint32_t len)
{
  #ifdef DEBUG_SERIAL_PROTOCOL
    static uint8_t debug_buffer[SERIAL_BUFFER_SIZE];
    static size_t pos = 0;
    static enum {
      WAIT_START,
      IN_FRAME
    } state = WAIT_START;

    for (size_t i = 0; i < len; i++) {
      if (state == WAIT_START) {
        pos = 0;
      }
      if (userCommand[i] == '$') {
        state = IN_FRAME;
      }
      if (state != IN_FRAME) {
        continue;
      }

      debug_buffer[pos++] = userCommand[i];
      if (userCommand[i] == '\n' || userCommand[i] == '\r') {
        state = WAIT_START;
        handle_input(debug_buffer, pos);
        continue;
      }
      
      if (pos >= SERIAL_BUFFER_SIZE) {
        state = WAIT_START;
        pos = 0;
      }
    }
  #endif
}

#endif // SERIAL_DEBUG

/*
 * Process command Rx data
 * - if the command_in data is valid (correct START_FRAME and checksum) copy the command_in to command_out
 */
#if defined(CONTROL_SERIAL_USART2) || defined(CONTROL_SERIAL_USART3)
void usart_process_command(SerialCommand *command_in, SerialCommand *command_out, uint8_t usart_idx)
{
  #ifdef CONTROL_IBUS
    uint16_t ibus_chksum;
    if (command_in->start == IBUS_LENGTH && command_in->type == IBUS_COMMAND) {
      ibus_chksum = 0xFFFF - IBUS_LENGTH - IBUS_COMMAND;
      for (uint8_t i = 0; i < (IBUS_NUM_CHANNELS * 2); i++) {
        ibus_chksum -= command_in->channels[i];
      }
      if (ibus_chksum == (uint16_t)((command_in->checksumh << 8) + command_in->checksuml)) {
        *command_out = *command_in;
        if (usart_idx == 2) {             // Sideboard USART2
          #ifdef CONTROL_SERIAL_USART2
          timeoutFlgSerial_L = 0;         // Clear timeout flag
          timeoutCntSerial_L = 0;         // Reset timeout counter
          #endif
        } else if (usart_idx == 3) {      // Sideboard USART3
          #ifdef CONTROL_SERIAL_USART3
          timeoutFlgSerial_R = 0;         // Clear timeout flag
          timeoutCntSerial_R = 0;         // Reset timeout counter
          #endif
        }
      }
    }
  #else
  uint16_t checksum;
  if (command_in->start == SERIAL_START_FRAME) {
    checksum = (uint16_t)(command_in->start ^ command_in->steer ^ command_in->speed);
    if (command_in->checksum == checksum) {
      *command_out = *command_in;
      if (usart_idx == 2) {             // Sideboard USART2
        #ifdef CONTROL_SERIAL_USART2
        timeoutFlgSerial_L = 0;         // Clear timeout flag
        timeoutCntSerial_L = 0;         // Reset timeout counter
        #endif
      } else if (usart_idx == 3) {      // Sideboard USART3
        #ifdef CONTROL_SERIAL_USART3
        timeoutFlgSerial_R = 0;         // Clear timeout flag
        timeoutCntSerial_R = 0;         // Reset timeout counter
        #endif
      }
    }
  }
  #endif
}
#endif

/*
 * Process Sideboard Rx data
 * - if the Sideboard_in data is valid (correct START_FRAME and checksum) copy the Sideboard_in to Sideboard_out
 */
#if defined(SIDEBOARD_SERIAL_USART2) || defined(SIDEBOARD_SERIAL_USART3)
void usart_process_sideboard(SerialSideboard *Sideboard_in, SerialSideboard *Sideboard_out, uint8_t usart_idx)
{
  uint16_t checksum;
  if (Sideboard_in->start == SERIAL_START_FRAME) {
    checksum = (uint16_t)(Sideboard_in->start ^ Sideboard_in->pitch ^ Sideboard_in->dPitch ^ Sideboard_in->cmd1 ^ Sideboard_in->cmd2 ^ Sideboard_in->sensors);
    if (Sideboard_in->checksum == checksum) {
      *Sideboard_out = *Sideboard_in;
      if (usart_idx == 2) {             // Sideboard USART2
        #ifdef SIDEBOARD_SERIAL_USART2
        timeoutCntSerial_L  = 0;        // Reset timeout counter
        timeoutFlgSerial_L = 0;         // Clear timeout flag
        #endif
      } else if (usart_idx == 3) {      // Sideboard USART3
        #ifdef SIDEBOARD_SERIAL_USART3
        timeoutCntSerial_R = 0;         // Reset timeout counter
        timeoutFlgSerial_R = 0;         // Clear timeout flag
        #endif
      }
    }
  }
}
#endif


/* =========================== Sideboard Functions =========================== */

/*
 * Sideboard LEDs Handling
 * This function manages the leds behavior connected to the sideboard
 */
void sideboardLeds(uint8_t *leds) {
  #if defined(SIDEBOARD_SERIAL_USART2) || defined(SIDEBOARD_SERIAL_USART3)
    // Enable flag: use LED4 (bottom Blue)
    // enable == 1, turn on led
    // enable == 0, blink led
    if (enable) {
      *leds |= LED4_SET;
    } else if (!enable && (main_loop_counter % 20 == 0)) {
      *leds ^= LED4_SET;
    }

    // Backward Drive: use LED5 (upper Blue)
    // backwardDrive == 1, blink led
    // backwardDrive == 0, turn off led
    if (backwardDrive && (main_loop_counter % 50 == 0)) {
      *leds ^= LED5_SET;
    }

    // Brake: use LED5 (upper Blue)
    // brakePressed == 1, turn on led
    // brakePressed == 0, turn off led
    #ifdef VARIANT_HOVERCAR
      if (brakePressed) {
        *leds |= LED5_SET;
      } else if (!brakePressed && !backwardDrive) {
        *leds &= ~LED5_SET;
      }
    #endif

    // Battery Level Indicator: use LED1, LED2, LED3
    if (main_loop_counter % BAT_BLINK_INTERVAL == 0) {              //  | RED (LED1) | YELLOW (LED3) | GREEN (LED2) |
      if (batVoltage < BAT_DEAD) {                                  //  |     0      |       0       |      0       |
        *leds &= ~LED1_SET & ~LED3_SET & ~LED2_SET;
      } else if (batVoltage < BAT_LVL1) {                           //  |     B      |       0       |      0       |
        *leds ^= LED1_SET;
        *leds &= ~LED3_SET & ~LED2_SET;
      } else if (batVoltage < BAT_LVL2) {                           //  |     1      |       0       |      0       |
        *leds |= LED1_SET;
        *leds &= ~LED3_SET & ~LED2_SET;
      } else if (batVoltage < BAT_LVL3) {                           //  |     0      |       B       |      0       |
        *leds ^= LED3_SET;
        *leds &= ~LED1_SET & ~LED2_SET;
      } else if (batVoltage < BAT_LVL4) {                           //  |     0      |       1       |      0       |
        *leds |= LED3_SET;
        *leds &= ~LED1_SET & ~LED2_SET;
      } else if (batVoltage < BAT_LVL5) {                           //  |     0      |       0       |      B       |
        *leds ^= LED2_SET;
        *leds &= ~LED1_SET & ~LED3_SET;
      } else {                                                      //  |     0      |       0       |      1       |
        *leds |= LED2_SET;
        *leds &= ~LED1_SET & ~LED3_SET;
      }
    }

    // Error handling
    // Critical error:  LED1 on (RED)     + high pitch beep (hadled in main)
    // Soft error:      LED3 on (YELLOW)  + low  pitch beep (hadled in main)
    if (rtY_Left.z_errCode || rtY_Right.z_errCode) {
      *leds |= LED1_SET;
      *leds &= ~LED3_SET & ~LED2_SET;
    }
    if (timeoutFlgADC || timeoutFlgSerial) {
      *leds |= LED3_SET;
      *leds &= ~LED1_SET & ~LED2_SET;
    }
  #endif
}

/*
 * Sideboard Sensor Handling
 * This function manages the sideboards photo sensors.
 * In non-hoverboard variants, the sensors are used as push buttons.
 */
void sideboardSensors(uint8_t sensors) {
  #if !defined(VARIANT_HOVERBOARD) && (defined(SIDEBOARD_SERIAL_USART2) || defined(SIDEBOARD_SERIAL_USART3))
    static uint8_t sensor1_index;                                 // holds the press index number for sensor1, when used as a button
    static uint8_t sensor1_prev,  sensor2_prev;
    uint8_t sensor1_trig = 0, sensor2_trig = 0;
    #if defined(SIDEBOARD_SERIAL_USART2)
    uint8_t  sideboardIdx = SIDEBOARD_SERIAL_USART2;
    uint16_t sideboardSns = Sideboard_L.sensors;
    #else
    uint8_t  sideboardIdx = SIDEBOARD_SERIAL_USART3;
    uint16_t sideboardSns = Sideboard_R.sensors;
    #endif

    if (inIdx == sideboardIdx) {                                  // Use Sideboard data
      sensor1_index = 2 + ((sideboardSns & SWB_SET) >> 9);        // SWB on RC transmitter is used to change Control Type
      if (sensor1_index == 2) {                                   // FOC control Type
        sensor1_index = (sideboardSns & SWC_SET) >> 11;           // SWC on RC transmitter is used to change Control Mode
      }
      sensor1_trig  = sensor1_index != sensor1_prev;              // rising or falling edge change detection
      if (inIdx != inIdx_prev) {                                  // Force one update at Input idx change
        sensor1_trig  = 1;
      }
      sensor1_prev  = sensor1_index;
    } else {                                                      // Use Optical switches
      sensor1_trig  = (sensors & SENSOR1_SET) && !sensor1_prev;   // rising edge detection
      sensor1_prev  =  sensors & SENSOR1_SET;
    }

    // Control MODE and Control Type Handling
    if (sensor1_trig) {
      switch (sensor1_index) {
        case 0:     // FOC VOLTAGE
          rtP_Left.z_ctrlTypSel = rtP_Right.z_ctrlTypSel = FOC_CTRL;
          ctrlModReqRaw         = VLT_MODE;
          break;
        case 1:     // FOC SPEED
          rtP_Left.z_ctrlTypSel = rtP_Right.z_ctrlTypSel = FOC_CTRL;
          ctrlModReqRaw         = SPD_MODE;
          break;
        case 2:     // FOC TORQUE
          rtP_Left.z_ctrlTypSel = rtP_Right.z_ctrlTypSel = FOC_CTRL;
          ctrlModReqRaw         = TRQ_MODE;
          break;
        case 3:     // SINUSOIDAL
          rtP_Left.z_ctrlTypSel = rtP_Right.z_ctrlTypSel = SIN_CTRL;
          break;
        case 4:     // COMMUTATION
          rtP_Left.z_ctrlTypSel = rtP_Right.z_ctrlTypSel = COM_CTRL;
          break;
      }
      if (inIdx == inIdx_prev) { beepShortMany(sensor1_index + 1, 1); }
      if (++sensor1_index > 4) { sensor1_index = 0; }
    }

                                                             // Field Weakening Activation/Deactivation
      static uint8_t  sensor2_index = 1;                          // holds the press index number for sensor2, when used as a button

      // Override in case the Sideboard control is Active
      if (inIdx == sideboardIdx) {                                // Use Sideboard data
        sensor2_index = (sideboardSns & SWD_SET) >> 13;           // SWD on RC transmitter is used to Activate/Deactivate Field Weakening
        sensor2_trig  = sensor2_index != sensor2_prev;            // rising or falling edge change detection
        if (inIdx != inIdx_prev) {                                // Force one update at Input idx change
          sensor2_trig  = 1;
        }
        sensor2_prev  = sensor2_index;
      }else{
        sensor2_trig  = (sensors & SENSOR2_SET) && !sensor2_prev;   // rising edge detection
        sensor2_prev  =  sensors & SENSOR2_SET;
      }

      #ifdef CRUISE_CONTROL_SUPPORT                                 // Cruise Control Activation/Deactivation
        if (sensor2_trig) {
          cruiseControl(sensor2_trig);
        }
      #else
        if (sensor2_trig) {
          switch (sensor2_index) {
            case 0:     // FW Disabled
              rtP_Left.b_fieldWeakEna  = 0; 
              rtP_Right.b_fieldWeakEna = 0;
              Input_Lim_Init();
              break;
            case 1:     // FW Enabled
              rtP_Left.b_fieldWeakEna  = 1; 
              rtP_Right.b_fieldWeakEna = 1;
              Input_Lim_Init();
              break; 
          }
          if (inIdx == inIdx_prev) { beepShortMany(sensor2_index + 1, 1); }
          if (++sensor2_index > 1) { sensor2_index = 0; }
        }
      #endif  // CRUISE_CONTROL_SUPPORT
  #endif
}



/* =========================== Poweroff Functions =========================== */

 /*
 * Save Configuration to Flash
 * This function makes sure data is not lost after power-off
 */
void saveConfig() {
  #ifdef VARIANT_TRANSPOTTER
    if (saveValue_valid) {
      HAL_FLASH_Unlock();
      EE_WriteVariable(VirtAddVarTab[0], saveValue);
      HAL_FLASH_Lock();
    }
  #endif
  #if !defined(VARIANT_HOVERBOARD) && !defined(VARIANT_TRANSPOTTER)
    if (inp_cal_valid || cur_spd_valid) {
      #if defined(DEBUG_SERIAL_USART2) || defined(DEBUG_SERIAL_USART3)
        printf("Saving configuration to EEprom\r\n");
      #endif

      HAL_FLASH_Unlock();
      EE_WriteVariable(VirtAddVarTab[0] , (uint16_t)FLASH_WRITE_KEY);
      EE_WriteVariable(VirtAddVarTab[1] , (uint16_t)rtP_Left.i_max);
      EE_WriteVariable(VirtAddVarTab[2] , (uint16_t)rtP_Left.n_max);
      for (uint8_t i=0; i<INPUTS_NR; i++) {
        EE_WriteVariable(VirtAddVarTab[ 3+8*i] , (uint16_t)input1[i].typ);
        EE_WriteVariable(VirtAddVarTab[ 4+8*i] , (uint16_t)input1[i].min);
        EE_WriteVariable(VirtAddVarTab[ 5+8*i] , (uint16_t)input1[i].mid);
        EE_WriteVariable(VirtAddVarTab[ 6+8*i] , (uint16_t)input1[i].max);
        EE_WriteVariable(VirtAddVarTab[ 7+8*i] , (uint16_t)input2[i].typ);
        EE_WriteVariable(VirtAddVarTab[ 8+8*i] , (uint16_t)input2[i].min);
        EE_WriteVariable(VirtAddVarTab[ 9+8*i] , (uint16_t)input2[i].mid);
        EE_WriteVariable(VirtAddVarTab[10+8*i] , (uint16_t)input2[i].max);
      }
      HAL_FLASH_Lock();
    }
  #endif 
}


void poweroff(void) {
  enable = 0;
  #if defined(DEBUG_SERIAL_USART2) || defined(DEBUG_SERIAL_USART3)
  printf("-- Motors disabled --\r\n");
  #endif
  buzzerCount = 0;  // prevent interraction with beep counter
  buzzerPattern = 0;
  for (int i = 0; i < 8; i++) {
    buzzerFreq = (uint8_t)i;
    HAL_Delay(100);
  }
  saveConfig();
  HAL_GPIO_WritePin(OFF_PORT, OFF_PIN, GPIO_PIN_RESET);
  while(1) {}
}


void poweroffPressCheck(void) {
  #if !defined(VARIANT_HOVERBOARD) && !defined(VARIANT_TRANSPOTTER)
    if (powerButtonPressed()) {
      uint16_t cnt_press = 0;
      while (powerButtonPressed()) {
        HAL_Delay(10);
        if (cnt_press++ == 5 * 100) { beepShort(5); }
      }

      if (cnt_press > 8) {
        enable = 0;
      }

      if (cnt_press >= 5 * 100) {                         // Check if press is more than 5 sec
        HAL_Delay(1000);
        if (powerButtonPressed()) {                       // Double press: Adjust Max Current, Max Speed
          while (powerButtonPressed()) { HAL_Delay(10); }
          beepLong(8);
          updateCurSpdLim();
          beepShort(5);
        } else {                                          // Long press: Calibrate ADC Limits
          #ifdef AUTO_CALIBRATION_ENA
          beepLong(16); 
          adcCalibLim();
          beepShort(5);
          #endif
        }
      } else if (cnt_press > 8) {                         // Short press: power off (80 ms debounce)
        #if defined(DEBUG_SERIAL_USART2) || defined(DEBUG_SERIAL_USART3)
          printf("Powering off, button has been pressed\r\n");
        #endif
        poweroff();
      }
    }
  #elif defined(VARIANT_TRANSPOTTER)
    if (powerButtonPressed()) {
      enable = 0;
      while (powerButtonPressed()) { HAL_Delay(10); }
      HAL_Delay(300);
      if (powerButtonPressed()) {
        while (powerButtonPressed()) { HAL_Delay(10); }
        beepLong(5);
        HAL_Delay(350);
        poweroff();
      } else {
        setDistance += 0.25;
        if (setDistance > 2.6) {
          setDistance = 0.5;
        }
        beepShort(setDistance / 0.25);
        saveValue = setDistance * 1000;
        saveValue_valid = 1;
      }
    }
  #else
    if (powerButtonPressed()) {
      enable = 0;                                             // disable motors
      while (powerButtonPressed()) {}                        // wait until button is released
      poweroff();                                             // release power-latch
    }
  #endif
}



/* =========================== Filtering Functions =========================== */
#if !defined(SW_PWM_RIGHT) && !defined(SW_PWM_LEFT) && !defined(HW_PWM)
  /* Low pass filter fixed-point 32 bits: fixdt(1,32,16)
  * Max:  32767.99998474121
  * Min: -32768
  * Res:  1.52587890625e-05
  * 
  * Inputs:       u     = int16 or int32
  * Outputs:      y     = fixdt(1,32,16)
  * Parameters:   coef  = fixdt(0,16,16) = [0,65535U]
  * 
  * Example: 
  * If coef = 0.8 (in floating point), then coef = 0.8 * 2^16 = 52429 (in fixed-point)
  * filtLowPass16(u, 52429, &y);
  * yint = (int16_t)(y >> 16); // the integer output is the fixed-point ouput shifted by 16 bits
  */
void filtLowPass32(int32_t u, uint16_t coef, int32_t *y) {
  int64_t tmp;  
  tmp = ((int64_t)((u << 4) - (*y >> 12)) * coef) >> 4;
  tmp = CLAMP(tmp, -2147483648LL, 2147483647LL);  // Overflow protection: 2147483647LL = 2^31 - 1
  *y = (int32_t)tmp + (*y);
}
  // Old filter
  // Inputs:       u     = int16
  // Outputs:      y     = fixdt(1,32,20)
  // Parameters:   coef  = fixdt(0,16,16) = [0,65535U]
  // yint = (int16_t)(y >> 20); // the integer output is the fixed-point ouput shifted by 20 bits
  // void filtLowPass32(int16_t u, uint16_t coef, int32_t *y) {
  //   int32_t tmp;  
  //   tmp = (int16_t)(u << 4) - (*y >> 16);  
  //   tmp = CLAMP(tmp, -32768, 32767);  // Overflow protection  
  //   *y  = coef * tmp + (*y);
  // }


  /* rateLimiter16(int16_t u, int16_t rate, int16_t *y);
  * Inputs:       u     = int16
  * Outputs:      y     = fixdt(1,16,4)
  * Parameters:   rate  = fixdt(1,16,4) = [0, 32767] Do NOT make rate negative (>32767)
  */
void rateLimiter16(int16_t u, int16_t rate, int16_t *y) {
  int16_t q0;
  int16_t q1;

  q0 = (u << 4)  - *y;

  if (q0 > rate) {
    q0 = rate;
  } else {
    q1 = -rate;
    if (q0 < q1) {
      q0 = q1;
    }
  }

  *y = q0 + *y;
}
#else

void filtLowPass32(int32_t u, uint16_t coef, int32_t *y) {
  int64_t tmp;  
  tmp = ((int64_t)((u << 4) - (*y >> 12)) * coef) >> 4;
  tmp = CLAMP(tmp, -2147483648LL, 2147483647LL);  // Overflow protection: 2147483647LL = 2^31 - 1
  *y = (int32_t)tmp + (*y);
}

void rateLimiter16(int16_t u, int16_t rate, int16_t *y) {
  int16_t q0;
  int16_t q1;

  q0 = u  - *y;

  if (q0 > rate) {
    q0 = rate;
  } else {
    q1 = -rate;
    if (q0 < q1) {
      q0 = q1;
    }
  }
  *y = q0 + *y;
}

#endif
  /* mixerFcn(rtu_speed, rtu_steer, &rty_speedR, &rty_speedL); 
  * Inputs:       rtu_speed, rtu_steer                  = fixdt(1,16,4)
  * Outputs:      rty_speedR, rty_speedL                = int16_t
  * Parameters:   SPEED_COEFFICIENT, STEER_COEFFICIENT  = fixdt(0,16,14)
  */
void mixerFcn(int16_t rtu_speed, int16_t rtu_steer, int16_t *rty_speedR, int16_t *rty_speedL) {
    int16_t prodSpeed;
    int16_t prodSteer;
    int32_t tmp;

    prodSpeed   = (int16_t)((rtu_speed * (int16_t)SPEED_COEFFICIENT) >> 14);
    prodSteer   = (int16_t)((rtu_steer * (int16_t)STEER_COEFFICIENT) >> 14);

    tmp         = prodSpeed - prodSteer;  
    tmp         = CLAMP(tmp, -32768, 32767);  // Overflow protection
    *rty_speedR = (int16_t)(tmp >> 4);        // Convert from fixed-point to int 
    *rty_speedR = CLAMP(*rty_speedR, INPUT_MIN, INPUT_MAX);

    tmp         = prodSpeed + prodSteer;
    tmp         = CLAMP(tmp, -32768, 32767);  // Overflow protection
    *rty_speedL = (int16_t)(tmp >> 4);        // Convert from fixed-point to int
    *rty_speedL = CLAMP(*rty_speedL, INPUT_MIN, INPUT_MAX);
}



/* =========================== Multiple Tap Function =========================== */

  /* multipleTapDet(int16_t u, uint32_t timeNow, MultipleTap *x)
  * This function detects multiple tap presses, such as double tapping, triple tapping, etc.
  * Inputs:       u = int16_t (input signal); timeNow = uint32_t (current time)  
  * Outputs:      x->b_multipleTap (get the output here)
  */
void multipleTapDet(int16_t u, uint32_t timeNow, MultipleTap *x) {
  uint8_t 	b_timeout;
  uint8_t 	b_hyst;
  uint8_t 	b_pulse;
  uint8_t 	z_pulseCnt;
  uint8_t   z_pulseCntRst;
  uint32_t 	t_time; 

  // Detect hysteresis
  if (x->b_hysteresis) {
    b_hyst = (u > MULTIPLE_TAP_LO);
  } else {
    b_hyst = (u > MULTIPLE_TAP_HI);
  }

  // Detect pulse
  b_pulse = (b_hyst != x->b_hysteresis);

  // Save time when first pulse is detected
  if (b_hyst && b_pulse && (x->z_pulseCntPrev == 0)) {
    t_time = timeNow;
  } else {
    t_time = x->t_timePrev;
  }

  // Create timeout boolean
  b_timeout = (timeNow - t_time > MULTIPLE_TAP_TIMEOUT);

  // Create pulse counter
  if ((!b_hyst) && (x->z_pulseCntPrev == 0)) {
    z_pulseCnt = 0U;
  } else {
    z_pulseCnt = b_pulse;
  }

  // Reset counter if we detected complete tap presses OR there is a timeout
  if ((x->z_pulseCntPrev >= MULTIPLE_TAP_NR) || b_timeout) {
    z_pulseCntRst = 0U;
  } else {
    z_pulseCntRst = x->z_pulseCntPrev;
  }
  z_pulseCnt = z_pulseCnt + z_pulseCntRst;

  // Check if complete tap presses are detected AND no timeout
  if ((z_pulseCnt >= MULTIPLE_TAP_NR) && (!b_timeout)) {
    x->b_multipleTap = !x->b_multipleTap;	// Toggle output
  }

  // Update states
  x->z_pulseCntPrev = z_pulseCnt;
  x->b_hysteresis 	= b_hyst;
  x->t_timePrev 	  = t_time;
}


