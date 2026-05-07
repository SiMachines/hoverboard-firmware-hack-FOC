/*
* This file implements FOC motor control.
* This control method offers superior performanace
* compared to previous cummutation method. The new method features:
* ► reduced noise and vibrations
* ► smooth torque output
* ► improved motor efficiency -> lower energy consumption
*
* Copyright (C) 2019-2020 Emanuel FERU <aerdronix@gmail.com>
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

#include "stm32f1xx_hal.h"
#include "defines.h"
#include "setup.h"
#include "config.h"
#include "util.h"




// Matlab includes and defines - from auto-code generation
// ###############################################################################
#include "BLDC_controller.h"           /* Model's header file */
#include "rtwtypes.h"

extern RT_MODEL *const rtM_Left;
extern RT_MODEL *const rtM_Right;

extern DW   rtDW_Left;                  /* Observable states */
extern ExtU rtU_Left;                   /* External inputs */
extern ExtY rtY_Left;                   /* External outputs */
extern P    rtP_Left;

extern DW   rtDW_Right;                 /* Observable states */
extern ExtU rtU_Right;                  /* External inputs */
extern ExtY rtY_Right;                  /* External outputs */
// ###############################################################################



static int16_t pwm_margin;              /* This margin allows to have a window in the PWM signal for proper FOC Phase currents measurement */

extern uint8_t ctrlModReq;
static int16_t curDC_max = (I_DC_MAX * A2BIT_CONV);
int16_t I_BusR=0; 

int16_t curL_phaA = 0, curL_phaB = 0, curL_DC = 0;
int16_t curR_phaB = 0, curR_phaC = 0, curR_DC = 0;

volatile int pwml = 0;
volatile int pwmr = 0;
extern volatile adc_buf_t adc_buffer;

#if defined(PERF_MONITOR)
volatile uint32_t perf_bldc_cycles_last = 0;
volatile uint32_t perf_bldc_cycles_max = 0;
volatile uint32_t perf_bldc_overrun_count = 0;
volatile uint8_t perf_bldc_overrun_latched = 0;
#endif

uint8_t buzzerFreq          = 0;
uint8_t buzzerPattern       = 0;
uint8_t buzzerCount         = 0;
volatile uint32_t buzzerTimer = 0;
static uint8_t  buzzerPrev  = 0;
static uint8_t  buzzerIdx   = 0;
static uint8_t  brkResActiveL = 0;
 #ifdef EXTBRK_EN
static uint8_t  brkResActiveExt = 0;
#endif

uint8_t        enable       = 0;        // initially motors are disabled for SAFETY
static uint8_t enableFin    = 0;

static uint16_t pwm_res;              /* Timer auto-reload value; set during initialization */
 static volatile int ur, vr, wr;

void BLDC_SetPwmResolution(uint16_t periodCounts) {
  pwm_res = periodCounts ? periodCounts : 1U;
}

static uint16_t offsetcount = 0;
static int16_t offsetrlA    = CURRENT_SENSE_OFFSET_INIT;
static int16_t offsetrlB    = CURRENT_SENSE_OFFSET_INIT;
static int16_t offsetrrB    = CURRENT_SENSE_OFFSET_INIT;
static int16_t offsetrrC    = CURRENT_SENSE_OFFSET_INIT;
static int16_t offsetdcl    = CURRENT_SENSE_OFFSET_INIT;
static int16_t offsetdcr    = CURRENT_SENSE_OFFSET_INIT;
#if defined(CURRENT_SENSE_OFFSET_CAL_MODE_AVG)
static int32_t offsetSumRlA = 0;
static int32_t offsetSumRlB = 0;
static int32_t offsetSumRrB = 0;
static int32_t offsetSumRrC = 0;
static int32_t offsetSumDcl = 0;
static int32_t offsetSumDcr = 0;
#endif
int16_t unf_VBUS;
extern int16_t batVoltageCalib;


uint8_t BLDC_CurrentOffsetCalDone(void) {
  // Export calibration completion so main-loop alignment can wait for valid current offsets.
  return (offsetcount >= CURRENT_SENSE_OFFSET_CAL_SAMPLES) ? 1U : 0U; 
}

// =================================
// DMA interrupt frequency =~ 16 kHz
// =================================
void DMA1_Channel1_IRQHandler(void) {

#if defined(PERF_ISR_SCOPE)
  LED_PORT->BSRR = (uint32_t)LED_PIN;
#endif

#if defined(PERF_MONITOR)
  uint32_t perf_start = DWT->CYCCNT;
#endif

  DMA1->IFCR = DMA_IFCR_CTCIF1;
  // HAL_GPIO_WritePin(LED_PORT, LED_PIN, 1);
  // HAL_GPIO_TogglePin(LED_PORT, LED_PIN);

  if(offsetcount < CURRENT_SENSE_OFFSET_CAL_SAMPLES) {  // calibrate ADC offsets
    enable = 0;
  #if defined(CURRENT_SENSE_OFFSET_CAL_MODE_AVG)
    if (offsetcount == 0U) {
      offsetSumRlA = 0;
      offsetSumRlB = 0;
      offsetSumRrB = 0;
      offsetSumRrC = 0;
      offsetSumDcl = 0;
      offsetSumDcr = 0;
    }

    offsetSumRlA += (int32_t)adc_buffer.adc12.value.rlA;
    offsetSumRlB += (int32_t)adc_buffer.adc12.value.rlB;
    offsetSumRrB += (int32_t)adc_buffer.adc12.value.rrB;
    offsetSumRrC += (int32_t)adc_buffer.adc12.value.rrC;
    offsetSumDcl += (int32_t)adc_buffer.adc12.value.dcl;
    offsetSumDcr += (int32_t)adc_buffer.adc12.value.dcr;

    offsetcount++;
    if (offsetcount == CURRENT_SENSE_OFFSET_CAL_SAMPLES) {
      // Final exact-average ADC offsets; +2 keeps measured DC-link current centered on this hardware.
      offsetrlA = (int16_t)(offsetSumRlA / (int32_t)CURRENT_SENSE_OFFSET_CAL_SAMPLES);
      offsetrlB = (int16_t)(offsetSumRlB / (int32_t)CURRENT_SENSE_OFFSET_CAL_SAMPLES);
      offsetrrB = (int16_t)(offsetSumRrB / (int32_t)CURRENT_SENSE_OFFSET_CAL_SAMPLES);
      offsetrrC = (int16_t)(offsetSumRrC / (int32_t)CURRENT_SENSE_OFFSET_CAL_SAMPLES);
      offsetdcl = (int16_t)(offsetSumDcl / (int32_t)CURRENT_SENSE_OFFSET_CAL_SAMPLES) + CURRENT_SENSE_OFFSET_DCL_TRIM;
      offsetdcr = (int16_t)(offsetSumDcr / (int32_t)CURRENT_SENSE_OFFSET_CAL_SAMPLES) + CURRENT_SENSE_OFFSET_DCR_TRIM;
    }
  #else
    offsetcount++;
    offsetrlA = (adc_buffer.adc12.value.rlA + offsetrlA) / 2;
    offsetrlB = (adc_buffer.adc12.value.rlB + offsetrlB) / 2;
    offsetrrB = (adc_buffer.adc12.value.rrB + offsetrrB) / 2;
    offsetrrC = (adc_buffer.adc12.value.rrC + offsetrrC) / 2;
    offsetdcl = (adc_buffer.adc12.value.dcl + offsetdcl) / 2;
    offsetdcr = (adc_buffer.adc12.value.dcr + offsetdcr) / 2;
  #endif
  #if defined(PERF_ISR_SCOPE)
    LED_PORT->BSRR = (uint32_t)LED_PIN << 16;
  #endif
    return;
  }else if(offsetcount == CURRENT_SENSE_OFFSET_CAL_SAMPLES) {
    // Re-enable outputs once when calibration completes, then move past this state.
    enable = 1;
    offsetcount++; // just to make sure we do not enter the calibration branch again
}

  // Convert raw battery ADC counts to calibrated V*100 domain.
  unf_VBUS = adc_buffer.adc3.value.batt1 * BAT_CALIB_SCALAR;
 // if (buzzerTimer % 100 == 0) {  // Filter battery voltage at a slower sampling rate
 // 
 // }

  // Get Left motor currents
  curL_phaA = (int16_t)(offsetrlA - adc_buffer.adc12.value.rlA);
  curL_phaB = (int16_t)(offsetrlB - adc_buffer.adc12.value.rlB);
  curL_DC   = (int16_t)(offsetdcl - adc_buffer.adc12.value.dcl);
  
  // Get Right motor currents
  curR_phaB = (int16_t)(offsetrrB - adc_buffer.adc12.value.rrB);
  curR_phaC = (int16_t)(offsetrrC - adc_buffer.adc12.value.rrC);
  curR_DC   = (int16_t)(offsetdcr - adc_buffer.adc12.value.dcr);

  // Disable PWM when current limit is reached (current chopping)
  // This is the Level 2 of current protection. The Level 1 should kick in first given by I_MOT_MAX
#if defined(HOCP) //Disable motors as precaution if overcurrent or short detected through bkin input
  if (LEFT_TIM->SR & TIM_SR_BIF) {
    enable = 0;
  }
#endif

  if(ABS(curL_DC) > curDC_max || enable == 0) {
    LEFT_TIM->BDTR &= ~TIM_BDTR_MOE;
  } else {
    LEFT_TIM->BDTR |= TIM_BDTR_MOE;
  }

#if defined(HOCP)
  if (RIGHT_TIM->SR & TIM_SR_BIF) {
    enable = 0;
  }
#endif

  if(ABS(curR_DC)  > curDC_max || enable == 0) {
    RIGHT_TIM->BDTR &= ~TIM_BDTR_MOE;
  } else {
    RIGHT_TIM->BDTR |= TIM_BDTR_MOE;
  }

  // Create square wave for buzzer
  buzzerTimer++;
  if (buzzerFreq != 0 && (buzzerTimer / 5000) % (buzzerPattern + 1) == 0) {
    if (buzzerPrev == 0) {
      buzzerPrev = 1;
      if (++buzzerIdx > (buzzerCount + 2)) {    // pause 2 periods
        buzzerIdx = 1;
      }
    }
    if (buzzerTimer % buzzerFreq == 0 && (buzzerIdx <= buzzerCount || buzzerCount == 0)) {
      #ifdef BEEPER_OFF
      HAL_GPIO_WritePin(LED_PORT, LED_PIN, 1);  // add line for LED
      #else
      HAL_GPIO_TogglePin(BUZZER_PORT, BUZZER_PIN);   // comment line for BEEPER off
      #endif
    }

  } else if (buzzerPrev) {
      #ifdef BEEPER_OFF
      HAL_GPIO_WritePin(LED_PORT, LED_PIN, 0);  // add line for LED
      #else
      HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET); 
      #endif
      buzzerPrev = 0;
  }

  // Adjust pwm_margin depending on the selected Control Type
  if (rtP_Left.z_ctrlTypSel == FOC_CTRL) {
    pwm_margin = 59;
  } else {
    pwm_margin = 0;
  }

  // ############################### MOTOR CONTROL ###############################
  #ifndef INTBRK_L_EN
  int ul, vl, wl;
  #endif
  //int ur, vr, wr;
  static boolean_T OverrunFlag = false;

  /* Check for overrun */
  if (OverrunFlag) {
#if defined(PERF_MONITOR)
    perf_bldc_overrun_count++;
    perf_bldc_overrun_latched = 1U;
#endif
#if defined(PERF_ISR_SCOPE)
    LED_PORT->BSRR = (uint32_t)LED_PIN << 16;
#endif
    return;
  }
  OverrunFlag = true;

  /* Make sure to stop BOTH motors in case of an error */
  enableFin = enable && !rtY_Left.z_errCode && !rtY_Right.z_errCode &&
              !encoder_alignment_faulted() && !overcurrent_fault() && !DLVPA() &&
              !estop_active();
 
  // ========================= LEFT MOTOR ============================ 
    // Get hall sensors values
    #ifndef ENCODER_Y
    uint8_t hall_ul = !(LEFT_HALL_U_PORT->IDR & LEFT_HALL_U_PIN);
    uint8_t hall_vl = !(LEFT_HALL_V_PORT->IDR & LEFT_HALL_V_PIN);
    uint8_t hall_wl = !(LEFT_HALL_W_PORT->IDR & LEFT_HALL_W_PIN);
    #else //Emulate static hall sensor position if using ENCODER to prevent driver from triggering hall error
    uint8_t hall_ul = 0;
    uint8_t hall_vl = 1;
    uint8_t hall_wl = 0;
    #endif
    /* Set motor inputs here */
    rtU_Left.b_motEna     = enableFin;
    rtU_Left.z_ctrlModReq = ctrlModReq;  
    rtU_Left.b_hallA      = hall_ul;
    rtU_Left.b_hallB      = hall_vl;
    rtU_Left.b_hallC      = hall_wl; 
    rtU_Left.i_phaAB      = curL_phaA;
    rtU_Left.i_phaBC      = curL_phaB;
    rtU_Left.i_DCLink     = curL_DC;
    
    #ifdef ENCODER_Y
    if (!encoder_y.align_state) {
      rtU_Left.r_inpTgt = pwml;
    } else {
      rtU_Left.r_inpTgt = encoder_y.align_inpTgt;
      rtU_Left.a_encoderCNT = encoder_y.emulated_mech_count;
    }
    if (encoder_y.ali) {
      rtU_Left.a_encoderCNT = encoder_y_handle.Instance->CNT;
    }
    #else
    rtU_Left.r_inpTgt = pwml;
    #endif
    
    /* Step the controller */
    #ifdef MOTOR_LEFT_ENA    
    BLDC_controller_step(rtM_Left);
    #endif


    #ifndef INTBRK_L_EN
    /* Get motor outputs here */
    ul            = (uint16_t)CLAMP(rtY_Left.DC_phaA + pwm_res / 2 - pwm_margin, 0, pwm_res-pwm_margin);
    vl            = (uint16_t)CLAMP(rtY_Left.DC_phaB + pwm_res / 2 - pwm_margin, 0, pwm_res-pwm_margin);
    wl            = (uint16_t)CLAMP(rtY_Left.DC_phaC + pwm_res / 2 - pwm_margin, 0, pwm_res-pwm_margin);
  // errCodeLeft  = rtY_Left.z_errCode;
  // motSpeedLeft = rtY_Left.n_mot;
  // motAngleLeft = rtY_Left.a_elecAngle;

    /* Apply commands */
    
    LEFT_TIM->LEFT_TIM_U    = ul;
    LEFT_TIM->LEFT_TIM_V    = vl;
    LEFT_TIM->LEFT_TIM_W    = wl;
    #else
    // INTBRK: ON threshold uses BRKRESACT_SENS, OFF threshold uses MAX_REGEN_CURRENT.
    int16_t regenCur = curR_DC - MAX_REGEN_CURRENT;
    const int16_t brkOnThresh = BRKRESACT_SENS;
    //const int16_t brkOffThresh = (BRKRESACT_SENS > 1) ? (BRKRESACT_SENS / 2) : 0;
    const int16_t vBusCalib = unf_VBUS;
  #if defined(BRK_VOLTAGE_RAMP_ENABLED)
    const uint8_t overvoltageRequest = (vBusCalib > BRK_OVERVOLTAGE_RAMP_START);
    const int16_t rampSpan = (BRK_OVERVOLTAGE_RAMP_END > BRK_OVERVOLTAGE_RAMP_START) ?
                             (BRK_OVERVOLTAGE_RAMP_END - BRK_OVERVOLTAGE_RAMP_START) : 1;
  #else
    const uint8_t overvoltageRequest = 0;
  #endif
    int32_t brakeDutyCounts = 0;

    if (regenCur < 0) {
      regenCur = 0;
    }

    if (!brkResActiveL) {
      if ((regenCur > brkOnThresh) || overvoltageRequest) {
        brkResActiveL = 1;
      }
    } else {
      if ((regenCur <= MAX_REGEN_CURRENT) && !overvoltageRequest) {
        brkResActiveL = 0;
      }
    }

    if (brkResActiveL) {
      const int16_t vBusForDuty = (vBusCalib > 1) ? vBusCalib : 1;
      brakeDutyCounts = (((int32_t)regenCur * BRAKE_RESISTANCE * pwm_res) / (50 * vBusForDuty));

#if defined(BRK_VOLTAGE_RAMP_ENABLED)
      if (vBusCalib > BRK_OVERVOLTAGE_RAMP_START) {
        brakeDutyCounts += (((int32_t)(vBusCalib - BRK_OVERVOLTAGE_RAMP_START) * pwm_res) / rampSpan);
      }
#endif

      I_BusR = CLAMP(brakeDutyCounts, 0, (((uint32_t)pwm_res * 90) / 100));
      LEFT_TIM->LEFT_TIM_U = I_BusR;
    } else {
      LEFT_TIM->LEFT_TIM_U = 0;
    }
     
    #endif
  // =================================================================
  

  // ========================= RIGHT MOTOR ===========================  
    // Get hall sensors values
    #ifndef ENCODER_X 
    uint8_t hall_ur = !(RIGHT_HALL_U_PORT->IDR & RIGHT_HALL_U_PIN);
    uint8_t hall_vr = !(RIGHT_HALL_V_PORT->IDR & RIGHT_HALL_V_PIN);
    uint8_t hall_wr = !(RIGHT_HALL_W_PORT->IDR & RIGHT_HALL_W_PIN);
    #else  //Emulate static hall sensor position if using ENCODER to prevent driver from triggering hall error
    uint8_t hall_ur = 0;
    uint8_t hall_vr = 1;
    uint8_t hall_wr = 0;
    #endif

    /* Set motor inputs here */
    rtU_Right.b_motEna      = enableFin;
    rtU_Right.z_ctrlModReq  = ctrlModReq;
    rtU_Right.b_hallA       = hall_ur;
    rtU_Right.b_hallB       = hall_vr;
    rtU_Right.b_hallC       = hall_wr;
    rtU_Right.i_phaAB       = curR_phaB;
    rtU_Right.i_phaBC       = curR_phaC;
    rtU_Right.i_DCLink      = curR_DC;
  

     #ifdef ENCODER_X
    if (!encoder_x.align_state) {
      #ifdef HSPWM
      rtU_Right.r_inpTgt = HS_PWM;  //~-1ms
      #else
      rtU_Right.r_inpTgt = pwmr;
      #endif

    } else {
      rtU_Right.r_inpTgt = encoder_x.align_inpTgt;
      rtU_Right.a_encoderCNT = encoder_x.emulated_mech_count;
    }
    if (encoder_x.ali){
    rtU_Right.a_encoderCNT = encoder_x_handle.Instance->CNT;
    } 
    #else
  rtU_Right.r_inpTgt = pwmr;
    #endif
    
    /* Step the controller */
    #ifdef MOTOR_RIGHT_ENA
    BLDC_controller_step(rtM_Right);
    #endif

    /* Get motor outputs here */
    ur            = (uint16_t)CLAMP(rtY_Right.DC_phaA + pwm_res / 2 - pwm_margin, 0, pwm_res-pwm_margin);
    vr            = (uint16_t)CLAMP(rtY_Right.DC_phaB + pwm_res / 2 - pwm_margin, 0, pwm_res-pwm_margin);
    wr            = (uint16_t)CLAMP(rtY_Right.DC_phaC + pwm_res / 2 - pwm_margin, 0, pwm_res-pwm_margin);
 // errCodeRight  = rtY_Right.z_errCode;
 // motSpeedRight = rtY_Right.n_mot;
 // motAngleRight = rtY_Right.a_elecAngle;

    /* Apply commands */
    RIGHT_TIM->RIGHT_TIM_U  = ur;
    RIGHT_TIM->RIGHT_TIM_V  = vr;
    RIGHT_TIM->RIGHT_TIM_W  = wr;
    
  // =================================================================
   

  //External Break Resistor Output Control
  #ifdef EXTBRK_EN
  // EXTBRK uses total bus regen current; thresholds mirror INTBRK semantics.
  int16_t busRegenCur = (curR_DC + curL_DC) - MAX_REGEN_CURRENT; // Total bus regen current in x10 resolution
  const int16_t brkOnThreshExt = BRKRESACT_SENS;
  //const int16_t brkOffThreshExt = (BRKRESACT_SENS > 1) ? (BRKRESACT_SENS / 2) : 0;
  const int16_t vBusCalibExt = batVoltageCalib;
#if defined(BRK_VOLTAGE_RAMP_ENABLED)
  const uint8_t overvoltageRequestExt = (vBusCalibExt > BRK_OVERVOLTAGE_RAMP_START);
  const int16_t rampSpanExt = (BRK_OVERVOLTAGE_RAMP_END > BRK_OVERVOLTAGE_RAMP_START) ?
                              (BRK_OVERVOLTAGE_RAMP_END - BRK_OVERVOLTAGE_RAMP_START) : 1;
#else
  const uint8_t overvoltageRequestExt = 0;
#endif
  int32_t brakeDutyCountsExt = 0;

  if (busRegenCur < 0) {
    busRegenCur = 0;
  }

  if (!brkResActiveExt) {
    if ((busRegenCur > brkOnThreshExt) || overvoltageRequestExt) {
      brkResActiveExt = 1;
    }
  } else {
    if ((busRegenCur <= MAX_REGEN_CURRENT) && !overvoltageRequestExt) {
      brkResActiveExt = 0;
    }
  }

  if (brkResActiveExt) {
    const int16_t vBusForDutyExt = (vBusCalibExt > 1) ? vBusCalibExt : 1;
    brakeDutyCountsExt = (((int32_t)busRegenCur * BRAKE_RESISTANCE * pwm_res) / (50 * vBusForDutyExt));

#if defined(BRK_VOLTAGE_RAMP_ENABLED)
    if (vBusCalibExt > BRK_OVERVOLTAGE_RAMP_START) {
      brakeDutyCountsExt += (((int32_t)(vBusCalibExt - BRK_OVERVOLTAGE_RAMP_START) * pwm_res) / rampSpanExt);
    }
#endif

    I_BusR = CLAMP(brakeDutyCountsExt, 0, (((uint32_t)pwm_res * 90) / 100));
    EXT_PWM_BRK = I_BusR;
  } else {
    EXT_PWM_BRK = 0;
    I_BusR = 0;
  }
  #endif
  /* Indicate task complete */
#if defined(PERF_MONITOR)
  {
    uint32_t perf_cycles = DWT->CYCCNT - perf_start;
    perf_bldc_cycles_last = perf_cycles;
    if (perf_cycles > perf_bldc_cycles_max) {
      perf_bldc_cycles_max = perf_cycles;
    }
  }
#endif
#if defined(PERF_ISR_SCOPE)
  LED_PORT->BSRR = (uint32_t)LED_PIN << 16;
#endif
  OverrunFlag = false;
 
 // ###############################################################################

}
