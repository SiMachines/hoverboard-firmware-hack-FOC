# Copilot / AI agent instructions — hoverboard-firmware-hack-FOC
Read before editing or reviewing.

**Platform snapshot**
- Target `STM32F103RCT6` or `GD32F013RCT6`  (Cortex-M3) controlling two BLDC motors via FOC; timing-critical ISR paths live in `Src/bldc.c`, `Src/util.c`, `Src/control.c`.
- Build with PlatformIO (`platformio run`, `platformio run --target upload`, `platformio run --target clean`); `.github/workflows/build_on_commit.yml` mirrors this flow.
- Firmware boots through `startup_stm32f103xe.s` → `system_stm32f1xx.c` → `Src/main.c`; hardware bring-up is concentrated in `Src/setup.c`.

**Architecture map**
- Simulink-generated control core: `Src/BLDC_controller*.c` plus `Inc/BLDC_controller.h` expose `BLDC_controller_step` for left/right motors; treat these files as generated artefacts—wrap, don’t fork.
- Application glue: `Src/bldc.c` handles the dual-motor state machine, current limits, and calls into the controller; `Src/util.c` manages encoder alignment, angle estimation, and safety fallbacks; `Src/control.c` fans out different input back-ends (ADC, PPM, PWM, Nunchuk, iBUS, USART) and feeds neutralised commands into `bldc.c`.
- Communications & persistence: `Src/comms.c` implements the debug/parameter protocol (guarded by `DEBUG_SERIAL_PROTOCOL`), `Src/eeprom.c` persists config words defined in `Inc/eeprom.h`.

**Configuration levers**
- `platformio.ini` selects `genericSTM32F103RC_btt` with `stm32cube` HAL; keep optimisation flags (`-Ofast`, `-mfpu`) and linkers (`STM32F103RCTx_FLASH.ld`) consistent when adding build flags.

**Implementation patterns**
- Control loops are fixed-point; `Src/bldc.c` and `Src/util.c` expect torque/speed/current in scaled integers (`A2BIT_CONV`, `MAP`, `CLAMP`). Avoid introducing `float`/`double` in hot paths unless guarded by config and justified with size/runtime impact.
- Input processing runs partly in ISRs (`PPM_ISR_Callback`, `PWM_ISR_*`, `HAL_TIM_IC_CaptureCallback`) and partly in the 1 kHz SysTick. Keep ISR work bounded and defer heavy logic to the main loop.
- Shared structs like `input1[]`, `input2[]`, `rtU_*`, `rtY_*` are global; any new access must remain re-entrant-safe across interrupts.
- EEPROM writes go through `setParamVal*` helpers in `Src/comms.c`;

**Forbidden / risky edits**
- Do not hand-edit generated Simulink files (`Src/BLDC_controller*.c`, `Src/BLDC_controller_data.c`, `Inc/BLDC_controller.h`) without documenting the delta and reasoning; prefer wrapper hooks in `Src/bldc.c` or `Src/util.c`.
- Avoid dynamic allocation, recursion, or large stack frames; all memory must remain static or on small stack segments.
- Interrupt vectors in `Src/stm32f1xx_it.c` and HAL drivers under `Drivers/STM32F1xx_HAL_Driver` should stay aligned with CubeMX defaults—alter only when you understand the peripheral clocks and NVIC priorities.

**Workflow tips**
- There are no automated tests; always run `platformio run` and, when possible, bench-test on hardware after large control changes.
- Preserve public headers: do not rename or remove symbols declared in `Inc/*.h` without explicit instruction.
- Avoid introducing dynamic memory allocation; the code is embedded and uses static/global structures.
- When suggesting patches, provide minimal diffs, reference the exact file paths and line ranges, and include a short justification and any measurable impact (size, stack, cycles) if applicable.
