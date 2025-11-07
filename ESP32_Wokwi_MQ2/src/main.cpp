#include <Arduino.h>

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print("Analog: ");
  Serial.println(analogRead(14));
  Serial.print("Digital: ");
  Serial.println(digitalRead(12));
  delay(5000);
}