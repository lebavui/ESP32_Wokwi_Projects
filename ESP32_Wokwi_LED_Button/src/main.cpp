#include <Arduino.h>

void setup() {
  pinMode(2, OUTPUT);
}

void loop() {
  if (digitalRead(4))
    digitalWrite(2, HIGH);
  else
    digitalWrite(2, LOW);
}