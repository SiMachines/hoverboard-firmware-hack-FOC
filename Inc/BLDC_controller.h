/*
 * File: BLDC_controller.h
 *
 * Code generated for Simulink model 'BLDC_controller'.
 *
 * Model version                  : 16.99
 * Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
 * C/C++ source code generated on : Thu Aug 13 05:39:44 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objective: Execution efficiency
 * Validation result: Not run
 */

#ifndef BLDC_controller_h_
#define BLDC_controller_h_
#ifndef BLDC_controller_COMMON_INCLUDES_
#define BLDC_controller_COMMON_INCLUDES_
#include <stdbool.h>
#include <stdint.h>
#ifndef mcu_model
#include "config.h"
#endif
#endif                                 /* BLDC_controller_COMMON_INCLUDES_ */

/* Model Code Variants */
/**
 * You can use either of the following to alter the value of "normal" variant
 * control variables:
 *
 *  1. -DVC_VARIABLE1=VALUE1 -DVC_VARIABLE2=VALUE2, etc.
 *  2. -DUSE_VARIANT_DEFINES_HEADER="header.h" and within header.h you
 *      specify "#define VC_VARIABLE VALUE1", etc.
 *
 * Variant control variables are the independent variables of variant control
 * expressions specified in a variant block. For example, a Variant Source block
 * may contain "V==1" and V is the variant control variable of this
 * expression. A normal variant control variable is a plain MATLAB variable,
 * i.e. not a Simulink.Parameter.  The default define for a normal variant
 * control variable is the value specified in MATLAB at time of code generation.
 *
 * Alternatively, you can use Simulink.Parameter's as variant control variables to
 * explicitly specify code generation behavior.
 */
#ifdef USE_VARIANT_DEFINES_HEADER
#define VARIANT_DEFINES_HEADER_STR(h)  #h
#define VARIANT_DEFINES_HEADER(h)      VARIANT_DEFINES_HEADER_STR(h)
#include VARIANT_DEFINES_HEADER(USE_VARIANT_DEFINES_HEADER)
#endif                                 /* USE_VARIANT_DEFINES_HEADER */

/*
 * Validate the variant control variables are consistent with the model requirements
 */
#ifndef FeedForwardEnable
#define FeedForwardEnable              0
#endif

#ifndef mcu_model
#define mcu_model                      1
#endif

/* MW_VALIDATE_PREPROCESSOR_VARIANT_CHOICES */
#undef MW_HAVE_ACTIVE_VARIANT_CHOICE
#undef MW_HAVE_MORE_THAN_ONE_ACTIVE_CHOICE
#if mcu_model == 0
#ifdef MW_HAVE_ACTIVE_VARIANT_CHOICE
#define MW_HAVE_MORE_THAN_ONE_ACTIVE_CHOICE
#else
#define MW_HAVE_ACTIVE_VARIANT_CHOICE
#endif
#endif

#if mcu_model == 1
#ifdef MW_HAVE_ACTIVE_VARIANT_CHOICE
#define MW_HAVE_MORE_THAN_ONE_ACTIVE_CHOICE
#else
#define MW_HAVE_ACTIVE_VARIANT_CHOICE
#endif
#endif

/* At most one variant choice can be active for block 'BLDC_controller/BLDC_controller/F03_Control_Mode_Manager/F03_03_Input_Target_Synthesis/FOC_Control_Type/Variant Subsystem1', 'BLDC_controller/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/Variant Subsystem1', 'BLDC_controller/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Variant Subsystem' */
#ifdef MW_HAVE_MORE_THAN_ONE_ACTIVE_CHOICE
#error "At most one variant choice can be active for block 'BLDC_controller/BLDC_controller/F03_Control_Mode_Manager/F03_03_Input_Target_Synthesis/FOC_Control_Type/Variant Subsystem1' and others"
#endif

/*At least one variant choice must be active for block 'BLDC_controller/BLDC_controller/F03_Control_Mode_Manager/F03_03_Input_Target_Synthesis/FOC_Control_Type/Variant Subsystem1', 'BLDC_controller/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/Variant Subsystem1', 'BLDC_controller/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Variant Subsystem' */
#ifndef MW_HAVE_ACTIVE_VARIANT_CHOICE
#error "At least one variant choice must be active for block 'BLDC_controller/BLDC_controller/F03_Control_Mode_Manager/F03_03_Input_Target_Synthesis/FOC_Control_Type/Variant Subsystem1' and others"
#endif

#undef MW_HAVE_ACTIVE_VARIANT_CHOICE
#undef MW_HAVE_MORE_THAN_ONE_ACTIVE_CHOICE
#if FeedForwardEnable == 0
#ifdef MW_HAVE_ACTIVE_VARIANT_CHOICE
#define MW_HAVE_MORE_THAN_ONE_ACTIVE_CHOICE
#else
#define MW_HAVE_ACTIVE_VARIANT_CHOICE
#endif
#endif

#if FeedForwardEnable == 1
#ifdef MW_HAVE_ACTIVE_VARIANT_CHOICE
#define MW_HAVE_MORE_THAN_ONE_ACTIVE_CHOICE
#else
#define MW_HAVE_ACTIVE_VARIANT_CHOICE
#endif
#endif

#if FeedForwardEnable == 2
#ifdef MW_HAVE_ACTIVE_VARIANT_CHOICE
#define MW_HAVE_MORE_THAN_ONE_ACTIVE_CHOICE
#else
#define MW_HAVE_ACTIVE_VARIANT_CHOICE
#endif
#endif

/* At most one variant choice can be active for block 'BLDC_controller/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant', 'BLDC_controller/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant' */
#ifdef MW_HAVE_MORE_THAN_ONE_ACTIVE_CHOICE
#error "At most one variant choice can be active for block 'BLDC_controller/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant' and others"
#endif

/*At least one variant choice must be active for block 'BLDC_controller/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant', 'BLDC_controller/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant' */
#ifndef MW_HAVE_ACTIVE_VARIANT_CHOICE
#error "At least one variant choice must be active for block 'BLDC_controller/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant' and others"
#endif

/* MW_END_VALIDATE_PREPROCESSOR_VARIANT_CHOICES */

/* Macros for accessing real-time model data structure */
#ifndef rtmGetRootDWork
#define rtmGetRootDWork(rtm)           ((rtm)->dwork)
#endif

#ifndef rtmSetRootDWork
#define rtmSetRootDWork(rtm, val)      ((rtm)->dwork = (val))
#endif

#ifndef rtmGetU
#define rtmGetU(rtm)                   ((rtm)->inputs)
#endif

#ifndef rtmSetU
#define rtmSetU(rtm, val)              ((rtm)->inputs = (val))
#endif

#ifndef rtmGetY
#define rtmGetY(rtm)                   ((rtm)->outputs)
#endif

#ifndef rtmSetY
#define rtmSetY(rtm, val)              ((rtm)->outputs = (val))
#endif

#define BLDC_controller_M              (rtM)

/* Forward declaration for rtModel */
typedef struct tag_RTM RT_MODEL;

/* Block signals and states (default storage) for system '<S13>/Counter' */
typedef struct {
  int16_t UnitDelay_DSTATE;            /* '<S19>/UnitDelay' */
} DW_Counter;

/* Block signals and states (default storage) for system '<S69>/Low_Pass_Filter' */
typedef struct {
  int32_t UnitDelay1_DSTATE[2];        /* '<S75>/UnitDelay1' */
} DW_Low_Pass_Filter;

/* Block signals and states (default storage) for system '<S41>/Counter' */
typedef struct {
  uint16_t UnitDelay_DSTATE;           /* '<S46>/UnitDelay' */
} DW_Counter_d;

/* Block signals and states (default storage) for system '<S40>/Counter' */
typedef struct {
  uint32_t UnitDelay_DSTATE;           /* '<S44>/UnitDelay' */
} DW_Counter_e;

/* Block signals and states (default storage) for system '<S37>/either_edge' */
typedef struct {
  bool UnitDelay_DSTATE;               /* '<S42>/UnitDelay' */
} DW_either_edge;

/* Block signals and states (default storage) for system '<S36>/Debounce_Filter' */
typedef struct {
  DW_either_edge either_edge_n;        /* '<S37>/either_edge' */
  DW_Counter_e Counter_ni;             /* '<S40>/Counter' */
  DW_Counter_d Counter_e2;             /* '<S41>/Counter' */
  bool UnitDelay_DSTATE;               /* '<S37>/UnitDelay' */
} DW_Debounce_Filter;

/* Block signals and states (default storage) for system '<S1>/F03_Control_Mode_Manager' */
typedef struct {
  int32_t UnitDelay_DSTATE;            /* '<S59>/UnitDelay' */
  int8_t If2_ActiveSubsystem;          /* '<S49>/If2' */
  uint8_t is_active_c2_BLDC_controller;/* '<S5>/F03_02_Control_Mode_Manager' */
  uint8_t is_c2_BLDC_controller;       /* '<S5>/F03_02_Control_Mode_Manager' */
  uint8_t is_ACTIVE;                   /* '<S5>/F03_02_Control_Mode_Manager' */
  bool UnitDelay_DSTATE_c;             /* '<S58>/UnitDelay' */
} DW_F03_Control_Mode_Manager;

/* Block signals and states (default storage) for system '<S238>/I_backCalc_fixdt' */
typedef struct {
  int32_t UnitDelay_DSTATE;            /* '<S299>/UnitDelay' */
  int32_t UnitDelay_DSTATE_a;          /* '<S301>/UnitDelay' */
} DW_I_backCalc_fixdt;

/* Block signals and states (default storage) for system '<S164>/PI_clamp_fixdtFF' */
typedef struct {
  int32_t ResettableDelay_DSTATE;      /* '<S225>/Resettable Delay' */
  bool UnitDelay1_DSTATE;              /* '<S223>/UnitDelay1' */
  bool icLoad;                         /* '<S225>/Resettable Delay' */
} DW_PI_clamp_fixdtFF;

/* Block signals and states (default storage) for system '<S161>/TMClassic' */
typedef struct {
  int32_t ResettableDelay_DSTATE;      /* '<S228>/Resettable Delay' */
  bool UnitDelay1_DSTATE;              /* '<S165>/UnitDelay1' */
  bool icLoad;                         /* '<S228>/Resettable Delay' */
} DW_TMClassic;

/* Block signals and states (default storage) for system '<S84>/PI_clamp_fixdt' */
typedef struct {
  int32_t ResettableDelay_DSTATE;      /* '<S90>/Resettable Delay' */
  bool UnitDelay1_DSTATE;              /* '<S88>/UnitDelay1' */
  bool icLoad;                         /* '<S90>/Resettable Delay' */
} DW_PI_clamp_fixdt;

/* Block signals and states (default storage) for system '<S95>/PI_clamp_fixdtFF' */
typedef struct {
  int32_t ResettableDelay_DSTATE;      /* '<S156>/Resettable Delay' */
  bool UnitDelay1_DSTATE;              /* '<S154>/UnitDelay1' */
  bool icLoad;                         /* '<S156>/Resettable Delay' */
} DW_PI_clamp_fixdtFF_k;

/* Block signals and states (default storage) for system '<S7>/FOC' */
typedef struct {

#if FeedForwardEnable == 2

  DW_TMClassic TMClassic_n;            /* '<S92>/TMClassic' */

#endif

#if FeedForwardEnable == 1

  DW_PI_clamp_fixdtFF_k PI_clamp_fixdtFF_ci;/* '<S95>/PI_clamp_fixdtFF' */

#endif

  DW_PI_clamp_fixdt PI_clamp_fixdt_j;  /* '<S84>/PI_clamp_fixdt' */

#if FeedForwardEnable == 2

  DW_TMClassic TMClassic_l;            /* '<S161>/TMClassic' */

#endif

#if FeedForwardEnable == 1

  DW_PI_clamp_fixdtFF PI_clamp_fixdtFF_c2;/* '<S164>/PI_clamp_fixdtFF' */

#endif

  int16_t Switch1;                     /* '<S91>/Switch1' */
  int16_t constant2;                   /* '<S86>/constant2' */

#if FeedForwardEnable == 0

  int16_t Integrator_DSTATE;           /* '<S135>/Integrator' */

#endif

#if FeedForwardEnable == 0

  int16_t Integrator_DSTATE_a;         /* '<S204>/Integrator' */

#endif

  int8_t If1_ActiveSubsystem;          /* '<S66>/If1' */
  int8_t If1_ActiveSubsystem_a;        /* '<S78>/If1' */

#if FeedForwardEnable == 0

  uint8_t Integrator_IC_LOADING;       /* '<S135>/Integrator' */

#endif

} DW_FOC;

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  DW_FOC FOC_h;                        /* '<S7>/FOC' */
  DW_I_backCalc_fixdt I_backCalc_fixdt1;/* '<S238>/I_backCalc_fixdt1' */
  DW_I_backCalc_fixdt I_backCalc_fixdt_c;/* '<S238>/I_backCalc_fixdt' */
  DW_F03_Control_Mode_Manager F03_Control_Mode_Manager_f;/* '<S1>/F03_Control_Mode_Manager' */
  DW_either_edge either_edge_j;        /* '<S36>/either_edge' */
  DW_Debounce_Filter Debounce_Filter_e;/* '<S36>/Debounce_Filter' */
  DW_Low_Pass_Filter Low_Pass_Filter_e;/* '<S69>/Low_Pass_Filter' */
  DW_Counter Counter_c;                /* '<S13>/Counter' */
  int32_t Divide1;                     /* '<S235>/Divide1' */
  int32_t i;
  int32_t i1;
  int32_t q0;
  int32_t q1;
  int16_t Gain4_o[3];                  /* '<S76>/Gain4' */
  int16_t DataTypeConversion[2];       /* '<S75>/Data Type Conversion' */
  int16_t TmpSignalConversionAtLow_Pa[2];
  int16_t Merge_o;                     /* '<S78>/Merge' */
  int16_t VariantMergeForOutportVq_Trq;
  int16_t Divide1_d;                   /* '<S231>/Divide1' */
  int16_t Gain1;                       /* '<S231>/Gain1' */
  int16_t Gain4;                       /* '<S231>/Gain4' */
  int16_t OutportBufferForid_max;
  int16_t Switch2_p;                   /* '<S304>/Switch2' */
  int16_t Switch2_b;                   /* '<S302>/Switch2' */
  int16_t VariantMergeForOutportVd_max;
  int16_t VariantMergeForOutportVd_min;
  int16_t VariantMergeForOutportVq_max;
  int16_t VariantMergeForOutportVq_min;
  int16_t Switch2_m;                   /* '<S284>/Switch2' */
  int16_t OutportBufferForr_fieldWeak;
  int16_t Divide3;                     /* '<S61>/Divide3' */
  int16_t Merge1;                      /* '<S49>/Merge1' */
  int16_t OutportBufferForid;
  int16_t OutportBufferForiq;
  int16_t OutportBufferForiqAbs;
  int16_t Abs5_d;                      /* '<S69>/Abs5' */
  int16_t OutportBufferForid_f;
  int16_t Divide11;                    /* '<S21>/Divide11' */
  int16_t r_sin_M1_1;                  /* '<S71>/r_sin_M1' */
  int16_t r_cos_M1_1;                  /* '<S71>/r_cos_M1' */
  int16_t z_counterRawPrev;            /* '<S21>/z_counterRawPrev' */
  int16_t UnitDelay3_DSTATE;           /* '<S13>/UnitDelay3' */
  int16_t UnitDelay4_DSTATE;           /* '<S21>/UnitDelay4' */
  int16_t UnitDelay2_DSTATE;           /* '<S21>/UnitDelay2' */
  int16_t UnitDelay3_DSTATE_k;         /* '<S21>/UnitDelay3' */
  int16_t UnitDelay5_DSTATE;           /* '<S21>/UnitDelay5' */
  int16_t UnitDelay4_DSTATE_f;         /* '<S13>/UnitDelay4' */
  int16_t UnitDelay4_DSTATE_a;         /* '<S8>/UnitDelay4' */
  int16_t Integrator_DSTATE;           /* '<S276>/Integrator' */
  int16_t UD_DSTATE;                   /* '<S20>/UD' */
  int16_t Sum1_f;                      /* '<S16>/Sum1' */
  int16_t Saturation;                  /* '<S1>/Saturation' */
  int16_t Saturation1;                 /* '<S1>/Saturation1' */
  int16_t Switch2_g;                   /* '<S14>/Switch2' */
  uint16_t UnitDelay4_DSTATE_m;        /* '<S17>/UnitDelay4' */
  uint16_t DataTypeConversion2;        /* '<S25>/Data Type Conversion2' */
  int8_t UnitDelay3;                   /* '<S12>/UnitDelay3' */
  int8_t Switch2_a;                    /* '<S12>/Switch2' */
  int8_t UnitDelay2_DSTATE_o;          /* '<S12>/UnitDelay2' */
  int8_t If1_ActiveSubsystem;          /* '<S7>/If1' */
  int8_t If2_ActiveSubsystem;          /* '<S7>/If2' */
  int8_t If2_ActiveSubsystem_a;        /* '<S64>/If2' */
  uint8_t OutportBufferForz_errCode;
  uint8_t Switch1;                     /* '<S36>/Switch1' */
  uint8_t UnitDelay3_DSTATE_f;         /* '<S10>/UnitDelay3' */
  uint8_t UnitDelay1_DSTATE;           /* '<S10>/UnitDelay1' */
  uint8_t UnitDelay2_DSTATE_k;         /* '<S10>/UnitDelay2' */
  uint8_t Integrator_IC_LOADING;       /* '<S276>/Integrator' */
  bool OutportBufferForb_errFlag;
  bool Merge_a;                        /* '<S37>/Merge' */
  bool dz_cntTrnsDet;                  /* '<S21>/dz_cntTrnsDet' */
  bool UnitDelay2_DSTATE_g;            /* '<S2>/UnitDelay2' */
  bool UnitDelay5_DSTATE_l;            /* '<S2>/UnitDelay5' */
  bool UnitDelay6_DSTATE;              /* '<S2>/UnitDelay6' */
  bool UnitDelay1_DSTATE_g;            /* '<S21>/UnitDelay1' */
  bool n_commDeacv_Mode;               /* '<S13>/n_commDeacv' */
  bool dz_cntTrnsDet_Mode;             /* '<S21>/dz_cntTrnsDet' */
} DW;

/* Constant parameters (default storage) */
typedef struct {
  /* Computed Parameter: r_sin_M1_1_Table
   * Referenced by: '<S71>/r_sin_M1'
   */
  int16_t r_sin_M1_1_Table[181];

  /* Computed Parameter: r_cos_M1_1_Table
   * Referenced by: '<S71>/r_cos_M1'
   */
  int16_t r_cos_M1_1_Table[181];

  /* Computed Parameter: r_sin3PhaA_M1_Table
   * Referenced by: '<S307>/r_sin3PhaA_M1'
   */
  int16_t r_sin3PhaA_M1_Table[181];

  /* Computed Parameter: r_sin3PhaB_M1_Table
   * Referenced by: '<S307>/r_sin3PhaB_M1'
   */
  int16_t r_sin3PhaB_M1_Table[181];

  /* Computed Parameter: r_sin3PhaC_M1_Table
   * Referenced by: '<S307>/r_sin3PhaC_M1'
   */
  int16_t r_sin3PhaC_M1_Table[181];

#if mcu_model == 1

  /* Computed Parameter: GD32_Vq_max_M1_Table
   * Referenced by: '<S297>/GD32_Vq_max_M1'
   */
  int16_t GD32_Vq_max_M1_Table[82];

#ifndef CONSTP_VARIANT_EXISTS
#define CONSTP_VARIANT_EXISTS
#endif
#endif

#if mcu_model == 0

  /* Computed Parameter: STM32_Vq_max_M1_Table
   * Referenced by: '<S298>/STM32_Vq_max_M1'
   */
  int16_t STM32_Vq_max_M1_Table[32];

#ifndef CONSTP_VARIANT_EXISTS
#define CONSTP_VARIANT_EXISTS
#endif
#endif

  /* Computed Parameter: iq_maxSca_M1_Table
   * Referenced by: '<S231>/iq_maxSca_M1'
   */
  uint16_t iq_maxSca_M1_Table[100];

  /* Computed Parameter: z_commutMap_M1_table
   * Referenced by: '<S305>/z_commutMap_M1'
   */
  int8_t z_commutMap_M1_table[18];

  /* Computed Parameter: vec_hallToPos_Value
   * Referenced by: '<S11>/vec_hallToPos'
   */
  int8_t vec_hallToPos_Value[8];
} ConstP;

/* External inputs (root inport signals with default storage) */
typedef struct {
  bool b_motEna;                       /* '<Root>/b_motEna' */
  uint8_t z_ctrlModReq;                /* '<Root>/z_ctrlModReq' */
  int16_t r_inpTgt;                    /* '<Root>/r_inpTgt' */
  uint8_t b_hallA;                     /* '<Root>/b_hallA ' */
  uint8_t b_hallB;                     /* '<Root>/b_hallB' */
  uint8_t b_hallC;                     /* '<Root>/b_hallC' */
  int16_t i_phaAB;                     /* '<Root>/i_phaAB' */
  int16_t i_phaBC;                     /* '<Root>/i_phaBC' */
  int16_t i_DCLink;                    /* '<Root>/i_DCLink' */
  uint16_t a_encoderCNT;               /* '<Root>/a_encoderCNT' */
  int16_t a_mechangle;                 /* '<Root>/a_mechangle' */
} ExtU;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  int16_t DC_phaA;                     /* '<Root>/DC_phaA' */
  int16_t DC_phaB;                     /* '<Root>/DC_phaB' */
  int16_t DC_phaC;                     /* '<Root>/DC_phaC' */
  uint8_t z_errCode;                   /* '<Root>/z_errCode' */
  int16_t n_mot;                       /* '<Root>/n_mot' */
  int16_t a_elecAngle;                 /* '<Root>/a_elecAngle' */
  int16_t iq;                          /* '<Root>/iq' */
  int16_t id;                          /* '<Root>/id' */
} ExtY;

/* Parameters (default storage) */
struct P_ {
  int32_t dV_openRate;                 /* Variable: dV_openRate
                                        * Referenced by: '<S53>/dV_openRate'
                                        */
  uint32_t a_fcpr;                     /* Variable: a_fcpr
                                        * Referenced by: '<S15>/Constant3'
                                        */
  uint32_t cf_speedCoef;               /* Variable: cf_speedCoef
                                        * Referenced by: '<S21>/cf_speedCoef'
                                        */
  uint32_t t_errDequal;                /* Variable: t_errDequal
                                        * Referenced by: '<S36>/t_errDequal'
                                        */
  int16_t dz_cntTrnsDetHi;             /* Variable: dz_cntTrnsDetHi
                                        * Referenced by: '<S21>/dz_cntTrnsDet'
                                        */
  int16_t dz_cntTrnsDetLo;             /* Variable: dz_cntTrnsDetLo
                                        * Referenced by: '<S21>/dz_cntTrnsDet'
                                        */
  int16_t n_cruiseMotTgt;              /* Variable: n_cruiseMotTgt
                                        * Referenced by: '<S84>/n_cruiseMotTgt'
                                        */
  int16_t z_maxCntRst;                 /* Variable: z_maxCntRst
                                        * Referenced by:
                                        *   '<S13>/z_maxCntRst'
                                        *   '<S13>/z_maxCntRst2'
                                        *   '<S13>/UnitDelay3'
                                        *   '<S21>/z_counter'
                                        */

#if FeedForwardEnable == 1

  int16_t ff_gain;                     /* Variable: ff_gain
                                        * Referenced by:
                                        *   '<S85>/Input'
                                        *   '<S86>/Input'
                                        */

#endif

  int16_t a_phaAdvMax;                 /* Variable: a_phaAdvMax
                                        * Referenced by: '<S61>/a_phaAdvMax'
                                        */
  int16_t i_max;                       /* Variable: i_max
                                        * Referenced by:
                                        *   '<S52>/i_max'
                                        *   '<S231>/i_max'
                                        */
  int16_t id_fieldWeakMax;             /* Variable: id_fieldWeakMax
                                        * Referenced by: '<S61>/id_fieldWeakMax'
                                        */
  int16_t n_commAcvLo;                 /* Variable: n_commAcvLo
                                        * Referenced by: '<S13>/n_commDeacv'
                                        */
  int16_t n_commDeacvHi;               /* Variable: n_commDeacvHi
                                        * Referenced by: '<S13>/n_commDeacv'
                                        */
  int16_t n_fieldWeakAuthHi;           /* Variable: n_fieldWeakAuthHi
                                        * Referenced by: '<S61>/n_fieldWeakAuthHi'
                                        */
  int16_t n_fieldWeakAuthLo;           /* Variable: n_fieldWeakAuthLo
                                        * Referenced by: '<S61>/n_fieldWeakAuthLo'
                                        */
  int16_t n_max;                       /* Variable: n_max
                                        * Referenced by:
                                        *   '<S52>/n_max'
                                        *   '<S231>/n_max'
                                        */
  int16_t n_stdStillDet;               /* Variable: n_stdStillDet
                                        * Referenced by: '<S13>/n_stdStillDet'
                                        */
  int16_t r_errInpTgtThres;            /* Variable: r_errInpTgtThres
                                        * Referenced by: '<S36>/r_errInpTgtThres'
                                        */
  int16_t r_fieldWeakHi;               /* Variable: r_fieldWeakHi
                                        * Referenced by: '<S61>/r_fieldWeakHi'
                                        */
  int16_t r_fieldWeakLo;               /* Variable: r_fieldWeakLo
                                        * Referenced by: '<S61>/r_fieldWeakLo'
                                        */
  uint16_t cf_nKp;                     /* Variable: cf_nKp
                                        * Referenced by: '<S84>/cf_nKp'
                                        */
  uint16_t cf_idKi;                    /* Variable: cf_idKi
                                        * Referenced by: '<S86>/cf_idKi2'
                                        */
  uint16_t cf_iqKi;                    /* Variable: cf_iqKi
                                        * Referenced by: '<S85>/cf_iqKi1'
                                        */
  uint16_t cf_currFilt;                /* Variable: cf_currFilt
                                        * Referenced by: '<S69>/cf_currFilt'
                                        */
  uint16_t cf_iqKiLimProt;             /* Variable: cf_iqKiLimProt
                                        * Referenced by:
                                        *   '<S235>/cf_iqKiLimProt'
                                        *   '<S238>/cf_iqKiLimProt'
                                        */
  uint16_t cf_nKi;                     /* Variable: cf_nKi
                                        * Referenced by: '<S84>/cf_nKi'
                                        */
  uint16_t cf_nKiLimProt;              /* Variable: cf_nKiLimProt
                                        * Referenced by:
                                        *   '<S236>/cf_nKiLimProt'
                                        *   '<S238>/cf_nKiLimProt'
                                        */
  uint16_t cf_idKp;                    /* Variable: cf_idKp
                                        * Referenced by: '<S86>/cf_idKp2'
                                        */
  uint16_t cf_iqKp;                    /* Variable: cf_iqKp
                                        * Referenced by: '<S85>/cf_iqKp1'
                                        */
  uint16_t a_cpr;                      /* Variable: a_cpr
                                        * Referenced by: '<S15>/Constant'
                                        */
  uint16_t n_polePairs;                /* Variable: n_polePairs
                                        * Referenced by: '<S15>/n_polePairs1'
                                        */
  uint16_t t_errQual;                  /* Variable: t_errQual
                                        * Referenced by: '<S36>/t_errQual'
                                        */
  bool b_angleMeasEna;                 /* Variable: b_angleMeasEna
                                        * Referenced by:
                                        *   '<S3>/b_angleMeasEna'
                                        *   '<S13>/b_angleMeasEna'
                                        *   '<S13>/b_angleMeasEna1'
                                        *   '<S13>/b_angleMeasEna2'
                                        */
  bool b_cruiseCtrlEna;                /* Variable: b_cruiseCtrlEna
                                        * Referenced by: '<S1>/b_cruiseCtrlEna'
                                        */
  bool b_diagEna;                      /* Variable: b_diagEna
                                        * Referenced by: '<S4>/b_diagEna'
                                        */
  bool b_fieldWeakEna;                 /* Variable: b_fieldWeakEna
                                        * Referenced by:
                                        *   '<S6>/b_fieldWeakEna'
                                        *   '<S308>/b_fieldWeakEna'
                                        */
  uint8_t z_ctrlTypSel;                /* Variable: z_ctrlTypSel
                                        * Referenced by: '<S1>/z_ctrlTypSel'
                                        */
  uint8_t z_selPhaCurMeasABC;          /* Variable: z_selPhaCurMeasABC
                                        * Referenced by: '<S68>/z_selPhaCurMeasABC'
                                        */
};

/* Parameters (default storage) */
typedef struct P_ P;

/* Real-time Model Data Structure */
struct tag_RTM {
    P *defaultParam;
ExtU *inputs;
  ExtY *outputs;
  DW *dwork;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    struct {
      uint8_t TID[2];
    } TaskCounters;
  } Timing;
};

/* Block parameters (default storage) */
//extern P rtP;
/* Constant parameters (default storage) */
extern const ConstP rtConstP;

/*
 * Exported Global Parameters
 *
 * Note: Exported global parameters are tunable parameters with an exported
 * global storage class designation.  Code generation will declare the memory for
 * these parameters and exports their symbols.
 *
 */
extern uint16_t cf_KbLimProt;          /* Variable: cf_KbLimProt
                                        * Referenced by:
                                        *   '<S238>/cf_KbLimProt'
                                        *   '<S268>/Kb'
                                        */

/* Model entry point functions */
extern void BLDC_controller_initialize(RT_MODEL *const rtM);
extern void BLDC_controller_step(RT_MODEL *const rtM);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S17>/Scope2' : Unused code path elimination
 * Block '<S20>/Data Type Duplicate' : Unused code path elimination
 * Block '<S13>/Scope' : Unused code path elimination
 * Block '<S13>/Scope2' : Unused code path elimination
 * Block '<S21>/Scope' : Unused code path elimination
 * Block '<S14>/Scope' : Unused code path elimination
 * Block '<S15>/MechElec' : Unused code path elimination
 * Block '<S25>/Data Type Duplicate' : Unused code path elimination
 * Block '<S3>/Scope' : Unused code path elimination
 * Block '<S60>/Data Type Duplicate' : Unused code path elimination
 * Block '<S60>/Data Type Propagation' : Unused code path elimination
 * Block '<S62>/Data Type Duplicate' : Unused code path elimination
 * Block '<S62>/Data Type Propagation' : Unused code path elimination
 * Block '<S63>/Data Type Duplicate' : Unused code path elimination
 * Block '<S63>/Data Type Propagation' : Unused code path elimination
 * Block '<S68>/Scope' : Unused code path elimination
 * Block '<S64>/Scope' : Unused code path elimination
 * Block '<S65>/Scope' : Unused code path elimination
 * Block '<S78>/Scope' : Unused code path elimination
 * Block '<S84>/Scope' : Unused code path elimination
 * Block '<S85>/FOC TQ PI' : Unused code path elimination
 * Block '<S143>/Data Type Duplicate' : Unused code path elimination
 * Block '<S143>/Data Type Propagation' : Unused code path elimination
 * Block '<S96>/Scope' : Unused code path elimination
 * Block '<S93>/Data Type Duplicate' : Unused code path elimination
 * Block '<S93>/Data Type Propagation' : Unused code path elimination
 * Block '<S212>/Data Type Duplicate' : Unused code path elimination
 * Block '<S212>/Data Type Propagation' : Unused code path elimination
 * Block '<S165>/Scope' : Unused code path elimination
 * Block '<S162>/Data Type Duplicate' : Unused code path elimination
 * Block '<S162>/Data Type Propagation' : Unused code path elimination
 * Block '<S230>/Data Type Duplicate' : Unused code path elimination
 * Block '<S230>/Data Type Propagation' : Unused code path elimination
 * Block '<S66>/Scope' : Unused code path elimination
 * Block '<S231>/Scope' : Unused code path elimination
 * Block '<S239>/Data Type Duplicate' : Unused code path elimination
 * Block '<S239>/Data Type Propagation' : Unused code path elimination
 * Block '<S284>/Data Type Duplicate' : Unused code path elimination
 * Block '<S284>/Data Type Propagation' : Unused code path elimination
 * Block '<S302>/Data Type Duplicate' : Unused code path elimination
 * Block '<S302>/Data Type Propagation' : Unused code path elimination
 * Block '<S304>/Data Type Duplicate' : Unused code path elimination
 * Block '<S304>/Data Type Propagation' : Unused code path elimination
 * Block '<S308>/Scope' : Unused code path elimination
 * Block '<S2>/Data Type Conversion' : Eliminate redundant data type conversion
 * Block '<S25>/Data Type Conversion' : Eliminate redundant data type conversion
 * Block '<S34>/Data Type Conversion' : Eliminate redundant data type conversion
 * Block '<S85>/Signal Conversion2' : Eliminate redundant signal conversion block
 * Block '<S25>/Constant' : Unused code path elimination
 * Block '<S25>/Constant1' : Unused code path elimination
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('BLDCmotor_FOC_R2017b_fixdt/BLDC_controller')    - opens subsystem BLDCmotor_FOC_R2017b_fixdt/BLDC_controller
 * hilite_system('BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'BLDCmotor_FOC_R2017b_fixdt'
 * '<S1>'   : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller'
 * '<S2>'   : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/Call_Scheduler'
 * '<S3>'   : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F01_Estimations'
 * '<S4>'   : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F02_Diagnostics'
 * '<S5>'   : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F03_Control_Mode_Manager'
 * '<S6>'   : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F04_Field_Weakening'
 * '<S7>'   : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control'
 * '<S8>'   : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F06_Control_Type_Management'
 * '<S9>'   : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/Task_Scheduler'
 * '<S10>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F01_Estimations/F01_01_Edge_Detector'
 * '<S11>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F01_Estimations/F01_02_Position_Calculation'
 * '<S12>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F01_Estimations/F01_03_Direction_Detection'
 * '<S13>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F01_Estimations/F01_04_Speed_Estimation'
 * '<S14>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F01_Estimations/F01_05_Electrical_Angle_Estimation'
 * '<S15>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F01_Estimations/F01_06_Electrical_Angle_Measurement'
 * '<S16>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F01_Estimations/F01_04_Speed_Estimation/Counter'
 * '<S17>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F01_Estimations/F01_04_Speed_Estimation/Enabled Subsystem'
 * '<S18>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F01_Estimations/F01_04_Speed_Estimation/Subsystem'
 * '<S19>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F01_Estimations/F01_04_Speed_Estimation/Counter/rst_Delay'
 * '<S20>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F01_Estimations/F01_04_Speed_Estimation/Enabled Subsystem/Subsystem'
 * '<S21>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F01_Estimations/F01_04_Speed_Estimation/Subsystem/Raw_Motor_Speed_Estimation'
 * '<S22>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F01_Estimations/F01_06_Electrical_Angle_Measurement/Mechanical to Electrical Position'
 * '<S23>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F01_Estimations/F01_06_Electrical_Angle_Measurement/Quadrature Decoder'
 * '<S24>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F01_Estimations/F01_06_Electrical_Angle_Measurement/Mechanical to Electrical Position/MechToElec'
 * '<S25>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F01_Estimations/F01_06_Electrical_Angle_Measurement/Mechanical to Electrical Position/MechToElec/fixed-point'
 * '<S26>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F01_Estimations/F01_06_Electrical_Angle_Measurement/Mechanical to Electrical Position/MechToElec/fixed-point/Variant Subsystem'
 * '<S27>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F01_Estimations/F01_06_Electrical_Angle_Measurement/Mechanical to Electrical Position/MechToElec/fixed-point/Variant Subsystem/Input Port'
 * '<S28>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F01_Estimations/F01_06_Electrical_Angle_Measurement/Quadrature Decoder/DT_Handle'
 * '<S29>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F01_Estimations/F01_06_Electrical_Angle_Measurement/Quadrature Decoder/PositionNoReset'
 * '<S30>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F01_Estimations/F01_06_Electrical_Angle_Measurement/Quadrature Decoder/PositionResetAtIndex'
 * '<S31>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F01_Estimations/F01_06_Electrical_Angle_Measurement/Quadrature Decoder/Variant Subsystem'
 * '<S32>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F01_Estimations/F01_06_Electrical_Angle_Measurement/Quadrature Decoder/DT_Handle/fixed-point'
 * '<S33>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F01_Estimations/F01_06_Electrical_Angle_Measurement/Quadrature Decoder/PositionNoReset/Variant Subsystem'
 * '<S34>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F01_Estimations/F01_06_Electrical_Angle_Measurement/Quadrature Decoder/PositionNoReset/Variant Subsystem/Input Port'
 * '<S35>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F01_Estimations/F01_06_Electrical_Angle_Measurement/Quadrature Decoder/Variant Subsystem/Input Port'
 * '<S36>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F02_Diagnostics/Diagnostics_Enabled'
 * '<S37>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F02_Diagnostics/Diagnostics_Enabled/Debounce_Filter'
 * '<S38>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F02_Diagnostics/Diagnostics_Enabled/either_edge'
 * '<S39>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F02_Diagnostics/Diagnostics_Enabled/Debounce_Filter/Default'
 * '<S40>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F02_Diagnostics/Diagnostics_Enabled/Debounce_Filter/Dequalification'
 * '<S41>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F02_Diagnostics/Diagnostics_Enabled/Debounce_Filter/Qualification'
 * '<S42>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F02_Diagnostics/Diagnostics_Enabled/Debounce_Filter/either_edge'
 * '<S43>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F02_Diagnostics/Diagnostics_Enabled/Debounce_Filter/Dequalification/Counter'
 * '<S44>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F02_Diagnostics/Diagnostics_Enabled/Debounce_Filter/Dequalification/Counter/rst_Delay'
 * '<S45>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F02_Diagnostics/Diagnostics_Enabled/Debounce_Filter/Qualification/Counter'
 * '<S46>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F02_Diagnostics/Diagnostics_Enabled/Debounce_Filter/Qualification/Counter/rst_Delay'
 * '<S47>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F03_Control_Mode_Manager/F03_01_Mode_Transition_Calculation'
 * '<S48>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F03_Control_Mode_Manager/F03_02_Control_Mode_Manager'
 * '<S49>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F03_Control_Mode_Manager/F03_03_Input_Target_Synthesis'
 * '<S50>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F03_Control_Mode_Manager/F03_03_Input_Target_Synthesis/Default_Control_Type'
 * '<S51>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F03_Control_Mode_Manager/F03_03_Input_Target_Synthesis/Default_Mode'
 * '<S52>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F03_Control_Mode_Manager/F03_03_Input_Target_Synthesis/FOC_Control_Type'
 * '<S53>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F03_Control_Mode_Manager/F03_03_Input_Target_Synthesis/Open_Mode'
 * '<S54>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F03_Control_Mode_Manager/F03_03_Input_Target_Synthesis/FOC_Control_Type/Variant Subsystem1'
 * '<S55>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F03_Control_Mode_Manager/F03_03_Input_Target_Synthesis/FOC_Control_Type/Variant Subsystem1/GD32F103'
 * '<S56>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F03_Control_Mode_Manager/F03_03_Input_Target_Synthesis/FOC_Control_Type/Variant Subsystem1/STM32F103'
 * '<S57>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F03_Control_Mode_Manager/F03_03_Input_Target_Synthesis/Open_Mode/Rate_Limiter'
 * '<S58>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F03_Control_Mode_Manager/F03_03_Input_Target_Synthesis/Open_Mode/rising_edge_init'
 * '<S59>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F03_Control_Mode_Manager/F03_03_Input_Target_Synthesis/Open_Mode/Rate_Limiter/Delay_Init1'
 * '<S60>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F03_Control_Mode_Manager/F03_03_Input_Target_Synthesis/Open_Mode/Rate_Limiter/Saturation Dynamic'
 * '<S61>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F04_Field_Weakening/Field_Weakening_Enabled'
 * '<S62>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F04_Field_Weakening/Field_Weakening_Enabled/Saturation Dynamic'
 * '<S63>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F04_Field_Weakening/Field_Weakening_Enabled/Saturation Dynamic1'
 * '<S64>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Clarke_Park_Transform_Forward'
 * '<S65>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Clarke_Park_Transform_Inverse'
 * '<S66>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC'
 * '<S67>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations'
 * '<S68>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Clarke_Park_Transform_Forward/Clarke_Transform'
 * '<S69>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Clarke_Park_Transform_Forward/Current_Filtering'
 * '<S70>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Clarke_Park_Transform_Forward/Park_Transform'
 * '<S71>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Clarke_Park_Transform_Forward/Sine_Cosine_Approximation'
 * '<S72>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Clarke_Park_Transform_Forward/Clarke_Transform/Clarke_PhasesAB'
 * '<S73>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Clarke_Park_Transform_Forward/Clarke_Transform/Clarke_PhasesAC'
 * '<S74>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Clarke_Park_Transform_Forward/Clarke_Transform/Clarke_PhasesBC'
 * '<S75>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Clarke_Park_Transform_Forward/Current_Filtering/Low_Pass_Filter'
 * '<S76>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Clarke_Park_Transform_Inverse/Inv_Clarke_Transform'
 * '<S77>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Clarke_Park_Transform_Inverse/Inv_Park_Transform'
 * '<S78>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled'
 * '<S79>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Compare To Constant'
 * '<S80>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Compare To Constant1'
 * '<S81>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Compare To Constant2'
 * '<S82>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Compare To Constant3'
 * '<S83>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Open_Mode'
 * '<S84>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Speed_Mode'
 * '<S85>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode'
 * '<S86>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation'
 * '<S87>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Voltage_Mode'
 * '<S88>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Speed_Mode/PI_clamp_fixdt'
 * '<S89>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Speed_Mode/PI_clamp_fixdt/Clamping_circuit'
 * '<S90>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Speed_Mode/PI_clamp_fixdt/Integrator'
 * '<S91>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Speed_Mode/PI_clamp_fixdt/Saturation_hit'
 * '<S92>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant'
 * '<S93>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Saturation Dynamic1'
 * '<S94>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff'
 * '<S95>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFon'
 * '<S96>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/TMClassic'
 * '<S97>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller'
 * '<S98>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Anti-windup'
 * '<S99>'  : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/D Gain'
 * '<S100>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/External Derivative'
 * '<S101>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Filter'
 * '<S102>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Filter ICs'
 * '<S103>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/I Gain'
 * '<S104>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Ideal P Gain'
 * '<S105>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Ideal P Gain Fdbk'
 * '<S106>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Integrator'
 * '<S107>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Integrator ICs'
 * '<S108>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/N Copy'
 * '<S109>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/N Gain'
 * '<S110>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/P Copy'
 * '<S111>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Parallel P Gain'
 * '<S112>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Reset Signal'
 * '<S113>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Saturation'
 * '<S114>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Saturation Fdbk'
 * '<S115>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Sum'
 * '<S116>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Sum Fdbk'
 * '<S117>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Tracking Mode'
 * '<S118>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Tracking Mode Sum'
 * '<S119>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Tsamp - Integral'
 * '<S120>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Tsamp - Ngain'
 * '<S121>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/postSat Signal'
 * '<S122>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/preInt Signal'
 * '<S123>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/preSat Signal'
 * '<S124>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Anti-windup/Disc. Clamping Parallel'
 * '<S125>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Anti-windup/Disc. Clamping Parallel/Dead Zone'
 * '<S126>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Anti-windup/Disc. Clamping Parallel/Dead Zone/External'
 * '<S127>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Anti-windup/Disc. Clamping Parallel/Dead Zone/External/Dead Zone Dynamic'
 * '<S128>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/D Gain/Disabled'
 * '<S129>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/External Derivative/Disabled'
 * '<S130>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Filter/Disabled'
 * '<S131>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Filter ICs/Disabled'
 * '<S132>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/I Gain/External Parameters'
 * '<S133>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Ideal P Gain/Passthrough'
 * '<S134>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Ideal P Gain Fdbk/Disabled'
 * '<S135>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Integrator/Discrete'
 * '<S136>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Integrator ICs/External IC'
 * '<S137>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/N Copy/Disabled wSignal Specification'
 * '<S138>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/N Gain/Disabled'
 * '<S139>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/P Copy/Disabled'
 * '<S140>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Parallel P Gain/External Parameters'
 * '<S141>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Reset Signal/Disabled'
 * '<S142>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Saturation/External'
 * '<S143>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Saturation/External/Saturation Dynamic'
 * '<S144>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Saturation Fdbk/Disabled'
 * '<S145>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Sum/Sum_PI'
 * '<S146>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Sum Fdbk/Disabled'
 * '<S147>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Tracking Mode/Disabled'
 * '<S148>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Tracking Mode Sum/Passthrough'
 * '<S149>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Tsamp - Integral/Passthrough'
 * '<S150>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/Tsamp - Ngain/Passthrough'
 * '<S151>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/postSat Signal/Forward_Path'
 * '<S152>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/preInt Signal/Internal PreInt'
 * '<S153>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFoff/PI Controller/preSat Signal/Forward_Path'
 * '<S154>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFon/PI_clamp_fixdtFF'
 * '<S155>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFon/PI_clamp_fixdtFF/Clamping_circuit'
 * '<S156>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFon/PI_clamp_fixdtFF/Integrator'
 * '<S157>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/FFon/PI_clamp_fixdtFF/Saturation_hit'
 * '<S158>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/TMClassic/Clamping_circuit'
 * '<S159>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/TMClassic/Integrator'
 * '<S160>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Torque_Mode/Pi Variant/TMClassic/Saturation_hit'
 * '<S161>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant'
 * '<S162>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Saturation Dynamic'
 * '<S163>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff'
 * '<S164>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFon'
 * '<S165>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/TMClassic'
 * '<S166>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller'
 * '<S167>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Anti-windup'
 * '<S168>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/D Gain'
 * '<S169>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/External Derivative'
 * '<S170>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Filter'
 * '<S171>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Filter ICs'
 * '<S172>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/I Gain'
 * '<S173>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Ideal P Gain'
 * '<S174>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Ideal P Gain Fdbk'
 * '<S175>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Integrator'
 * '<S176>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Integrator ICs'
 * '<S177>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/N Copy'
 * '<S178>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/N Gain'
 * '<S179>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/P Copy'
 * '<S180>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Parallel P Gain'
 * '<S181>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Reset Signal'
 * '<S182>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Saturation'
 * '<S183>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Saturation Fdbk'
 * '<S184>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Sum'
 * '<S185>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Sum Fdbk'
 * '<S186>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Tracking Mode'
 * '<S187>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Tracking Mode Sum'
 * '<S188>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Tsamp - Integral'
 * '<S189>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Tsamp - Ngain'
 * '<S190>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/postSat Signal'
 * '<S191>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/preInt Signal'
 * '<S192>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/preSat Signal'
 * '<S193>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Anti-windup/Disc. Clamping Parallel'
 * '<S194>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Anti-windup/Disc. Clamping Parallel/Dead Zone'
 * '<S195>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Anti-windup/Disc. Clamping Parallel/Dead Zone/External'
 * '<S196>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Anti-windup/Disc. Clamping Parallel/Dead Zone/External/Dead Zone Dynamic'
 * '<S197>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/D Gain/Disabled'
 * '<S198>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/External Derivative/Disabled'
 * '<S199>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Filter/Disabled'
 * '<S200>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Filter ICs/Disabled'
 * '<S201>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/I Gain/External Parameters'
 * '<S202>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Ideal P Gain/Passthrough'
 * '<S203>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Ideal P Gain Fdbk/Disabled'
 * '<S204>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Integrator/Discrete'
 * '<S205>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Integrator ICs/External IC'
 * '<S206>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/N Copy/Disabled wSignal Specification'
 * '<S207>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/N Gain/Disabled'
 * '<S208>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/P Copy/Disabled'
 * '<S209>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Parallel P Gain/External Parameters'
 * '<S210>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Reset Signal/Disabled'
 * '<S211>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Saturation/External'
 * '<S212>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Saturation/External/Saturation Dynamic'
 * '<S213>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Saturation Fdbk/Disabled'
 * '<S214>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Sum/Sum_PI'
 * '<S215>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Sum Fdbk/Disabled'
 * '<S216>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Tracking Mode/Disabled'
 * '<S217>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Tracking Mode Sum/Passthrough'
 * '<S218>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Tsamp - Integral/Passthrough'
 * '<S219>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/Tsamp - Ngain/Passthrough'
 * '<S220>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/postSat Signal/Forward_Path'
 * '<S221>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/preInt Signal/Internal PreInt'
 * '<S222>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFoff/PI Controller/preSat Signal/Forward_Path'
 * '<S223>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFon/PI_clamp_fixdtFF'
 * '<S224>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFon/PI_clamp_fixdtFF/Clamping_circuit'
 * '<S225>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFon/PI_clamp_fixdtFF/Integrator'
 * '<S226>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/FFon/PI_clamp_fixdtFF/Saturation_hit'
 * '<S227>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/TMClassic/Clamping_circuit'
 * '<S228>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/TMClassic/Integrator'
 * '<S229>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Vd_Calculation/Pi Variant/TMClassic/Saturation_hit'
 * '<S230>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/FOC/FOC_Enabled/Voltage_Mode/Saturation Dynamic1'
 * '<S231>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled'
 * '<S232>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Compare To Constant'
 * '<S233>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Compare To Constant1'
 * '<S234>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Compare To Constant2'
 * '<S235>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Speed_Mode_Protection'
 * '<S236>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection'
 * '<S237>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Variant Subsystem'
 * '<S238>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Voltage_Mode_Protection'
 * '<S239>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Speed_Mode_Protection/Saturation Dynamic'
 * '<S240>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller'
 * '<S241>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/Variant Subsystem1'
 * '<S242>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Anti-windup'
 * '<S243>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/D Gain'
 * '<S244>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/External Derivative'
 * '<S245>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Filter'
 * '<S246>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Filter ICs'
 * '<S247>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/I Gain'
 * '<S248>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Ideal P Gain'
 * '<S249>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Ideal P Gain Fdbk'
 * '<S250>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Integrator'
 * '<S251>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Integrator ICs'
 * '<S252>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/N Copy'
 * '<S253>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/N Gain'
 * '<S254>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/P Copy'
 * '<S255>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Parallel P Gain'
 * '<S256>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Reset Signal'
 * '<S257>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Saturation'
 * '<S258>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Saturation Fdbk'
 * '<S259>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Sum'
 * '<S260>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Sum Fdbk'
 * '<S261>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Tracking Mode'
 * '<S262>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Tracking Mode Sum'
 * '<S263>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Tsamp - Integral'
 * '<S264>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Tsamp - Ngain'
 * '<S265>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/postSat Signal'
 * '<S266>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/preInt Signal'
 * '<S267>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/preSat Signal'
 * '<S268>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Anti-windup/Back Calculation'
 * '<S269>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/D Gain/Disabled'
 * '<S270>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/External Derivative/Disabled'
 * '<S271>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Filter/Disabled'
 * '<S272>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Filter ICs/Disabled'
 * '<S273>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/I Gain/External Parameters'
 * '<S274>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Ideal P Gain/Passthrough'
 * '<S275>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Ideal P Gain Fdbk/Disabled'
 * '<S276>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Integrator/Discrete'
 * '<S277>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Integrator ICs/External IC'
 * '<S278>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/N Copy/Disabled wSignal Specification'
 * '<S279>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/N Gain/Disabled'
 * '<S280>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/P Copy/Disabled'
 * '<S281>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Parallel P Gain/Disabled'
 * '<S282>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Reset Signal/Disabled'
 * '<S283>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Saturation/External'
 * '<S284>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Saturation/External/Saturation Dynamic'
 * '<S285>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Saturation Fdbk/Disabled'
 * '<S286>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Sum/Passthrough_I'
 * '<S287>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Sum Fdbk/Disabled'
 * '<S288>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Tracking Mode/Disabled'
 * '<S289>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Tracking Mode Sum/Passthrough'
 * '<S290>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Tsamp - Integral/Passthrough'
 * '<S291>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/Tsamp - Ngain/Passthrough'
 * '<S292>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/postSat Signal/Forward_Path'
 * '<S293>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/preInt Signal/Internal PreInt'
 * '<S294>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/PI Controller/preSat Signal/Forward_Path'
 * '<S295>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/Variant Subsystem1/GD32F103'
 * '<S296>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Torque_Mode_Protection/Variant Subsystem1/STM32F103'
 * '<S297>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Variant Subsystem/GD32F103'
 * '<S298>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Variant Subsystem/STM32F103'
 * '<S299>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Voltage_Mode_Protection/I_backCalc_fixdt'
 * '<S300>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Voltage_Mode_Protection/I_backCalc_fixdt1'
 * '<S301>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Voltage_Mode_Protection/I_backCalc_fixdt/Integrator'
 * '<S302>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Voltage_Mode_Protection/I_backCalc_fixdt/Saturation Dynamic1'
 * '<S303>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Voltage_Mode_Protection/I_backCalc_fixdt1/Integrator'
 * '<S304>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F05_Field_Oriented_Control/Motor_Limitations/Motor_Limitations_Enabled/Voltage_Mode_Protection/I_backCalc_fixdt1/Saturation Dynamic1'
 * '<S305>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F06_Control_Type_Management/COM_Method'
 * '<S306>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F06_Control_Type_Management/FOC_Method'
 * '<S307>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F06_Control_Type_Management/SIN_Method'
 * '<S308>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F06_Control_Type_Management/SIN_Method/Final_Phase_Advance_Calculation'
 * '<S309>' : 'BLDCmotor_FOC_R2017b_fixdt/BLDC_controller/F06_Control_Type_Management/SIN_Method/Final_Phase_Advance_Calculation/Modulo_fixdt'
 */
#endif                                 /* BLDC_controller_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
