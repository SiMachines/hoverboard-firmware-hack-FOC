/*
 * File: BLDC_controller.c
 *
 * Code generated for Simulink model 'BLDC_controller'.
 *
 * Model version                  : 16.101
 * Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
 * C/C++ source code generated on : Sat Aug 22 16:19:28 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objective: Execution efficiency
 * Validation result: Not run
 */

#include "BLDC_controller.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Named constants for Chart: '<S5>/F03_02_Control_Mode_Manager' */
#define IN_ACTIVE                      ((uint8_t)1U)
#define IN_NO_ACTIVE_CHILD             ((uint8_t)0U)
#define IN_OPEN                        ((uint8_t)2U)
#define IN_SPEED_MODE                  ((uint8_t)1U)
#define IN_TORQUE_MODE                 ((uint8_t)2U)
#define IN_VOLTAGE_MODE                ((uint8_t)3U)
#define OPEN_MODE                      ((uint8_t)0U)
#define SPD_MODE                       ((uint8_t)2U)
#define TRQ_MODE                       ((uint8_t)3U)
#define VLT_MODE                       ((uint8_t)1U)
#ifndef UCHAR_MAX
#include <limits.h>
#endif

#if ( UCHAR_MAX != (0xFFU) ) || ( SCHAR_MAX != (0x7F) )
#error Code was generated for compiler with different sized uchar/char. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( USHRT_MAX != (0xFFFFU) ) || ( SHRT_MAX != (0x7FFF) )
#error Code was generated for compiler with different sized ushort/short. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( UINT_MAX != (0xFFFFFFFFU) ) || ( INT_MAX != (0x7FFFFFFF) )
#error Code was generated for compiler with different sized uint/int. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( ULONG_MAX != (0xFFFFFFFFU) ) || ( LONG_MAX != (0x7FFFFFFF) )
#error Code was generated for compiler with different sized ulong/long. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

/* Skipping ulong_long/long_long check: insufficient preprocessor integer range. */

/* Exported block parameters */
uint16_t cf_KbLimProt = 246U;          /* Variable: cf_KbLimProt
                                        * Referenced by:
                                        *   '<S238>/cf_KbLimProt'
                                        *   '<S268>/Kb'
                                        */
uint8_t plook_u8u16u16n15_even8ca_gs(uint16_t u, uint16_t bp0, uint16_t
  *fraction);
int16_t intrp1d_s16s32s32u8u16n15la_s(uint8_t bpIndex, uint16_t frac, const
  int16_t table[], uint32_t maxIndex);
uint8_t plook_u8u16_evencka(uint16_t u, uint16_t bp0, uint16_t bpSpace, uint32_t
  maxIndex);
uint8_t plook_u8u16_evenckag(uint16_t u, uint16_t bp0, uint16_t bpSpace);
uint8_t plook_u8s16_evenckag(int16_t u, int16_t bp0, uint16_t bpSpace);
int32_t div_nzp_sus32(uint32_t numerator, int32_t denominator);
extern void Counter_Init(int16_t rtp_z_cntInit, DW_Counter *localDW);
extern int16_t Counter(int16_t rtu_inc, int16_t rtu_max, bool rtu_rst,
  DW_Counter *localDW);
extern void Low_Pass_Filter_Init(DW_Low_Pass_Filter *localDW);
extern void Low_Pass_Filter_Reset(DW_Low_Pass_Filter *localDW);
extern void Low_Pass_Filter(const int16_t rtu_u[2], uint16_t rtu_coef, int16_t
  rty_y[2], DW_Low_Pass_Filter *localDW);
extern void Counter_e_Init(uint16_t rtp_z_cntInit, DW_Counter_d *localDW);
extern uint16_t Counter_e(uint16_t rtu_inc, uint16_t rtu_max, bool rtu_rst,
  DW_Counter_d *localDW);
extern void Counter_b_Init(uint32_t rtp_z_cntInit, DW_Counter_e *localDW);
extern void Counter_n(uint16_t rtu_inc, uint32_t rtu_max, bool rtu_rst, uint16_t
                      *rty_cnt, DW_Counter_e *localDW);
extern bool either_edge(bool rtu_u, DW_either_edge *localDW);
extern void Debounce_Filter_Init(bool *rty_y, DW_Debounce_Filter *localDW);
extern void Debounce_Filter(bool rtu_u, uint16_t rtu_tAcv, uint32_t rtu_tDeacv,
  bool *rty_y, DW_Debounce_Filter *localDW);
extern void F03_Control_Mode_Manager_Init(uint8_t *rty_z_ctrlMod, int16_t
  *rty_r_inpTgtSca, DW_F03_Control_Mode_Manager *localDW);
extern void F03_Control_Mode_Manager(bool rtu_b_errFlag, bool rtu_b_motEna, bool
  rtu_b_cruiseCtrlEna, uint8_t rtu_z_ctrlTypSel, uint8_t rtu_z_ctrlModReq,
  int16_t rtu_r_inpTgt, int16_t rtu_VqFinPrev, uint8_t *rty_z_ctrlMod, int16_t
  *rty_r_inpTgtScaAbs, int16_t *rty_r_inpTgtSca, int16_t *rty_r_inpTgtConv,
  const P *rtP, DW_F03_Control_Mode_Manager *localDW);
extern void I_backCalc_fixdt_Init(int32_t rtp_yInit, DW_I_backCalc_fixdt
  *localDW);
extern void I_backCalc_fixdt(int16_t rtu_err, uint16_t rtu_I, uint16_t rtu_Kb,
  int16_t rtu_satMax, int16_t rtu_satMin, int16_t *rty_out, DW_I_backCalc_fixdt *
  localDW);

#if FeedForwardEnable == 1

extern void PI_clamp_fixdtFF_Init(DW_PI_clamp_fixdtFF *localDW);

#endif

#if FeedForwardEnable == 1

extern void PI_clamp_fixdtFF_Reset(DW_PI_clamp_fixdtFF *localDW);

#endif

#if FeedForwardEnable == 1

extern int16_t PI_clamp_fixdtFF(int16_t rtu_err, uint16_t rtu_P, uint16_t rtu_I,
  int16_t rtu_init, int16_t rtu_satMax, int16_t rtu_satMin, int16_t rtu_tgtInput,
  int16_t rtu_FFGain, DW_PI_clamp_fixdtFF *localDW);

#endif

#if FeedForwardEnable == 2

extern void TMClassic_Init(DW_TMClassic *localDW);

#endif

#if FeedForwardEnable == 2

extern void TMClassic_Reset(DW_TMClassic *localDW);

#endif

#if FeedForwardEnable == 2

extern int16_t TMClassic(int16_t rtu_err, uint16_t rtu_P, uint16_t rtu_I,
  int16_t rtu_init, int16_t rtu_satMax, int16_t rtu_satMin, DW_TMClassic
  *localDW);

#endif

extern void PI_clamp_fixdt_Init(DW_PI_clamp_fixdt *localDW);
extern int16_t PI_clamp_fixdt(int16_t rtu_err, uint16_t rtu_P, uint16_t rtu_I,
  int16_t rtu_init, int16_t rtu_satMax, int16_t rtu_satMin, int32_t
  rtu_ext_limProt, DW_PI_clamp_fixdt *localDW);

#if FeedForwardEnable == 1

extern void PI_clamp_fixdtFF_d_Init(DW_PI_clamp_fixdtFF_k *localDW);

#endif

#if FeedForwardEnable == 1

extern int16_t PI_clamp_fixdtFF_c(int16_t rtu_err, uint16_t rtu_P, uint16_t
  rtu_I, int16_t rtu_init, int16_t rtu_satMax, int16_t rtu_satMin, int16_t
  rtu_tgtInput, int16_t rtu_FFGain, DW_PI_clamp_fixdtFF_k *localDW);

#endif

extern void FOC_Init(int16_t *rty_Vq, DW_FOC *localDW);
extern void FOC(uint8_t rtu_z_ctrlTypSel, bool rtu_b_enaAdvCtrl, uint8_t
                rtu_z_ctrlMod, int16_t rtu_r_inpTgtSca, int16_t
                rtu_r_inpTgtScaAbs, int16_t rtu_n_mot, int16_t rtu_id_TgtAbs,
                int16_t rtu_id, int16_t rtu_iq, int16_t rtu_Vd_max, int16_t
                rtu_Vd_min, int16_t rtu_Vq_max, int16_t rtu_Vq_min, int16_t
                rtu_id_max, int16_t rtu_id_min, int16_t rtu_iq_max, int16_t
                rtu_iq_min, int16_t rtu_r_iqLimProtMax, int16_t
                rtu_r_nLimProtMax, int32_t rtu_iq_limProtErr, int16_t
                rtu_Vq_nLimProtMax, int16_t rtu_VqFinPrev, bool
                rtu_b_cruiseCtrlEna, const P *rtP, int16_t *rty_Vq, int16_t *rty_Vd, DW_FOC
                *localDW);

/* Forward declaration for local functions */
void enter_internal_ACTIVE(const bool *LogicalOperator1, const bool
  *LogicalOperator2, uint8_t *rty_z_ctrlMod, DW_F03_Control_Mode_Manager
  *localDW);

/*===========*
 * Constants *
 *===========*/
#define RT_PI                          3.14159265358979323846
#define RT_PIF                         3.1415927F
#define RT_LN_10                       2.30258509299404568402
#define RT_LN_10F                      2.3025851F
#define RT_LOG10E                      0.43429448190325182765
#define RT_LOG10EF                     0.43429449F
#define RT_E                           2.7182818284590452354
#define RT_EF                          2.7182817F

/*
 * UNUSED_PARAMETER(x)
 *   Used to specify that a function parameter (argument) is required but not
 *   accessed by the function body.
 */
#ifndef UNUSED_PARAMETER
#if defined(__LCC__)
#define UNUSED_PARAMETER(x)                                      /* do nothing */
#else

/*
 * This is the semi-ANSI standard way of indicating that an
 * unused function parameter is required.
 */
#define UNUSED_PARAMETER(x)            (void) (x)
#endif
#endif

uint8_t plook_u8u16u16n15_even8ca_gs(uint16_t u, uint16_t bp0, uint16_t
  *fraction)
{
  uint16_t uAdjust;
  uint8_t bpIndex;

  /* Prelookup - Index and Fraction
     Index Search method: 'even'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'on'
     Rounding mode: 'simplest'
   */
  uAdjust = (uint16_t)((uint32_t)u - bp0);
  bpIndex = (uint8_t)((uint32_t)uAdjust >> 8U);
  *fraction = (uint16_t)((uint16_t)(uAdjust & 255) << 7);
  return bpIndex;
}

int16_t intrp1d_s16s32s32u8u16n15la_s(uint8_t bpIndex, uint16_t frac, const
  int16_t table[], uint32_t maxIndex)
{
  int16_t y;

  /* Column-major Interpolation 1-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'on'
     Rounding mode: 'simplest'
     Overflow mode: 'wrapping'
   */
  if (bpIndex == maxIndex) {
    y = table[(uint32_t)bpIndex];
  } else {
    int16_t yL_0d0;
    yL_0d0 = table[(uint32_t)bpIndex];
    y = (int16_t)((int16_t)(((table[bpIndex + 1U] - yL_0d0) * frac) >> 15) +
                  yL_0d0);
  }

  return y;
}

uint8_t plook_u8u16_evencka(uint16_t u, uint16_t bp0, uint16_t bpSpace, uint32_t
  maxIndex)
{
  uint8_t bpIndex;

  /* Prelookup - Index only
     Index Search method: 'even'
     Extrapolation method: 'Clip'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u <= bp0) {
    bpIndex = 0U;
  } else {
    uint16_t fbpIndex;
    fbpIndex = (uint16_t)((uint32_t)(uint16_t)((uint32_t)u - bp0) / bpSpace);
    if (fbpIndex < maxIndex) {
      bpIndex = (uint8_t)fbpIndex;
    } else {
      bpIndex = (uint8_t)maxIndex;
    }
  }

  return bpIndex;
}

uint8_t plook_u8u16_evenckag(uint16_t u, uint16_t bp0, uint16_t bpSpace)
{
  /* Prelookup - Index only
     Index Search method: 'even'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'on'
   */
  return (uint8_t)((uint32_t)(uint16_t)((uint32_t)u - bp0) / bpSpace);
}

uint8_t plook_u8s16_evenckag(int16_t u, int16_t bp0, uint16_t bpSpace)
{
  /* Prelookup - Index only
     Index Search method: 'even'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'on'
   */
  return (uint8_t)((uint32_t)(uint16_t)(u - bp0) / bpSpace);
}

int32_t div_nzp_sus32(uint32_t numerator, int32_t denominator)
{
  uint32_t tempAbsQuotient;
  tempAbsQuotient = numerator / (denominator < 0 ? ~(uint32_t)denominator + 1U :
    (uint32_t)denominator);
  return denominator < 0 ? -/*MW:OvOk*/ ((int32_t)tempAbsQuotient) : (int32_t)
    tempAbsQuotient;
}

/* System initialize for atomic system: '<S13>/Counter' */
void Counter_Init(int16_t rtp_z_cntInit, DW_Counter *localDW)
{
  /* InitializeConditions for UnitDelay: '<S19>/UnitDelay' */
  localDW->UnitDelay_DSTATE = rtp_z_cntInit;
}

/* Output and update for atomic system: '<S13>/Counter' */
int16_t Counter(int16_t rtu_inc, int16_t rtu_max, bool rtu_rst, DW_Counter
                *localDW)
{
  int16_t rty_cnt_0;

  /* UnitDelay: '<S19>/UnitDelay' */
  rty_cnt_0 = localDW->UnitDelay_DSTATE;

  /* Switch: '<S19>/Switch1' incorporates:
   *  Constant: '<S19>/Constant23'
   */
  if (rtu_rst) {
    rty_cnt_0 = 0;
  }

  /* End of Switch: '<S19>/Switch1' */

  /* Sum: '<S16>/Sum1' */
  rty_cnt_0 += rtu_inc;

  /* MinMax: '<S16>/MinMax' */
  if (rty_cnt_0 <= rtu_max) {
    /* Update for UnitDelay: '<S19>/UnitDelay' */
    localDW->UnitDelay_DSTATE = rty_cnt_0;
  } else {
    /* Update for UnitDelay: '<S19>/UnitDelay' */
    localDW->UnitDelay_DSTATE = rtu_max;
  }

  /* End of MinMax: '<S16>/MinMax' */
  return rty_cnt_0;
}

/* System initialize for atomic system: '<S69>/Low_Pass_Filter' */
void Low_Pass_Filter_Init(DW_Low_Pass_Filter *localDW)
{
  /* InitializeConditions for UnitDelay: '<S75>/UnitDelay1' */
  localDW->UnitDelay1_DSTATE[0] = 0;
  localDW->UnitDelay1_DSTATE[1] = 0;
}

/* System reset for atomic system: '<S69>/Low_Pass_Filter' */
void Low_Pass_Filter_Reset(DW_Low_Pass_Filter *localDW)
{
  /* InitializeConditions for UnitDelay: '<S75>/UnitDelay1' */
  localDW->UnitDelay1_DSTATE[0] = 0;
  localDW->UnitDelay1_DSTATE[1] = 0;
}

/* Output and update for atomic system: '<S69>/Low_Pass_Filter' */
void Low_Pass_Filter(const int16_t rtu_u[2], uint16_t rtu_coef, int16_t rty_y[2],
                     DW_Low_Pass_Filter *localDW)
{
  int32_t rtb_Sum3_c_0;

  /* Sum: '<S75>/Sum2' incorporates:
   *  UnitDelay: '<S75>/UnitDelay1'
   */
  rtb_Sum3_c_0 = rtu_u[0] - (localDW->UnitDelay1_DSTATE[0] >> 16);
  if (rtb_Sum3_c_0 > 32767) {
    rtb_Sum3_c_0 = 32767;
  } else if (rtb_Sum3_c_0 < -32768) {
    rtb_Sum3_c_0 = -32768;
  }

  /* Sum: '<S75>/Sum3' incorporates:
   *  Product: '<S75>/Divide3'
   *  Sum: '<S75>/Sum2'
   *  UnitDelay: '<S75>/UnitDelay1'
   */
  rtb_Sum3_c_0 = rtu_coef * rtb_Sum3_c_0 + localDW->UnitDelay1_DSTATE[0];

  /* DataTypeConversion: '<S75>/Data Type Conversion' incorporates:
   *  Sum: '<S75>/Sum3'
   */
  rty_y[0] = (int16_t)(rtb_Sum3_c_0 >> 16);

  /* Update for UnitDelay: '<S75>/UnitDelay1' incorporates:
   *  Sum: '<S75>/Sum3'
   */
  localDW->UnitDelay1_DSTATE[0] = rtb_Sum3_c_0;

  /* Sum: '<S75>/Sum2' incorporates:
   *  UnitDelay: '<S75>/UnitDelay1'
   */
  rtb_Sum3_c_0 = rtu_u[1] - (localDW->UnitDelay1_DSTATE[1] >> 16);
  if (rtb_Sum3_c_0 > 32767) {
    rtb_Sum3_c_0 = 32767;
  } else if (rtb_Sum3_c_0 < -32768) {
    rtb_Sum3_c_0 = -32768;
  }

  /* Sum: '<S75>/Sum3' incorporates:
   *  Product: '<S75>/Divide3'
   *  Sum: '<S75>/Sum2'
   *  UnitDelay: '<S75>/UnitDelay1'
   */
  rtb_Sum3_c_0 = rtu_coef * rtb_Sum3_c_0 + localDW->UnitDelay1_DSTATE[1];

  /* DataTypeConversion: '<S75>/Data Type Conversion' incorporates:
   *  Sum: '<S75>/Sum3'
   */
  rty_y[1] = (int16_t)(rtb_Sum3_c_0 >> 16);

  /* Update for UnitDelay: '<S75>/UnitDelay1' incorporates:
   *  Sum: '<S75>/Sum3'
   */
  localDW->UnitDelay1_DSTATE[1] = rtb_Sum3_c_0;
}

/* System initialize for atomic system: '<S41>/Counter' */
void Counter_e_Init(uint16_t rtp_z_cntInit, DW_Counter_d *localDW)
{
  /* InitializeConditions for UnitDelay: '<S46>/UnitDelay' */
  localDW->UnitDelay_DSTATE = rtp_z_cntInit;
}

/* Output and update for atomic system: '<S41>/Counter' */
uint16_t Counter_e(uint16_t rtu_inc, uint16_t rtu_max, bool rtu_rst,
                   DW_Counter_d *localDW)
{
  uint16_t rty_cnt_0;
  uint16_t tmp;

  /* Switch: '<S46>/Switch1' incorporates:
   *  Constant: '<S46>/Constant23'
   *  UnitDelay: '<S46>/UnitDelay'
   */
  if (rtu_rst) {
    tmp = 0U;
  } else {
    tmp = localDW->UnitDelay_DSTATE;
  }

  /* Sum: '<S45>/Sum1' incorporates:
   *  Switch: '<S46>/Switch1'
   */
  rty_cnt_0 = (uint16_t)(rtu_inc + tmp);

  /* MinMax: '<S45>/MinMax' */
  if (rty_cnt_0 <= rtu_max) {
    /* Update for UnitDelay: '<S46>/UnitDelay' */
    localDW->UnitDelay_DSTATE = rty_cnt_0;
  } else {
    /* Update for UnitDelay: '<S46>/UnitDelay' */
    localDW->UnitDelay_DSTATE = rtu_max;
  }

  /* End of MinMax: '<S45>/MinMax' */
  return rty_cnt_0;
}

/* System initialize for atomic system: '<S40>/Counter' */
void Counter_b_Init(uint32_t rtp_z_cntInit, DW_Counter_e *localDW)
{
  /* InitializeConditions for UnitDelay: '<S44>/UnitDelay' */
  localDW->UnitDelay_DSTATE = rtp_z_cntInit;
}

/* Output and update for atomic system: '<S40>/Counter' */
void Counter_n(uint16_t rtu_inc, uint32_t rtu_max, bool rtu_rst, uint16_t
               *rty_cnt, DW_Counter_e *localDW)
{
  uint16_t tmp;

  /* Switch: '<S44>/Switch1' incorporates:
   *  Constant: '<S44>/Constant23'
   *  UnitDelay: '<S44>/UnitDelay'
   */
  if (rtu_rst) {
    tmp = 0U;
  } else {
    tmp = (uint16_t)localDW->UnitDelay_DSTATE;
  }

  /* Sum: '<S43>/Sum1' incorporates:
   *  Switch: '<S44>/Switch1'
   */
  *rty_cnt = (uint16_t)(rtu_inc + tmp);

  /* MinMax: '<S43>/MinMax' */
  if (*rty_cnt <= rtu_max) {
    /* Update for UnitDelay: '<S44>/UnitDelay' */
    localDW->UnitDelay_DSTATE = *rty_cnt;
  } else {
    /* Update for UnitDelay: '<S44>/UnitDelay' */
    localDW->UnitDelay_DSTATE = rtu_max;
  }

  /* End of MinMax: '<S43>/MinMax' */
}

/*
 * Output and update for atomic system:
 *    '<S37>/either_edge'
 *    '<S36>/either_edge'
 */
bool either_edge(bool rtu_u, DW_either_edge *localDW)
{
  bool rty_y_0;

  /* RelationalOperator: '<S42>/Relational Operator' incorporates:
   *  UnitDelay: '<S42>/UnitDelay'
   */
  rty_y_0 = (rtu_u != localDW->UnitDelay_DSTATE);

  /* Update for UnitDelay: '<S42>/UnitDelay' */
  localDW->UnitDelay_DSTATE = rtu_u;
  return rty_y_0;
}

/* System initialize for atomic system: '<S36>/Debounce_Filter' */
void Debounce_Filter_Init(bool *rty_y, DW_Debounce_Filter *localDW)
{
  /* SystemInitialize for IfAction SubSystem: '<S37>/Qualification' */
  /* SystemInitialize for Atomic SubSystem: '<S41>/Counter' */
  Counter_e_Init(0, &localDW->Counter_e2);

  /* End of SystemInitialize for SubSystem: '<S41>/Counter' */
  /* End of SystemInitialize for SubSystem: '<S37>/Qualification' */

  /* SystemInitialize for IfAction SubSystem: '<S37>/Dequalification' */
  /* SystemInitialize for Atomic SubSystem: '<S40>/Counter' */
  Counter_b_Init(0U, &localDW->Counter_ni);

  /* End of SystemInitialize for SubSystem: '<S40>/Counter' */
  /* End of SystemInitialize for SubSystem: '<S37>/Dequalification' */

  /* SystemInitialize for Merge: '<S37>/Merge' */
  *rty_y = false;
}

/* Output and update for atomic system: '<S36>/Debounce_Filter' */
void Debounce_Filter(bool rtu_u, uint16_t rtu_tAcv, uint32_t rtu_tDeacv, bool
                     *rty_y, DW_Debounce_Filter *localDW)
{
  uint16_t rtb_Sum1_i0;
  bool rtb_RelationalOperator_o;

  /* Outputs for Atomic SubSystem: '<S37>/either_edge' */
  rtb_RelationalOperator_o = either_edge(rtu_u, &localDW->either_edge_n);

  /* End of Outputs for SubSystem: '<S37>/either_edge' */

  /* If: '<S37>/If2' incorporates:
   *  Constant: '<S40>/Constant6'
   *  Constant: '<S41>/Constant6'
   *  Logic: '<S37>/Logical Operator1'
   *  Logic: '<S37>/Logical Operator2'
   *  Logic: '<S37>/Logical Operator3'
   *  Logic: '<S37>/Logical Operator4'
   *  UnitDelay: '<S37>/UnitDelay'
   */
  if (rtu_u && (!localDW->UnitDelay_DSTATE)) {
    /* Outputs for IfAction SubSystem: '<S37>/Qualification' incorporates:
     *  ActionPort: '<S41>/Action Port'
     */
    /* Outputs for Atomic SubSystem: '<S41>/Counter' */
    rtb_Sum1_i0 = Counter_e(1, rtu_tAcv, rtb_RelationalOperator_o,
      &localDW->Counter_e2);

    /* End of Outputs for SubSystem: '<S41>/Counter' */

    /* Switch: '<S41>/Switch2' incorporates:
     *  Constant: '<S41>/Constant6'
     *  RelationalOperator: '<S41>/Relational Operator2'
     */
    *rty_y = (rtb_Sum1_i0 > rtu_tAcv);

    /* End of Outputs for SubSystem: '<S37>/Qualification' */
  } else if ((!rtu_u) && localDW->UnitDelay_DSTATE) {
    /* Outputs for IfAction SubSystem: '<S37>/Dequalification' incorporates:
     *  ActionPort: '<S40>/Action Port'
     */
    /* Outputs for Atomic SubSystem: '<S40>/Counter' */
    Counter_n(1, rtu_tDeacv, rtb_RelationalOperator_o, &rtb_Sum1_i0,
              &localDW->Counter_ni);

    /* End of Outputs for SubSystem: '<S40>/Counter' */

    /* Switch: '<S40>/Switch2' incorporates:
     *  Constant: '<S40>/Constant6'
     *  RelationalOperator: '<S40>/Relational Operator2'
     */
    *rty_y = (rtb_Sum1_i0 <= rtu_tDeacv);

    /* End of Outputs for SubSystem: '<S37>/Dequalification' */
  } else {
    /* Outputs for IfAction SubSystem: '<S37>/Default' incorporates:
     *  ActionPort: '<S39>/Action Port'
     */
    /* SignalConversion generated from: '<S39>/yPrev' */
    *rty_y = localDW->UnitDelay_DSTATE;

    /* End of Outputs for SubSystem: '<S37>/Default' */
  }

  /* End of If: '<S37>/If2' */

  /* Update for UnitDelay: '<S37>/UnitDelay' */
  localDW->UnitDelay_DSTATE = *rty_y;
}

/* Function for Chart: '<S5>/F03_02_Control_Mode_Manager' */
void enter_internal_ACTIVE(const bool *LogicalOperator1, const bool
  *LogicalOperator2, uint8_t *rty_z_ctrlMod, DW_F03_Control_Mode_Manager
  *localDW)
{
  if (*LogicalOperator2) {
    localDW->is_ACTIVE = IN_TORQUE_MODE;

    /* Chart: '<S5>/F03_02_Control_Mode_Manager' */
    *rty_z_ctrlMod = TRQ_MODE;
  } else if (*LogicalOperator1) {
    localDW->is_ACTIVE = IN_SPEED_MODE;

    /* Chart: '<S5>/F03_02_Control_Mode_Manager' */
    *rty_z_ctrlMod = SPD_MODE;
  } else {
    localDW->is_ACTIVE = IN_VOLTAGE_MODE;

    /* Chart: '<S5>/F03_02_Control_Mode_Manager' */
    *rty_z_ctrlMod = VLT_MODE;
  }
}

/* System initialize for function-call system: '<S1>/F03_Control_Mode_Manager' */
void F03_Control_Mode_Manager_Init(uint8_t *rty_z_ctrlMod, int16_t
  *rty_r_inpTgtSca, DW_F03_Control_Mode_Manager *localDW)
{
  /* Start for If: '<S49>/If2' */
  localDW->If2_ActiveSubsystem = -1;

  /* SystemInitialize for Chart: '<S5>/F03_02_Control_Mode_Manager' */
  *rty_z_ctrlMod = 0U;
  localDW->is_active_c2_BLDC_controller = 0U;
  localDW->is_c2_BLDC_controller = IN_NO_ACTIVE_CHILD;
  localDW->is_ACTIVE = IN_NO_ACTIVE_CHILD;

  /* SystemInitialize for IfAction SubSystem: '<S49>/Open_Mode' */
  /* SystemInitialize for Atomic SubSystem: '<S53>/rising_edge_init' */
  /* InitializeConditions for UnitDelay: '<S58>/UnitDelay' */
  localDW->UnitDelay_DSTATE_c = true;

  /* End of SystemInitialize for SubSystem: '<S53>/rising_edge_init' */

  /* SystemInitialize for Atomic SubSystem: '<S53>/Rate_Limiter' */
  /* InitializeConditions for UnitDelay: '<S59>/UnitDelay' */
  localDW->UnitDelay_DSTATE = 0;

  /* End of SystemInitialize for SubSystem: '<S53>/Rate_Limiter' */
  /* End of SystemInitialize for SubSystem: '<S49>/Open_Mode' */

  /* SystemInitialize for Merge: '<S49>/Merge1' */
  *rty_r_inpTgtSca = 0;
}

/* Output and update for function-call system: '<S1>/F03_Control_Mode_Manager' */
void F03_Control_Mode_Manager(bool rtu_b_errFlag, bool rtu_b_motEna, bool
  rtu_b_cruiseCtrlEna, uint8_t rtu_z_ctrlTypSel, uint8_t rtu_z_ctrlModReq,
  int16_t rtu_r_inpTgt, int16_t rtu_VqFinPrev, uint8_t *rty_z_ctrlMod, int16_t
  *rty_r_inpTgtScaAbs, int16_t *rty_r_inpTgtSca, int16_t *rty_r_inpTgtConv,
  const P *rtP, DW_F03_Control_Mode_Manager *localDW)
{
  int32_t rtb_DataTypeConversion;
  int32_t rtb_Gain3;
  int32_t rtb_Sum1_ae;
  int32_t rtb_Switch1_c;
  int16_t tmp[4];
  int16_t rtb_VariantMergeForOutportVd__g;
  int8_t rtAction;
  int8_t rtPrevAction;
  bool LogicalOperator1;
  bool LogicalOperator2;
  bool LogicalOperator4;

  /* Logic: '<S47>/Logical Operator4' incorporates:
   *  Constant: '<S47>/constant8'
   *  Logic: '<S47>/Logical Operator7'
   *  RelationalOperator: '<S47>/Relational Operator10'
   */
  LogicalOperator4 = (rtu_b_errFlag || (!rtu_b_motEna) || (rtu_z_ctrlModReq == 0));

  /* Logic: '<S47>/Logical Operator1' incorporates:
   *  Constant: '<S47>/constant1'
   *  RelationalOperator: '<S47>/Relational Operator1'
   */
  LogicalOperator1 = ((rtu_z_ctrlModReq == 2) || rtu_b_cruiseCtrlEna);

  /* Logic: '<S47>/Logical Operator2' incorporates:
   *  Constant: '<S47>/constant'
   *  Logic: '<S47>/Logical Operator5'
   *  RelationalOperator: '<S47>/Relational Operator4'
   */
  LogicalOperator2 = ((rtu_z_ctrlModReq == 3) && (!rtu_b_cruiseCtrlEna));

  /* Chart: '<S5>/F03_02_Control_Mode_Manager' incorporates:
   *  Constant: '<S47>/constant5'
   *  Logic: '<S47>/Logical Operator3'
   *  Logic: '<S47>/Logical Operator6'
   *  Logic: '<S47>/Logical Operator9'
   *  RelationalOperator: '<S47>/Relational Operator5'
   */
  if (localDW->is_active_c2_BLDC_controller == 0) {
    localDW->is_active_c2_BLDC_controller = 1U;
    localDW->is_c2_BLDC_controller = IN_OPEN;
    *rty_z_ctrlMod = OPEN_MODE;
  } else if (localDW->is_c2_BLDC_controller == IN_ACTIVE) {
    if (LogicalOperator4) {
      localDW->is_ACTIVE = IN_NO_ACTIVE_CHILD;
      localDW->is_c2_BLDC_controller = IN_OPEN;
      *rty_z_ctrlMod = OPEN_MODE;
    } else {
      switch (localDW->is_ACTIVE) {
       case IN_SPEED_MODE:
        *rty_z_ctrlMod = SPD_MODE;
        if (!LogicalOperator1) {
          enter_internal_ACTIVE(&LogicalOperator1, &LogicalOperator2,
                                rty_z_ctrlMod, localDW);
        }
        break;

       case IN_TORQUE_MODE:
        *rty_z_ctrlMod = TRQ_MODE;
        if (!LogicalOperator2) {
          enter_internal_ACTIVE(&LogicalOperator1, &LogicalOperator2,
                                rty_z_ctrlMod, localDW);
        }
        break;

       default:
        /* case IN_VOLTAGE_MODE: */
        *rty_z_ctrlMod = VLT_MODE;
        if (LogicalOperator2 || LogicalOperator1) {
          enter_internal_ACTIVE(&LogicalOperator1, &LogicalOperator2,
                                rty_z_ctrlMod, localDW);
        }
        break;
      }
    }
  } else {
    /* case IN_OPEN: */
    *rty_z_ctrlMod = OPEN_MODE;
    if ((!LogicalOperator4) && ((rtu_z_ctrlModReq == 1) || LogicalOperator1 ||
         LogicalOperator2)) {
      localDW->is_c2_BLDC_controller = IN_ACTIVE;
      enter_internal_ACTIVE(&LogicalOperator1, &LogicalOperator2, rty_z_ctrlMod,
                            localDW);
    }
  }

  /* End of Chart: '<S5>/F03_02_Control_Mode_Manager' */

  /* DataTypeConversion: '<S49>/Data Type Conversion22' */
  *rty_r_inpTgtConv = rtu_r_inpTgt;

  /* If: '<S49>/If1' incorporates:
   *  Constant: '<S55>/Vd_max1'
   *  Constant: '<S56>/Vd_max3'
   *  VariantMerge generated from: '<S54>/Vd_max_margin'
   */
  if (rtu_z_ctrlTypSel == 2) {
    /* Outputs for IfAction SubSystem: '<S49>/FOC_Control_Type' incorporates:
     *  ActionPort: '<S52>/Action Port'
     */
    /* Outputs for Atomic SubSystem: '<S52>/Variant Subsystem1' */
#if mcu_model == 1

    /* Outputs for Atomic SubSystem: '<S54>/GD32F103' */
    rtb_VariantMergeForOutportVd__g = 26032;

    /* End of Outputs for SubSystem: '<S54>/GD32F103' */
#elif mcu_model == 0

    /* Outputs for Atomic SubSystem: '<S54>/STM32F103' */
    rtb_VariantMergeForOutportVd__g = 15040;

    /* End of Outputs for SubSystem: '<S54>/STM32F103' */
#endif

    /* End of Outputs for SubSystem: '<S52>/Variant Subsystem1' */

    /* SignalConversion generated from: '<S52>/Selector' incorporates:
     *  Constant: '<S52>/constant1'
     *  Constant: '<S52>/i_max'
     *  Constant: '<S52>/n_max'
     *  Constant: '<S55>/Vd_max1'
     *  Constant: '<S56>/Vd_max3'
     *  VariantMerge generated from: '<S54>/Vd_max_margin'
     */
    tmp[0] = 0;
    tmp[1] = rtb_VariantMergeForOutportVd__g;
    tmp[2] = rtP->n_max;
    tmp[3] = rtP->i_max;

    /* Product: '<S52>/Divide1' incorporates:
     *  Product: '<S52>/Divide4'
     *  Selector: '<S52>/Selector'
     */
    *rty_r_inpTgtSca = (int16_t)(((uint16_t)((tmp[rtu_z_ctrlModReq] << 10) /
      2047) * *rty_r_inpTgtConv) >> 14);

    /* End of Outputs for SubSystem: '<S49>/FOC_Control_Type' */
  } else {
    /* Outputs for IfAction SubSystem: '<S49>/Default_Control_Type' incorporates:
     *  ActionPort: '<S50>/Action Port'
     */
    /* SignalConversion generated from: '<S50>/r_inpTgtConv' */
    *rty_r_inpTgtSca = *rty_r_inpTgtConv;

    /* End of Outputs for SubSystem: '<S49>/Default_Control_Type' */
  }

  /* End of If: '<S49>/If1' */

  /* If: '<S49>/If2' */
  rtPrevAction = localDW->If2_ActiveSubsystem;
  rtAction = (int8_t)(*rty_z_ctrlMod != 0);
  localDW->If2_ActiveSubsystem = rtAction;
  if (rtAction == 0) {
    if (rtPrevAction != 0) {
      /* SystemReset for IfAction SubSystem: '<S49>/Open_Mode' incorporates:
       *  ActionPort: '<S53>/Action Port'
       */
      /* SystemReset for Atomic SubSystem: '<S53>/rising_edge_init' */
      /* SystemReset for If: '<S49>/If2' incorporates:
       *  UnitDelay: '<S58>/UnitDelay'
       *  UnitDelay: '<S59>/UnitDelay'
       */
      localDW->UnitDelay_DSTATE_c = true;

      /* End of SystemReset for SubSystem: '<S53>/rising_edge_init' */

      /* SystemReset for Atomic SubSystem: '<S53>/Rate_Limiter' */
      localDW->UnitDelay_DSTATE = 0;

      /* End of SystemReset for SubSystem: '<S53>/Rate_Limiter' */
      /* End of SystemReset for SubSystem: '<S49>/Open_Mode' */
    }

    /* Outputs for IfAction SubSystem: '<S49>/Open_Mode' incorporates:
     *  ActionPort: '<S53>/Action Port'
     */
    /* DataTypeConversion: '<S53>/Data Type Conversion' */
    rtb_DataTypeConversion = rtu_VqFinPrev << 12;

    /* Outputs for Atomic SubSystem: '<S53>/rising_edge_init' */
    /* UnitDelay: '<S58>/UnitDelay' */
    LogicalOperator4 = localDW->UnitDelay_DSTATE_c;

    /* Update for UnitDelay: '<S58>/UnitDelay' incorporates:
     *  Constant: '<S58>/Constant'
     */
    localDW->UnitDelay_DSTATE_c = false;

    /* End of Outputs for SubSystem: '<S53>/rising_edge_init' */

    /* Outputs for Atomic SubSystem: '<S53>/Rate_Limiter' */
    /* Switch: '<S59>/Switch1' incorporates:
     *  DataTypeConversion: '<S53>/Data Type Conversion'
     *  UnitDelay: '<S59>/UnitDelay'
     */
    if (LogicalOperator4) {
      rtb_Switch1_c = rtb_DataTypeConversion;
    } else {
      rtb_Switch1_c = localDW->UnitDelay_DSTATE;
    }

    /* End of Switch: '<S59>/Switch1' */

    /* Sum: '<S57>/Sum1' incorporates:
     *  Switch: '<S59>/Switch1'
     */
    rtb_Sum1_ae = ((uint32_t)-rtb_Switch1_c & 134217728U) != 0U ? -rtb_Switch1_c
      | -134217728 : (int32_t)((uint32_t)-rtb_Switch1_c & 134217727U);

    /* Switch: '<S60>/Switch2' incorporates:
     *  Constant: '<S53>/dV_openRate'
     *  RelationalOperator: '<S60>/LowerRelop1'
     *  Sum: '<S57>/Sum1'
     */
    if (rtb_Sum1_ae > rtP->dV_openRate) {
      rtb_Sum1_ae = rtP->dV_openRate;
    } else {
      /* Gain: '<S53>/Gain3' */
      rtb_Gain3 = ((uint32_t)-rtP->dV_openRate & 134217728U) != 0U ?
        -rtP->dV_openRate | -134217728 : (int32_t)((uint32_t)-rtP->dV_openRate &
        134217727U);

      /* Switch: '<S60>/Switch' incorporates:
       *  Gain: '<S53>/Gain3'
       *  RelationalOperator: '<S60>/UpperRelop'
       *  Switch: '<S60>/Switch2'
       */
      if (rtb_Sum1_ae < rtb_Gain3) {
        rtb_Sum1_ae = rtb_Gain3;
      }

      /* End of Switch: '<S60>/Switch' */
    }

    /* End of Switch: '<S60>/Switch2' */

    /* Sum: '<S57>/Sum2' incorporates:
     *  Switch: '<S59>/Switch1'
     *  Switch: '<S60>/Switch2'
     */
    rtb_Switch1_c += rtb_Sum1_ae;
    rtb_Switch1_c = ((uint32_t)rtb_Switch1_c & 134217728U) != 0U ? rtb_Switch1_c
      | -134217728 : (int32_t)((uint32_t)rtb_Switch1_c & 134217727U);

    /* Switch: '<S59>/Switch2' */
    if (LogicalOperator4) {
      /* Update for UnitDelay: '<S59>/UnitDelay' incorporates:
       *  DataTypeConversion: '<S53>/Data Type Conversion'
       */
      localDW->UnitDelay_DSTATE = rtb_DataTypeConversion;
    } else {
      /* Update for UnitDelay: '<S59>/UnitDelay' incorporates:
       *  Sum: '<S57>/Sum2'
       */
      localDW->UnitDelay_DSTATE = rtb_Switch1_c;
    }

    /* End of Switch: '<S59>/Switch2' */
    /* End of Outputs for SubSystem: '<S53>/Rate_Limiter' */

    /* DataTypeConversion: '<S53>/Data Type Conversion1' incorporates:
     *  Sum: '<S57>/Sum2'
     */
    *rty_r_inpTgtSca = (int16_t)(rtb_Switch1_c >> 12);

    /* End of Outputs for SubSystem: '<S49>/Open_Mode' */
  }

  /* End of If: '<S49>/If2' */

  /* Abs: '<S5>/Abs1' */
  if (*rty_r_inpTgtSca < 0) {
    *rty_r_inpTgtScaAbs = (int16_t)-*rty_r_inpTgtSca;
  } else {
    *rty_r_inpTgtScaAbs = *rty_r_inpTgtSca;
  }

  /* End of Abs: '<S5>/Abs1' */
}

/*
 * System initialize for atomic system:
 *    '<S238>/I_backCalc_fixdt'
 *    '<S238>/I_backCalc_fixdt1'
 */
void I_backCalc_fixdt_Init(int32_t rtp_yInit, DW_I_backCalc_fixdt *localDW)
{
  /* InitializeConditions for UnitDelay: '<S301>/UnitDelay' */
  localDW->UnitDelay_DSTATE_a = rtp_yInit;
}

/*
 * Output and update for atomic system:
 *    '<S238>/I_backCalc_fixdt'
 *    '<S238>/I_backCalc_fixdt1'
 */
void I_backCalc_fixdt(int16_t rtu_err, uint16_t rtu_I, uint16_t rtu_Kb, int16_t
                      rtu_satMax, int16_t rtu_satMin, int16_t *rty_out,
                      DW_I_backCalc_fixdt *localDW)
{
  int64_t tmp;
  int32_t rtb_Sum1_gm;
  int16_t rtb_DataTypeConversion1_l;

  /* Sum: '<S299>/Sum2' incorporates:
   *  Product: '<S299>/Divide2'
   *  UnitDelay: '<S299>/UnitDelay'
   */
  tmp = (rtu_err * rtu_I + ((int64_t)localDW->UnitDelay_DSTATE << 4)) >> 4;
  if (tmp > 2147483647LL) {
    tmp = 2147483647LL;
  } else if (tmp < -2147483648LL) {
    tmp = -2147483648LL;
  }

  /* Sum: '<S301>/Sum1' incorporates:
   *  Sum: '<S299>/Sum2'
   *  UnitDelay: '<S301>/UnitDelay'
   */
  rtb_Sum1_gm = (int32_t)tmp + localDW->UnitDelay_DSTATE_a;

  /* DataTypeConversion: '<S301>/Data Type Conversion1' incorporates:
   *  Sum: '<S301>/Sum1'
   */
  rtb_DataTypeConversion1_l = (int16_t)(rtb_Sum1_gm >> 12);

  /* Switch: '<S302>/Switch2' incorporates:
   *  DataTypeConversion: '<S301>/Data Type Conversion1'
   *  RelationalOperator: '<S302>/LowerRelop1'
   *  RelationalOperator: '<S302>/UpperRelop'
   *  Switch: '<S302>/Switch'
   */
  if (rtb_DataTypeConversion1_l > rtu_satMax) {
    *rty_out = rtu_satMax;
  } else if (rtb_DataTypeConversion1_l < rtu_satMin) {
    /* Switch: '<S302>/Switch' */
    *rty_out = rtu_satMin;
  } else {
    *rty_out = rtb_DataTypeConversion1_l;
  }

  /* End of Switch: '<S302>/Switch2' */

  /* Update for UnitDelay: '<S299>/UnitDelay' incorporates:
   *  DataTypeConversion: '<S301>/Data Type Conversion1'
   *  Product: '<S299>/Divide1'
   *  Sum: '<S299>/Sum3'
   */
  localDW->UnitDelay_DSTATE = (int16_t)(*rty_out - rtb_DataTypeConversion1_l) *
    rtu_Kb;

  /* Update for UnitDelay: '<S301>/UnitDelay' incorporates:
   *  Sum: '<S301>/Sum1'
   */
  localDW->UnitDelay_DSTATE_a = rtb_Sum1_gm;
}

/* System initialize for atomic system: '<S164>/PI_clamp_fixdtFF' */
#if FeedForwardEnable == 1

void PI_clamp_fixdtFF_Init(DW_PI_clamp_fixdtFF *localDW)
{
  /* InitializeConditions for UnitDelay: '<S223>/UnitDelay1' */
  localDW->UnitDelay1_DSTATE = false;

  /* InitializeConditions for Delay: '<S225>/Resettable Delay' */
  localDW->icLoad = true;
}

#endif

/* System reset for atomic system: '<S164>/PI_clamp_fixdtFF' */
#if FeedForwardEnable == 1

void PI_clamp_fixdtFF_Reset(DW_PI_clamp_fixdtFF *localDW)
{
  /* InitializeConditions for UnitDelay: '<S223>/UnitDelay1' */
  localDW->UnitDelay1_DSTATE = false;

  /* InitializeConditions for Delay: '<S225>/Resettable Delay' */
  localDW->icLoad = true;
}

#endif

/* Output and update for atomic system: '<S164>/PI_clamp_fixdtFF' */
#if FeedForwardEnable == 1

int16_t PI_clamp_fixdtFF(int16_t rtu_err, uint16_t rtu_P, uint16_t rtu_I,
  int16_t rtu_init, int16_t rtu_satMax, int16_t rtu_satMin, int16_t rtu_tgtInput,
  int16_t rtu_FFGain, DW_PI_clamp_fixdtFF *localDW)
{
  int16_t rty_out_0;
  int32_t rtb_SignDeltaU2_c;
  int32_t rtb_Sum1_l;
  int32_t tmp;
  int32_t tmp_0;
  int16_t tmp_1;
  bool rtb_LowerRelop1_bq;
  bool rtb_UpperRelop_gv;

  /* Product: '<S223>/Divide2' incorporates:
   *  Signum: '<S224>/SignDeltaU2'
   */
  rtb_SignDeltaU2_c = rtu_err * rtu_I;

  /* Delay: '<S225>/Resettable Delay' */
  if (localDW->icLoad) {
    localDW->ResettableDelay_DSTATE = rtu_init << 14;
  }

  /* Switch: '<S223>/Switch1' incorporates:
   *  Constant: '<S223>/Constant'
   *  Signum: '<S224>/SignDeltaU2'
   *  UnitDelay: '<S223>/UnitDelay1'
   */
  if (localDW->UnitDelay1_DSTATE) {
    tmp_0 = 0;
  } else {
    tmp_0 = rtb_SignDeltaU2_c;
  }

  /* Sum: '<S225>/Sum1' incorporates:
   *  Delay: '<S225>/Resettable Delay'
   *  Switch: '<S223>/Switch1'
   */
  rtb_Sum1_l = tmp_0 + localDW->ResettableDelay_DSTATE;

  /* Product: '<S223>/Product' */
  tmp_0 = (rtu_FFGain * rtu_tgtInput) >> 12;
  if (tmp_0 > 32767) {
    tmp_0 = 32767;
  } else if (tmp_0 < -32768) {
    tmp_0 = -32768;
  }

  /* Product: '<S223>/Divide5' */
  tmp = (rtu_err * rtu_P) >> 7;
  if (tmp > 32767) {
    tmp = 32767;
  } else if (tmp < -32768) {
    tmp = -32768;
  }

  /* Sum: '<S223>/Sum1' incorporates:
   *  DataTypeConversion: '<S225>/Data Type Conversion1'
   *  Product: '<S223>/Divide5'
   *  Product: '<S223>/Product'
   *  Sum: '<S225>/Sum1'
   */
  tmp_0 = (((tmp_0 << 1) + tmp) + ((int16_t)(rtb_Sum1_l >> 14) << 1)) >> 1;
  if (tmp_0 > 32767) {
    tmp_0 = 32767;
  } else if (tmp_0 < -32768) {
    tmp_0 = -32768;
  }

  /* RelationalOperator: '<S226>/LowerRelop1' incorporates:
   *  Sum: '<S223>/Sum1'
   */
  rtb_LowerRelop1_bq = (tmp_0 > rtu_satMax);

  /* RelationalOperator: '<S226>/UpperRelop' incorporates:
   *  Sum: '<S223>/Sum1'
   */
  rtb_UpperRelop_gv = (tmp_0 < rtu_satMin);

  /* Switch: '<S226>/Switch1' incorporates:
   *  Sum: '<S223>/Sum1'
   *  Switch: '<S226>/Switch3'
   */
  if (rtb_LowerRelop1_bq) {
    rty_out_0 = rtu_satMax;
  } else if (rtb_UpperRelop_gv) {
    /* Switch: '<S226>/Switch3' */
    rty_out_0 = rtu_satMin;
  } else {
    rty_out_0 = (int16_t)tmp_0;
  }

  /* End of Switch: '<S226>/Switch1' */

  /* Signum: '<S224>/SignDeltaU2' */
  if (rtb_SignDeltaU2_c < 0) {
    tmp = -1;
  } else {
    tmp = (rtb_SignDeltaU2_c > 0);
  }

  /* Signum: '<S224>/SignDeltaU3' incorporates:
   *  Sum: '<S223>/Sum1'
   */
  if (tmp_0 < 0) {
    tmp_1 = -1;
  } else {
    tmp_1 = (int16_t)(tmp_0 > 0);
  }

  /* Update for UnitDelay: '<S223>/UnitDelay1' incorporates:
   *  Logic: '<S223>/AND1'
   *  Logic: '<S224>/AND1'
   *  RelationalOperator: '<S224>/Equal1'
   *  Signum: '<S224>/SignDeltaU2'
   *  Signum: '<S224>/SignDeltaU3'
   */
  localDW->UnitDelay1_DSTATE = ((tmp == tmp_1) && (rtb_LowerRelop1_bq ||
    rtb_UpperRelop_gv));

  /* Update for Delay: '<S225>/Resettable Delay' incorporates:
   *  Sum: '<S225>/Sum1'
   */
  localDW->icLoad = false;
  localDW->ResettableDelay_DSTATE = rtb_Sum1_l;
  return rty_out_0;
}

#endif

/*
 * System initialize for atomic system:
 *    '<S161>/TMClassic'
 *    '<S92>/TMClassic'
 */
#if FeedForwardEnable == 2

void TMClassic_Init(DW_TMClassic *localDW)
{
  /* InitializeConditions for UnitDelay: '<S165>/UnitDelay1' */
  localDW->UnitDelay1_DSTATE = false;

  /* InitializeConditions for Delay: '<S228>/Resettable Delay' */
  localDW->icLoad = true;
}

#endif

/*
 * System reset for atomic system:
 *    '<S161>/TMClassic'
 *    '<S92>/TMClassic'
 */
#if FeedForwardEnable == 2

void TMClassic_Reset(DW_TMClassic *localDW)
{
  /* InitializeConditions for UnitDelay: '<S165>/UnitDelay1' */
  localDW->UnitDelay1_DSTATE = false;

  /* InitializeConditions for Delay: '<S228>/Resettable Delay' */
  localDW->icLoad = true;
}

#endif

/*
 * Output and update for atomic system:
 *    '<S161>/TMClassic'
 *    '<S92>/TMClassic'
 */
#if FeedForwardEnable == 2

int16_t TMClassic(int16_t rtu_err, uint16_t rtu_P, uint16_t rtu_I, int16_t
                  rtu_init, int16_t rtu_satMax, int16_t rtu_satMin, DW_TMClassic
                  *localDW)
{
  int16_t rty_Vq_Trq_0;
  int32_t rtb_Sum1_b;
  int32_t tmp;
  int16_t tmp_0;
  int16_t tmp_1;
  bool rtb_LowerRelop1_g3;
  bool rtb_UpperRelop_c;

  /* Product: '<S165>/Divide5' */
  tmp = (rtu_err * rtu_P) >> 8;
  if (tmp > 32767) {
    tmp = 32767;
  } else if (tmp < -32768) {
    tmp = -32768;
  }

  /* Delay: '<S228>/Resettable Delay' */
  if (localDW->icLoad) {
    localDW->ResettableDelay_DSTATE = rtu_init << 14;
  }

  /* Switch: '<S165>/Switch1' incorporates:
   *  Constant: '<S165>/Constant'
   *  Product: '<S165>/Divide5'
   *  UnitDelay: '<S165>/UnitDelay1'
   */
  if (localDW->UnitDelay1_DSTATE) {
    tmp_0 = 0;
  } else {
    tmp_0 = (int16_t)tmp;
  }

  /* Sum: '<S228>/Sum1' incorporates:
   *  Delay: '<S228>/Resettable Delay'
   *  Product: '<S165>/Product'
   *  Switch: '<S165>/Switch1'
   */
  rtb_Sum1_b = tmp_0 * rtu_I + localDW->ResettableDelay_DSTATE;

  /* Sum: '<S165>/Sum1' incorporates:
   *  DataTypeConversion: '<S228>/Data Type Conversion1'
   *  Product: '<S165>/Divide5'
   *  Sum: '<S228>/Sum1'
   */
  tmp += (int16_t)(rtb_Sum1_b >> 14);
  if (tmp > 32767) {
    tmp = 32767;
  } else if (tmp < -32768) {
    tmp = -32768;
  }

  /* RelationalOperator: '<S229>/LowerRelop1' incorporates:
   *  Sum: '<S165>/Sum1'
   */
  rtb_LowerRelop1_g3 = (tmp > rtu_satMax);

  /* RelationalOperator: '<S229>/UpperRelop' incorporates:
   *  Sum: '<S165>/Sum1'
   */
  rtb_UpperRelop_c = (tmp < rtu_satMin);

  /* Switch: '<S229>/Switch1' incorporates:
   *  Sum: '<S165>/Sum1'
   *  Switch: '<S229>/Switch3'
   */
  if (rtb_LowerRelop1_g3) {
    rty_Vq_Trq_0 = rtu_satMax;
  } else if (rtb_UpperRelop_c) {
    /* Switch: '<S229>/Switch3' */
    rty_Vq_Trq_0 = rtu_satMin;
  } else {
    rty_Vq_Trq_0 = (int16_t)tmp;
  }

  /* End of Switch: '<S229>/Switch1' */

  /* Signum: '<S227>/SignDeltaU2' */
  if (rtu_err < 0) {
    tmp_0 = -1;
  } else {
    tmp_0 = (int16_t)(rtu_err > 0);
  }

  /* Signum: '<S227>/SignDeltaU3' incorporates:
   *  Sum: '<S165>/Sum1'
   */
  if (tmp < 0) {
    tmp_1 = -1;
  } else {
    tmp_1 = (int16_t)(tmp > 0);
  }

  /* Update for UnitDelay: '<S165>/UnitDelay1' incorporates:
   *  Logic: '<S165>/AND1'
   *  Logic: '<S227>/AND1'
   *  RelationalOperator: '<S227>/Equal1'
   *  Signum: '<S227>/SignDeltaU2'
   *  Signum: '<S227>/SignDeltaU3'
   */
  localDW->UnitDelay1_DSTATE = ((tmp_0 == tmp_1) && (rtb_LowerRelop1_g3 ||
    rtb_UpperRelop_c));

  /* Update for Delay: '<S228>/Resettable Delay' incorporates:
   *  Sum: '<S228>/Sum1'
   */
  localDW->icLoad = false;
  localDW->ResettableDelay_DSTATE = rtb_Sum1_b;
  return rty_Vq_Trq_0;
}

#endif

/* System initialize for atomic system: '<S84>/PI_clamp_fixdt' */
void PI_clamp_fixdt_Init(DW_PI_clamp_fixdt *localDW)
{
  /* InitializeConditions for Delay: '<S90>/Resettable Delay' */
  localDW->icLoad = true;
}

/* Output and update for atomic system: '<S84>/PI_clamp_fixdt' */
int16_t PI_clamp_fixdt(int16_t rtu_err, uint16_t rtu_P, uint16_t rtu_I, int16_t
  rtu_init, int16_t rtu_satMax, int16_t rtu_satMin, int32_t rtu_ext_limProt,
  DW_PI_clamp_fixdt *localDW)
{
  int16_t rty_out_0;
  int64_t tmp;
  int32_t rtb_Sum1_p;
  int32_t tmp_0;
  int32_t tmp_1;
  int16_t tmp_2;
  bool rtb_LowerRelop1_lv;
  bool rtb_UpperRelop_mn;

  /* Sum: '<S88>/Sum2' incorporates:
   *  Product: '<S88>/Divide2'
   */
  tmp = (int64_t)(rtu_err * rtu_I) + rtu_ext_limProt;
  if (tmp > 2147483647LL) {
    tmp = 2147483647LL;
  } else if (tmp < -2147483648LL) {
    tmp = -2147483648LL;
  }

  /* Delay: '<S90>/Resettable Delay' */
  if (localDW->icLoad) {
    localDW->ResettableDelay_DSTATE = rtu_init << 16;
  }

  /* Switch: '<S88>/Switch1' incorporates:
   *  Constant: '<S88>/Constant'
   *  Sum: '<S88>/Sum2'
   *  UnitDelay: '<S88>/UnitDelay1'
   */
  if (localDW->UnitDelay1_DSTATE) {
    tmp_0 = 0;
  } else {
    tmp_0 = (int32_t)tmp;
  }

  /* Sum: '<S90>/Sum1' incorporates:
   *  Delay: '<S90>/Resettable Delay'
   *  Switch: '<S88>/Switch1'
   */
  rtb_Sum1_p = tmp_0 + localDW->ResettableDelay_DSTATE;

  /* Product: '<S88>/Divide5' */
  tmp_0 = (rtu_err * rtu_P) >> 11;
  if (tmp_0 > 32767) {
    tmp_0 = 32767;
  } else if (tmp_0 < -32768) {
    tmp_0 = -32768;
  }

  /* Sum: '<S88>/Sum1' incorporates:
   *  DataTypeConversion: '<S90>/Data Type Conversion1'
   *  Product: '<S88>/Divide5'
   *  Sum: '<S90>/Sum1'
   */
  tmp_0 = (((rtb_Sum1_p >> 16) << 1) + tmp_0) >> 1;
  if (tmp_0 > 32767) {
    tmp_0 = 32767;
  } else if (tmp_0 < -32768) {
    tmp_0 = -32768;
  }

  /* RelationalOperator: '<S91>/LowerRelop1' incorporates:
   *  Sum: '<S88>/Sum1'
   */
  rtb_LowerRelop1_lv = (tmp_0 > rtu_satMax);

  /* RelationalOperator: '<S91>/UpperRelop' incorporates:
   *  Sum: '<S88>/Sum1'
   */
  rtb_UpperRelop_mn = (tmp_0 < rtu_satMin);

  /* Switch: '<S91>/Switch1' incorporates:
   *  Sum: '<S88>/Sum1'
   *  Switch: '<S91>/Switch3'
   */
  if (rtb_LowerRelop1_lv) {
    rty_out_0 = rtu_satMax;
  } else if (rtb_UpperRelop_mn) {
    /* Switch: '<S91>/Switch3' */
    rty_out_0 = rtu_satMin;
  } else {
    rty_out_0 = (int16_t)tmp_0;
  }

  /* End of Switch: '<S91>/Switch1' */

  /* Signum: '<S89>/SignDeltaU2' incorporates:
   *  Sum: '<S88>/Sum2'
   */
  if ((int32_t)tmp < 0) {
    tmp_1 = -1;
  } else {
    tmp_1 = ((int32_t)tmp > 0);
  }

  /* Signum: '<S89>/SignDeltaU3' incorporates:
   *  Sum: '<S88>/Sum1'
   */
  if (tmp_0 < 0) {
    tmp_2 = -1;
  } else {
    tmp_2 = (int16_t)(tmp_0 > 0);
  }

  /* Update for UnitDelay: '<S88>/UnitDelay1' incorporates:
   *  Logic: '<S88>/AND1'
   *  Logic: '<S89>/AND1'
   *  RelationalOperator: '<S89>/Equal1'
   *  Signum: '<S89>/SignDeltaU2'
   *  Signum: '<S89>/SignDeltaU3'
   */
  localDW->UnitDelay1_DSTATE = ((tmp_1 == tmp_2) && (rtb_LowerRelop1_lv ||
    rtb_UpperRelop_mn));

  /* Update for Delay: '<S90>/Resettable Delay' incorporates:
   *  Sum: '<S90>/Sum1'
   */
  localDW->icLoad = false;
  localDW->ResettableDelay_DSTATE = rtb_Sum1_p;
  return rty_out_0;
}

/* System initialize for atomic system: '<S95>/PI_clamp_fixdtFF' */
#if FeedForwardEnable == 1

void PI_clamp_fixdtFF_d_Init(DW_PI_clamp_fixdtFF_k *localDW)
{
  /* InitializeConditions for Delay: '<S156>/Resettable Delay' */
  localDW->icLoad = true;
}

#endif

/* Output and update for atomic system: '<S95>/PI_clamp_fixdtFF' */
#if FeedForwardEnable == 1

int16_t PI_clamp_fixdtFF_c(int16_t rtu_err, uint16_t rtu_P, uint16_t rtu_I,
  int16_t rtu_init, int16_t rtu_satMax, int16_t rtu_satMin, int16_t rtu_tgtInput,
  int16_t rtu_FFGain, DW_PI_clamp_fixdtFF_k *localDW)
{
  int16_t rty_out_0;
  int32_t rtb_SignDeltaU2_h;
  int32_t rtb_Sum1_b;
  int32_t tmp;
  int32_t tmp_0;
  int16_t tmp_1;
  bool rtb_LowerRelop1_c;
  bool rtb_UpperRelop_ok;

  /* Product: '<S154>/Divide2' incorporates:
   *  Signum: '<S155>/SignDeltaU2'
   */
  rtb_SignDeltaU2_h = rtu_err * rtu_I;

  /* Delay: '<S156>/Resettable Delay' */
  if (localDW->icLoad) {
    localDW->ResettableDelay_DSTATE = rtu_init << 14;
  }

  /* Switch: '<S154>/Switch1' incorporates:
   *  Constant: '<S154>/Constant'
   *  Signum: '<S155>/SignDeltaU2'
   *  UnitDelay: '<S154>/UnitDelay1'
   */
  if (localDW->UnitDelay1_DSTATE) {
    tmp_0 = 0;
  } else {
    tmp_0 = rtb_SignDeltaU2_h;
  }

  /* Sum: '<S156>/Sum1' incorporates:
   *  Delay: '<S156>/Resettable Delay'
   *  Switch: '<S154>/Switch1'
   */
  rtb_Sum1_b = tmp_0 + localDW->ResettableDelay_DSTATE;

  /* Product: '<S154>/Product' */
  tmp_0 = (rtu_FFGain * rtu_tgtInput) >> 12;
  if (tmp_0 > 32767) {
    tmp_0 = 32767;
  } else if (tmp_0 < -32768) {
    tmp_0 = -32768;
  }

  /* Product: '<S154>/Divide5' */
  tmp = (rtu_err * rtu_P) >> 8;
  if (tmp > 32767) {
    tmp = 32767;
  } else if (tmp < -32768) {
    tmp = -32768;
  }

  /* Sum: '<S154>/Sum1' incorporates:
   *  DataTypeConversion: '<S156>/Data Type Conversion1'
   *  Product: '<S154>/Divide5'
   *  Product: '<S154>/Product'
   *  Sum: '<S156>/Sum1'
   */
  tmp_0 = (tmp_0 + tmp) + (int16_t)(rtb_Sum1_b >> 14);
  if (tmp_0 > 32767) {
    tmp_0 = 32767;
  } else if (tmp_0 < -32768) {
    tmp_0 = -32768;
  }

  /* RelationalOperator: '<S157>/LowerRelop1' incorporates:
   *  Sum: '<S154>/Sum1'
   */
  rtb_LowerRelop1_c = (tmp_0 > rtu_satMax);

  /* RelationalOperator: '<S157>/UpperRelop' incorporates:
   *  Sum: '<S154>/Sum1'
   */
  rtb_UpperRelop_ok = (tmp_0 < rtu_satMin);

  /* Switch: '<S157>/Switch1' incorporates:
   *  Sum: '<S154>/Sum1'
   *  Switch: '<S157>/Switch3'
   */
  if (rtb_LowerRelop1_c) {
    rty_out_0 = rtu_satMax;
  } else if (rtb_UpperRelop_ok) {
    /* Switch: '<S157>/Switch3' */
    rty_out_0 = rtu_satMin;
  } else {
    rty_out_0 = (int16_t)tmp_0;
  }

  /* End of Switch: '<S157>/Switch1' */

  /* Signum: '<S155>/SignDeltaU2' */
  if (rtb_SignDeltaU2_h < 0) {
    tmp = -1;
  } else {
    tmp = (rtb_SignDeltaU2_h > 0);
  }

  /* Signum: '<S155>/SignDeltaU3' incorporates:
   *  Sum: '<S154>/Sum1'
   */
  if (tmp_0 < 0) {
    tmp_1 = -1;
  } else {
    tmp_1 = (int16_t)(tmp_0 > 0);
  }

  /* Update for UnitDelay: '<S154>/UnitDelay1' incorporates:
   *  Logic: '<S154>/AND1'
   *  Logic: '<S155>/AND1'
   *  RelationalOperator: '<S155>/Equal1'
   *  Signum: '<S155>/SignDeltaU2'
   *  Signum: '<S155>/SignDeltaU3'
   */
  localDW->UnitDelay1_DSTATE = ((tmp == tmp_1) && (rtb_LowerRelop1_c ||
    rtb_UpperRelop_ok));

  /* Update for Delay: '<S156>/Resettable Delay' incorporates:
   *  Sum: '<S156>/Sum1'
   */
  localDW->icLoad = false;
  localDW->ResettableDelay_DSTATE = rtb_Sum1_b;
  return rty_out_0;
}

#endif

/* System initialize for function-call system: '<S7>/FOC' */
void FOC_Init(int16_t *rty_Vq, DW_FOC *localDW)
{
  /* Start for If: '<S66>/If1' */
  localDW->If1_ActiveSubsystem = -1;

  /* SystemInitialize for IfAction SubSystem: '<S66>/FOC_Enabled' */
  /* Start for If: '<S78>/If1' */
  localDW->If1_ActiveSubsystem_a = -1;

  /* SystemInitialize for Enabled SubSystem: '<S78>/Speed_Mode' */
  /* SystemInitialize for Atomic SubSystem: '<S84>/PI_clamp_fixdt' */
  PI_clamp_fixdt_Init(&localDW->PI_clamp_fixdt_j);

  /* End of SystemInitialize for SubSystem: '<S84>/PI_clamp_fixdt' */
  /* End of SystemInitialize for SubSystem: '<S78>/Speed_Mode' */

  /* SystemInitialize for Enabled SubSystem: '<S78>/Torque_Mode' */
  /* SystemInitialize for Atomic SubSystem: '<S85>/Pi Variant' */
#if FeedForwardEnable == 0

  /* SystemInitialize for Atomic SubSystem: '<S92>/FFoff' */
  /* InitializeConditions for DiscreteIntegrator: '<S135>/Integrator' */
  localDW->Integrator_IC_LOADING = 1U;

  /* End of SystemInitialize for SubSystem: '<S92>/FFoff' */
#elif FeedForwardEnable == 1

  /* SystemInitialize for Atomic SubSystem: '<S92>/FFon' */
  /* SystemInitialize for Atomic SubSystem: '<S95>/PI_clamp_fixdtFF' */
  PI_clamp_fixdtFF_d_Init(&localDW->PI_clamp_fixdtFF_ci);

  /* End of SystemInitialize for SubSystem: '<S95>/PI_clamp_fixdtFF' */
  /* End of SystemInitialize for SubSystem: '<S92>/FFon' */
#elif FeedForwardEnable == 2

  /* SystemInitialize for Atomic SubSystem: '<S92>/TMClassic' */
  TMClassic_Init(&localDW->TMClassic_n);

  /* End of SystemInitialize for SubSystem: '<S92>/TMClassic' */
#endif

  /* End of SystemInitialize for SubSystem: '<S85>/Pi Variant' */
  /* End of SystemInitialize for SubSystem: '<S78>/Torque_Mode' */

  /* SystemInitialize for Merge: '<S78>/Merge' */
  *rty_Vq = 0;

  /* SystemInitialize for IfAction SubSystem: '<S78>/Vd_Calculation' */
  /* Start for Constant: '<S86>/constant2' */
  localDW->constant2 = 0;

  /* SystemInitialize for Atomic SubSystem: '<S86>/Pi Variant' */
#if FeedForwardEnable == 0

  /* SystemInitialize for Atomic SubSystem: '<S161>/FFoff' */
  /* InitializeConditions for DiscreteIntegrator: '<S204>/Integrator' incorporates:
   *  Constant: '<S86>/constant2'
   */
  localDW->Integrator_DSTATE_a = 0;

  /* End of SystemInitialize for SubSystem: '<S161>/FFoff' */
#elif FeedForwardEnable == 1

  /* SystemInitialize for Atomic SubSystem: '<S161>/FFon' */
  /* SystemInitialize for Atomic SubSystem: '<S164>/PI_clamp_fixdtFF' */
  PI_clamp_fixdtFF_Init(&localDW->PI_clamp_fixdtFF_c2);

  /* End of SystemInitialize for SubSystem: '<S164>/PI_clamp_fixdtFF' */
  /* End of SystemInitialize for SubSystem: '<S161>/FFon' */
#elif FeedForwardEnable == 2

  /* SystemInitialize for Atomic SubSystem: '<S161>/TMClassic' */
  TMClassic_Init(&localDW->TMClassic_l);

  /* End of SystemInitialize for SubSystem: '<S161>/TMClassic' */
#endif

  /* End of SystemInitialize for SubSystem: '<S86>/Pi Variant' */
  /* End of SystemInitialize for SubSystem: '<S78>/Vd_Calculation' */
  /* End of SystemInitialize for SubSystem: '<S66>/FOC_Enabled' */
}

/* Output and update for function-call system: '<S7>/FOC' */
void FOC(uint8_t rtu_z_ctrlTypSel, bool rtu_b_enaAdvCtrl, uint8_t rtu_z_ctrlMod,
         int16_t rtu_r_inpTgtSca, int16_t rtu_r_inpTgtScaAbs, int16_t rtu_n_mot,
         int16_t rtu_id_TgtAbs, int16_t rtu_id, int16_t rtu_iq, int16_t
         rtu_Vd_max, int16_t rtu_Vd_min, int16_t rtu_Vq_max, int16_t rtu_Vq_min,
         int16_t rtu_id_max, int16_t rtu_id_min, int16_t rtu_iq_max, int16_t
         rtu_iq_min, int16_t rtu_r_iqLimProtMax, int16_t rtu_r_nLimProtMax,
         int32_t rtu_iq_limProtErr, int16_t rtu_Vq_nLimProtMax, int16_t
         rtu_VqFinPrev, bool rtu_b_cruiseCtrlEna, const P *rtP, int16_t *rty_Vq, int16_t
         *rty_Vd, DW_FOC *localDW)
{
  /* local block i/o variables */
  int16_t rtb_VariantMergeForOutportVq_Tr;
  int16_t rtb_DataTypeConversion2_f;
  int16_t rtb_MinMax1_d;
  int16_t rtb_Switch4_idx_1;
  int16_t rtb_Switch_lb;
  int8_t rtAction;
  int8_t rtPrevAction;
  bool rtb_Compare_ob;

#if FeedForwardEnable == 0

  int32_t tmp;
  int32_t tmp_0;

#endif

  int32_t tmp_1;

  /* If: '<S66>/If1' incorporates:
   *  Constant: '<S79>/Constant'
   *  Constant: '<S81>/Constant'
   *  Constant: '<S82>/Constant'
   *  Constant: '<S84>/cf_nKi'
   *  Constant: '<S84>/cf_nKp'
   *  Constant: '<S85>/Input'
   *  Constant: '<S85>/cf_iqKi1'
   *  Constant: '<S85>/cf_iqKp1'
   *  Logic: '<S78>/OR'
   *  RelationalOperator: '<S79>/Compare'
   *  RelationalOperator: '<S81>/Compare'
   *  RelationalOperator: '<S82>/Compare'
   *  Sum: '<S84>/Sum3'
   *  Sum: '<S85>/Sum2'
   */
  rtPrevAction = localDW->If1_ActiveSubsystem;
  rtAction = -1;
  if (rtu_z_ctrlTypSel == 2) {
    rtAction = 0;
  }

  localDW->If1_ActiveSubsystem = rtAction;
  if ((rtPrevAction != rtAction) && (rtPrevAction == 0)) {
    /* Disable for If: '<S78>/If1' */
    localDW->If1_ActiveSubsystem_a = -1;
  }

  if (rtAction == 0) {
    /* Outputs for IfAction SubSystem: '<S66>/FOC_Enabled' incorporates:
     *  ActionPort: '<S78>/Action Port'
     */
    /* RelationalOperator: '<S80>/Compare' incorporates:
     *  Constant: '<S80>/Constant'
     */
    rtb_Compare_ob = (rtu_z_ctrlMod == 2);

    /* Outputs for Enabled SubSystem: '<S78>/Speed_Mode' incorporates:
     *  EnablePort: '<S84>/Enable'
     */
    if (rtb_Compare_ob) {
      /* DataTypeConversion: '<S84>/Data Type Conversion2' incorporates:
       *  Constant: '<S84>/n_cruiseMotTgt'
       */
      rtb_DataTypeConversion2_f = (int16_t)(rtP->n_cruiseMotTgt << 4);

      /* Switch: '<S84>/Switch4' incorporates:
       *  DataTypeConversion: '<S84>/Data Type Conversion2'
       *  Logic: '<S84>/Logical Operator1'
       *  RelationalOperator: '<S84>/Relational Operator3'
       */
      if (rtu_b_cruiseCtrlEna && (rtb_DataTypeConversion2_f != 0)) {
        /* Switch: '<S84>/Switch3' incorporates:
         *  MinMax: '<S84>/MinMax4'
         */
        if (rtb_DataTypeConversion2_f > 0) {
          /* Switch: '<S84>/Switch4' */
          rtb_Switch_lb = rtu_Vq_max;

          /* MinMax: '<S84>/MinMax3' */
          if (rtu_r_inpTgtSca >= rtu_Vq_min) {
            /* Switch: '<S84>/Switch4' */
            rtb_Switch4_idx_1 = rtu_r_inpTgtSca;
          } else {
            /* Switch: '<S84>/Switch4' */
            rtb_Switch4_idx_1 = rtu_Vq_min;
          }

          /* End of MinMax: '<S84>/MinMax3' */
        } else {
          if (rtu_Vq_max <= rtu_r_inpTgtSca) {
            /* MinMax: '<S84>/MinMax4' incorporates:
             *  Switch: '<S84>/Switch4'
             */
            rtb_Switch_lb = rtu_Vq_max;
          } else {
            /* Switch: '<S84>/Switch4' incorporates:
             *  MinMax: '<S84>/MinMax4'
             */
            rtb_Switch_lb = rtu_r_inpTgtSca;
          }

          /* Switch: '<S84>/Switch4' */
          rtb_Switch4_idx_1 = rtu_Vq_min;
        }

        /* End of Switch: '<S84>/Switch3' */
      } else {
        rtb_Switch_lb = rtu_Vq_max;
        rtb_Switch4_idx_1 = rtu_Vq_min;
      }

      /* End of Switch: '<S84>/Switch4' */

      /* Switch: '<S84>/Switch2' */
      if (!rtu_b_cruiseCtrlEna) {
        rtb_DataTypeConversion2_f = rtu_r_inpTgtSca;
      }

      /* Sum: '<S84>/Sum3' incorporates:
       *  Switch: '<S84>/Switch2'
       */
      tmp_1 = rtb_DataTypeConversion2_f - rtu_n_mot;
      if (tmp_1 > 32767) {
        tmp_1 = 32767;
      } else if (tmp_1 < -32768) {
        tmp_1 = -32768;
      }

      /* Outputs for Atomic SubSystem: '<S84>/PI_clamp_fixdt' */
      localDW->Switch1 = PI_clamp_fixdt((int16_t)tmp_1, rtP->cf_nKp, rtP->cf_nKi,
        rtu_VqFinPrev, rtb_Switch_lb, rtb_Switch4_idx_1, rtu_iq_limProtErr,
        &localDW->PI_clamp_fixdt_j);

      /* End of Outputs for SubSystem: '<S84>/PI_clamp_fixdt' */

      /* Switch: '<S78>/Switch' incorporates:
       *  Constant: '<S84>/cf_nKi'
       *  Constant: '<S84>/cf_nKp'
       *  Sum: '<S84>/Sum3'
       *  Switch: '<S91>/Switch1'
       */
      rtb_Switch_lb = localDW->Switch1;
    } else {
      /* Switch: '<S78>/Switch' */
      rtb_Switch_lb = rtu_r_inpTgtSca;
    }

    /* End of Outputs for SubSystem: '<S78>/Speed_Mode' */

    /* Outputs for Enabled SubSystem: '<S78>/Voltage_Mode' incorporates:
     *  EnablePort: '<S87>/Enable'
     */
    if (rtu_z_ctrlMod == 1) {
      /* MinMax: '<S87>/MinMax' */
      if (rtu_r_inpTgtScaAbs <= rtu_r_iqLimProtMax) {
        rtb_DataTypeConversion2_f = rtu_r_inpTgtScaAbs;
      } else {
        rtb_DataTypeConversion2_f = rtu_r_iqLimProtMax;
      }

      if (rtb_DataTypeConversion2_f > rtu_r_nLimProtMax) {
        rtb_DataTypeConversion2_f = rtu_r_nLimProtMax;
      }

      /* Signum: '<S87>/SignDeltaU2' */
      if (rtu_r_inpTgtSca < 0) {
        rtb_Switch4_idx_1 = -1;
      } else {
        rtb_Switch4_idx_1 = (int16_t)(rtu_r_inpTgtSca > 0);
      }

      /* Product: '<S87>/Divide1' incorporates:
       *  MinMax: '<S87>/MinMax'
       *  Signum: '<S87>/SignDeltaU2'
       */
      rtb_DataTypeConversion2_f *= rtb_Switch4_idx_1;

      /* Switch: '<S230>/Switch2' incorporates:
       *  Product: '<S87>/Divide1'
       *  RelationalOperator: '<S230>/LowerRelop1'
       *  RelationalOperator: '<S230>/UpperRelop'
       *  Switch: '<S230>/Switch'
       */
      if (rtb_DataTypeConversion2_f > rtu_Vq_max) {
        /* SignalConversion: '<S87>/Signal Conversion2' */
        *rty_Vq = rtu_Vq_max;
      } else if (rtb_DataTypeConversion2_f < rtu_Vq_min) {
        /* Switch: '<S230>/Switch' incorporates:
         *  SignalConversion: '<S87>/Signal Conversion2'
         */
        *rty_Vq = rtu_Vq_min;
      } else {
        /* SignalConversion: '<S87>/Signal Conversion2' incorporates:
         *  Switch: '<S230>/Switch'
         */
        *rty_Vq = rtb_DataTypeConversion2_f;
      }

      /* End of Switch: '<S230>/Switch2' */
    }

    /* End of Outputs for SubSystem: '<S78>/Voltage_Mode' */

    /* Outputs for Enabled SubSystem: '<S78>/Torque_Mode' incorporates:
     *  EnablePort: '<S85>/Enable'
     */
    if (rtb_Compare_ob || (rtu_z_ctrlMod == 3)) {
      /* Gain: '<S85>/Gain4' */
      rtb_DataTypeConversion2_f = (int16_t)-rtu_Vq_nLimProtMax;

      /* MinMax: '<S85>/MinMax2' incorporates:
       *  Gain: '<S85>/Gain4'
       */
      if ((int16_t)-rtu_Vq_nLimProtMax < rtu_Vq_min) {
        rtb_DataTypeConversion2_f = rtu_Vq_min;
      }

      /* End of MinMax: '<S85>/MinMax2' */

      /* MinMax: '<S85>/MinMax1' */
      if (rtu_Vq_max <= rtu_Vq_nLimProtMax) {
        rtb_MinMax1_d = rtu_Vq_max;
      } else {
        rtb_MinMax1_d = rtu_Vq_nLimProtMax;
      }

      /* End of MinMax: '<S85>/MinMax1' */

      /* Switch: '<S93>/Switch2' incorporates:
       *  RelationalOperator: '<S93>/LowerRelop1'
       *  RelationalOperator: '<S93>/UpperRelop'
       *  Switch: '<S78>/Switch'
       *  Switch: '<S93>/Switch'
       */
      if (rtb_Switch_lb > rtu_iq_max) {
        rtb_Switch_lb = rtu_iq_max;
      } else if (rtb_Switch_lb < rtu_iq_min) {
        /* Switch: '<S93>/Switch' incorporates:
         *  Switch: '<S93>/Switch2'
         */
        rtb_Switch_lb = rtu_iq_min;
      }

      /* End of Switch: '<S93>/Switch2' */

      /* Sum: '<S85>/Sum2' incorporates:
       *  Switch: '<S93>/Switch2'
       */
      tmp_1 = rtb_Switch_lb - rtu_iq;
      if (tmp_1 > 32767) {
        tmp_1 = 32767;
      } else if (tmp_1 < -32768) {
        tmp_1 = -32768;
      }

      /* Outputs for Atomic SubSystem: '<S85>/Pi Variant' */
#if FeedForwardEnable == 0

      /* Outputs for Atomic SubSystem: '<S92>/FFoff' */
      /* Product: '<S132>/IProd Out' incorporates:
       *  Constant: '<S85>/cf_iqKi1'
       *  Sum: '<S85>/Sum2'
       */
      tmp_0 = (tmp_1 * rtP->cf_iqKi) >> 14;
      if (tmp_0 > 32767) {
        tmp_0 = 32767;
      } else if (tmp_0 < -32768) {
        tmp_0 = -32768;
      }

      /* DiscreteIntegrator: '<S135>/Integrator' */
      if (localDW->Integrator_IC_LOADING != 0) {
        localDW->Integrator_DSTATE = rtu_VqFinPrev;
      }

      /* Product: '<S140>/PProd Out' incorporates:
       *  Constant: '<S85>/cf_iqKp1'
       *  Sum: '<S85>/Sum2'
       */
      tmp = (tmp_1 * rtP->cf_iqKp) >> 8;
      if (tmp > 32767) {
        tmp = 32767;
      } else if (tmp < -32768) {
        tmp = -32768;
      }

      /* Sum: '<S145>/Sum' incorporates:
       *  DiscreteIntegrator: '<S135>/Integrator'
       *  Product: '<S140>/PProd Out'
       */
      tmp += localDW->Integrator_DSTATE;
      if (tmp > 32767) {
        tmp = 32767;
      } else if (tmp < -32768) {
        tmp = -32768;
      }

      /* Switch: '<S127>/Switch' incorporates:
       *  MinMax: '<S85>/MinMax1'
       *  MinMax: '<S85>/MinMax2'
       *  RelationalOperator: '<S127>/u_GTE_up'
       *  RelationalOperator: '<S127>/u_GT_lo'
       *  Sum: '<S145>/Sum'
       *  Switch: '<S127>/Switch1'
       */
      if (tmp >= rtb_MinMax1_d) {
        rtb_Switch4_idx_1 = rtb_MinMax1_d;
      } else if (tmp > rtb_DataTypeConversion2_f) {
        /* Switch: '<S127>/Switch1' */
        rtb_Switch4_idx_1 = (int16_t)tmp;
      } else {
        rtb_Switch4_idx_1 = rtb_DataTypeConversion2_f;
      }

      /* Sum: '<S127>/Diff' incorporates:
       *  Sum: '<S145>/Sum'
       *  Switch: '<S127>/Switch'
       */
      rtb_Switch4_idx_1 = (int16_t)(tmp - rtb_Switch4_idx_1);

      /* Switch: '<S143>/Switch2' incorporates:
       *  MinMax: '<S85>/MinMax1'
       *  MinMax: '<S85>/MinMax2'
       *  RelationalOperator: '<S143>/LowerRelop1'
       *  RelationalOperator: '<S143>/UpperRelop'
       *  Sum: '<S145>/Sum'
       *  Switch: '<S143>/Switch'
       */
      if (tmp > rtb_MinMax1_d) {
        /* VariantMerge generated from: '<S92>/Vq_Trq' */
        rtb_VariantMergeForOutportVq_Tr = rtb_MinMax1_d;
      } else if (tmp < rtb_DataTypeConversion2_f) {
        /* Switch: '<S143>/Switch' incorporates:
         *  MinMax: '<S85>/MinMax2'
         *  VariantMerge generated from: '<S92>/Vq_Trq'
         */
        rtb_VariantMergeForOutportVq_Tr = rtb_DataTypeConversion2_f;
      } else {
        /* VariantMerge generated from: '<S92>/Vq_Trq' */
        rtb_VariantMergeForOutportVq_Tr = (int16_t)tmp;
      }

      /* End of Switch: '<S143>/Switch2' */

      /* Update for DiscreteIntegrator: '<S135>/Integrator' */
      localDW->Integrator_IC_LOADING = 0U;

      /* Switch: '<S124>/Switch1' incorporates:
       *  Constant: '<S124>/Constant'
       *  Constant: '<S124>/Constant2'
       *  RelationalOperator: '<S124>/fix for DT propagation issue'
       *  Sum: '<S127>/Diff'
       */
      if (rtb_Switch4_idx_1 > 0) {
        rtPrevAction = 1;
      } else {
        rtPrevAction = -1;
      }

      /* Switch: '<S124>/Switch2' incorporates:
       *  Constant: '<S124>/Constant3'
       *  Constant: '<S124>/Constant4'
       *  Product: '<S132>/IProd Out'
       *  RelationalOperator: '<S124>/fix for DT propagation issue1'
       */
      if (tmp_0 > 0) {
        rtAction = 1;
      } else {
        rtAction = -1;
      }

      /* Switch: '<S124>/Switch' incorporates:
       *  Constant: '<S124>/Constant1'
       *  Logic: '<S124>/AND3'
       *  Product: '<S132>/IProd Out'
       *  RelationalOperator: '<S124>/Equal1'
       *  RelationalOperator: '<S124>/Relational Operator'
       *  Sum: '<S127>/Diff'
       *  Switch: '<S124>/Switch1'
       *  Switch: '<S124>/Switch2'
       */
      if ((rtb_Switch4_idx_1 != 0) && (rtPrevAction == rtAction)) {
        rtb_Switch4_idx_1 = 0;
      } else {
        rtb_Switch4_idx_1 = (int16_t)tmp_0;
      }

      /* Update for DiscreteIntegrator: '<S135>/Integrator' incorporates:
       *  Switch: '<S124>/Switch'
       */
      tmp_0 = localDW->Integrator_DSTATE + rtb_Switch4_idx_1;
      if (tmp_0 > 32767) {
        tmp_0 = 32767;
      } else if (tmp_0 < -32768) {
        tmp_0 = -32768;
      }

      localDW->Integrator_DSTATE = (int16_t)tmp_0;

      /* End of Outputs for SubSystem: '<S92>/FFoff' */
#elif FeedForwardEnable == 1

      /* Outputs for Atomic SubSystem: '<S92>/FFon' */
      /* Outputs for Atomic SubSystem: '<S95>/PI_clamp_fixdtFF' */
      rtb_VariantMergeForOutportVq_Tr = PI_clamp_fixdtFF_c((int16_t)tmp_1,
        rtP->cf_iqKp, rtP->cf_iqKi, rtu_VqFinPrev, rtb_MinMax1_d,
        rtb_DataTypeConversion2_f, rtb_Switch_lb, rtP->ff_gain,
        &localDW->PI_clamp_fixdtFF_ci);

      /* End of Outputs for SubSystem: '<S95>/PI_clamp_fixdtFF' */
      /* End of Outputs for SubSystem: '<S92>/FFon' */
#elif FeedForwardEnable == 2

      /* Outputs for Atomic SubSystem: '<S92>/TMClassic' */
      rtb_VariantMergeForOutportVq_Tr = TMClassic((int16_t)tmp_1, rtP->cf_iqKp,
        rtP->cf_iqKi, rtu_VqFinPrev, rtb_MinMax1_d, rtb_DataTypeConversion2_f,
        &localDW->TMClassic_n);

      /* End of Outputs for SubSystem: '<S92>/TMClassic' */
#endif

      /* End of Outputs for SubSystem: '<S85>/Pi Variant' */

      /* SignalConversion generated from: '<S85>/Vq_Trq' incorporates:
       *  Constant: '<S85>/Input'
       *  Constant: '<S85>/cf_iqKi1'
       *  Constant: '<S85>/cf_iqKp1'
       *  Sum: '<S85>/Sum2'
       *  VariantMerge generated from: '<S92>/Vq_Trq'
       */
      *rty_Vq = rtb_VariantMergeForOutportVq_Tr;
    }

    /* End of Outputs for SubSystem: '<S78>/Torque_Mode' */

    /* Outputs for Enabled SubSystem: '<S78>/Open_Mode' incorporates:
     *  EnablePort: '<S83>/Enable'
     */
    if (rtu_z_ctrlMod == 0) {
      /* SignalConversion generated from: '<S83>/r_inpTgtSca' */
      *rty_Vq = rtu_r_inpTgtSca;
    }

    /* End of Outputs for SubSystem: '<S78>/Open_Mode' */

    /* If: '<S78>/If1' incorporates:
     *  Constant: '<S79>/Constant'
     *  Constant: '<S81>/Constant'
     *  Constant: '<S82>/Constant'
     *  Constant: '<S84>/cf_nKi'
     *  Constant: '<S84>/cf_nKp'
     *  Constant: '<S85>/Input'
     *  Constant: '<S85>/cf_iqKi1'
     *  Constant: '<S85>/cf_iqKp1'
     *  Constant: '<S86>/Input'
     *  Constant: '<S86>/cf_idKi2'
     *  Constant: '<S86>/cf_idKp2'
     *  Constant: '<S86>/constant2'
     *  Logic: '<S78>/OR'
     *  RelationalOperator: '<S79>/Compare'
     *  RelationalOperator: '<S81>/Compare'
     *  RelationalOperator: '<S82>/Compare'
     *  Sum: '<S84>/Sum3'
     *  Sum: '<S85>/Sum2'
     *  Sum: '<S86>/Sum3'
     */
    rtPrevAction = localDW->If1_ActiveSubsystem_a;
    rtAction = -1;
    if (rtu_b_enaAdvCtrl) {
      rtAction = 0;
    }

    localDW->If1_ActiveSubsystem_a = rtAction;
    if (rtAction == 0) {
      if (rtPrevAction != 0) {
        /* SystemReset for IfAction SubSystem: '<S78>/Vd_Calculation' incorporates:
         *  ActionPort: '<S86>/Action Port'
         */
        /* SystemReset for Atomic SubSystem: '<S86>/Pi Variant' */
        /* SystemReset for If: '<S78>/If1' */
#if FeedForwardEnable == 0

        /* SystemReset for Atomic SubSystem: '<S161>/FFoff' */
        /* InitializeConditions for DiscreteIntegrator: '<S204>/Integrator' incorporates:
         *  Constant: '<S86>/constant2'
         */
        localDW->Integrator_DSTATE_a = localDW->constant2;

        /* End of SystemReset for SubSystem: '<S161>/FFoff' */
#elif FeedForwardEnable == 1

        /* SystemReset for Atomic SubSystem: '<S161>/FFon' */
        /* SystemReset for Atomic SubSystem: '<S164>/PI_clamp_fixdtFF' */
        PI_clamp_fixdtFF_Reset(&localDW->PI_clamp_fixdtFF_c2);

        /* End of SystemReset for SubSystem: '<S164>/PI_clamp_fixdtFF' */
        /* End of SystemReset for SubSystem: '<S161>/FFon' */
#elif FeedForwardEnable == 2

        /* SystemReset for Atomic SubSystem: '<S161>/TMClassic' */
        TMClassic_Reset(&localDW->TMClassic_l);

        /* End of SystemReset for SubSystem: '<S161>/TMClassic' */
#endif

        /* End of SystemReset for If: '<S78>/If1' */
        /* End of SystemReset for SubSystem: '<S86>/Pi Variant' */
        /* End of SystemReset for SubSystem: '<S78>/Vd_Calculation' */
      }

      /* Outputs for IfAction SubSystem: '<S78>/Vd_Calculation' incorporates:
       *  ActionPort: '<S86>/Action Port'
       */
      /* Gain: '<S86>/toNegative' */
      rtb_DataTypeConversion2_f = (int16_t)-rtu_id_TgtAbs;

      /* Switch: '<S162>/Switch2' incorporates:
       *  Gain: '<S86>/toNegative'
       *  RelationalOperator: '<S162>/LowerRelop1'
       *  RelationalOperator: '<S162>/UpperRelop'
       *  Switch: '<S162>/Switch'
       */
      if ((int16_t)-rtu_id_TgtAbs > rtu_id_max) {
        rtb_DataTypeConversion2_f = rtu_id_max;
      } else if ((int16_t)-rtu_id_TgtAbs < rtu_id_min) {
        /* Switch: '<S162>/Switch' incorporates:
         *  Switch: '<S162>/Switch2'
         */
        rtb_DataTypeConversion2_f = rtu_id_min;
      }

      /* End of Switch: '<S162>/Switch2' */

      /* Sum: '<S86>/Sum3' incorporates:
       *  Switch: '<S162>/Switch2'
       */
      tmp_1 = rtb_DataTypeConversion2_f - rtu_id;
      if (tmp_1 > 32767) {
        tmp_1 = 32767;
      } else if (tmp_1 < -32768) {
        tmp_1 = -32768;
      }

      localDW->constant2 = 0;

      /* Outputs for Atomic SubSystem: '<S86>/Pi Variant' */
#if FeedForwardEnable == 0

      /* Outputs for Atomic SubSystem: '<S161>/FFoff' */
      /* Product: '<S201>/IProd Out' incorporates:
       *  Constant: '<S86>/cf_idKi2'
       *  Sum: '<S86>/Sum3'
       */
      tmp_0 = (tmp_1 * rtP->cf_idKi) >> 14;
      if (tmp_0 > 32767) {
        tmp_0 = 32767;
      } else if (tmp_0 < -32768) {
        tmp_0 = -32768;
      }

      /* Product: '<S209>/PProd Out' incorporates:
       *  Constant: '<S86>/cf_idKp2'
       *  Sum: '<S86>/Sum3'
       */
      tmp = (tmp_1 * rtP->cf_idKp) >> 8;
      if (tmp > 32767) {
        tmp = 32767;
      } else if (tmp < -32768) {
        tmp = -32768;
      }

      /* Sum: '<S214>/Sum' incorporates:
       *  DiscreteIntegrator: '<S204>/Integrator'
       *  Product: '<S209>/PProd Out'
       */
      tmp += localDW->Integrator_DSTATE_a;
      if (tmp > 32767) {
        tmp = 32767;
      } else if (tmp < -32768) {
        tmp = -32768;
      }

      /* Switch: '<S196>/Switch' incorporates:
       *  RelationalOperator: '<S196>/u_GTE_up'
       *  RelationalOperator: '<S196>/u_GT_lo'
       *  Sum: '<S214>/Sum'
       *  Switch: '<S196>/Switch1'
       */
      if (tmp >= rtu_Vd_max) {
        rtb_Switch4_idx_1 = rtu_Vd_max;
      } else if (tmp > rtu_Vd_min) {
        /* Switch: '<S196>/Switch1' */
        rtb_Switch4_idx_1 = (int16_t)tmp;
      } else {
        rtb_Switch4_idx_1 = rtu_Vd_min;
      }

      /* Sum: '<S196>/Diff' incorporates:
       *  Sum: '<S214>/Sum'
       *  Switch: '<S196>/Switch'
       */
      rtb_Switch_lb = (int16_t)(tmp - rtb_Switch4_idx_1);

      /* Switch: '<S212>/Switch2' incorporates:
       *  RelationalOperator: '<S212>/LowerRelop1'
       *  RelationalOperator: '<S212>/UpperRelop'
       *  Sum: '<S214>/Sum'
       *  Switch: '<S212>/Switch'
       */
      if (tmp > rtu_Vd_max) {
        *rty_Vd = rtu_Vd_max;
      } else if (tmp < rtu_Vd_min) {
        /* Switch: '<S212>/Switch' */
        *rty_Vd = rtu_Vd_min;
      } else {
        *rty_Vd = (int16_t)tmp;
      }

      /* End of Switch: '<S212>/Switch2' */

      /* Switch: '<S193>/Switch1' incorporates:
       *  Constant: '<S193>/Constant'
       *  Constant: '<S193>/Constant2'
       *  RelationalOperator: '<S193>/fix for DT propagation issue'
       *  Sum: '<S196>/Diff'
       */
      if (rtb_Switch_lb > 0) {
        rtPrevAction = 1;
      } else {
        rtPrevAction = -1;
      }

      /* Switch: '<S193>/Switch2' incorporates:
       *  Constant: '<S193>/Constant3'
       *  Constant: '<S193>/Constant4'
       *  Product: '<S201>/IProd Out'
       *  RelationalOperator: '<S193>/fix for DT propagation issue1'
       */
      if (tmp_0 > 0) {
        rtAction = 1;
      } else {
        rtAction = -1;
      }

      /* Switch: '<S193>/Switch' incorporates:
       *  Constant: '<S193>/Constant1'
       *  Logic: '<S193>/AND3'
       *  Product: '<S201>/IProd Out'
       *  RelationalOperator: '<S193>/Equal1'
       *  RelationalOperator: '<S193>/Relational Operator'
       *  Sum: '<S196>/Diff'
       *  Switch: '<S193>/Switch1'
       *  Switch: '<S193>/Switch2'
       */
      if ((rtb_Switch_lb != 0) && (rtPrevAction == rtAction)) {
        rtb_Switch4_idx_1 = 0;
      } else {
        rtb_Switch4_idx_1 = (int16_t)tmp_0;
      }

      /* Update for DiscreteIntegrator: '<S204>/Integrator' incorporates:
       *  Switch: '<S193>/Switch'
       */
      tmp_0 = localDW->Integrator_DSTATE_a + rtb_Switch4_idx_1;
      if (tmp_0 > 32767) {
        tmp_0 = 32767;
      } else if (tmp_0 < -32768) {
        tmp_0 = -32768;
      }

      localDW->Integrator_DSTATE_a = (int16_t)tmp_0;

      /* End of Update for DiscreteIntegrator: '<S204>/Integrator' */
      /* End of Outputs for SubSystem: '<S161>/FFoff' */
#elif FeedForwardEnable == 1

      /* Outputs for Atomic SubSystem: '<S161>/FFon' */
      /* Outputs for Atomic SubSystem: '<S164>/PI_clamp_fixdtFF' */
#if FeedForwardEnable != 0 && FeedForwardEnable != 1 && FeedForwardEnable != 2

      UNUSED_PARAMETER(rtu_Vd_max);

#endif

#if FeedForwardEnable != 0 && FeedForwardEnable != 1 && FeedForwardEnable != 2

      UNUSED_PARAMETER(rtu_Vd_min);

#endif

#if FeedForwardEnable != 0 && FeedForwardEnable != 1 && FeedForwardEnable != 2

      UNUSED_PARAMETER(rty_Vd);

#endif

      *rty_Vd = PI_clamp_fixdtFF((int16_t)tmp_1, rtP->cf_idKp, rtP->cf_idKi, 0,
        rtu_Vd_max, rtu_Vd_min, rtb_DataTypeConversion2_f, rtP->ff_gain,
        &localDW->PI_clamp_fixdtFF_c2);

      /* End of Outputs for SubSystem: '<S164>/PI_clamp_fixdtFF' */
      /* End of Outputs for SubSystem: '<S161>/FFon' */
#elif FeedForwardEnable == 2

      /* Outputs for Atomic SubSystem: '<S161>/TMClassic' */
      *rty_Vd = TMClassic((int16_t)tmp_1, rtP->cf_idKp, rtP->cf_idKi, 0,
                          rtu_Vd_max, rtu_Vd_min, &localDW->TMClassic_l);

      /* End of Outputs for SubSystem: '<S161>/TMClassic' */
#endif

      /* End of Outputs for SubSystem: '<S86>/Pi Variant' */
      /* End of Outputs for SubSystem: '<S78>/Vd_Calculation' */
    }

    /* End of If: '<S78>/If1' */
    /* End of Outputs for SubSystem: '<S66>/FOC_Enabled' */
  }

  /* End of If: '<S66>/If1' */
}

/* Model step function */
void BLDC_controller_step(RT_MODEL *const rtM)
{
  P *rtP = ((P *) rtM->defaultParam);
  DW *rtDW = rtM->dwork;
  ExtU *rtU = (ExtU *) rtM->inputs;
  ExtY *rtY = (ExtY *) rtM->outputs;
  int16_t Abs5;
  int16_t Merge;
  int16_t rtb_VariantMergeForOutportVd_ma;
  int8_t rtAction;
  int8_t rtb_Sum2_ii;
  uint8_t Sum;
  uint8_t rtb_k;
  bool rtb_Compare_j;
  bool rtb_LogicalOperator;
  bool rtb_RelationalOperator4;
  bool rtb_n_commDeacv;

  /* Outputs for Atomic SubSystem: '<Root>/BLDC_controller' */
  /* Sum: '<S11>/Sum' incorporates:
   *  Gain: '<S11>/g_Ha'
   *  Gain: '<S11>/g_Hb'
   */
  Sum = (uint8_t)((uint8_t)((uint8_t)(rtU->b_hallA << 2) + (uint8_t)
    (rtU->b_hallB << 1)) + rtU->b_hallC);

  /* Logic: '<S10>/Logical Operator' incorporates:
   *  UnitDelay: '<S10>/UnitDelay1'
   *  UnitDelay: '<S10>/UnitDelay2'
   *  UnitDelay: '<S10>/UnitDelay3'
   */
  rtb_LogicalOperator = (bool)((rtU->b_hallA != 0) ^ (rtU->b_hallB != 0) ^
    (rtU->b_hallC != 0) ^ (rtDW->UnitDelay3_DSTATE_f != 0) ^
    (rtDW->UnitDelay1_DSTATE != 0)) ^ (rtDW->UnitDelay2_DSTATE_k != 0);

  /* If: '<S3>/If2' */
  if (rtb_LogicalOperator) {
    /* Outputs for IfAction SubSystem: '<S3>/F01_03_Direction_Detection' incorporates:
     *  ActionPort: '<S12>/Action Port'
     */
    /* UnitDelay: '<S12>/UnitDelay3' */
    rtDW->UnitDelay3 = rtDW->Switch2_a;

    /* Sum: '<S12>/Sum2' incorporates:
     *  Constant: '<S11>/vec_hallToPos'
     *  Selector: '<S11>/Selector'
     *  UnitDelay: '<S12>/UnitDelay2'
     */
    rtb_Sum2_ii = (int8_t)(rtConstP.vec_hallToPos_Value[Sum] -
      rtDW->UnitDelay2_DSTATE_o);

    /* Switch: '<S12>/Switch2' incorporates:
     *  Constant: '<S12>/Constant20'
     *  Constant: '<S12>/Constant8'
     *  Logic: '<S12>/Logical Operator3'
     *  RelationalOperator: '<S12>/Relational Operator1'
     *  RelationalOperator: '<S12>/Relational Operator6'
     */
    if ((rtb_Sum2_ii == 1) || (rtb_Sum2_ii == -5)) {
      /* Switch: '<S12>/Switch2' incorporates:
       *  Constant: '<S12>/Constant24'
       */
      rtDW->Switch2_a = 1;
    } else {
      /* Switch: '<S12>/Switch2' incorporates:
       *  Constant: '<S12>/Constant23'
       */
      rtDW->Switch2_a = -1;
    }

    /* End of Switch: '<S12>/Switch2' */

    /* Update for UnitDelay: '<S12>/UnitDelay2' incorporates:
     *  Constant: '<S11>/vec_hallToPos'
     *  Selector: '<S11>/Selector'
     */
    rtDW->UnitDelay2_DSTATE_o = rtConstP.vec_hallToPos_Value[Sum];

    /* End of Outputs for SubSystem: '<S3>/F01_03_Direction_Detection' */
  }

  /* End of If: '<S3>/If2' */

  /* Outputs for Enabled SubSystem: '<S13>/Enabled Subsystem' incorporates:
   *  EnablePort: '<S17>/Enable'
   */
  /* Outputs for Enabled SubSystem: '<S13>/Subsystem' incorporates:
   *  EnablePort: '<S18>/Enable'
   */
  /* Logic: '<S13>/NOT' incorporates:
   *  Constant: '<S13>/b_angleMeasEna1'
   */
  if (!rtP->b_angleMeasEna) {
    /* If: '<S18>/If2' */
    if (rtb_LogicalOperator) {
      /* Outputs for IfAction SubSystem: '<S18>/Raw_Motor_Speed_Estimation' incorporates:
       *  ActionPort: '<S21>/Action Port'
       */
      /* SignalConversion generated from: '<S21>/z_counterRawPrev' incorporates:
       *  UnitDelay: '<S13>/UnitDelay3'
       */
      rtDW->z_counterRawPrev = rtDW->UnitDelay3_DSTATE;

      /* Sum: '<S21>/Sum7' incorporates:
       *  UnitDelay: '<S21>/UnitDelay4'
       */
      rtDW->Sum1_f = (int16_t)(rtDW->z_counterRawPrev - rtDW->UnitDelay4_DSTATE);

      /* Abs: '<S21>/Abs2' */
      if (rtDW->Sum1_f < 0) {
        rtDW->Sum1_f = (int16_t)-rtDW->Sum1_f;
      }

      /* End of Abs: '<S21>/Abs2' */

      /* Relay: '<S21>/dz_cntTrnsDet' */
      rtDW->dz_cntTrnsDet_Mode = ((rtDW->Sum1_f >= rtP->dz_cntTrnsDetHi) ||
        ((rtDW->Sum1_f > rtP->dz_cntTrnsDetLo) && rtDW->dz_cntTrnsDet_Mode));

      /* Relay: '<S21>/dz_cntTrnsDet' */
      rtDW->dz_cntTrnsDet = rtDW->dz_cntTrnsDet_Mode;

      /* RelationalOperator: '<S21>/Relational Operator4' */
      rtb_RelationalOperator4 = (rtDW->Switch2_a != rtDW->UnitDelay3);

      /* Switch: '<S21>/Switch3' incorporates:
       *  Constant: '<S21>/Constant4'
       *  Logic: '<S21>/Logical Operator1'
       *  Switch: '<S21>/Switch2'
       *  UnitDelay: '<S21>/UnitDelay1'
       */
      if (rtb_RelationalOperator4 && rtDW->UnitDelay1_DSTATE_g) {
        rtDW->Sum1_f = 0;
      } else if (rtb_RelationalOperator4) {
        /* Switch: '<S21>/Switch3' incorporates:
         *  Switch: '<S21>/Switch2'
         *  UnitDelay: '<S13>/UnitDelay4'
         */
        rtDW->Sum1_f = rtDW->UnitDelay4_DSTATE_f;
      } else {
        /* Switch: '<S21>/Switch3' incorporates:
         *  Constant: '<S21>/cf_speedCoef'
         *  Gain: '<S21>/g_Ha'
         *  Product: '<S21>/Divide13'
         *  Sum: '<S21>/Sum13'
         *  Switch: '<S21>/Switch2'
         *  UnitDelay: '<S21>/UnitDelay2'
         *  UnitDelay: '<S21>/UnitDelay3'
         *  UnitDelay: '<S21>/UnitDelay5'
         */
        rtDW->Sum1_f = (int16_t)div_nzp_sus32((uint32_t)rtP->cf_speedCoef << 6,
          (int16_t)((int16_t)((int16_t)(rtDW->UnitDelay2_DSTATE +
          rtDW->UnitDelay3_DSTATE_k) + rtDW->UnitDelay5_DSTATE) +
                    rtDW->z_counterRawPrev));
      }

      /* End of Switch: '<S21>/Switch3' */

      /* Product: '<S21>/Divide11' incorporates:
       *  Switch: '<S21>/Switch3'
       */
      rtDW->Divide11 = (int16_t)(rtDW->Sum1_f * rtDW->Switch2_a);

      /* Update for UnitDelay: '<S21>/UnitDelay4' */
      rtDW->UnitDelay4_DSTATE = rtDW->z_counterRawPrev;

      /* Update for UnitDelay: '<S21>/UnitDelay2' incorporates:
       *  UnitDelay: '<S21>/UnitDelay3'
       */
      rtDW->UnitDelay2_DSTATE = rtDW->UnitDelay3_DSTATE_k;

      /* Update for UnitDelay: '<S21>/UnitDelay3' incorporates:
       *  UnitDelay: '<S21>/UnitDelay5'
       */
      rtDW->UnitDelay3_DSTATE_k = rtDW->UnitDelay5_DSTATE;

      /* Update for UnitDelay: '<S21>/UnitDelay5' */
      rtDW->UnitDelay5_DSTATE = rtDW->z_counterRawPrev;

      /* Update for UnitDelay: '<S21>/UnitDelay1' */
      rtDW->UnitDelay1_DSTATE_g = rtb_RelationalOperator4;

      /* End of Outputs for SubSystem: '<S18>/Raw_Motor_Speed_Estimation' */
    }

    /* End of If: '<S18>/If2' */

    /* Merge: '<S13>/Merge' incorporates:
     *  Product: '<S21>/Divide11'
     *  SignalConversion generated from: '<S18>/n_motRaw'
     */
    Merge = rtDW->Divide11;
  } else {
    /* Product: '<S17>/Product' incorporates:
     *  Constant: '<S17>/b_angleMeasEna1'
     *  Sum: '<S17>/Minus'
     *  UnitDelay: '<S17>/UnitDelay4'
     */
    Merge = (int16_t)(((uint16_t)(rtU->a_encoderCNT - rtDW->UnitDelay4_DSTATE_m)
                       * 977) >> 2);

    /* Sum: '<S20>/Sum' incorporates:
     *  Product: '<S17>/Product'
     *  Product: '<S20>/Product'
     *  Sum: '<S20>/Diff'
     *  UnitDelay: '<S20>/UD'
     *
     * Block description for '<S20>/Sum':
     *
     *  Add in CPU
     *
     * Block description for '<S20>/Diff':
     *
     *  Add in CPU
     *
     * Block description for '<S20>/UD':
     *
     *  Store in Global RAM
     */
    rtDW->Sum1_f = (int16_t)((((int16_t)(rtDW->UD_DSTATE - Merge) * 31785) >> 15)
      + Merge);

    /* Merge: '<S13>/Merge' incorporates:
     *  SignalConversion generated from: '<S17>/Out1'
     *  Sum: '<S20>/Sum'
     *
     * Block description for '<S20>/Sum':
     *
     *  Add in CPU
     */
    Merge = rtDW->Sum1_f;

    /* Update for UnitDelay: '<S17>/UnitDelay4' */
    rtDW->UnitDelay4_DSTATE_m = rtU->a_encoderCNT;

    /* Update for UnitDelay: '<S20>/UD' incorporates:
     *  Sum: '<S20>/Sum'
     *
     * Block description for '<S20>/UD':
     *
     *  Store in Global RAM
     *
     * Block description for '<S20>/Sum':
     *
     *  Add in CPU
     */
    rtDW->UD_DSTATE = rtDW->Sum1_f;
  }

  /* End of Logic: '<S13>/NOT' */
  /* End of Outputs for SubSystem: '<S13>/Subsystem' */
  /* End of Outputs for SubSystem: '<S13>/Enabled Subsystem' */

  /* Outputs for Atomic SubSystem: '<S13>/Counter' */
  /* Constant: '<S13>/Constant6' incorporates:
   *  Constant: '<S13>/z_maxCntRst2'
   */
  rtDW->Sum1_f = Counter(1, rtP->z_maxCntRst, rtb_LogicalOperator,
    &rtDW->Counter_c);

  /* End of Outputs for SubSystem: '<S13>/Counter' */

  /* Switch: '<S13>/Switch2' incorporates:
   *  Constant: '<S13>/z_maxCntRst'
   *  RelationalOperator: '<S13>/Relational Operator2'
   */
  if (rtDW->Sum1_f > rtP->z_maxCntRst) {
    /* Switch: '<S13>/Switch2' incorporates:
     *  Constant: '<S13>/Constant4'
     */
    Merge = 0;
  }

  /* End of Switch: '<S13>/Switch2' */

  /* Abs: '<S13>/Abs5' incorporates:
   *  Switch: '<S13>/Switch2'
   */
  if (Merge < 0) {
    /* Abs: '<S13>/Abs5' */
    Abs5 = (int16_t)-Merge;
  } else {
    /* Abs: '<S13>/Abs5' */
    Abs5 = Merge;
  }

  /* End of Abs: '<S13>/Abs5' */

  /* Relay: '<S13>/n_commDeacv' incorporates:
   *  Abs: '<S13>/Abs5'
   */
  rtDW->n_commDeacv_Mode = ((Abs5 >= rtP->n_commDeacvHi) || ((Abs5 >
    rtP->n_commAcvLo) && rtDW->n_commDeacv_Mode));

  /* Logic: '<S13>/Logical Operator3' incorporates:
   *  Constant: '<S13>/b_angleMeasEna'
   *  Logic: '<S13>/Logical Operator1'
   *  Logic: '<S13>/Logical Operator2'
   *  Relay: '<S13>/n_commDeacv'
   */
  rtb_LogicalOperator = (rtP->b_angleMeasEna || (rtDW->n_commDeacv_Mode &&
    (!rtDW->dz_cntTrnsDet)));

  /* UnitDelay: '<S2>/UnitDelay2' */
  rtb_RelationalOperator4 = rtDW->UnitDelay2_DSTATE_g;

  /* UnitDelay: '<S2>/UnitDelay5' */
  rtb_n_commDeacv = rtDW->UnitDelay5_DSTATE_l;

  /* Saturate: '<S1>/Saturation' */
  rtDW->sigIdx = rtU->i_phaAB << 4;
  if (rtDW->sigIdx >= 27200) {
    rtDW->Saturation = 27200;
  } else if (rtDW->sigIdx <= -27200) {
    rtDW->Saturation = -27200;
  } else {
    rtDW->Saturation = (int16_t)(rtU->i_phaAB << 4);
  }

  /* End of Saturate: '<S1>/Saturation' */

  /* Saturate: '<S1>/Saturation1' */
  rtDW->sigIdx = rtU->i_phaBC << 4;
  if (rtDW->sigIdx >= 27200) {
    rtDW->Saturation1 = 27200;
  } else if (rtDW->sigIdx <= -27200) {
    rtDW->Saturation1 = -27200;
  } else {
    rtDW->Saturation1 = (int16_t)(rtU->i_phaBC << 4);
  }

  /* End of Saturate: '<S1>/Saturation1' */

  /* If: '<S3>/If1' incorporates:
   *  Constant: '<S3>/b_angleMeasEna'
   */
  if (!rtP->b_angleMeasEna) {
    /* Outputs for IfAction SubSystem: '<S3>/F01_05_Electrical_Angle_Estimation' incorporates:
     *  ActionPort: '<S14>/Action Port'
     */
    /* Switch: '<S14>/Switch3' incorporates:
     *  Constant: '<S11>/vec_hallToPos'
     *  Constant: '<S14>/Constant16'
     *  Constant: '<S14>/Constant2'
     *  RelationalOperator: '<S14>/Relational Operator7'
     *  Selector: '<S11>/Selector'
     *  Sum: '<S14>/Sum1'
     */
    if (rtDW->Switch2_a == 1) {
      rtb_Sum2_ii = rtConstP.vec_hallToPos_Value[Sum];
    } else {
      rtb_Sum2_ii = (int8_t)(rtConstP.vec_hallToPos_Value[Sum] + 1);
    }

    /* End of Switch: '<S14>/Switch3' */

    /* Switch: '<S14>/Switch2' incorporates:
     *  MinMax: '<S14>/MinMax'
     *  Product: '<S14>/Divide1'
     *  Product: '<S14>/Divide3'
     *  Sum: '<S14>/Sum3'
     */
    if (rtb_LogicalOperator) {
      /* MinMax: '<S14>/MinMax' */
      if (rtDW->Sum1_f <= rtDW->z_counterRawPrev) {
        rtDW->Switch2_g = rtDW->Sum1_f;
      } else {
        rtDW->Switch2_g = rtDW->z_counterRawPrev;
      }

      rtDW->Switch2_g = (int16_t)(((int16_t)((int16_t)((rtDW->Switch2_g << 14) /
        rtDW->z_counterRawPrev) * rtDW->Switch2_a) + (rtb_Sum2_ii << 14)) >> 2);
    } else {
      rtDW->Switch2_g = (int16_t)(rtb_Sum2_ii << 12);
    }

    /* End of Switch: '<S14>/Switch2' */

    /* MinMax: '<S14>/MinMax1' incorporates:
     *  Constant: '<S14>/Constant1'
     *  Switch: '<S14>/Switch2'
     */
    if (rtDW->Switch2_g < 0) {
      rtDW->Switch2_g = 0;
    }

    /* SignalConversion: '<S14>/Signal Conversion2' incorporates:
     *  Merge: '<S3>/Merge'
     *  MinMax: '<S14>/MinMax1'
     *  Product: '<S14>/Divide2'
     */
    rtDW->DataTypeConversion2 = (uint16_t)((15 * rtDW->Switch2_g) >> 3);

    /* End of Outputs for SubSystem: '<S3>/F01_05_Electrical_Angle_Estimation' */
  } else {
    /* Outputs for IfAction SubSystem: '<S3>/F01_06_Electrical_Angle_Measurement' incorporates:
     *  ActionPort: '<S15>/Action Port'
     */
    /* SignalConversion generated from: '<S15>/a_elecAngle' incorporates:
     *  Constant: '<S15>/Constant1'
     *  Constant: '<S15>/Constant3'
     *  Constant: '<S15>/n_polePairs1'
     *  DataTypeConversion: '<S25>/Data Type Conversion1'
     *  DataTypeConversion: '<S25>/Data Type Conversion2'
     *  DataTypeConversion: '<S25>/Data Type Conversion4'
     *  Gain: '<S35>/Gain'
     *  Merge: '<S3>/Merge'
     *  Product: '<S23>/Product'
     *  Product: '<S27>/Product'
     *  Sum: '<S25>/Sum1'
     *  Switch: '<S25>/Switch'
     */
    rtDW->DataTypeConversion2 = (uint16_t)((((uint32_t)(((uint64_t)(uint16_t)
      (((uint32_t)((45ULL * rtP->a_fcpr) >> 6) * rtU->a_encoderCNT) >> 16) << 22)
      / 45ULL) - 89478485U) * rtP->n_polePairs * 45ULL) >> 22);

    /* End of Outputs for SubSystem: '<S3>/F01_06_Electrical_Angle_Measurement' */
  }

  /* End of If: '<S3>/If1' */

  /* If: '<S7>/If1' incorporates:
   *  Constant: '<S1>/z_ctrlTypSel'
   */
  rtb_Sum2_ii = rtDW->If1_ActiveSubsystem;
  rtAction = -1;
  if (rtP->z_ctrlTypSel == 2) {
    rtAction = 0;
  }

  rtDW->If1_ActiveSubsystem = rtAction;
  if ((rtb_Sum2_ii != rtAction) && (rtb_Sum2_ii == 0)) {
    /* Disable for If: '<S64>/If2' */
    if (rtDW->If2_ActiveSubsystem_a == 0) {
      /* Disable for Outport: '<S69>/iq' incorporates:
       *  DataTypeConversion: '<S75>/Data Type Conversion'
       * */
      rtDW->DataTypeConversion[0] = 0;

      /* Disable for Abs: '<S69>/Abs5' incorporates:
       *  Outport: '<S69>/iqAbs'
       */
      rtDW->Abs5_d = 0;

      /* Disable for SignalConversion generated from: '<S69>/id' incorporates:
       *  Outport: '<S69>/id'
       */
      rtDW->OutportBufferForid_f = 0;
    }

    rtDW->If2_ActiveSubsystem_a = -1;

    /* End of Disable for If: '<S64>/If2' */

    /* Disable for Interpolation_n-D generated from: '<S71>/r_sin_M1' incorporates:
     *  Outport: '<S64>/r_sin'
     */
    rtDW->r_sin_M1_1 = 0;

    /* Disable for Interpolation_n-D generated from: '<S71>/r_cos_M1' incorporates:
     *  Outport: '<S64>/r_cos'
     */
    rtDW->r_cos_M1_1 = 0;

    /* Disable for Outport: '<Root>/iq' incorporates:
     *  Outport: '<S64>/iq'
     */
    rtY->iq = 0;

    /* Disable for Outport: '<Root>/id' incorporates:
     *  Outport: '<S64>/id'
     */
    rtY->id = 0;

    /* Disable for SignalConversion generated from: '<S64>/iqAbs' incorporates:
     *  Outport: '<S64>/iqAbs'
     */
    rtDW->OutportBufferForiqAbs = 0;
  }

  if (rtAction == 0) {
    /* Outputs for IfAction SubSystem: '<S7>/Clarke_Park_Transform_Forward' incorporates:
     *  ActionPort: '<S64>/Action Port'
     */
    /* If: '<S68>/If1' incorporates:
     *  Constant: '<S68>/z_selPhaCurMeasABC'
     */
    if (rtP->z_selPhaCurMeasABC == 0) {
      /* Outputs for IfAction SubSystem: '<S68>/Clarke_PhasesAB' incorporates:
       *  ActionPort: '<S72>/Action Port'
       */
      /* Gain: '<S72>/one_by_sqrt3' incorporates:
       *  Merge: '<S68>/Merge1'
       *  Saturate: '<S1>/Saturation'
       *  Saturate: '<S1>/Saturation1'
       *  SignalConversion generated from: '<S72>/i_phaA'
       *  Sum: '<S72>/a_plus_2b'
       */
      rtDW->Switch2_g = (int16_t)(((int16_t)((int16_t)(rtDW->Saturation +
        rtDW->Saturation1) + rtDW->Saturation1) * 18919) >> 15);

      /* End of Outputs for SubSystem: '<S68>/Clarke_PhasesAB' */
    } else if (rtP->z_selPhaCurMeasABC == 1) {
      /* Outputs for IfAction SubSystem: '<S68>/Clarke_PhasesBC' incorporates:
       *  ActionPort: '<S74>/Action Port'
       */
      /* Sum: '<S74>/Sum3' incorporates:
       *  Saturate: '<S1>/Saturation'
       *  Saturate: '<S1>/Saturation1'
       */
      rtDW->sigIdx = rtDW->Saturation - rtDW->Saturation1;
      if (rtDW->sigIdx > 32767) {
        rtDW->sigIdx = 32767;
      } else if (rtDW->sigIdx < -32768) {
        rtDW->sigIdx = -32768;
      }

      /* Gain: '<S74>/Gain2' incorporates:
       *  Merge: '<S68>/Merge1'
       *  Sum: '<S74>/Sum3'
       */
      rtDW->sigIdx *= 18919;
      rtDW->Switch2_g = (int16_t)(((rtDW->sigIdx < 0 ? 32767 : 0) + rtDW->sigIdx)
        >> 15);

      /* Sum: '<S74>/Sum1' incorporates:
       *  Merge: '<S68>/Merge2'
       *  Saturate: '<S1>/Saturation'
       *  Saturate: '<S1>/Saturation1'
       */
      rtDW->sigIdx = -rtDW->Saturation - rtDW->Saturation1;
      if (rtDW->sigIdx > 32767) {
        rtDW->sigIdx = 32767;
      } else if (rtDW->sigIdx < -32768) {
        rtDW->sigIdx = -32768;
      }

      rtDW->Saturation = (int16_t)rtDW->sigIdx;

      /* End of Sum: '<S74>/Sum1' */
      /* End of Outputs for SubSystem: '<S68>/Clarke_PhasesBC' */
    } else {
      /* Outputs for IfAction SubSystem: '<S68>/Clarke_PhasesAC' incorporates:
       *  ActionPort: '<S73>/Action Port'
       */
      /* Sum: '<S73>/a_plus_2b' incorporates:
       *  Saturate: '<S1>/Saturation'
       *  Saturate: '<S1>/Saturation1'
       *  SignalConversion generated from: '<S73>/i_phaA'
       */
      rtDW->sigIdx = (-rtDW->Saturation - rtDW->Saturation1) - rtDW->Saturation1;
      if (rtDW->sigIdx > 32767) {
        rtDW->sigIdx = 32767;
      } else if (rtDW->sigIdx < -32768) {
        rtDW->sigIdx = -32768;
      }

      /* Gain: '<S73>/one_by_sqrt3' incorporates:
       *  Merge: '<S68>/Merge1'
       *  Sum: '<S73>/a_plus_2b'
       */
      rtDW->Switch2_g = (int16_t)((18919 * rtDW->sigIdx) >> 15);

      /* End of Outputs for SubSystem: '<S68>/Clarke_PhasesAC' */
    }

    /* End of If: '<S68>/If1' */

    /* PreLookup generated from: '<S71>/a_elecAngle_XA' incorporates:
     *  Merge: '<S3>/Merge'
     */
    rtb_k = plook_u8u16u16n15_even8ca_gs(rtDW->DataTypeConversion2, 0U, &rtDW->f);

    /* Interpolation_n-D generated from: '<S71>/r_sin_M1' incorporates:
     *  PreLookup generated from: '<S71>/a_elecAngle_XA'
     */
    rtDW->r_sin_M1_1 = intrp1d_s16s32s32u8u16n15la_s(rtb_k, rtDW->f,
      rtConstP.r_sin_M1_1_Table, 180U);

    /* Interpolation_n-D generated from: '<S71>/r_cos_M1' incorporates:
     *  PreLookup generated from: '<S71>/a_elecAngle_XA'
     */
    rtDW->r_cos_M1_1 = intrp1d_s16s32s32u8u16n15la_s(rtb_k, rtDW->f,
      rtConstP.r_cos_M1_1_Table, 180U);

    /* If: '<S64>/If2' incorporates:
     *  Constant: '<S69>/cf_currFilt'
     */
    rtb_Sum2_ii = rtDW->If2_ActiveSubsystem_a;
    rtAction = -1;
    if (rtU->b_motEna) {
      rtAction = 0;
    }

    rtDW->If2_ActiveSubsystem_a = rtAction;
    if ((rtb_Sum2_ii != rtAction) && (rtb_Sum2_ii == 0)) {
      /* Disable for Outport: '<S69>/iq' incorporates:
       *  DataTypeConversion: '<S75>/Data Type Conversion'
       * */
      rtDW->DataTypeConversion[0] = 0;

      /* Disable for Abs: '<S69>/Abs5' incorporates:
       *  Outport: '<S69>/iqAbs'
       */
      rtDW->Abs5_d = 0;

      /* Disable for SignalConversion generated from: '<S69>/id' incorporates:
       *  Outport: '<S69>/id'
       */
      rtDW->OutportBufferForid_f = 0;
    }

    if (rtAction == 0) {
      if (rtb_Sum2_ii != 0) {
        /* SystemReset for IfAction SubSystem: '<S64>/Current_Filtering' incorporates:
         *  ActionPort: '<S69>/Action Port'
         */
        /* SystemReset for Atomic SubSystem: '<S69>/Low_Pass_Filter' */
        /* SystemReset for If: '<S64>/If2' */
        Low_Pass_Filter_Reset(&rtDW->Low_Pass_Filter_e);

        /* End of SystemReset for SubSystem: '<S69>/Low_Pass_Filter' */
        /* End of SystemReset for SubSystem: '<S64>/Current_Filtering' */
      }

      /* Sum: '<S70>/Sum6' incorporates:
       *  Interpolation_n-D generated from: '<S71>/r_cos_M1'
       *  Interpolation_n-D generated from: '<S71>/r_sin_M1'
       *  Merge: '<S68>/Merge1'
       *  Merge: '<S68>/Merge2'
       *  Product: '<S70>/Divide1'
       *  Product: '<S70>/Divide4'
       */
      rtDW->sigIdx = (int16_t)((rtDW->Switch2_g * rtDW->r_cos_M1_1) >> 14) -
        (int16_t)((rtDW->Saturation * rtDW->r_sin_M1_1) >> 14);
      if (rtDW->sigIdx > 32767) {
        rtDW->sigIdx = 32767;
      } else if (rtDW->sigIdx < -32768) {
        rtDW->sigIdx = -32768;
      }

      /* Outputs for IfAction SubSystem: '<S64>/Current_Filtering' incorporates:
       *  ActionPort: '<S69>/Action Port'
       */
      /* SignalConversion generated from: '<S69>/Low_Pass_Filter' incorporates:
       *  Sum: '<S70>/Sum6'
       */
      rtDW->TmpSignalConversionAtLow_Pa[0] = (int16_t)rtDW->sigIdx;

      /* End of Outputs for SubSystem: '<S64>/Current_Filtering' */

      /* Sum: '<S70>/Sum1' incorporates:
       *  Interpolation_n-D generated from: '<S71>/r_cos_M1'
       *  Interpolation_n-D generated from: '<S71>/r_sin_M1'
       *  Merge: '<S68>/Merge1'
       *  Merge: '<S68>/Merge2'
       *  Product: '<S70>/Divide2'
       *  Product: '<S70>/Divide3'
       */
      rtDW->sigIdx = (int16_t)((rtDW->Saturation * rtDW->r_cos_M1_1) >> 14) +
        (int16_t)((rtDW->Switch2_g * rtDW->r_sin_M1_1) >> 14);
      if (rtDW->sigIdx > 32767) {
        rtDW->sigIdx = 32767;
      } else if (rtDW->sigIdx < -32768) {
        rtDW->sigIdx = -32768;
      }

      /* Outputs for IfAction SubSystem: '<S64>/Current_Filtering' incorporates:
       *  ActionPort: '<S69>/Action Port'
       */
      /* SignalConversion generated from: '<S69>/Low_Pass_Filter' incorporates:
       *  Sum: '<S70>/Sum1'
       */
      rtDW->TmpSignalConversionAtLow_Pa[1] = (int16_t)rtDW->sigIdx;

      /* Outputs for Atomic SubSystem: '<S69>/Low_Pass_Filter' */
      Low_Pass_Filter(rtDW->TmpSignalConversionAtLow_Pa, rtP->cf_currFilt,
                      rtDW->DataTypeConversion, &rtDW->Low_Pass_Filter_e);

      /* End of Outputs for SubSystem: '<S69>/Low_Pass_Filter' */

      /* Abs: '<S69>/Abs5' incorporates:
       *  Constant: '<S69>/cf_currFilt'
       */
      if (rtDW->DataTypeConversion[0] < 0) {
        /* Abs: '<S69>/Abs5' */
        rtDW->Abs5_d = (int16_t)-rtDW->DataTypeConversion[0];
      } else {
        /* Abs: '<S69>/Abs5' */
        rtDW->Abs5_d = rtDW->DataTypeConversion[0];
      }

      /* End of Abs: '<S69>/Abs5' */

      /* SignalConversion generated from: '<S69>/id' */
      rtDW->OutportBufferForid_f = rtDW->DataTypeConversion[1];

      /* End of Outputs for SubSystem: '<S64>/Current_Filtering' */
    }

    /* End of If: '<S64>/If2' */

    /* Outport: '<Root>/id' incorporates:
     *  SignalConversion generated from: '<S64>/id'
     *  SignalConversion generated from: '<S69>/id'
     */
    rtY->id = rtDW->OutportBufferForid_f;

    /* Outport: '<Root>/iq' incorporates:
     *  SignalConversion generated from: '<S64>/iq'
     */
    rtY->iq = rtDW->DataTypeConversion[0];

    /* SignalConversion generated from: '<S64>/iqAbs' incorporates:
     *  Abs: '<S69>/Abs5'
     */
    rtDW->OutportBufferForiqAbs = rtDW->Abs5_d;

    /* End of Outputs for SubSystem: '<S7>/Clarke_Park_Transform_Forward' */
  }

  /* End of If: '<S7>/If1' */

  /* Chart: '<S1>/Task_Scheduler' incorporates:
   *  Constant: '<S1>/b_cruiseCtrlEna'
   *  Constant: '<S1>/z_ctrlTypSel'
   *  UnitDelay: '<S2>/UnitDelay2'
   *  UnitDelay: '<S2>/UnitDelay5'
   *  UnitDelay: '<S2>/UnitDelay6'
   *  UnitDelay: '<S8>/UnitDelay4'
   */
  if (rtDW->UnitDelay2_DSTATE_g) {
    /* Outputs for Function Call SubSystem: '<S1>/F02_Diagnostics' */
    /* If: '<S4>/If2' incorporates:
     *  Constant: '<S36>/CTRL_COMM2'
     *  Constant: '<S36>/t_errDequal'
     *  Constant: '<S36>/t_errQual'
     *  Constant: '<S4>/b_diagEna'
     *  RelationalOperator: '<S36>/Relational Operator2'
     */
    if (rtP->b_diagEna) {
      /* Outputs for IfAction SubSystem: '<S4>/Diagnostics_Enabled' incorporates:
       *  ActionPort: '<S36>/Action Port'
       */
      /* Switch: '<S36>/Switch3' incorporates:
       *  Abs: '<S13>/Abs5'
       *  Abs: '<S36>/Abs4'
       *  Constant: '<S13>/n_stdStillDet'
       *  Constant: '<S36>/CTRL_COMM4'
       *  Constant: '<S36>/r_errInpTgtThres'
       *  Logic: '<S36>/Logical Operator1'
       *  RelationalOperator: '<S13>/Relational Operator9'
       *  RelationalOperator: '<S36>/Relational Operator7'
       *  S-Function (sfix_bitop): '<S36>/Bitwise Operator1'
       *  UnitDelay: '<S36>/UnitDelay'
       *  UnitDelay: '<S8>/UnitDelay4'
       */
      if ((rtDW->Switch1 & 4U) != 0U) {
        rtb_Compare_j = true;
      } else {
        if (rtDW->UnitDelay4_DSTATE_a < 0) {
          /* Abs: '<S36>/Abs4' incorporates:
           *  UnitDelay: '<S8>/UnitDelay4'
           */
          rtDW->Switch2_g = (int16_t)-rtDW->UnitDelay4_DSTATE_a;
        } else {
          /* Abs: '<S36>/Abs4' incorporates:
           *  UnitDelay: '<S8>/UnitDelay4'
           */
          rtDW->Switch2_g = rtDW->UnitDelay4_DSTATE_a;
        }

        rtb_Compare_j = (rtU->b_motEna && (Abs5 < rtP->n_stdStillDet) &&
                         (rtDW->Switch2_g > rtP->r_errInpTgtThres));
      }

      /* End of Switch: '<S36>/Switch3' */

      /* Sum: '<S36>/Sum' incorporates:
       *  Constant: '<S36>/CTRL_COMM'
       *  Constant: '<S36>/CTRL_COMM1'
       *  DataTypeConversion: '<S36>/Data Type Conversion3'
       *  Gain: '<S36>/g_Hb'
       *  Gain: '<S36>/g_Hb1'
       *  RelationalOperator: '<S36>/Relational Operator1'
       *  RelationalOperator: '<S36>/Relational Operator3'
       */
      rtb_k = (uint8_t)(((uint32_t)((Sum == 7) << 1) + (uint32_t)(Sum == 0)) +
                        (uint32_t)(rtb_Compare_j << 2));

      /* Outputs for Atomic SubSystem: '<S36>/Debounce_Filter' */
      Debounce_Filter((rtb_k != 0), rtP->t_errQual, rtP->t_errDequal,
                      &rtDW->Merge_a, &rtDW->Debounce_Filter_e);

      /* End of Outputs for SubSystem: '<S36>/Debounce_Filter' */

      /* Outputs for Atomic SubSystem: '<S36>/either_edge' */
      rtb_Compare_j = either_edge(rtDW->Merge_a, &rtDW->either_edge_j);

      /* End of Outputs for SubSystem: '<S36>/either_edge' */

      /* Switch: '<S36>/Switch1' incorporates:
       *  Constant: '<S36>/CTRL_COMM2'
       *  Constant: '<S36>/t_errDequal'
       *  Constant: '<S36>/t_errQual'
       *  RelationalOperator: '<S36>/Relational Operator2'
       */
      if (rtb_Compare_j) {
        /* Switch: '<S36>/Switch1' */
        rtDW->Switch1 = rtb_k;
      }

      /* End of Switch: '<S36>/Switch1' */
      /* End of Outputs for SubSystem: '<S4>/Diagnostics_Enabled' */
    }

    /* End of If: '<S4>/If2' */

    /* SignalConversion generated from: '<S4>/b_errFlag' */
    rtDW->OutportBufferForb_errFlag = rtDW->Merge_a;

    /* Outport: '<Root>/z_errCode' incorporates:
     *  SignalConversion generated from: '<S4>/z_errCode '
     */
    rtY->z_errCode = rtDW->Switch1;

    /* End of Outputs for SubSystem: '<S1>/F02_Diagnostics' */

    /* Outputs for Function Call SubSystem: '<S1>/F03_Control_Mode_Manager' */
    F03_Control_Mode_Manager(rtDW->OutportBufferForb_errFlag, rtU->b_motEna,
      rtP->b_cruiseCtrlEna, rtP->z_ctrlTypSel, rtU->z_ctrlModReq, rtU->r_inpTgt,
      rtDW->UnitDelay4_DSTATE_a, &rtb_k, &rtDW->Saturation, &rtDW->Merge1,
      &rtDW->Saturation1, rtP, &rtDW->F03_Control_Mode_Manager_f);

    /* End of Outputs for SubSystem: '<S1>/F03_Control_Mode_Manager' */

    /* Outputs for Function Call SubSystem: '<S7>/FOC' */
    FOC(rtP->z_ctrlTypSel, rtb_LogicalOperator, rtb_k, rtDW->Merge1,
        rtDW->Saturation, Merge, rtDW->OutportBufferForr_fieldWeak, rtY->id,
        rtY->iq, rtDW->VariantMergeForOutportVd_max,
        rtDW->VariantMergeForOutportVd_min, rtDW->VariantMergeForOutportVq_max,
        rtDW->VariantMergeForOutportVq_min, rtDW->OutportBufferForid_max,
        rtDW->Gain4, rtDW->Divide1_d, rtDW->Gain1, rtDW->Switch2_b,
        rtDW->Switch2_p, rtDW->Divide1, rtDW->Switch2_m,
        rtDW->UnitDelay4_DSTATE_a, rtP->b_cruiseCtrlEna, rtP, &rtDW->Merge_o,
        &rtDW->VariantMergeForOutportVq_Trq, &rtDW->FOC_h);

    /* End of Outputs for SubSystem: '<S7>/FOC' */
  } else if (rtDW->UnitDelay5_DSTATE_l) {
    /* Outputs for Function Call SubSystem: '<S1>/F03_Control_Mode_Manager' */
    F03_Control_Mode_Manager(rtDW->OutportBufferForb_errFlag, rtU->b_motEna,
      rtP->b_cruiseCtrlEna, rtP->z_ctrlTypSel, rtU->z_ctrlModReq, rtU->r_inpTgt,
      rtDW->UnitDelay4_DSTATE_a, &rtb_k, &rtDW->Saturation, &rtDW->Merge1,
      &rtDW->Saturation1, rtP, &rtDW->F03_Control_Mode_Manager_f);

    /* End of Outputs for SubSystem: '<S1>/F03_Control_Mode_Manager' */

    /* Outputs for Function Call SubSystem: '<S1>/F04_Field_Weakening' */
    /* If: '<S6>/If3' incorporates:
     *  Constant: '<S1>/b_cruiseCtrlEna'
     *  Constant: '<S1>/z_ctrlTypSel'
     *  Constant: '<S6>/b_fieldWeakEna'
     *  UnitDelay: '<S8>/UnitDelay4'
     */
    if (rtP->b_fieldWeakEna) {
      /* Outputs for IfAction SubSystem: '<S6>/Field_Weakening_Enabled' incorporates:
       *  ActionPort: '<S61>/Action Port'
       */
      /* Abs: '<S61>/Abs5' incorporates:
       *  DataTypeConversion: '<S49>/Data Type Conversion22'
       */
      if (rtDW->Saturation1 < 0) {
        rtDW->Saturation1 = (int16_t)-rtDW->Saturation1;
      }

      /* End of Abs: '<S61>/Abs5' */

      /* Switch: '<S63>/Switch2' incorporates:
       *  Abs: '<S61>/Abs5'
       *  Constant: '<S61>/r_fieldWeakHi'
       *  Constant: '<S61>/r_fieldWeakLo'
       *  RelationalOperator: '<S63>/LowerRelop1'
       *  RelationalOperator: '<S63>/UpperRelop'
       *  Sum: '<S61>/Sum4'
       *  Switch: '<S63>/Switch'
       */
      if (rtDW->Saturation1 > rtP->r_fieldWeakHi) {
        rtDW->Saturation1 = rtP->r_fieldWeakHi;
      } else if (rtDW->Saturation1 < rtP->r_fieldWeakLo) {
        /* Switch: '<S63>/Switch' incorporates:
         *  Constant: '<S61>/r_fieldWeakLo'
         *  Sum: '<S61>/Sum4'
         */
        rtDW->Saturation1 = rtP->r_fieldWeakLo;
      }

      /* End of Switch: '<S63>/Switch2' */

      /* Product: '<S61>/Divide14' incorporates:
       *  Constant: '<S61>/r_fieldWeakHi'
       *  Constant: '<S61>/r_fieldWeakLo'
       *  Sum: '<S61>/Sum1'
       *  Sum: '<S61>/Sum3'
       *  Sum: '<S61>/Sum4'
       */
      rtDW->f = (uint16_t)(((int16_t)(rtDW->Saturation1 - rtP->r_fieldWeakLo) <<
                            15) / (int16_t)(rtP->r_fieldWeakHi -
        rtP->r_fieldWeakLo));

      /* Switch: '<S62>/Switch2' incorporates:
       *  Abs: '<S13>/Abs5'
       *  Constant: '<S61>/n_fieldWeakAuthHi'
       *  Constant: '<S61>/n_fieldWeakAuthLo'
       *  RelationalOperator: '<S62>/LowerRelop1'
       *  RelationalOperator: '<S62>/UpperRelop'
       *  Switch: '<S62>/Switch'
       */
      if (Abs5 > rtP->n_fieldWeakAuthHi) {
        rtDW->Switch2_g = rtP->n_fieldWeakAuthHi;
      } else if (Abs5 < rtP->n_fieldWeakAuthLo) {
        /* Switch: '<S62>/Switch' incorporates:
         *  Constant: '<S61>/n_fieldWeakAuthLo'
         */
        rtDW->Switch2_g = rtP->n_fieldWeakAuthLo;
      } else {
        rtDW->Switch2_g = Abs5;
      }

      /* Product: '<S61>/Divide1' incorporates:
       *  Constant: '<S61>/n_fieldWeakAuthHi'
       *  Constant: '<S61>/n_fieldWeakAuthLo'
       *  Sum: '<S61>/Sum2'
       *  Sum: '<S61>/Sum4'
       *  Switch: '<S62>/Switch2'
       */
      rtDW->Divide1_m = (uint16_t)(((int16_t)(rtDW->Switch2_g -
        rtP->n_fieldWeakAuthLo) << 15) / (int16_t)(rtP->n_fieldWeakAuthHi -
        rtP->n_fieldWeakAuthLo));

      /* Switch: '<S61>/Switch1' incorporates:
       *  MinMax: '<S61>/MinMax1'
       *  Product: '<S61>/Divide1'
       *  Product: '<S61>/Divide14'
       *  RelationalOperator: '<S61>/Relational Operator6'
       */
      if (rtDW->f < rtDW->Divide1_m) {
        /* MinMax: '<S61>/MinMax' incorporates:
         *  Switch: '<S61>/Switch1'
         */
        if (rtDW->f < rtDW->Divide1_m) {
          rtDW->f = rtDW->Divide1_m;
        }

        /* End of MinMax: '<S61>/MinMax' */
      } else if (rtDW->Divide1_m <= rtDW->f) {
        /* MinMax: '<S61>/MinMax1' incorporates:
         *  Switch: '<S61>/Switch1'
         */
        rtDW->f = rtDW->Divide1_m;
      }

      /* End of Switch: '<S61>/Switch1' */

      /* Switch: '<S61>/Switch2' incorporates:
       *  Constant: '<S61>/CTRL_COMM2'
       *  Constant: '<S61>/a_phaAdvMax'
       *  Constant: '<S61>/id_fieldWeakMax'
       *  RelationalOperator: '<S61>/Relational Operator1'
       */
      if (rtP->z_ctrlTypSel == 2) {
        rtDW->Switch2_g = rtP->id_fieldWeakMax;
      } else {
        rtDW->Switch2_g = rtP->a_phaAdvMax;
      }

      /* Product: '<S61>/Divide3' incorporates:
       *  Switch: '<S61>/Switch1'
       *  Switch: '<S61>/Switch2'
       */
      rtDW->Divide3 = (int16_t)((rtDW->Switch2_g * rtDW->f) >> 15);

      /* End of Outputs for SubSystem: '<S6>/Field_Weakening_Enabled' */
    }

    /* End of If: '<S6>/If3' */

    /* SignalConversion generated from: '<S6>/r_fieldWeak ' incorporates:
     *  Product: '<S61>/Divide3'
     */
    rtDW->OutportBufferForr_fieldWeak = rtDW->Divide3;

    /* End of Outputs for SubSystem: '<S1>/F04_Field_Weakening' */

    /* Outputs for Function Call SubSystem: '<S7>/Motor_Limitations' */
    /* If: '<S67>/If1' incorporates:
     *  Abs: '<S13>/Abs5'
     *  Constant: '<S1>/z_ctrlTypSel'
     *  Constant: '<S231>/n_max'
     *  Constant: '<S232>/Constant'
     *  Constant: '<S234>/Constant'
     *  Constant: '<S238>/Constant'
     *  Constant: '<S238>/Constant1'
     *  Constant: '<S238>/cf_KbLimProt'
     *  Constant: '<S238>/cf_iqKiLimProt'
     *  Constant: '<S238>/cf_nKiLimProt'
     *  Constant: '<S295>/Vd_max1'
     *  Constant: '<S296>/Vd_max3'
     *  Logic: '<S231>/OR'
     *  Product: '<S231>/Divide1'
     *  RelationalOperator: '<S232>/Compare'
     *  RelationalOperator: '<S234>/Compare'
     *  SignalConversion generated from: '<S64>/iqAbs'
     *  Sum: '<S238>/Sum1'
     *  Sum: '<S238>/Sum2'
     *  VariantMerge generated from: '<S241>/Vd_max_margin'
     */
    if (rtP->z_ctrlTypSel == 2) {
      /* Outputs for IfAction SubSystem: '<S67>/Motor_Limitations_Enabled' incorporates:
       *  ActionPort: '<S231>/Action Port'
       */
      /* RelationalOperator: '<S233>/Compare' incorporates:
       *  Constant: '<S233>/Constant'
       */
      rtb_Compare_j = (rtb_k == 2);

      /* Product: '<S231>/Divide4' incorporates:
       *  Constant: '<S231>/i_max'
       *  SignalConversion generated from: '<S6>/r_fieldWeak '
       */
      rtDW->sigIdx = rtDW->OutportBufferForr_fieldWeak << 16;
      rtDW->sigIdx = (rtDW->sigIdx == INT32_MIN) && (rtP->i_max == -1) ?
        INT32_MAX : rtDW->sigIdx / rtP->i_max;
      if (rtDW->sigIdx < 0) {
        rtDW->sigIdx = 0;
      } else if (rtDW->sigIdx > 65535) {
        rtDW->sigIdx = 65535;
      }

      /* Product: '<S231>/Divide1' incorporates:
       *  Constant: '<S231>/i_max'
       *  Interpolation_n-D: '<S231>/iq_maxSca_M1'
       *  PreLookup: '<S231>/iq_maxSca_XA'
       *  Product: '<S231>/Divide4'
       */
      rtDW->Divide1_d = (int16_t)
        ((rtConstP.iq_maxSca_M1_Table[plook_u8u16_evenckag((uint16_t)
           rtDW->sigIdx, 0U, 655U)] * rtP->i_max) >> 16);

      /* Gain: '<S231>/Gain1' incorporates:
       *  Product: '<S231>/Divide1'
       */
      rtDW->Gain1 = (int16_t)-rtDW->Divide1_d;

      /* Outputs for Enabled SubSystem: '<S231>/Speed_Mode_Protection' incorporates:
       *  EnablePort: '<S235>/Enable'
       */
      if (rtb_Compare_j) {
        /* Switch: '<S239>/Switch2' incorporates:
         *  Gain: '<S231>/Gain1'
         *  Product: '<S231>/Divide1'
         *  RelationalOperator: '<S239>/LowerRelop1'
         *  RelationalOperator: '<S239>/UpperRelop'
         *  Switch: '<S239>/Switch'
         */
        if (rtY->iq > rtDW->Divide1_d) {
          rtDW->Switch2_g = rtDW->Divide1_d;
        } else if (rtY->iq < rtDW->Gain1) {
          /* Switch: '<S239>/Switch' incorporates:
           *  Gain: '<S231>/Gain1'
           */
          rtDW->Switch2_g = rtDW->Gain1;
        } else {
          rtDW->Switch2_g = rtY->iq;
        }

        /* Product: '<S235>/Divide1' incorporates:
         *  Constant: '<S235>/cf_iqKiLimProt'
         *  Sum: '<S235>/Sum3'
         *  Switch: '<S239>/Switch2'
         */
        rtDW->Divide1 = (int16_t)(rtDW->Switch2_g - rtY->iq) *
          rtP->cf_iqKiLimProt;
      }

      /* End of Outputs for SubSystem: '<S231>/Speed_Mode_Protection' */

      /* Outputs for Atomic SubSystem: '<S231>/Variant Subsystem' */
#if mcu_model == 1

      /* Outputs for Atomic SubSystem: '<S237>/GD32F103' */
      /* Abs: '<S297>/Abs5' incorporates:
       *  UnitDelay: '<S7>/UnitDelay4'
       */
      if (rtDW->VariantMergeForOutportVq_Trq < 0) {
        rtDW->Switch2_g = (int16_t)-rtDW->VariantMergeForOutportVq_Trq;
      } else {
        rtDW->Switch2_g = rtDW->VariantMergeForOutportVq_Trq;
      }

      /* VariantMerge generated from: '<S237>/Vq_max' incorporates:
       *  Abs: '<S297>/Abs5'
       *  Interpolation_n-D: '<S297>/GD32_Vq_max_M1'
       *  PreLookup: '<S297>/GD32_Vq_max_XA'
       */
      rtDW->VariantMergeForOutportVq_max =
        rtConstP.GD32_Vq_max_M1_Table[plook_u8s16_evenckag(rtDW->Switch2_g, 0,
        320U)];

      /* VariantMerge generated from: '<S237>/Vd_min' incorporates:
       *  Gain: '<S297>/Gain5'
       *  VariantMerge generated from: '<S237>/Vq_max'
       */
      rtDW->VariantMergeForOutportVd_min = (int16_t)-
        rtDW->VariantMergeForOutportVq_max;

      /* VariantMerge generated from: '<S237>/Vd_max' incorporates:
       *  Constant: '<S297>/Vd_max1'
       */
      rtDW->VariantMergeForOutportVd_max = 26032;

      /* VariantMerge generated from: '<S237>/Vq_min' incorporates:
       *  Gain: '<S297>/Gain3'
       */
      rtDW->VariantMergeForOutportVq_min = -26032;

      /* End of Outputs for SubSystem: '<S237>/GD32F103' */
#elif mcu_model == 0

      /* Outputs for Atomic SubSystem: '<S237>/STM32F103' */
      /* Abs: '<S298>/Abs5' incorporates:
       *  UnitDelay: '<S7>/UnitDelay4'
       */
      if (rtDW->VariantMergeForOutportVq_Trq < 0) {
        rtDW->Switch2_g = (int16_t)-rtDW->VariantMergeForOutportVq_Trq;
      } else {
        rtDW->Switch2_g = rtDW->VariantMergeForOutportVq_Trq;
      }

      /* VariantMerge generated from: '<S237>/Vq_max' incorporates:
       *  Abs: '<S298>/Abs5'
       *  Interpolation_n-D: '<S298>/STM32_Vq_max_M1'
       *  PreLookup: '<S298>/STM32_Vq_max_XA'
       */
      rtDW->VariantMergeForOutportVq_max =
        rtConstP.STM32_Vq_max_M1_Table[plook_u8s16_evenckag(rtDW->Switch2_g, 0,
        480U)];

      /* VariantMerge generated from: '<S237>/Vd_min' incorporates:
       *  Gain: '<S298>/Gain5'
       *  VariantMerge generated from: '<S237>/Vq_max'
       */
      rtDW->VariantMergeForOutportVd_min = (int16_t)-
        rtDW->VariantMergeForOutportVq_max;

      /* VariantMerge generated from: '<S237>/Vd_max' incorporates:
       *  Constant: '<S298>/Vd_max2'
       */
      rtDW->VariantMergeForOutportVd_max = 15040;

      /* VariantMerge generated from: '<S237>/Vq_min' incorporates:
       *  Gain: '<S298>/Gain3'
       */
      rtDW->VariantMergeForOutportVq_min = -15040;

      /* End of Outputs for SubSystem: '<S237>/STM32F103' */
#endif

      /* End of Outputs for SubSystem: '<S231>/Variant Subsystem' */

      /* Outputs for Enabled SubSystem: '<S231>/Torque_Mode_Protection' incorporates:
       *  EnablePort: '<S236>/Enable'
       */
      if (rtb_Compare_j || (rtb_k == 3)) {
        /* Outputs for Atomic SubSystem: '<S236>/Variant Subsystem1' */
#if mcu_model == 1

        /* Outputs for Atomic SubSystem: '<S241>/GD32F103' */
        rtb_VariantMergeForOutportVd_ma = 26032;

        /* End of Outputs for SubSystem: '<S241>/GD32F103' */
#elif mcu_model == 0

        /* Outputs for Atomic SubSystem: '<S241>/STM32F103' */
        rtb_VariantMergeForOutportVd_ma = 15040;

        /* End of Outputs for SubSystem: '<S241>/STM32F103' */
#endif

        /* End of Outputs for SubSystem: '<S236>/Variant Subsystem1' */

        /* DiscreteIntegrator: '<S276>/Integrator' incorporates:
         *  Constant: '<S295>/Vd_max1'
         *  Constant: '<S296>/Vd_max3'
         *  VariantMerge generated from: '<S241>/Vd_max_margin'
         */
        if (rtDW->Integrator_IC_LOADING != 0) {
          rtDW->Integrator_DSTATE = rtb_VariantMergeForOutportVd_ma;
        }

        /* Switch: '<S284>/Switch2' incorporates:
         *  DiscreteIntegrator: '<S276>/Integrator'
         *  RelationalOperator: '<S284>/LowerRelop1'
         *  RelationalOperator: '<S284>/UpperRelop'
         *  Switch: '<S284>/Switch'
         *  VariantMerge generated from: '<S237>/Vq_max'
         */
        if (rtDW->Integrator_DSTATE > rtDW->VariantMergeForOutportVq_max) {
          /* Switch: '<S284>/Switch2' */
          rtDW->Switch2_m = rtDW->VariantMergeForOutportVq_max;
        } else if (rtDW->Integrator_DSTATE < 0) {
          /* Switch: '<S284>/Switch' incorporates:
           *  Constant: '<S236>/Constant1'
           *  Switch: '<S284>/Switch2'
           */
          rtDW->Switch2_m = 0;
        } else {
          /* Switch: '<S284>/Switch2' */
          rtDW->Switch2_m = rtDW->Integrator_DSTATE;
        }

        /* End of Switch: '<S284>/Switch2' */

        /* Update for DiscreteIntegrator: '<S276>/Integrator' */
        rtDW->Integrator_IC_LOADING = 0U;

        /* Sum: '<S268>/SumI2' incorporates:
         *  DiscreteIntegrator: '<S276>/Integrator'
         *  Switch: '<S284>/Switch2'
         */
        rtDW->sigIdx = rtDW->Switch2_m - rtDW->Integrator_DSTATE;
        if (rtDW->sigIdx > 32767) {
          rtDW->sigIdx = 32767;
        } else if (rtDW->sigIdx < -32768) {
          rtDW->sigIdx = -32768;
        }

        /* Gain: '<S268>/Kb' */
        if (cf_KbLimProt > 255) {
          rtDW->Switch2_g = INT16_MAX;
        } else {
          rtDW->Switch2_g = (int16_t)(cf_KbLimProt << 7);
        }

        /* Sum: '<S268>/SumI4' incorporates:
         *  Abs: '<S13>/Abs5'
         *  Constant: '<S231>/n_max'
         *  Constant: '<S236>/cf_nKiLimProt'
         *  Gain: '<S268>/Kb'
         *  Product: '<S273>/IProd Out'
         *  Sum: '<S236>/Sum1'
         *  Sum: '<S268>/SumI2'
         */
        rtDW->i = (((int64_t)(((int16_t)(rtP->n_max - Abs5) * rtP->cf_nKiLimProt) >>
                              16) << 19) + rtDW->Switch2_g * rtDW->sigIdx) >> 3;
        if (rtDW->i > 2147483647LL) {
          rtDW->i = 2147483647LL;
        } else if (rtDW->i < -2147483648LL) {
          rtDW->i = -2147483648LL;
        }

        /* Update for DiscreteIntegrator: '<S276>/Integrator' incorporates:
         *  Sum: '<S268>/SumI4'
         */
        rtDW->sigIdx = ((int32_t)rtDW->i >> 16) + rtDW->Integrator_DSTATE;
        if (rtDW->sigIdx > 32767) {
          rtDW->sigIdx = 32767;
        } else if (rtDW->sigIdx < -32768) {
          rtDW->sigIdx = -32768;
        }

        rtDW->Integrator_DSTATE = (int16_t)rtDW->sigIdx;
      }

      /* End of Outputs for SubSystem: '<S231>/Torque_Mode_Protection' */

      /* Outputs for Enabled SubSystem: '<S231>/Voltage_Mode_Protection' incorporates:
       *  EnablePort: '<S238>/Enable'
       */
      if (rtb_k == 1) {
        /* Outputs for Atomic SubSystem: '<S238>/I_backCalc_fixdt' */
        I_backCalc_fixdt((int16_t)(rtDW->Divide1_d - rtDW->OutportBufferForiqAbs),
                         rtP->cf_iqKiLimProt, cf_KbLimProt, rtDW->Saturation, 0,
                         &rtDW->Switch2_b, &rtDW->I_backCalc_fixdt_c);

        /* End of Outputs for SubSystem: '<S238>/I_backCalc_fixdt' */

        /* Outputs for Atomic SubSystem: '<S238>/I_backCalc_fixdt1' */
        I_backCalc_fixdt((int16_t)(rtP->n_max - Abs5), rtP->cf_nKiLimProt,
                         cf_KbLimProt, rtDW->Saturation, 0, &rtDW->Switch2_p,
                         &rtDW->I_backCalc_fixdt1);

        /* End of Outputs for SubSystem: '<S238>/I_backCalc_fixdt1' */
      }

      /* End of Outputs for SubSystem: '<S231>/Voltage_Mode_Protection' */

      /* Gain: '<S231>/Gain4' incorporates:
       *  Abs: '<S13>/Abs5'
       *  Constant: '<S231>/i_max'
       *  Constant: '<S231>/n_max'
       *  Constant: '<S232>/Constant'
       *  Constant: '<S234>/Constant'
       *  Constant: '<S238>/Constant'
       *  Constant: '<S238>/Constant1'
       *  Constant: '<S238>/cf_KbLimProt'
       *  Constant: '<S238>/cf_iqKiLimProt'
       *  Constant: '<S238>/cf_nKiLimProt'
       *  Constant: '<S295>/Vd_max1'
       *  Constant: '<S296>/Vd_max3'
       *  Logic: '<S231>/OR'
       *  Product: '<S231>/Divide1'
       *  RelationalOperator: '<S232>/Compare'
       *  RelationalOperator: '<S234>/Compare'
       *  SignalConversion generated from: '<S64>/iqAbs'
       *  Sum: '<S238>/Sum1'
       *  Sum: '<S238>/Sum2'
       *  VariantMerge generated from: '<S241>/Vd_max_margin'
       */
      rtDW->Gain4 = (int16_t)-rtP->i_max;

      /* SignalConversion generated from: '<S231>/id_max' incorporates:
       *  Constant: '<S231>/i_max'
       */
      rtDW->OutportBufferForid_max = rtP->i_max;

      /* End of Outputs for SubSystem: '<S67>/Motor_Limitations_Enabled' */
    }

    /* End of If: '<S67>/If1' */
    /* End of Outputs for SubSystem: '<S7>/Motor_Limitations' */

    /* Outputs for Function Call SubSystem: '<S7>/FOC' */
    FOC(rtP->z_ctrlTypSel, rtb_LogicalOperator, rtb_k, rtDW->Merge1,
        rtDW->Saturation, Merge, rtDW->OutportBufferForr_fieldWeak, rtY->id,
        rtY->iq, rtDW->VariantMergeForOutportVd_max,
        rtDW->VariantMergeForOutportVd_min, rtDW->VariantMergeForOutportVq_max,
        rtDW->VariantMergeForOutportVq_min, rtDW->OutportBufferForid_max,
        rtDW->Gain4, rtDW->Divide1_d, rtDW->Gain1, rtDW->Switch2_b,
        rtDW->Switch2_p, rtDW->Divide1, rtDW->Switch2_m,
        rtDW->UnitDelay4_DSTATE_a, rtP->b_cruiseCtrlEna, rtP, &rtDW->Merge_o,
        &rtDW->VariantMergeForOutportVq_Trq, &rtDW->FOC_h);

    /* End of Outputs for SubSystem: '<S7>/FOC' */
  } else if (rtDW->UnitDelay6_DSTATE) {
    /* Outputs for Function Call SubSystem: '<S1>/F03_Control_Mode_Manager' */
    F03_Control_Mode_Manager(rtDW->OutportBufferForb_errFlag, rtU->b_motEna,
      rtP->b_cruiseCtrlEna, rtP->z_ctrlTypSel, rtU->z_ctrlModReq, rtU->r_inpTgt,
      rtDW->UnitDelay4_DSTATE_a, &rtb_k, &rtDW->Saturation, &rtDW->Merge1,
      &rtDW->Saturation1, rtP, &rtDW->F03_Control_Mode_Manager_f);

    /* End of Outputs for SubSystem: '<S1>/F03_Control_Mode_Manager' */

    /* Outputs for Function Call SubSystem: '<S7>/FOC' */
    FOC(rtP->z_ctrlTypSel, rtb_LogicalOperator, rtb_k, rtDW->Merge1,
        rtDW->Saturation, Merge, rtDW->OutportBufferForr_fieldWeak, rtY->id,
        rtY->iq, rtDW->VariantMergeForOutportVd_max,
        rtDW->VariantMergeForOutportVd_min, rtDW->VariantMergeForOutportVq_max,
        rtDW->VariantMergeForOutportVq_min, rtDW->OutportBufferForid_max,
        rtDW->Gain4, rtDW->Divide1_d, rtDW->Gain1, rtDW->Switch2_b,
        rtDW->Switch2_p, rtDW->Divide1, rtDW->Switch2_m,
        rtDW->UnitDelay4_DSTATE_a, rtP->b_cruiseCtrlEna, rtP, &rtDW->Merge_o,
        &rtDW->VariantMergeForOutportVq_Trq, &rtDW->FOC_h);

    /* End of Outputs for SubSystem: '<S7>/FOC' */
  }

  /* End of Chart: '<S1>/Task_Scheduler' */

  /* If: '<S7>/If2' incorporates:
   *  Constant: '<S1>/z_ctrlTypSel'
   *  Constant: '<S8>/CTRL_COMM1'
   *  Merge: '<S49>/Merge1'
   *  Merge: '<S78>/Merge'
   *  RelationalOperator: '<S8>/Relational Operator6'
   *  Switch: '<S8>/Switch2'
   */
  rtb_Sum2_ii = rtDW->If2_ActiveSubsystem;
  rtAction = -1;
  if (rtP->z_ctrlTypSel == 2) {
    rtb_VariantMergeForOutportVd_ma = rtDW->Merge_o;
    rtAction = 0;
  } else {
    rtb_VariantMergeForOutportVd_ma = rtDW->Merge1;
  }

  rtDW->If2_ActiveSubsystem = rtAction;
  if ((rtb_Sum2_ii != rtAction) && (rtb_Sum2_ii == 0)) {
    /* Disable for Gain: '<S76>/Gain4' incorporates:
     *  Outport: '<S65>/V_phaABC_FOC'
     */
    rtDW->Gain4_o[0] = 0;
    rtDW->Gain4_o[1] = 0;
    rtDW->Gain4_o[2] = 0;
  }

  if (rtAction == 0) {
    /* Outputs for IfAction SubSystem: '<S7>/Clarke_Park_Transform_Inverse' incorporates:
     *  ActionPort: '<S65>/Action Port'
     */
    /* Sum: '<S77>/Sum6' incorporates:
     *  Interpolation_n-D generated from: '<S71>/r_cos_M1'
     *  Interpolation_n-D generated from: '<S71>/r_sin_M1'
     *  Merge: '<S78>/Merge'
     *  Product: '<S77>/Divide1'
     *  Product: '<S77>/Divide4'
     *  VariantMerge generated from: '<S161>/Vq_Trq'
     */
    rtDW->sigIdx = (int16_t)((rtDW->VariantMergeForOutportVq_Trq *
      rtDW->r_cos_M1_1) >> 14) - (int16_t)((rtDW->Merge_o * rtDW->r_sin_M1_1) >>
      14);
    if (rtDW->sigIdx > 32767) {
      rtDW->sigIdx = 32767;
    } else if (rtDW->sigIdx < -32768) {
      rtDW->sigIdx = -32768;
    }

    /* Sum: '<S77>/Sum1' incorporates:
     *  Interpolation_n-D generated from: '<S71>/r_cos_M1'
     *  Interpolation_n-D generated from: '<S71>/r_sin_M1'
     *  Merge: '<S78>/Merge'
     *  Product: '<S77>/Divide2'
     *  Product: '<S77>/Divide3'
     *  VariantMerge generated from: '<S161>/Vq_Trq'
     */
    rtDW->i3 = (int16_t)((rtDW->VariantMergeForOutportVq_Trq * rtDW->r_sin_M1_1)
                         >> 14) + (int16_t)((rtDW->Merge_o * rtDW->r_cos_M1_1) >>
      14);
    if (rtDW->i3 > 32767) {
      rtDW->i3 = 32767;
    } else if (rtDW->i3 < -32768) {
      rtDW->i3 = -32768;
    }

    /* Gain: '<S76>/Gain1' incorporates:
     *  Sum: '<S77>/Sum1'
     */
    rtDW->i3 *= 14189;

    /* Sum: '<S76>/Sum6' incorporates:
     *  Gain: '<S76>/Gain1'
     *  Gain: '<S76>/Gain3'
     *  Sum: '<S77>/Sum6'
     */
    rtDW->i3 = (((rtDW->i3 < 0 ? 16383 : 0) + rtDW->i3) >> 14) - ((int16_t)
      ((rtDW->sigIdx < 0) + rtDW->sigIdx) >> 1);
    if (rtDW->i3 > 32767) {
      rtDW->i3 = 32767;
    } else if (rtDW->i3 < -32768) {
      rtDW->i3 = -32768;
    }

    /* Sum: '<S76>/Sum2' incorporates:
     *  Sum: '<S76>/Sum6'
     *  Sum: '<S77>/Sum6'
     */
    rtDW->i2 = -rtDW->sigIdx - rtDW->i3;
    if (rtDW->i2 > 32767) {
      rtDW->i2 = 32767;
    } else if (rtDW->i2 < -32768) {
      rtDW->i2 = -32768;
    }

    /* MinMax: '<S76>/MinMax1' incorporates:
     *  Sum: '<S77>/Sum6'
     */
    rtDW->Saturation = (int16_t)rtDW->sigIdx;

    /* MinMax: '<S76>/MinMax2' incorporates:
     *  MinMax: '<S76>/MinMax1'
     *  Sum: '<S77>/Sum6'
     */
    rtDW->Saturation1 = (int16_t)rtDW->sigIdx;

    /* MinMax: '<S76>/MinMax1' incorporates:
     *  Sum: '<S76>/Sum6'
     *  Sum: '<S77>/Sum6'
     */
    if (rtDW->sigIdx > rtDW->i3) {
      rtDW->Saturation = (int16_t)rtDW->i3;
    }

    /* MinMax: '<S76>/MinMax2' incorporates:
     *  Sum: '<S76>/Sum6'
     *  Sum: '<S77>/Sum6'
     */
    if (rtDW->sigIdx < rtDW->i3) {
      rtDW->Saturation1 = (int16_t)rtDW->i3;
    }

    /* MinMax: '<S76>/MinMax1' incorporates:
     *  Sum: '<S76>/Sum2'
     */
    if (rtDW->Saturation > rtDW->i2) {
      rtDW->Saturation = (int16_t)rtDW->i2;
    }

    /* MinMax: '<S76>/MinMax2' incorporates:
     *  Sum: '<S76>/Sum2'
     */
    if (rtDW->Saturation1 < rtDW->i2) {
      rtDW->Saturation1 = (int16_t)rtDW->i2;
    }

    /* Sum: '<S76>/Add' incorporates:
     *  MinMax: '<S76>/MinMax1'
     *  MinMax: '<S76>/MinMax2'
     */
    rtDW->i1 = rtDW->Saturation + rtDW->Saturation1;
    if (rtDW->i1 > 32767) {
      rtDW->i1 = 32767;
    } else if (rtDW->i1 < -32768) {
      rtDW->i1 = -32768;
    }

    /* Gain: '<S76>/Gain2' incorporates:
     *  Sum: '<S76>/Add'
     */
    rtDW->Switch2_g = (int16_t)(rtDW->i1 >> 1);

    /* Sum: '<S76>/Add1' incorporates:
     *  Gain: '<S76>/Gain2'
     *  Sum: '<S77>/Sum6'
     */
    rtDW->sigIdx -= rtDW->Switch2_g;
    if (rtDW->sigIdx > 32767) {
      rtDW->sigIdx = 32767;
    } else if (rtDW->sigIdx < -32768) {
      rtDW->sigIdx = -32768;
    }

    /* Gain: '<S76>/Gain4' incorporates:
     *  Sum: '<S76>/Add1'
     */
    rtDW->Gain4_o[0] = (int16_t)((18919 * rtDW->sigIdx) >> 14);

    /* Sum: '<S76>/Add1' incorporates:
     *  Gain: '<S76>/Gain2'
     *  Sum: '<S76>/Sum6'
     */
    rtDW->sigIdx = rtDW->i3 - rtDW->Switch2_g;
    if (rtDW->sigIdx > 32767) {
      rtDW->sigIdx = 32767;
    } else if (rtDW->sigIdx < -32768) {
      rtDW->sigIdx = -32768;
    }

    /* Gain: '<S76>/Gain4' incorporates:
     *  Sum: '<S76>/Add1'
     */
    rtDW->Gain4_o[1] = (int16_t)((18919 * rtDW->sigIdx) >> 14);

    /* Sum: '<S76>/Add1' incorporates:
     *  Gain: '<S76>/Gain2'
     *  Sum: '<S76>/Sum2'
     */
    rtDW->sigIdx = rtDW->i2 - rtDW->Switch2_g;
    if (rtDW->sigIdx > 32767) {
      rtDW->sigIdx = 32767;
    } else if (rtDW->sigIdx < -32768) {
      rtDW->sigIdx = -32768;
    }

    /* Gain: '<S76>/Gain4' incorporates:
     *  Sum: '<S76>/Add1'
     */
    rtDW->Gain4_o[2] = (int16_t)((18919 * rtDW->sigIdx) >> 14);

    /* End of Outputs for SubSystem: '<S7>/Clarke_Park_Transform_Inverse' */
  }

  /* End of If: '<S7>/If2' */

  /* If: '<S8>/If' incorporates:
   *  Constant: '<S1>/z_ctrlTypSel'
   *  Constant: '<S8>/CTRL_COMM2'
   *  Constant: '<S8>/CTRL_COMM3'
   *  Logic: '<S8>/Logical Operator1'
   *  Logic: '<S8>/Logical Operator2'
   *  RelationalOperator: '<S8>/Relational Operator1'
   *  RelationalOperator: '<S8>/Relational Operator2'
   */
  if (rtb_LogicalOperator && (rtP->z_ctrlTypSel == 2)) {
    /* Outputs for IfAction SubSystem: '<S8>/FOC_Method' incorporates:
     *  ActionPort: '<S306>/Action Port'
     */
    /* SignalConversion generated from: '<S306>/V_phaABC_FOC_in' incorporates:
     *  Gain: '<S76>/Gain4'
     *  Merge: '<S8>/Merge'
     */
    rtDW->Saturation = rtDW->Gain4_o[0];
    rtDW->Saturation1 = rtDW->Gain4_o[1];
    rtDW->Switch2_g = rtDW->Gain4_o[2];

    /* End of Outputs for SubSystem: '<S8>/FOC_Method' */
  } else if (rtb_LogicalOperator && (rtP->z_ctrlTypSel == 1)) {
    /* Outputs for IfAction SubSystem: '<S8>/SIN_Method' incorporates:
     *  ActionPort: '<S307>/Action Port'
     */
    /* Switch: '<S308>/Switch_PhaAdv' incorporates:
     *  Constant: '<S308>/a_elecPeriod2'
     *  Constant: '<S308>/b_fieldWeakEna'
     *  Merge: '<S3>/Merge'
     *  Product: '<S309>/Divide2'
     *  Product: '<S309>/Divide3'
     *  Sum: '<S308>/Sum3'
     *  Sum: '<S309>/Sum3'
     */
    if (rtP->b_fieldWeakEna) {
      /* Sum: '<S308>/Sum3' incorporates:
       *  Merge: '<S3>/Merge'
       *  Product: '<S308>/Product2'
       *  SignalConversion generated from: '<S6>/r_fieldWeak '
       */
      rtDW->f = (uint16_t)((uint16_t)((uint16_t)
        (rtDW->OutportBufferForr_fieldWeak * rtDW->Switch2_a) << 3) +
                           rtDW->DataTypeConversion2);
      rtDW->f = (uint16_t)(rtDW->f - (uint32_t)(((int32_t)(rtDW->f / 46080U) *
        360) << 7));
    } else {
      rtDW->f = rtDW->DataTypeConversion2;
    }

    /* End of Switch: '<S308>/Switch_PhaAdv' */

    /* PreLookup: '<S307>/a_elecAngle_XA' incorporates:
     *  Switch: '<S308>/Switch_PhaAdv'
     */
    Sum = plook_u8u16_evencka(rtDW->f, 0U, 256U, 180U);

    /* Product: '<S307>/Divide2' incorporates:
     *  Interpolation_n-D: '<S307>/r_sin3PhaA_M1'
     *  Interpolation_n-D: '<S307>/r_sin3PhaB_M1'
     *  Interpolation_n-D: '<S307>/r_sin3PhaC_M1'
     *  Merge: '<S8>/Merge'
     *  Switch: '<S8>/Switch2'
     */
    rtDW->Saturation = (int16_t)((rtb_VariantMergeForOutportVd_ma *
      rtConstP.r_sin3PhaA_M1_Table[Sum]) >> 14);
    rtDW->Saturation1 = (int16_t)((rtb_VariantMergeForOutportVd_ma *
      rtConstP.r_sin3PhaB_M1_Table[Sum]) >> 14);
    rtDW->Switch2_g = (int16_t)((rtb_VariantMergeForOutportVd_ma *
      rtConstP.r_sin3PhaC_M1_Table[Sum]) >> 14);

    /* End of Outputs for SubSystem: '<S8>/SIN_Method' */
  } else {
    /* Selector: '<S11>/Selector' incorporates:
     *  Constant: '<S11>/vec_hallToPos'
     */
    rtb_Sum2_ii = rtConstP.vec_hallToPos_Value[Sum];

    /* Outputs for IfAction SubSystem: '<S8>/COM_Method' incorporates:
     *  ActionPort: '<S305>/Action Port'
     */
    /* LookupNDDirect: '<S305>/z_commutMap_M1'
     *
     * About '<S305>/z_commutMap_M1':
     *  2-dimensional Direct Look-Up returning a Column,
     *  which is contiguous for column-major array
     *     Remove protection against out-of-range input in generated code: 'off'
     *   */
    if (rtb_Sum2_ii > 5) {
      rtb_Sum2_ii = 5;
    } else if (rtb_Sum2_ii < 0) {
      rtb_Sum2_ii = 0;
    }

    rtDW->sigIdx = rtb_Sum2_ii * 3;

    /* Product: '<S305>/Divide2' incorporates:
     *  LookupNDDirect: '<S305>/z_commutMap_M1'
     *  Merge: '<S8>/Merge'
     *  Switch: '<S8>/Switch2'
     *
     * About '<S305>/z_commutMap_M1':
     *  2-dimensional Direct Look-Up returning a Column,
     *  which is contiguous for column-major array
     *     Remove protection against out-of-range input in generated code: 'off'
     *   */
    rtDW->Saturation = (int16_t)(rtb_VariantMergeForOutportVd_ma *
      rtConstP.z_commutMap_M1_table[rtDW->sigIdx]);
    rtDW->Saturation1 = (int16_t)(rtConstP.z_commutMap_M1_table[rtDW->sigIdx + 1]
      * rtb_VariantMergeForOutportVd_ma);
    rtDW->Switch2_g = (int16_t)(rtConstP.z_commutMap_M1_table[rtDW->sigIdx + 2] *
      rtb_VariantMergeForOutportVd_ma);

    /* End of Outputs for SubSystem: '<S8>/COM_Method' */
  }

  /* End of If: '<S8>/If' */

  /* Update for UnitDelay: '<S10>/UnitDelay3' */
  rtDW->UnitDelay3_DSTATE_f = rtU->b_hallA;

  /* Update for UnitDelay: '<S10>/UnitDelay1' */
  rtDW->UnitDelay1_DSTATE = rtU->b_hallB;

  /* Update for UnitDelay: '<S10>/UnitDelay2' */
  rtDW->UnitDelay2_DSTATE_k = rtU->b_hallC;

  /* Update for UnitDelay: '<S13>/UnitDelay3' */
  rtDW->UnitDelay3_DSTATE = rtDW->Sum1_f;

  /* Update for UnitDelay: '<S13>/UnitDelay4' incorporates:
   *  Abs: '<S13>/Abs5'
   */
  rtDW->UnitDelay4_DSTATE_f = Abs5;

  /* Update for UnitDelay: '<S2>/UnitDelay2' incorporates:
   *  UnitDelay: '<S2>/UnitDelay6'
   */
  rtDW->UnitDelay2_DSTATE_g = rtDW->UnitDelay6_DSTATE;

  /* Update for UnitDelay: '<S2>/UnitDelay5' */
  rtDW->UnitDelay5_DSTATE_l = rtb_RelationalOperator4;

  /* Update for UnitDelay: '<S2>/UnitDelay6' */
  rtDW->UnitDelay6_DSTATE = rtb_n_commDeacv;

  /* Update for UnitDelay: '<S8>/UnitDelay4' incorporates:
   *  Switch: '<S8>/Switch2'
   */
  rtDW->UnitDelay4_DSTATE_a = rtb_VariantMergeForOutportVd_ma;

  /* Outport: '<Root>/DC_phaA' incorporates:
   *  DataTypeConversion: '<S8>/Data Type Conversion6'
   *  Merge: '<S8>/Merge'
   */
  rtY->DC_phaA = (int16_t)(rtDW->Saturation >> 4);

  /* Outport: '<Root>/DC_phaB' incorporates:
   *  DataTypeConversion: '<S8>/Data Type Conversion6'
   *  Merge: '<S8>/Merge'
   */
  rtY->DC_phaB = (int16_t)(rtDW->Saturation1 >> 4);

  /* Outport: '<Root>/DC_phaC' incorporates:
   *  DataTypeConversion: '<S8>/Data Type Conversion6'
   *  Merge: '<S8>/Merge'
   */
  rtY->DC_phaC = (int16_t)(rtDW->Switch2_g >> 4);

  /* Outport: '<Root>/n_mot' incorporates:
   *  DataTypeConversion: '<S1>/Data Type Conversion1'
   *  Switch: '<S13>/Switch2'
   */
  rtY->n_mot = (int16_t)(Merge >> 4);

  /* Outport: '<Root>/a_elecAngle' incorporates:
   *  DataTypeConversion: '<S1>/Data Type Conversion3'
   *  Merge: '<S3>/Merge'
   */
  rtY->a_elecAngle = (int16_t)(rtDW->DataTypeConversion2 >> 7);

  /* End of Outputs for SubSystem: '<Root>/BLDC_controller' */
}

/* Model initialize function */
void BLDC_controller_initialize(RT_MODEL *const rtM)
{
  P *rtP = ((P *) rtM->defaultParam);
  DW *rtDW = rtM->dwork;
  ExtU *rtU = (ExtU *) rtM->inputs;
  ExtY *rtY = (ExtY *) rtM->outputs;

  /* Registration code */

  /* states (dwork) */
  (void) memset((void *)rtDW, 0,
                sizeof(DW));

  /* external inputs */
  (void)memset(rtU, 0, sizeof(ExtU));

  /* external outputs */
  (void)memset(rtY, 0, sizeof(ExtY));

  {
    uint8_t z_ctrlMod;

    /* SystemInitialize for Atomic SubSystem: '<Root>/BLDC_controller' */
    /* Start for If: '<S7>/If1' */
    rtDW->If1_ActiveSubsystem = -1;

    /* Start for If: '<S7>/If2' */
    rtDW->If2_ActiveSubsystem = -1;

    /* InitializeConditions for UnitDelay: '<S13>/UnitDelay3' */
    rtDW->UnitDelay3_DSTATE = rtP->z_maxCntRst;

    /* InitializeConditions for UnitDelay: '<S2>/UnitDelay2' */
    rtDW->UnitDelay2_DSTATE_g = true;

    /* SystemInitialize for Enabled SubSystem: '<S13>/Subsystem' */
    /* SystemInitialize for IfAction SubSystem: '<S18>/Raw_Motor_Speed_Estimation' */
    /* SystemInitialize for SignalConversion generated from: '<S21>/z_counterRawPrev' incorporates:
     *  Outport: '<S21>/z_counter'
     */
    rtDW->z_counterRawPrev = rtP->z_maxCntRst;

    /* End of SystemInitialize for SubSystem: '<S18>/Raw_Motor_Speed_Estimation' */
    /* End of SystemInitialize for SubSystem: '<S13>/Subsystem' */

    /* SystemInitialize for Atomic SubSystem: '<S13>/Counter' */
    Counter_Init(0, &rtDW->Counter_c);

    /* End of SystemInitialize for SubSystem: '<S13>/Counter' */

    /* SystemInitialize for IfAction SubSystem: '<S7>/Clarke_Park_Transform_Forward' */
    /* Start for If: '<S64>/If2' */
    rtDW->If2_ActiveSubsystem_a = -1;

    /* SystemInitialize for IfAction SubSystem: '<S64>/Current_Filtering' */
    /* SystemInitialize for Atomic SubSystem: '<S69>/Low_Pass_Filter' */
    Low_Pass_Filter_Init(&rtDW->Low_Pass_Filter_e);

    /* End of SystemInitialize for SubSystem: '<S69>/Low_Pass_Filter' */
    /* End of SystemInitialize for SubSystem: '<S64>/Current_Filtering' */
    /* End of SystemInitialize for SubSystem: '<S7>/Clarke_Park_Transform_Forward' */

    /* SystemInitialize for Chart: '<S1>/Task_Scheduler' incorporates:
     *  SubSystem: '<S1>/F02_Diagnostics'
     */
    /* SystemInitialize for IfAction SubSystem: '<S4>/Diagnostics_Enabled' */
    /* SystemInitialize for Atomic SubSystem: '<S36>/Debounce_Filter' */
    Debounce_Filter_Init(&rtDW->Merge_a, &rtDW->Debounce_Filter_e);

    /* End of SystemInitialize for SubSystem: '<S36>/Debounce_Filter' */
    /* End of SystemInitialize for SubSystem: '<S4>/Diagnostics_Enabled' */

    /* SystemInitialize for Chart: '<S1>/Task_Scheduler' incorporates:
     *  SubSystem: '<S1>/F03_Control_Mode_Manager'
     */
    F03_Control_Mode_Manager_Init(&z_ctrlMod, &rtDW->Merge1,
      &rtDW->F03_Control_Mode_Manager_f);

    /* SystemInitialize for Chart: '<S1>/Task_Scheduler' incorporates:
     *  SubSystem: '<S7>/Motor_Limitations'
     */
    /* SystemInitialize for IfAction SubSystem: '<S67>/Motor_Limitations_Enabled' */
    /* SystemInitialize for Enabled SubSystem: '<S231>/Torque_Mode_Protection' */
    /* InitializeConditions for DiscreteIntegrator: '<S276>/Integrator' */
    rtDW->Integrator_IC_LOADING = 1U;

    /* End of SystemInitialize for SubSystem: '<S231>/Torque_Mode_Protection' */

    /* SystemInitialize for Enabled SubSystem: '<S231>/Voltage_Mode_Protection' */
    /* SystemInitialize for Atomic SubSystem: '<S238>/I_backCalc_fixdt' */
    I_backCalc_fixdt_Init(65536000, &rtDW->I_backCalc_fixdt_c);

    /* End of SystemInitialize for SubSystem: '<S238>/I_backCalc_fixdt' */

    /* SystemInitialize for Atomic SubSystem: '<S238>/I_backCalc_fixdt1' */
    I_backCalc_fixdt_Init(65536000, &rtDW->I_backCalc_fixdt1);

    /* End of SystemInitialize for SubSystem: '<S238>/I_backCalc_fixdt1' */
    /* End of SystemInitialize for SubSystem: '<S231>/Voltage_Mode_Protection' */

    /* SystemInitialize for VariantMerge generated from: '<S237>/Vd_max' incorporates:
     *  Outport: '<S231>/Vd_max'
     */
    rtDW->VariantMergeForOutportVd_max = 14400;

    /* SystemInitialize for VariantMerge generated from: '<S237>/Vd_min' incorporates:
     *  Outport: '<S231>/Vd_min'
     */
    rtDW->VariantMergeForOutportVd_min = -14400;

    /* SystemInitialize for VariantMerge generated from: '<S237>/Vq_max' incorporates:
     *  Outport: '<S231>/Vq_max'
     */
    rtDW->VariantMergeForOutportVq_max = 14400;

    /* SystemInitialize for VariantMerge generated from: '<S237>/Vq_min' incorporates:
     *  Outport: '<S231>/Vq_min'
     */
    rtDW->VariantMergeForOutportVq_min = -14400;

    /* SystemInitialize for SignalConversion generated from: '<S231>/id_max' incorporates:
     *  Outport: '<S231>/id_max'
     */
    rtDW->OutportBufferForid_max = 12000;

    /* SystemInitialize for Gain: '<S231>/Gain4' incorporates:
     *  Outport: '<S231>/id_min'
     */
    rtDW->Gain4 = -12000;

    /* SystemInitialize for Product: '<S231>/Divide1' incorporates:
     *  Outport: '<S231>/iq_max'
     */
    rtDW->Divide1_d = 12000;

    /* SystemInitialize for Gain: '<S231>/Gain1' incorporates:
     *  Outport: '<S231>/iq_min'
     */
    rtDW->Gain1 = -12000;

    /* End of SystemInitialize for SubSystem: '<S67>/Motor_Limitations_Enabled' */

    /* SystemInitialize for Chart: '<S1>/Task_Scheduler' incorporates:
     *  SubSystem: '<S7>/FOC'
     */
    FOC_Init(&rtDW->Merge_o, &rtDW->FOC_h);

    /* End of SystemInitialize for SubSystem: '<Root>/BLDC_controller' */
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
