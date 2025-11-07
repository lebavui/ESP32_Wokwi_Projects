#include <WiFi.h>
#include <HTTPClient.h>

#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

DHT_Unified dht(5, DHT22); // Output Pin: 5, Type DHT22

#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET     -1   // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // < See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* ssid = "Wokwi-GUEST";
const char* password = "";

void setup() {
  Serial.begin(115200);
  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextSize(1.5);
  display.setTextColor(SSD1306_WHITE);
  display.println("Connect to WiFi");
  display.display();

  // Connect to Wi-Fi
  Serial.printf("Connecting to Wi-Fi: %s\n", ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to Wi-Fi!");

  pinMode(4, OUTPUT);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    display.clearDisplay();

    // Gửi yêu cầu lấy trạng thái LED

    http.begin("http://host.wokwi.internal:1880/get_state");

    int httpCode = http.GET();

    if (httpCode > 0) {
      Serial.printf("Response code: %d\n", httpCode);

      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println("Response body:");
        Serial.println(payload);

        display.setCursor(5, 5);
        display.print(payload);
        display.display();

        if (payload.equals("{\"switch\":true}")) {
          digitalWrite(4, HIGH);
        } else {
          digitalWrite(4, LOW);
        }
      }
    } else {
      Serial.printf("Error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();

    sensors_event_t event;

    dht.temperature().getEvent(&event);
    float temperature = event.temperature;
    Serial.print("Temperature: ");
    Serial.println(temperature);

    dht.humidity().getEvent(&event);
    float humidity = event.relative_humidity;
    Serial.print("Humidity: ");
    Serial.println(humidity);

    display.setCursor(5, 25);
    display.printf("Temperature: %.2f", temperature);
    display.setCursor(5, 45);
    display.printf("Humidity: %.2f", humidity);
    display.display();

    // Gửi yêu cầu cập nhật nhiệt độ và độ ẩm
    char url[80];
    sprintf(url, "http://host.wokwi.internal:1880/get_to_send?temperature=%.1f&humidity=%.1f", temperature, humidity);

    http.begin(url);

    httpCode = http.GET();

    display.setCursor(5, 65);
    display.printf("Response Code: %d", httpCode);

    if (httpCode > 0) {
      Serial.printf("Response code: %d\n", httpCode);

      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println("Response body:");
        Serial.println(payload);
      }
    } else {
      Serial.printf("Error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(5000);
}
