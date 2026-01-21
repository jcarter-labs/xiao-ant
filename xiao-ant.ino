#include <Bounce2.h>

const uint8_t BUTTON_PIN = D10;
const uint8_t ANT_PINS[] = {3, 4, 5};
const uint8_t NUM_ANTENNAS = 3;
const uint8_t DEBOUNCE_MS = 10;

Bounce button = Bounce();
uint8_t antenna_state = 1;

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("XIAO ESP32C3 Antenna Controller 1.20.26  v2");
  
  button.attach(BUTTON_PIN, INPUT_PULLUP);
  button.interval(DEBOUNCE_MS);
  
  for (uint8_t i = 0; i < NUM_ANTENNAS; i++) {
    pinMode(ANT_PINS[i], OUTPUT);
    digitalWrite(ANT_PINS[i], LOW);
  }
  
  set_antenna(antenna_state);
}

void loop() {
  button.update();
  
  if (button.rose()) {
    antenna_state = (antenna_state % NUM_ANTENNAS) + 1;
    set_antenna(antenna_state);
  }
}

void set_antenna(uint8_t ant) {
  for (uint8_t i = 0; i < NUM_ANTENNAS; i++) {
    digitalWrite(ANT_PINS[i], LOW);
  }
  
  digitalWrite(ANT_PINS[ant - 1], HIGH);
  
  Serial.print("Antenna: ");
  Serial.println(ant);
}