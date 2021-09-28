#include "Blinkenlight.h"

void Blinkenlight::init() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LED_ON_STATE);
}

void Blinkenlight::update() {
  static Counter counter;

  if (counter) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }
}
