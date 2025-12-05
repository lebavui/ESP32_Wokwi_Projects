#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message: ");
  for (unsigned int i = 0; i < length; i++) 
    Serial.print((char)payload[i]);
  Serial.println("");  
}

void setup() {
  Serial.begin(115200);

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

  client.setServer("broker.hivemq.com", 1883);
  client.setCallback(callback);
  client.connect("client_id_123456");

  while (!client.connected()) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected to MQTT broker!");

  client.subscribe("iotclass/state");
}

void loop() {
  client.loop(); 
  client.publish("iotclass/telemetry", "{\"temperature\":25, \"humidity\":60}");
  if (Serial.available()) {
    String msg = Serial.readStringUntil('\n');
    Serial.print("Echoing: ");
    Serial.println(msg);
  }
  delay(3000);
}
