
#include <stdbool.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include "defines.h"
#include "setup.h"
#include "config.h"

#define NUNCHUK_I2C_ADDRESS 0xA4

TIM_HandleTypeDef TimHandle;
TIM_HandleTypeDef TimHandle2;
uint8_t  ppm_count = 0;
uint8_t  pwm_count = 0;
uint32_t timeoutCntGen = TIMEOUT;
uint8_t  timeoutFlgGen = 0;
uint8_t  nunchuk_data[6] = {0};

uint8_t i2cBuffer[2];
nunchuk_state nunchukState = NUNCHUK_CONNECTING;

extern I2C_HandleTypeDef hi2c2;
extern DMA_HandleTypeDef hdma_i2c2_rx;
extern DMA_HandleTypeDef hdma_i2c2_tx;

#if defined(CONTROL_PPM_LEFT) || defined(CONTROL_PPM_RIGHT)
uint16_t ppm_captured_value[PPM_NUM_CHANNELS + 1] = {500, 500};
uint16_t ppm_captured_value_buffer[PPM_NUM_CHANNELS+1] = {500, 500};
uint32_t ppm_timeout = 0;

bool ppm_valid = true;

void PPM_ISR_Callback(void) {
  // Dummy loop with 16 bit count wrap around
  uint16_t rc_delay = TIM2->CNT;
  TIM2->CNT = 0;

  if (rc_delay > 3000) {
    if (ppm_valid && ppm_count == PPM_NUM_CHANNELS) {
      ppm_timeout = 0;
      timeoutCntGen = 0;
      timeoutFlgGen = 0;
      memcpy(ppm_captured_value, ppm_captured_value_buffer, sizeof(ppm_captured_value));
    }
    ppm_valid = true;
    ppm_count = 0;
  }
  else if (ppm_count < PPM_NUM_CHANNELS && IN_RANGE(rc_delay, 900, 2100)){
    ppm_captured_value_buffer[ppm_count++] = CLAMP(rc_delay, 1000, 2000) - 1000;
  } else {
    ppm_valid = false;
  }
}

// SysTick executes once each ms
void PPM_SysTick_Callback(void) {
  ppm_timeout++;
  // Stop after 500 ms without PPM signal
  if(ppm_timeout > 500) {
    int i;
    for(i = 0; i < PPM_NUM_CHANNELS; i++) {
      ppm_captured_value[i] = 500;
    }
    ppm_timeout = 0;
  }
}

void PPM_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /*Configure GPIO pin : PA3 */
  GPIO_InitStruct.Pin = PPM_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(PPM_PORT, &GPIO_InitStruct);

  __HAL_RCC_TIM2_CLK_ENABLE();
  TimHandle.Instance = TIM2;
  TimHandle.Init.Period = UINT16_MAX;
  TimHandle.Init.Prescaler = (SystemCoreClock/DELAY_TIM_FREQUENCY_US)-1;;
  TimHandle.Init.ClockDivision = 0;
  TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  HAL_TIM_Base_Init(&TimHandle);

  #if defined(CONTROL_PPM_LEFT)  
  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);
  #endif

  #if defined(CONTROL_PPM_RIGHT)  
  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
  #endif

  HAL_TIM_Base_Start(&TimHandle);
}
#endif


#if defined(RC_PWM_LEFT) || defined(RC_PWM_RIGHT)
 /*
  * Illustration of the PWM functionality
  * CH1 ________|‾‾‾‾‾‾‾‾‾‾|________
  * CH2 ______________|‾‾‾‾‾‾‾‾‾‾‾|________
  *             ↑     ↑    ↑      ↑
  * TIM2       RST  SAVE RC_CH1 RC_CH1
 */

int16_t pwm_captured_ch1_value = 500;
int16_t pwm_captured_ch2_value = 500;
uint16_t pwm_CNT_prev_ch1 = 0;
uint16_t pwm_CNT_prev_ch2 = 0;
uint32_t pwm_timeout_ch1 = 0;
uint32_t pwm_timeout_ch2 = 0;

void PWM_ISR_CH1_Callback(void) {
  // Dummy loop with 16 bit count wrap around
  if(HAL_GPIO_ReadPin(PWM_PORT_CH1, PWM_PIN_CH1)) {   // Rising  Edge interrupt -> save timer value OR reset timer
    if (HAL_GPIO_ReadPin(PWM_PORT_CH2, PWM_PIN_CH2)) {
      pwm_CNT_prev_ch1 = TIM2->CNT;
    } else {
      TIM2->CNT = 0;
      pwm_CNT_prev_ch1 = 0;
    }
  } else {                                    // Falling Edge interrupt -> measure pulse duration
    uint16_t rc_signal = TIM2->CNT - pwm_CNT_prev_ch1;
    if (IN_RANGE(rc_signal, 900, 2100)){
      timeoutCntGen = 0;
      timeoutFlgGen = 0;
      pwm_timeout_ch1 = 0;
      pwm_captured_ch1_value = CLAMP(rc_signal, 1000, 2000) - 1000;
    }
  }
}

void PWM_ISR_CH2_Callback(void) {
  // Dummy loop with 16 bit count wrap around
  if(HAL_GPIO_ReadPin(PWM_PORT_CH2, PWM_PIN_CH2)) {   // Rising  Edge interrupt -> save timer value OR reset timer
    if (HAL_GPIO_ReadPin(PWM_PORT_CH1, PWM_PIN_CH1)) {
      pwm_CNT_prev_ch2 = TIM2->CNT;
    } else {
      TIM2->CNT = 0;
      pwm_CNT_prev_ch2 = 0;
    }
  } else {                                    // Falling Edge interrupt -> measure pulse duration
    uint16_t rc_signal = TIM2->CNT - pwm_CNT_prev_ch2;
    if (IN_RANGE(rc_signal, 900, 2100)){
      timeoutCntGen = 0;
      timeoutFlgGen = 0;
      pwm_timeout_ch2 = 0;
      pwm_captured_ch2_value = CLAMP(rc_signal, 1000, 2000) - 1000;
    }
  }
}

// SysTick executes once each ms
void PWM_SysTick_Callback(void) {
  pwm_timeout_ch1++;
  pwm_timeout_ch2++;
  // Stop after 500 ms without PWM signal
  if(pwm_timeout_ch1 > 500) {
    pwm_captured_ch1_value = 500;
    pwm_timeout_ch1 = 0;
  }
  if(pwm_timeout_ch2 > 500) {
    pwm_captured_ch2_value = 500;
    pwm_timeout_ch2 = 0;
  }
}

void PWM_Init(void) {
  // PWM Timer (TIM2)
  __HAL_RCC_TIM2_CLK_ENABLE();
  TimHandle.Instance            = TIM2;
  TimHandle.Init.Period         = UINT16_MAX;
  TimHandle.Init.Prescaler      = (SystemCoreClock/DELAY_TIM_FREQUENCY_US)-1;;
  TimHandle.Init.ClockDivision  = 0;
  TimHandle.Init.CounterMode    = TIM_COUNTERMODE_UP;
  HAL_TIM_Base_Init(&TimHandle);  
  
  // Channel 1 (steering)
  GPIO_InitTypeDef GPIO_InitStruct1 = {0};
  // Configure GPIO pin : PA2 (Left) or PB10 (Right)
  GPIO_InitStruct1.Pin          = PWM_PIN_CH1;
  GPIO_InitStruct1.Mode         = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct1.Speed        = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct1.Pull         = GPIO_PULLDOWN;
  HAL_GPIO_Init(PWM_PORT_CH1, &GPIO_InitStruct1);

  // Channel 2 (speed)
  GPIO_InitTypeDef GPIO_InitStruct2 = {0};
  /*Configure GPIO pin : PA3 (Left) or PB11 (Right) */
  GPIO_InitStruct2.Pin          = PWM_PIN_CH2;
  GPIO_InitStruct2.Mode         = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct2.Speed        = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct2.Pull         = GPIO_PULLDOWN;
  HAL_GPIO_Init(PWM_PORT_CH2, &GPIO_InitStruct2);

  #ifdef RC_PWM_LEFT
  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);
  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);
  #endif

  #ifdef RC_PWM_RIGHT
  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
  #endif

  // Start timer
  HAL_TIM_Base_Start(&TimHandle);
}
#endif


#if defined(SW_PWM_LEFT) || defined(SW_PWM_RIGHT)
int16_t pwm_captured_ch1_value = 0;
int16_t pwm_captured_ch2_value = 0;
uint16_t pwm_CNT_prev_ch1 = 0;
uint16_t pwm_CNT_prev_ch2 = 0;
uint32_t pwm_timeout_ch1 = 0;
uint32_t pwm_timeout_ch2 = 0;
uint32_t pwm_period_ch1 = 0;
uint32_t pwm_period_ch2 = 0;
uint32_t pwm_CNT_falling_ch1 = 0;
uint32_t pwm_CNT_falling_ch2 = 0;

void PWM_ISR_CH1_Callback(void) {
  if(HAL_GPIO_ReadPin(PWM_PORT_CH1, PWM_PIN_CH1)) {   // Rising Edge interrupt
    // Save rising edge time for period calculation
    uint16_t current_cnt = TIM2->CNT;
    pwm_period_ch1 = current_cnt - pwm_CNT_prev_ch1;
    // Update previous rising edge time for next period calculation
    pwm_CNT_prev_ch1 = current_cnt;
    
  } else {                                    // Falling Edge interrupt
    // Save falling edge time for duty cycle calculation
    pwm_CNT_falling_ch1 = TIM2->CNT;
    uint16_t duty_ticks = pwm_CNT_falling_ch1 - pwm_CNT_prev_ch1;
    
    // Store duty cycle if valid and avoid division by zero
    // Add reasonable bounds checking for PWM period (1kHz to 100kHz range)
    if (pwm_period_ch1 > 1 && pwm_period_ch1 < 50000 && duty_ticks <= pwm_period_ch1) {
      // Fixed: proper calculation with parentheses for order of operations  
      uint32_t duty_cycle_pct = (duty_ticks * 10000) / pwm_period_ch1;
      
      // Only update if value is reasonable (prevent glitches)
      if (duty_cycle_pct <= 10000) {
        pwm_captured_ch1_value = MAP(duty_cycle_pct, 0, 10000, -1000, 1000);
        
        // Reset timeouts on valid signal
        timeoutCntGen = 0;
        timeoutFlgGen = 0;
        pwm_timeout_ch1 = 0;
      }
    }
  }
}

void PWM_ISR_CH2_Callback(void) {
  // Dummy loop with 16 bit count wrap around
  if(HAL_GPIO_ReadPin(PWM_PORT_CH2, PWM_PIN_CH2)) {   // Rising  Edge interrupt -> save timer value OR reset timer
    uint16_t current_cnt = TIM2->CNT;
    pwm_period_ch2 = current_cnt - pwm_CNT_prev_ch2;
    // Update previous rising edge time for next period calculation
    pwm_CNT_prev_ch2 = current_cnt;
    
  } else {                                    // Falling Edge interrupt
    // Save falling edge time for duty cycle calculation
    pwm_CNT_falling_ch2 = TIM2->CNT;
    uint16_t duty_ticks = pwm_CNT_falling_ch2 - pwm_CNT_prev_ch2;
    
    // Store duty cycle if valid and avoid division by zero
    // Add reasonable bounds checking for PWM period (1kHz to 100kHz range)
    if (pwm_period_ch2 > 1 && pwm_period_ch2 < 50000 && duty_ticks <= pwm_period_ch2) {
      // Calculate duty cycle percentage
      uint32_t duty_cycle_pct = (duty_ticks * 10000) / pwm_period_ch2;
      
      // Only update if value is reasonable (prevent glitches)
      if (duty_cycle_pct <= 10000) {
        pwm_captured_ch2_value = MAP(duty_cycle_pct, 0, 10000, -1000, 1000);
        
        // Reset timeouts on valid signal
        timeoutCntGen = 0;
        timeoutFlgGen = 0;
        pwm_timeout_ch2 = 0;
      }
    }
  }
}

// SysTick executes once each ms
void PWM_SysTick_Callback(void) {
  pwm_timeout_ch1++;
  pwm_timeout_ch2++;
  // Stop after 500 ms without PWM signal
  if(pwm_timeout_ch1 > 500) {
    pwm_captured_ch1_value = 0;
    pwm_timeout_ch1 = 0;
  }
  if(pwm_timeout_ch2 > 500) {
    pwm_captured_ch2_value = 0;
    pwm_timeout_ch2 = 0;
  }
}

void PWM_Init(void) {
  // PWM Timer (TIM2)
  __HAL_RCC_TIM2_CLK_ENABLE();
  TimHandle.Instance            = TIM2;
  TimHandle.Init.Period         = UINT16_MAX;
  TimHandle.Init.Prescaler      = (SystemCoreClock/1000000)-1;
  TimHandle.Init.ClockDivision  = 0;
  TimHandle.Init.CounterMode    = TIM_COUNTERMODE_UP;
  HAL_TIM_Base_Init(&TimHandle);  
  
  // Channel 1 (steering)
  GPIO_InitTypeDef GPIO_InitStruct1 = {0};
  // Configure GPIO pin : PA2 (Left) or PB10 (Right)
  GPIO_InitStruct1.Pin          = PWM_PIN_CH1;
  GPIO_InitStruct1.Mode         = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct1.Speed        = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct1.Pull         = GPIO_PULLDOWN;
  HAL_GPIO_Init(PWM_PORT_CH1, &GPIO_InitStruct1);

  // Channel 2 (speed)
  GPIO_InitTypeDef GPIO_InitStruct2 = {0};
  /*Configure GPIO pin : PA3 (Left) or PB11 (Right) */
  GPIO_InitStruct2.Pin          = PWM_PIN_CH2;
  GPIO_InitStruct2.Mode         = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct2.Speed        = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct2.Pull         = GPIO_PULLDOWN;
  HAL_GPIO_Init(PWM_PORT_CH2, &GPIO_InitStruct2);

  #ifdef SW_PWM_LEFT
  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);
  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);
  #endif

  #ifdef SW_PWM_RIGHT
  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
  #endif

  // Start timer
  HAL_TIM_Base_Start(&TimHandle);
}
#endif


#if defined(HW_PWM)
TIM_HandleTypeDef TimHandle_PWM;

static volatile uint32_t pwm_period_ticks_ch2 = 0;
static volatile uint32_t pwm_duty_ticks_ch2 = 0;

int16_t pwm_captured_ch1_value = 0;
int16_t pwm_captured_ch2_value = 0;
uint32_t pwm_timeout_ch1 = 0;
uint32_t pwm_timeout_ch2 = 0;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance != TIM3) {
    return;
  }

  if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) {
    uint32_t period_ticks = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
    uint32_t duty_ticks = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

    if (period_ticks > 0u) {
      if (duty_ticks > period_ticks) {
        duty_ticks = period_ticks;
      }

      pwm_period_ticks_ch2 = period_ticks;
      pwm_duty_ticks_ch2 = duty_ticks;

      uint32_t duty_scaled = (duty_ticks * 10000u) / period_ticks;
      if (duty_scaled <= 10000u) {
        pwm_captured_ch2_value = MAP(duty_scaled, 0, 10000, -1000, 1000);
        timeoutCntGen = 0;
        timeoutFlgGen = 0;
        pwm_timeout_ch2 = 0;
        pwm_timeout_ch1 = 0;
        pwm_captured_ch1_value = 0;
      }
    }
  }
}

void PWM_SysTick_Callback(void) {
  pwm_timeout_ch1++;
  pwm_timeout_ch2++;

  if (pwm_timeout_ch1 > 500u) {
    pwm_captured_ch1_value = 0;
    pwm_timeout_ch1 = 0;
  }

  if (pwm_timeout_ch2 > 500u) {
    pwm_captured_ch2_value = 0;
    pwm_timeout_ch2 = 0;
  }
}

void PWM_Init(void) {
  GPIO_InitTypeDef gpio_pwm = {0};

  gpio_pwm.Pin   = PWM_PIN_CH2;
  gpio_pwm.Mode  = GPIO_MODE_AF_INPUT;
  gpio_pwm.Pull  = GPIO_PULLUP;
  gpio_pwm.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(PWM_PORT_CH2, &gpio_pwm);

  __HAL_RCC_TIM3_CLK_ENABLE();
  __HAL_RCC_AFIO_CLK_ENABLE();
  __HAL_AFIO_REMAP_TIM3_PARTIAL();

  uint32_t timer_clk = HAL_RCC_GetPCLK1Freq();
  if ((RCC->CFGR & RCC_CFGR_PPRE1) != RCC_CFGR_PPRE1_DIV1) {
    timer_clk *= 2u;
  }

  const uint32_t target_tick_hz = 1000000u;
  uint32_t prescaler = (timer_clk + (target_tick_hz - 1u)) / target_tick_hz;
  if (prescaler == 0u) {
    prescaler = 1u;
  }

  TimHandle_PWM.Instance = TIM3;
  TimHandle_PWM.Init.Prescaler = prescaler - 1u;
  TimHandle_PWM.Init.CounterMode = TIM_COUNTERMODE_UP;
  TimHandle_PWM.Init.Period = 0xFFFFu;
  TimHandle_PWM.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  TimHandle_PWM.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  HAL_TIM_Base_Init(&TimHandle_PWM);
  HAL_TIM_IC_Init(&TimHandle_PWM);

  TIM_SlaveConfigTypeDef slave_cfg = {0};
  slave_cfg.SlaveMode = TIM_SLAVEMODE_RESET;
  slave_cfg.InputTrigger = TIM_TS_TI2FP2;
  slave_cfg.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  slave_cfg.TriggerPrescaler = TIM_ICPSC_DIV1;
  slave_cfg.TriggerFilter = 0;
  HAL_TIM_SlaveConfigSynchro(&TimHandle_PWM, &slave_cfg);

  TIM_IC_InitTypeDef ic_cfg = {0};
  ic_cfg.ICPrescaler = TIM_ICPSC_DIV1;
  ic_cfg.ICFilter = 0u;

  ic_cfg.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  ic_cfg.ICSelection = TIM_ICSELECTION_DIRECTTI;
  HAL_TIM_IC_ConfigChannel(&TimHandle_PWM, &ic_cfg, TIM_CHANNEL_2);

  ic_cfg.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  ic_cfg.ICSelection = TIM_ICSELECTION_INDIRECTTI;
  HAL_TIM_IC_ConfigChannel(&TimHandle_PWM, &ic_cfg, TIM_CHANNEL_1);

  TIM_MasterConfigTypeDef master_cfg = {0};
  master_cfg.MasterOutputTrigger = TIM_TRGO_RESET;
  master_cfg.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&TimHandle_PWM, &master_cfg);

  HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM3_IRQn);

  HAL_TIM_Base_Start(&TimHandle_PWM);
  HAL_TIM_IC_Start_IT(&TimHandle_PWM, TIM_CHANNEL_2);
  HAL_TIM_IC_Start_IT(&TimHandle_PWM, TIM_CHANNEL_1);

  pwm_period_ticks_ch2 = 0u;
  pwm_duty_ticks_ch2 = 0u;
  pwm_timeout_ch1 = 0u;
  pwm_timeout_ch2 = 0u;
  pwm_captured_ch1_value = 0;
  pwm_captured_ch2_value = 0;
}

#endif


uint8_t Nunchuk_tx(uint8_t i2cBuffer[], uint8_t i2cBufferLength) {
  if(HAL_I2C_Master_Transmit(&hi2c2,NUNCHUK_I2C_ADDRESS,(uint8_t*)i2cBuffer, i2cBufferLength, 100) == HAL_OK) {
    return true;
  }
  return false;
}

uint8_t Nunchuk_rx(uint8_t i2cBuffer[], uint8_t i2cBufferLength) {
  if(HAL_I2C_Master_Receive(&hi2c2,NUNCHUK_I2C_ADDRESS,(uint8_t*)i2cBuffer, i2cBufferLength, 100) == HAL_OK) {
    return true;
  }
  return false;
}

uint8_t Nunchuk_Init(void) {
  //-- START -- init WiiNunchuk
  i2cBuffer[0] = 0xF0;
  i2cBuffer[1] = 0x55;

  if(Nunchuk_tx(i2cBuffer, 2) == false) {
    return false;
  }
  HAL_Delay(10);

  i2cBuffer[0] = 0xFB;
  i2cBuffer[1] = 0x00;

  if(Nunchuk_tx(i2cBuffer, 2) == false) {
    return false;
  }
  HAL_Delay(10);

  return true;
}

uint8_t Nunchuk_Connect() {
  /* Initialise / re-initialise I2C peripheral */
  I2C_Init();
  
  /* Initialise / re-initialise nunchuk */
  if(Nunchuk_Init() == true) {
    nunchukState = NUNCHUK_CONNECTED;
    return true;
  } else {
    return false;
  }
}

nunchuk_state Nunchuk_Read(void) {
  static uint8_t delay_counter = 0;
  uint16_t checksum = 0;
  uint8_t success = true;
  uint8_t i = 0;

  switch(nunchukState) {
    case NUNCHUK_DISCONNECTED:
      success = false;
      /* Delay a bit before reconnecting */
      if(delay_counter++ > 100) {
        success = Nunchuk_Connect();
        delay_counter = 0;
      }
      break;
      
    case NUNCHUK_CONNECTING:
    case NUNCHUK_RECONNECTING:
        /* Try to reconnect once, if fails again fall back to disconnected state */
        success = Nunchuk_Connect();
        if(!success) {
          nunchukState = NUNCHUK_DISCONNECTED;
        }
      break;

    case NUNCHUK_CONNECTED:
      /* Send read address of 0x00 to the Nunchuk */
      i2cBuffer[0] = 0x00;
      if(!Nunchuk_tx(i2cBuffer, 1)) {
        success = false;
      }
      HAL_Delay(3);

      /* Clear the receive data buffer */
      for(i = 0; i<6; i++) {
        nunchuk_data[i] = 0;
      }

      /* Read back 6 bytes from the Nunchuk */
      if(!Nunchuk_rx(nunchuk_data, 6)) {
        success = false;
      }
      HAL_Delay(3);

      /* Checksum the receive buffer to ensure it is not in an error condition, i.e. all 0x00 or 0xFF */
      for(i = 0; i<6; i++) {
        checksum += nunchuk_data[i];
      }
      if(checksum == 0 || checksum == 0x5FA) {
        success = false;
      }

      /* Comms failure or timeout counter reached timeout limit */
      if(success == false || timeoutCntGen > 3) {
        /* Clear the receive data buffer */
        for(i = 0; i<6; i++) {
          nunchuk_data[i] = 0;
        }
        /* Brings motors to safe stop */
        /* Expected values from nunchuk for stopped (mid) position */
        nunchuk_data[0] = 127;
        nunchuk_data[1] = 128;
        timeoutFlgGen = 1;
        nunchukState = NUNCHUK_RECONNECTING;
      }
      break;
  }
  /* Reset the timeout flag and counter if successful communication */
  if(success == true) {
    timeoutCntGen = 0;
    timeoutFlgGen = 0;
  }
  return nunchukState;
  //setScopeChannel(0, (int)nunchuk_data[0]);
  //setScopeChannel(1, (int)nunchuk_data[1]);
  //setScopeChannel(2, (int)nunchuk_data[5] & 1);
  //setScopeChannel(3, ((int)nunchuk_data[5] >> 1) & 1);
}
