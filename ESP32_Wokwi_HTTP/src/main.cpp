#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Connect to Wi-Fi
  Serial.printf("Connecting to Wi-Fi: %s\n", ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // === Send GET ===
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = "https://postman-echo.com/get?param1=value1&param2=value2";
    http.begin(url);

    int httpCode = http.GET();

    if (httpCode > 0) {
      Serial.printf("Response code: %d\n", httpCode);

      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println("GET response body:");
        Serial.println(payload);
      }
    } else {
      Serial.printf("Error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(2000);

  // === Send POST ===
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = "https://postman-echo.com/post";
    http.begin(url);

    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String body = "param1=value1&param2=value2";

    int httpCode = http.POST(body);

    if (httpCode > 0) {
      Serial.printf("Response code: %d\n", httpCode);

      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println("POST response body:");
        Serial.println(payload);
      }
    } else {
      Serial.printf("Error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

}

void loop() {
}
