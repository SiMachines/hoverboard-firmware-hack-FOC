# Hoverboard Driver FOC Motor Control with Encoder and Brake Resistor Support

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

This is a fork of the [hoverboard-firmware-hack-FOC](https://github.com/EFeru/hoverboard-firmware-hack-FOC) project that adds AB encoder support and brake resistor functionality for precision motor control applications. 

For general setup instructions, hardware information, and base firmware features, please refer to the [original repository](https://github.com/EFeru/hoverboard-firmware-hack-FOC).

---

## Key Improvements Over Original Repository

1. **Smooth Torque Output at Zero RPM**

Encoder feedback enables true zero-speed torque control with no cogging or vibration. Perfect for robotics applications requiring precise positioning and smooth low-speed operation.

2. **PSU Support**

Integrated brake resistor support allows safe operation from bench power supplies. The brake resistor dissipates regenerative energy that would otherwise cause voltage spikes and damage PSUs or trip overvoltage protection.

3. **High-Quality Input Control Performance**

Enhanced PWM input processing (both hardware and software implementations) provides:
- low latency 1000hz polling
- Noise-free
- High Resolution

For information on all improvments see [Pull Request #3](https://github.com/SiMachines/hoverboard-firmware-hack-FOC/pull/3#issue-3584417632).

---

## New Configurations

- **ONE_AXIS_VARIANT**: Single motor control with AB encoder, hardware pwm input and internal left driver brake resistor support
- **TWO_AXIS_VARIANT**: Dual motor control with AB encoders, software pwm input and external brake resistor support
---

## Quick Start Guide

### 1. Select Your Variant

In `platformio.ini`, uncomment your desired configuration:

```ini
default_envs = ONE_AXIS_VARIANT    ; Single motor with encoder
;default_envs = TWO_AXIS_VARIANT   ; Dual motors with encoders
```

### 2. Essential Configuration

Edit `Inc/config.h` with your specific parameters:

#### Battery Configuration
```c
#define BAT_CELLS               10      // Your battery cell count:
                                        // 6s  = 24V (22.2V - 25.2V)
                                        // 10s = 36V (37V - 42V)
                                        // 13s = 48V (48.1V - 54.6V)
```

#### Motor Configuration
```c
#define N_POLE_PAIRS            15      // Standard hoverboard motors: 15
                                        // Check your motor specs if different
```
---

## ONE_AXIS_VARIANT Setup

Single motor control with encoder feedback - default seetings setup for FFB WheelBase, change if your project is different

#### MCU Selection
```c
#define GD32F103Rx              1       // Uncomment if using GD32 MCU (108MHz)
                                        // Comment out for STM32F103 (72MHz)
```

### Encoder Configuration

```c
#define ENCODER_X
#define ENCODER_X_PPR              2048     // Your encoder pulses per revolution
#define ALIGNMENT_X_POWER          6553     // Sensor alignment voltage out of 16000 for HW & SW-PWM, out of 1000 for the others
```

### Motor Enable/Disable

```c
#undef MOTOR_LEFT_ENA                       // Disable left motor driver
#define MOTOR_RIGHT_ENA                     // Enable right motor only
```

### Current and Speed Limits

```c
#define I_MOT_MAX                  15       // [A] Maximum motor current
#define I_DC_MAX                   17       // [A] DC link current limit, (Above this value, current chopping is applied. To avoid this make sure that I_DC_MAX = I_MOT_MAX + 2A)  
#define N_MOT_MAX                  2000     // [rpm] Maximum motor speed
```

**Safety Tip**: Start with conservative limits (e.g., `I_MOT_MAX = 5A`) for initial testing, then increase gradually.

### Brake Resistor Setup

Choose **ONE** of the following options:

#### Option A: Internal Brake Resistor
Uses the left motor driver as a brake resistor (left motor must be disabled).

```c
#define INTBRK_L_EN                         // Enable internal brake on left driver
```

#### Option B: External Brake Resistor
Uses an external brake resistor connected positive psu supply in series with a mosfet/gate driver conntected to ground, can be a bts7960 for example if using with under 24v supply.

```c
#define EXTBRK_EN                           // Enable external brake resistor
#define EXTBRK_USE_CH3                      // Use channel 3 for brake control
// OR
//#define EXTBRK_USE_CH4                    // Use channel 4 for brake control
```

#### Brake Resistor Parameters

```c
#define BRAKE_RESISTANCE           300      // [Ohm × 100] Your resistor value
                                            // Example: 3.0Ω = 300, 2.2Ω = 220, enter slightly higher resistance then measured for example 3.0Ω instead of 2.2Ω to ensure adequate braking

#define BRKRESACT_SENS            40/20     // [A] Activation threshold (40mA typical), increase in 20mA steps if braking resistor gets warm while wheel is stationary

#define MAX_REGEN_CURRENT         0/20      // [A] Maximum regen current in 20mA steps
                                            // PSU: Set to 0/20 (0mA) 
                                            // Battery: Set how much regenative braking you want to allow 20/20 (20ma), 40/20(40ma) etc.
```

**⚠️ Critical Notes:**
- **PSU users MUST set `MAX_REGEN_CURRENT = 0/20`** to prevent voltage spikes
- If `BRKRESACT_SENS` is set too high it may cause over voltage in psu
- Add 10-20% safety margin to `BRAKE_RESISTANCE` value

### Control Input Selection

Choose your preferred input method:

```c
// Hardware PWM (best performance)
#define HW_PWM                     0        // Priority 0 (highest), HW pwm outputs to Right Motor
```

### Input Range Configuration

```c
#define PRI_INPUT1                 0, -16000, 0, 16000, 0  //Left Motor disabled
#define PRI_INPUT2                 2, -16000, 0, 16000, 0  //Right Motor Output
// Format: TYPE, MIN, MID, MAX, DEADBAND
//   TYPE: 0=Disabled, 1=Normal Pot, 2=Middle Resting, 3=Auto-detect
//   MIN/MAX: Input signal range
//   MID: Center position for middle-resting inputs
//   DEADBAND: Center deadzone width
```

### Response Tuning

```c
#define RATE                       32767    // Acceleration rate (32767 = fastest)
#define FILTER                     65535    // Input filtering (65535 = minimal)
```

Lower `RATE` values make acceleration smoother but slower. Lower `FILTER` values increase filtering (smoother but more lag).

### Motor Direction

```c
//#define INVERT_R_DIRECTION                // Uncomment to reverse motor direction
```
---

## TWO_AXIS_VARIANT Setup

Dual motor control with encoders on both motors - default setup is for ffb joystick or seat belt puller using Software pwm inputs on right side uart port and external brake resistor on PA2 left uart port

### Encoder Configuration

```c
#define ENCODER_X                           // Enable encoder on motor X (right)
#define ENCODER_Y                           // Enable encoder on motor Y (left)

#define ENCODER_X_PPR              2048     // Right motor encoder PPR
#define ALIGNMENT_X_POWER          6553     // Right motor alignment voltage

#define ENCODER_Y_PPR              2048     // Left motor encoder PPR  
#define ALIGNMENT_Y_POWER          6553     // Left motor alignment voltage
```

### Motor Enable

```c
#define MOTOR_LEFT_ENA                      // Enable left motor
#define MOTOR_RIGHT_ENA                     // Enable right motor
```
### Current and Speed Limits

```c
#define I_MOT_MAX                  15       // [A] Maximum current per motor
#define I_DC_MAX                   17       // [A] DC link current limit
#define N_MOT_MAX                  2000     // [rpm] Maximum motor speed
```

### Brake Resistor Setup

External brake resistor configuration (same as single-axis):

```c
#define EXTBRK_EN                           // Enable external brake resistor
#define EXTBRK_USE_CH3                      // Use channel 3 output

#define BRAKE_RESISTANCE           300      // [Ohm × 100]
#define BRKRESACT_SENS            40/20     // [A] Activation threshold (40mA typical), increase in 20mA steps if braking resistor gets warm while wheel is stationary
#define MAX_REGEN_CURRENT         0/20      // [A] Set to 0 for PSU operation
```

### Control Input

```c
#define SW_PWM_RIGHT               0        // Software PWM on PB10/PB11

#define PRI_INPUT1                 2, -1000, 0, 1000, 0  // Motor 1 control
#define PRI_INPUT2                 2, -1000, 0, 1000, 0  // Motor 2 control
```

### Motor Direction

```c
//#define INVERT_R_DIRECTION                  // Invert right motor
//#define INVERT_L_DIRECTION                // Invert left motor if needed
```

For differential drive robots, you typically need to invert one motor to achieve forward motion with matched inputs.

### Response Tuning

```c
#define RATE                       32767    // Acceleration rate
#define FILTER                     65535    // Input filtering
```

---

## Common Configuration (Both Variants)

### Control Type and Mode

```c
#define CTRL_TYP_SEL               FOC_CTRL // Field Oriented Control (recommended)
#define CTRL_MOD_REQ               TRQ_MODE // Torque mode (best for encoders)
```

**Control Modes:**
- `TRQ_MODE`: Torque control with freewheeling (recommended)
- `SPD_MODE`: Speed control with active velocity regulation
- `VLT_MODE`: Voltage control (fastest response, no feedback)

### Additional Features

```c
#define DIAG_ENA                   0        // Disable diagnostics (allows stall)
#define INACTIVITY_TIMEOUT         100      // Power-off timeout (minutes)
#define HOCP                                // Hardware over-current protection
#define BEEPER_OFF                          // Disable buzzer
#define DC_LINK_WATCHDOG_ENABLE             // Enable voltage monitoring
#define FIELD_WEAK_ENA             0        // Disable field weakening
```

---
### Flash the Firmware

Connect your ST-Link programmer and upload.

## Wiring Guide

### Encoder Connections

**For ONE_AXIS_VARIANT (Right motor - ENCODER_X):**
- Connect encoder channels A and B to the right sensor cable
- Ensure proper encoder power supply (3.3V or 5V depending on encoder type)
- Use shielded cable for long encoder runs

**For TWO_AXIS_VARIANT:**
- ENCODER_X: Right motor encoder → Right sensor cable
- ENCODER_Y: Left motor encoder → Left sensor cable

### Brake Resistor Connections

**Internal Brake (`INTBRK_L_EN`):**
- Uses left motor driver phase outputs
- No additional wiring required
- Left motor must be disabled

**External Brake (`EXTBRK_EN`):**
- Connect brake resistor between brake output pin and GND
- Use high-wattage resistor (minimum 50W for typical applications)
- Ensure proper cooling for the resistor

### Control Input Connections

**Hardware PWM (`HW_PWM`):**
- PWM input on designated pins (see pinout diagram in original repo)

**Software PWM (`SW_PWM_RIGHT`):**
- PWM inputs on PB10 and PB11
- 5V tolerant inputs

**ADC (`CONTROL_ADC`):**
- Potentiometer inputs on left sensor cable
- **NOT 5V tolerant** - use 0-3.3V signals only

---

## Troubleshooting

### Motor Doesn't Spin

**Check encoder configuration:**
- Verify `ENCODER_X_PPR` matches your encoder specification
- Try increasing `ALIGNMENT_X_POWER` (up to 8000-10000)
- Check encoder wiring (A, B channels and power)
- Ensure encoder power supply is stable

**Verify motor enable:**
- Confirm correct motor is enabled (`MOTOR_LEFT_ENA` / `MOTOR_RIGHT_ENA`)
- Check that internal brake isn't enabled on active motor

**Test control input:**
- Monitor input values via serial debug
- Verify input range configuration matches actual signal
- Check for proper signal connections

### Brake Resistor Issues

**PSU overvoltage trips:**
- Ensure `MAX_REGEN_CURRENT = 0/20` for PSU operation
- Verify brake resistor is properly connected
- Check `BRAKE_RESISTANCE` value matches actual resistor
- Reduce motor speed/torque to lower regen current

**Excessive resistor heating:**
- Use higher wattage resistor
- Increase `BRAKE_RESISTANCE` value (use larger resistor)
- Add cooling (heatsink or fan)
- Reduce motor deceleration rate

### Over-Current Protection Trips

**Frequent over-current faults:**
- Reduce `I_MOT_MAX` to more conservative value
- Increase `RATE` value for slower acceleration
- Check for mechanical binding or excessive load
- Verify motor phase connections are secure

**Startup over-current:**
- Reduce `ALIGNMENT_X_POWER` 
- Ensure motor is free to rotate during startup
- Check for short circuits in motor windings

### Encoder Not Detected

**No encoder feedback:**
- Verify encoder power supply voltage and current capacity
- Check encoder signal levels (should be clean square waves)
- Confirm `ENCODER_X_PPR` matches encoder spec exactly
- Test encoder with oscilloscope if available
- Try different encoder (some types are not compatible)

**Erratic position readings:**
- Use shielded cable for encoder signals
- Keep encoder cables away from motor power wires
- Add pull-up resistors if encoder has open-collector outputs
- Check for loose connections

### Poor Control Response

**Sluggish or jerky motion:**
- Increase `RATE` for faster response (decrease for smoother)
- Adjust `FILTER` to balance noise rejection vs. responsiveness
- Verify control input signal quality
- Check that `CTRL_MOD_REQ` is set appropriately for application

**Motor oscillates or vibrates:**
- Reduce controller gains (requires modifying controller parameters)
- Increase `FILTER` value to reduce noise sensitivity
- Check mechanical coupling for excessive backlash
- Verify encoder mounting is rigid

---

## Safety Guidelines

⚠️ **Read before operating:**

1. **Initial Testing:**
   - Start with low current limits (`I_MOT_MAX = 5A`)
   - Test at low speeds first
   - Keep emergency stop accessible
   - Secure motor to prevent unexpected motion

2. **PSU Operation:**
   - **ALWAYS use brake resistor with `MAX_REGEN_CURRENT = 0/20`**
   - Verify PSU has overvoltage protection
   - Use adequate PSU power rating (2x motor power minimum)

3. **Battery Operation:**
   - Configure `BAT_CELLS` correctly for your pack
   - Set `MAX_REGEN_CURRENT` within BMS charge limits
   - Monitor battery voltage during operation
   - Never exceed battery maximum voltage

4. **Mechanical Safety:**
   - Secure all rotating parts with proper guards
   - Use emergency stop in accessible location
   - Keep hands and loose items away from motors
   - Ensure proper mechanical coupling strength

5. **Electrical Safety:**
   - Double-check all connections before power-on
   - Use proper wire gauge for current levels
   - Ensure good ventilation for brake resistor
   - Monitor component temperatures during operation

---

## Performance Tuning

### Optimizing for Your Application

**High-Speed Applications:**
- Increase `N_MOT_MAX` if motors can handle it
- Consider enabling field weakening (`FIELD_WEAK_ENA = 1`)
- Use faster acceleration (`RATE = 32767`)
- Minimize filtering (`FILTER = 65535`)

**Precision Positioning:**
- Use torque mode (`CTRL_MOD_REQ = TRQ_MODE`)
- Moderate acceleration rate (`RATE = 16000-24000`)
- Balanced filtering (`FILTER = 32000-48000`)
- Conservative current limits for smooth motion

**Heavy Load Applications:**
- Increase current limits if drivers can handle it
- Use slower acceleration (`RATE = 8000-16000`)
- Monitor motor and driver temperatures
- Consider active cooling for extended operation

**Low-Noise Operation:**
- Increase filtering (`FILTER = 16000-32000`)
- Moderate acceleration rate
- Keep current limits reasonable
- Ensure good mechanical assembly (tight tolerances)

---

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

When submitting PRs:
- Test thoroughly on actual hardware
- Document any configuration changes needed
- Include clear descriptions of improvements
- Follow existing code style

---

## License

This project inherits the GPLv3 license from the original [hoverboard-firmware-hack-FOC](https://github.com/EFeru/hoverboard-firmware-hack-FOC) repository.

---

## Support

For issues specific to encoder and brake resistor functionality, please open an issue in this repository.

For general hoverboard firmware questions, refer to the [original repository](https://github.com/EFeru/hoverboard-firmware-hack-FOC) and its wiki.

---

## Acknowledgments

- Original FOC firmware: [EFeru/hoverboard-firmware-hack-FOC](https://github.com/EFeru/hoverboard-firmware-hack-FOC)
- FOC controller design: [EFeru/bldc-motor-control-FOC](https://github.com/EFeru/bldc-motor-control-FOC)
- Hoverboard hacking community for extensive documentation and support

---

## Recent Changes (This Fork)

- Added AB quadrature encoder support for precise motor control
- Implemented internal and external brake resistor functionality
- Enhanced PWM input processing (hardware and software)
- Improved ADC watchdog handling with hysteresis
- Added fast startup undervoltage detection
- Centralized DC-link voltage monitoring
- Various safety and reliability improvements

For detailed technical changes, see [Pull Request #3](https://github.com/SiMachines/hoverboard-firmware-hack-FOC/pull/3#issue-3584417632).
