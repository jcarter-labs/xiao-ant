/*
 * LED Selection Test - XIAO ESP32C3
 * 
 * PURPOSE: Test white LED brightness with 470Ω resistors
 *          before soldering permanent connections
 * 
 * HARDWARE:
 * - GPIO3 (D1): White LED + 470Ω → GND (will drive ULN2803 input 1)
 * - GPIO4 (D2): White LED + 470Ω → GND (will drive ULN2803 input 2)
 * - GPIO5 (D3): Reserved for ULN2803 input 3 (not used in this test)
 * 
 * BEHAVIOR:
 * - Both LEDs flash simultaneously
 * - 95% duty cycle: 950ms ON / 50ms OFF
 * - Validates LED brightness before final assembly
 * 
 * SUCCESS CRITERIA:
 * - LEDs visible and synchronized
 * - If too dim: decrease resistor to 330Ω or 220Ω
 * 
 * NEXT STEP: Add pushbutton on GPIO21 (D6)
 */

// GPIO Pin Definitions
#define LED1_PIN    3   // D1 - Antenna 1 indicator + ULN2803 input
#define LED2_PIN    4   // D2 - Antenna 2 indicator + ULN2803 input
#define LED3_PIN    5   // D3 - Antenna 3 (reserved, not used in this test)

// Timing Constants (milliseconds)
#define ON_TIME     950  // 95% duty cycle
#define OFF_TIME    50   // 5% duty cycle

void setup() {
  // Configure GPIO pins as outputs
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  
  // Initialize all outputs LOW (LEDs off)
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
}

void loop() {
  // Turn both LEDs ON simultaneously
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED2_PIN, HIGH);
  delay(ON_TIME);
  
  // Turn both LEDs OFF simultaneously
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  delay(OFF_TIME);
}
