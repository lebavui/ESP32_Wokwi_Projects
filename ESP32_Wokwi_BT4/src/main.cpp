#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define LED_PIN 12
#define PIR_PIN 13
#define DHT_PIN 4

LiquidCrystal_I2C lcd(0x27,16,2);
DHT_Unified dht(DHT_PIN, DHT22);
sensors_event_t event;

int count = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  lcd.init();
  dht.begin();
}

void loop() {
  dht.temperature().getEvent(&event);
  float temperature = event.temperature;
  dht.humidity().getEvent(&event);
  float humidity = event.relative_humidity;

  Serial.printf("Temperature: %.1foC\r\n", temperature);
  Serial.printf("Humidity: %.1f%%\r\n", humidity);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.printf("Temp: %.1foC", temperature);
  lcd.setCursor(0, 1);
  lcd.printf("Humidity: %.1f%%", humidity);

  if (temperature > 50) {
    Serial.println("Overheat detected! Turn on alarm!\r");
    digitalWrite(LED_PIN, HIGH);
  }
  else
    digitalWrite(LED_PIN, LOW);

  if (digitalRead(PIR_PIN)) {
    Serial.println("Motion detected! Turn on backlight!\r");
    lcd.backlight();
    count = 0;
  } else {
    count++;
    if (count == 5) {
      Serial.println("No motion detected in 10s! Turn off backlight!\r");
      lcd.noBacklight();
    }      
  }

  delay(2000);
}