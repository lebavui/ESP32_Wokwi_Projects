#include <WiFi.h>
#include <PubSubClient.h>

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

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message: ");
  String msg = "";
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    msg += (char) payload[i];
  }
    
  Serial.println("");  

  if (msg.equals("{\"switch\":true}")) {
    digitalWrite(4, HIGH);
  } else {
    digitalWrite(4, LOW);
  }
}

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
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer("host.wokwi.internal", 1883);
  client.setCallback(callback);
  client.connect("wokwi_client_id_123456");

  while (!client.connected()) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected to MQTT broker!");

  client.subscribe("local/state");

  pinMode(4, OUTPUT);
}

void loop() {
  display.clearDisplay();

  client.loop(); 

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

  char msg[80];
  sprintf(msg, "{\"temperature\":%.1f, \"humidity\":%.1f}", temperature, humidity);
  client.publish("local/telemetry", msg);

  delay(3000);
}
