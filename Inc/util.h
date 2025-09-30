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

// Define to prevent recursive inclusion
#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include "rtwtypes.h"

// Rx Structures USART
#if defined(CONTROL_SERIAL_USART2) || defined(CONTROL_SERIAL_USART3)
  #ifdef CONTROL_IBUS
    typedef struct{
      uint8_t  start;
      uint8_t  type; 
      uint8_t  channels[IBUS_NUM_CHANNELS*2];
      uint8_t  checksuml;
      uint8_t  checksumh;
    } SerialCommand;
  #else
    typedef struct{
      uint16_t  start;
      int16_t   steer;
      int16_t   speed;
      uint16_t  checksum;
    } SerialCommand;
  #endif
#endif
#if defined(SIDEBOARD_SERIAL_USART2) || defined(SIDEBOARD_SERIAL_USART3)
    typedef struct{
      uint16_t  start;
      int16_t   pitch;      // Angle
      int16_t   dPitch;     // Angle derivative
      int16_t   cmd1;       // RC Channel 1
      int16_t   cmd2;       // RC Channel 2
      uint16_t  sensors;    // RC Switches and Optical sideboard sensors
      uint16_t  checksum;
    } SerialSideboard;
#endif

// Input Structure
typedef struct {
  int16_t   raw;    // raw input
  int16_t   cmd;    // command
  uint8_t   typ;    // type
  uint8_t   typDef; // type Defined
  int16_t   min;    // minimum
  int16_t   mid;    // middle
  int16_t   max;    // maximum
  int16_t   dband;  // deadband
} InputStruct;

#ifdef ENCODER
typedef struct {
  boolean_T ini;
  boolean_T ali;
  int32_t offset;
  uint8_t direction; // 1 = CW, 0 = CCW
  int32_t current_count;
  int32_t aligned_count;
  int32_t mech_angle_deg;
  int16_t align_inpTgt; // in util.c or as part of encoder struct
  // Non-blocking alignment state variables
  uint8_t align_state;        // Current alignment state (0=inactive, 1=sequence, 2=return, 3=wait, 4=calc)
  uint32_t align_timer;       // Timer for alignment sequence timing
  uint32_t align_start_time;  // Start time for alignment
  uint16_t align_step_counter;// Step counter for alignment sequence
  int32_t align_ini_pos;      // Initial encoder position before alignment
  uint8_t align_sequence_step;// Current step in hall sequence
  // Smooth ramping variables
  uint32_t ramp_timer;        // Timer for power ramping
  int16_t ramp_target;        // Target power for current step
  uint32_t step_start_time;   // Time when current step started
} SensorState;
extern SensorState encoder;
    uint8_t hall_ul;
    uint8_t hall_vl;
    uint8_t hall_wl;
TIM_HandleTypeDef encoder_handle;
#endif

// Initialization Functions
void BLDC_Init(void);
void Input_Lim_Init(void);
void Input_Init(void);
void UART_DisableRxErrors(UART_HandleTypeDef *huart);
#if defined (ENCODER)
void Encoder_Init(void);
void Encoder_Align(void);
void Encoder_Align_NonBlocking(void);  // Non-blocking encoder alignment state machine - call from main loop
void Encoder_Align_Start(void);        // Start non-blocking encoder alignment sequence
#endif

// General Functions
void poweronMelody(void);
void beepCount(uint8_t cnt, uint8_t freq, uint8_t pattern);
void beepLong(uint8_t freq);
void beepShort(uint8_t freq);
void beepShortMany(uint8_t cnt, int8_t dir);
void calcAvgSpeed(void);
void adcCalibLim(void);
void updateCurSpdLim(void);
void standstillHold(void);
void electricBrake(uint16_t speedBlend, uint8_t reverseDir);
void cruiseControl(uint8_t button);
int  checkInputType(int16_t min, int16_t mid, int16_t max);

// Input Functions
void calcInputCmd(InputStruct *in, int16_t out_min, int16_t out_max);
void readInputRaw(void);
void handleTimeout(void);
void readCommand(void);
void usart2_rx_check(void);
void usart3_rx_check(void);
#if defined(DEBUG_SERIAL_USART2) || defined(DEBUG_SERIAL_USART3)
void usart_process_debug(uint8_t *userCommand, uint32_t len);
#endif
#if defined(CONTROL_SERIAL_USART2) || defined(CONTROL_SERIAL_USART3)
void usart_process_command(SerialCommand *command_in, SerialCommand *command_out, uint8_t usart_idx);
#endif
#if defined(SIDEBOARD_SERIAL_USART2) || defined(SIDEBOARD_SERIAL_USART3)
void usart_process_sideboard(SerialSideboard *Sideboard_in, SerialSideboard *Sideboard_out, uint8_t usart_idx);
#endif

// Sideboard functions
void sideboardLeds(uint8_t *leds);
void sideboardSensors(uint8_t sensors);

// Poweroff Functions
void saveConfig(void);
void poweroff(void);
void poweroffPressCheck(void);

// Filtering Functions
void filtLowPass32(int32_t u, uint16_t coef, int32_t *y);
void rateLimiter16(int16_t u, int16_t rate, int16_t *y);
void mixerFcn(int16_t rtu_speed, int16_t rtu_steer, int16_t *rty_speedR, int16_t *rty_speedL);

// Multiple Tap Function
typedef struct {
  uint32_t  t_timePrev;
  uint8_t   z_pulseCntPrev;
  uint8_t   b_hysteresis;
  uint8_t   b_multipleTap;
} MultipleTap;
void multipleTapDet(int16_t u, uint32_t timeNow, MultipleTap *x);

#endif

