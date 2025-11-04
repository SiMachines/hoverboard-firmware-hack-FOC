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

#if defined(ENCODER_X) || defined(ENCODER_Y)
typedef struct {
  boolean_T ini;
  boolean_T ali;
  boolean_T align_fault;
  int32_t offset;
  boolean_T direction; // 1 = CW, 0 = CCW
  uint32_t aligned_count;
  int32_t normalized;
  int32_t ENCODER_COUNT;
  int16_t align_inpTgt; // in util.c or as part of encoder struct
  // Non-blocking alignment state variables
  uint8_t align_state;               // Current alignment state 
  uint32_t align_timer;              // Timer for alignment sequence timing
  uint32_t align_start_time;         // Start time for alignment
  int32_t align_ini_pos;             // Initial encoder position before alignment
  int32_t align_zero_pos;            
  int32_t align_target_pos;          // Target position to return to after high-power phase
  uint32_t align_move_start;         // Timestamp when the move-back began
  int32_t align_total_ini_pos;        // Starting emulated position for the move-back
  int32_t align_total_mid_pos;
  int32_t align_total_end_pos;
  // Mechanical angle simulation variables (integer only)
  int32_t emulated_mech_count;      // emulated encoder count for mechanical angle
  int32_t count_increment_x1000;    // Count increment per tick * 1000 (for precision)
  uint32_t power_ramp_timer;         // Timer for power ramping phases
  int32_t full_rotations;
  int32_t count_prev;
} SensorState;
#endif

#ifdef ENCODER_X
extern SensorState encoder_x;
extern TIM_HandleTypeDef encoder_x_handle;
void Encoder_X_Init(void);
void Encoder_X_Align(void);
void Encoder_X_Align_Start(void);
int32_t normalize_x_encoder_count(int32_t count);
void handle_x_rotation_phase(uint32_t elapsed_ticks, uint32_t ramp_ms, uint32_t move_ms, uint32_t current_time);
void handle_x_high_power_phase(uint32_t elapsed_ticks, uint32_t ramp_ms, uint32_t current_time);
void handle_x_move_back_phase(uint32_t elapsed_ticks, uint32_t ramp_ms, uint32_t move_ms, uint32_t current_time);
void finalize_x_alignment(void);
int32_t get_x_TotalCount(void);
void count_x_update(void);
int32_t get_y_TotalCount(void);
void count_y_update(void);
#endif

#ifdef ENCODER_Y
extern SensorState encoder_y;
extern TIM_HandleTypeDef encoder_y_handle;
void Encoder_Y_Init(void);
void Encoder_Y_Align(void);
void Encoder_Y_Align_Start(void);
int32_t normalize_y_encoder_count(int32_t count);
void handle_y_rotation_phase(uint32_t elapsed_ticks, uint32_t ramp_ms, uint32_t move_ms, uint32_t current_time);
void handle_y_high_power_phase(uint32_t elapsed_ticks, uint32_t ramp_ms, uint32_t current_time);
void handle_y_move_back_phase(uint32_t elapsed_ticks, uint32_t ramp_ms, uint32_t move_ms, uint32_t current_time);
void finalize_y_alignment(void);
#endif


// Initialization Functions
void BLDC_Init(void);
void BLDC_SetPwmResolution(uint16_t periodCounts);
void Input_Lim_Init(void);
void Input_Init(void);
void UART_DisableRxErrors(UART_HandleTypeDef *huart);
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
boolean_T encoder_alignment_faulted(void);

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
uint8_t powerButtonPressed(void);

#if defined(ANALOG_BUTTON)
void AnalogButton_Init(void);
#endif


void DcLinkWatchdog_Init(void);
uint8_t DLVPA(void);


/* Overcurrent fault handler (returns non-zero if fault active) */
boolean_T overcurrent_fault(void);

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

/*
 * BASEPRI helpers
 * - basepri_set_threshold(prio): set BASEPRI so that interrupts with
 *   numeric priority >= prio are masked. 'prio' is the numeric NVIC
 *   priority (0 = highest). Returns the previous BASEPRI value which
 *   can be passed to basepri_restore().
 * - basepri_restore(old): restore previous BASEPRI value.
 *
 * Note: use short critical sections. BASEPRI does not mask NMI or HardFault.
 */
uint32_t basepri_set_threshold(uint32_t prio);
void basepri_restore(uint32_t previous_basepri);

#endif

