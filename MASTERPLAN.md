# XIAO Antenna Controller Master Plan

## XIAO ESP32C3 Antenna Controller 1/19/2026

## Goal

Build reliable 3-antenna controller replacing crash-prone Raspberry Pi Pico system.

**System Architecture:**
- 5-conductor cable connecting control panel (XIAO, ULN2803, LEDs, pushbutton) to relay enclosure (3×12V relays, power, ground)
- Single pushbutton cycles through 3 antennas with visual LED indicators

**Primary Objectives:**
- Eliminate overnight crashes (symptom: system unresponsive by morning; root cause unknown but likely Linux/Pico stability issues)
- Achieve low RFI operation in 3-30MHz range
- Simple, maintainable C codebase with minimal complexity
- Ease of standing up and getting operational without extensive testing overhead

**Secondary Objectives:**
- Architecture supports future wifi expansion (not implemented this phase)
- Power-on default state acceptable (no selection memory required)

---

## Context

### Development Environment
- **Dev Machine:** Beelink SER8 running Ubuntu 25
- **Target Hardware:** XIAO ESP32C3 (Seeed Studio)
- **Toolchain:** Arduino IDE 2.3.7, ESP32 board package 3.3.5 (**FROZEN**)
- **Version Control:** Git + GitHub repository from dev machine
- **Code Storage:** Local object code on XIAO (flashed via USB)
- **Project Directory:** `xaio-ant` on dev machine
- **Code Transfer:** USB connection to XIAO

### Hardware Architecture

**GPIO Pin Assignments:**
- **GPIO3, GPIO4, GPIO5:** Outputs driving LEDs + ULN2803 inputs for 12V relays
  - Active-high logic to drive ULN2803 transistor bases sufficiently
  - Each pin drives: white LED (1kΩ resistor, reducible to 270Ω if brightness insufficient) + ULN2803 input
- **GPIO10:** Input for pushbutton (active-low with internal pullup)

**Power Distribution:**
- **XIAO:** USB-powered (development and production)
- **Relays:** External 12V regulated supply (stable, no brownout concerns)
- **Cable:** 5-conductor (3×relay coil, +12V, GND)

**RFI Mitigation:**
- **Cable chokes:** FT63-43 ferrite toroids, 4 turns each on:
  - 12V power cable
  - USB cable to XIAO
  - 12V return path
- **Enclosure/grounding:** Primary RFI strategy (not code-level concerns)
- **Type 43 material:** Effective suppression for 3-30MHz HF range

**Physical Build:**
- **Prototype:** Solderless breadboard only (no PCB/perfboard this phase)
- **Components:** 
  - ULN2803 driver array (internal flyback diodes assumed sufficient)
  - 3×12V relays
  - 3×ultra-bright white LEDs with 1kΩ current-limiting resistors
  - Single pushbutton switch

### Software Stack
- **Language:** C with Arduino framework
- **Libraries:** Arduino core, Adafruit, Seeed Studio, other standard libraries as needed
- **Core Logic:** No external dependencies for debounce/GPIO (prefer Arduino library calls)
- **KISS Philosophy:** Defer interrupts and watchdog timers for this project scope

---

## Factors to Watch

### Typical Failure Modes
1. **Debounce failures:** Acceptable false-trigger rate ≤0.1% (1 in 1000 presses)
2. **Relay contact bounce:** Reduce for UI reliability; RF safety not a concern for this application
3. **GPIO state corruption:** ESP32 must maintain output states reliably overnight
4. **Power-on glitches:** Relays should not chatter during XIAO boot sequence
5. **RFI generation:** HF switching noise from XIAO coupling into 3-30MHz receive path

### Hardware Risks
- **GPIO pin damage:** Breadboard testing may damage pins 3/4/5 (assignments may need flexibility)
- **ULN2803 protection:** Relay coil flyback assumed sufficient with internal diodes
- **LED brightness:** 1kΩ resistors may require reduction to 270Ω for visibility
- **Cable coupling:** 5-conductor bundle may pick up/radiate RFI despite FT63-43 chokes

### Software Risks
- **Debounce strategy:** Polling vs interrupt tradeoff (recommend polling for KISS)
- **State machine:** Rapid button presses causing confusion or missed transitions
- **Watchdog interaction:** Deferred per KISS; may revisit if overnight crashes persist

### Validation Challenges
- **Overnight crash detection:** Manual morning check (cycle antennas, verify response)
- **RFI measurement:** Requires K4 receiver audit on 80M, 40M, 20M, 15M, 10M bands
- **Long-term reliability:** <1 crash/month target requires extended usage testing

---

## Approach

### Development Methodology: Lightweight TDD

**Feature Integration Order (Setup → Input → Output):**
0. **Setup:** Toolchain validation, GPIO initialization
1. **Pushbutton debounce logic**
2. **LED + Relay control** (combined - same GPIO drives both)
3. **Integration test** (full system validation)

**Test Philosophy:**
- **4 test points** at key functional boundaries (setup + 3 features)
- Focus on **typical failure modes**, not exhaustive edge cases
- Test code is **throwaway** (delete after feature validation)
- Serial monitor provides automated pass/fail output
- **Query before implementing specific tests** (test design requires approval)

---

### Test Points

**Test 0: Setup Validation**
- **Purpose:** Verify toolchain, board connectivity, basic GPIO control
- **Method:** Simple LED blink on GPIO3/4/5, serial output confirms board identity
- **Pass Criteria:** LEDs toggle, serial monitor shows ESP32C3 board info
- **Validates:** Arduino IDE → XIAO USB link, GPIO output capability
- **Git Milestone 0:** Setup complete, toolchain frozen

---

**Test 1: Debounce Validation**
- **Purpose:** Verify clean edge detection on GPIO10
- **Method:** Serial output logs each valid button press with timestamp
- **Pass Criteria:** 
  - No false triggers during 100-press test
  - No missed presses during deliberate slow/fast press sequences
  - Contact bounce suppressed (single press = single state change)
- **Validates:** Input reliability, state machine foundation
- **Git Milestone 1:** Debounce logic validated and committed

---

**Test 2: LED + Relay Driver Validation**
- **Purpose:** Verify GPIO3/4/5 drive LEDs and ULN2803 → relays correctly
- **Method:** 
  - Cycle through 3 states (one LED/relay on at a time)
  - Serial output confirms GPIO state changes
  - Visual: LED brightness, only one lit per state
  - Audible: Relay click on transition
  - Electrical: Multimeter confirms relay contact closure
- **Pass Criteria:**
  - All 3 LEDs illuminate with acceptable brightness
  - All 3 relays switch cleanly (audible click, contact continuity)
  - Rapid cycling (10 transitions/sec) shows no lockup or chatter
- **Validates:** Output driver capability, ULN2803 interface, relay coil drive
- **Git Milestone 2:** LED + relay control validated and committed

---

**Test 3: Integration Test**
- **Purpose:** Full system validation (button → debounce → state → output)
- **Method:**
  - Press button, verify antenna cycles 1→2→3→1→...
  - Serial output logs: button press, debounce result, state change, GPIO updates
  - Rapid button mashing test (20 presses in 5 seconds)
- **Pass Criteria:**
  - Correct antenna cycling with LED/relay correlation
  - No missed presses or false triggers during normal use
  - Rapid mashing cycles smoothly without lockup
  - Power-on default state repeatable (antenna 1 selected)
- **Validates:** Complete system operation, state machine robustness
- **Git Milestone 3:** Integration complete, production-ready code committed

---

### Development Workflow

**Per Test Cycle:**
1. Write test sketch for current feature
2. Run automated test, observe serial output + hardware behavior
3. Iterate until pass criteria met
4. Delete test code, commit working feature to Git
5. Move to next test

**Git Strategy:**
- **4 major commits:** Setup, Debounce, LED+Relay, Integration
- Clean history (no test code remnants)
- Each commit represents validated, production-ready feature

**Tool Usage:**
- **Serial Monitor:** Primary validation interface (automated pass/fail messages)
- **Hardware Observation:** LED states, relay clicks, multimeter for continuity
- **Version Control:** Commit after each major milestone passes

---

## Success Criteria

### Functional Requirements
✓ **Antenna Selection:** Pushbutton cycles through 3 antennas (1→2→3→1...)  
✓ **Visual Feedback:** Active antenna indicated by LED (only one LED on at a time)  
✓ **Relay Control:** Corresponding relay energized for selected antenna  
✓ **Debounce Quality:** ≤0.1% false trigger rate (1 in 1000 presses)  
✓ **Rapid Press Handling:** Quick button mashing cycles antennas smoothly without lockup

### Reliability Requirements
✓ **Overnight Stability:** <1 crash per month demonstrated through usage testing  
✓ **Power-On Behavior:** Repeatable default state (antenna 1), no relay chatter during boot  
✓ **State Integrity:** GPIO outputs maintain correct levels indefinitely

### RFI Requirements
✓ **Noise Floor Test:** K4 receiver shows acceptable noise floor on 80M, 40M, 20M, 15M, 10M bands  
✓ **Comparison Baseline:** XIAO controller generates less RFI than Pico system (RMS RF levels comparison)  
✓ **Measurement Method:** Band sampling with XIAO powered ON vs OFF

### Development Completion
✓ **All 4 test points passed** (Setup, Debounce, LED+Relay, Integration)  
✓ **Code committed to GitHub** with clean history (4 major milestones, test code removed)  
✓ **Breadboard prototype validated** for functionality and RFI  
✓ **Documentation:** README with GPIO assignments, toolchain versions, known issues, FT63-43 choke specs

---

## Next Steps

1. **Confirm Test 0 design** (setup validation sketch)
2. Execute Test 0: toolchain validation, GPIO blink test
3. **Confirm Test 1 design** (debounce validation sketch)
4. Execute Test 1-3 with serial monitor automation
5. **Integrate with usage:** Verify overnight stability during multi-week operation
6. **RFI baseline:** K4 receiver measurements vs Pico system
