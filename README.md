# XIAO ESP32C3 Antenna Controller

3-antenna selector for ham radio operations (3-30MHz) with pushbutton control and relay switching.

## Hardware

**Microcontroller:** Seeed Studio XIAO ESP32C3  
**Relays:** 3x 12V SPDT relays (antenna switching)  
**Driver:** ULN2803 Darlington array (relay coil drive)  
**Input:** Momentary pushbutton (antenna selection)  
**Indicators:** 3x LEDs (antenna status)

## Pin Connections

| Function | XIAO Pin | GPIO | Component |
|----------|----------|------|-----------|
| Antenna 1 Output | D0 | GPIO3 | LED1 + ULN2803 IN1 |
| Antenna 2 Output | D1 | GPIO4 | LED2 + ULN2803 IN2 |
| Antenna 3 Output | D2 | GPIO5 | LED3 + ULN2803 IN3 |
| Button Input | D10 | GPIO10 | Pushbutton to GND |

**ULN2803 Outputs:**
- OUT1 → Relay 1 coil (12V)
- OUT2 → Relay 2 coil (12V)
- OUT3 → Relay 3 coil (12V)

**Power:**
- XIAO: 5V via USB-C
- Relays: External 12V supply

## Operation

**Power-On:** Antenna 1 selected (ANT1 LED lit)  
**Button Press:** Cycles through antennas  
**Sequence:** ANT1 → ANT2 → ANT3 → ANT1 (continuous)  
**Safety:** Always one antenna connected (no OFF state)

**Serial Output (115200 baud):**
- Reports antenna changes: "Antenna: 1", "Antenna: 2", "Antenna: 3"
- No output on startup (USB CDC buffering avoidance)

## Build Instructions

**Prerequisites:**
- Arduino IDE 2.3.7 or later
- ESP32 board package 3.3.5
- Bounce2 library (via Library Manager)

**Installation:**

1. **Install ESP32 Board Support:**
   - Arduino IDE → Preferences
   - Additional Board Manager URLs: `https://espressif.github.io/arduino-esp32/package_esp32_index.json`
   - Tools → Board → Boards Manager → search "esp32" → Install 3.3.5

2. **Install Bounce2 Library:**
   - Tools → Manage Libraries
   - Search "Bounce2" → Install

3. **Select Board:**
   - Tools → Board → ESP32 → XIAO_ESP32C3

4. **Upload:**
   - Connect XIAO via USB-C
   - Verify port (Tools → Port)
   - Upload sketch

## RFI Mitigation

**Ferrite Suppression:**
- FT63-43 ferrite toroid
- 4 turns on power cables
- 4 turns on USB cable

## Design Notes

- **Debounce:** 50ms (Bounce2 library)
- **Relay Drive:** GPIO HIGH → ULN2803 output LOW → relay energized
- **Ham Radio Safety:** No OFF state ensures continuous antenna connection
- **D10 Hardware:** Do NOT add capacitor across button input (causes false triggers on boot)

## Version History

**v3 (1.22.26):**
- Production release
- 50ms debounce
- Removed startup serial output
- Validated hardware operation

## License

MIT License - See LICENSE file

## Author

John Carter (N6YU)  
https://github.com/jcarter-labs/xiao-ant
