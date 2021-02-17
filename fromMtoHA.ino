#include <ESP8266WiFi.h>
#include <Wire.h>
#include <PubSubClient.h>

#define wifi_ssid "Ayoub"
#define wifi_password "Ayoub1927"

#define mqtt_server "192.168.0.113"
#define mqtt_user "mqttuser"
#define mqtt_password "Ayoub1927"

#define humidity_topic "sensor/humidity"
#define temperature_topic "sensor/temperature"


#include "DHT.h"
#define DHTPIN D2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);



WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  // Start sensor
  dht.begin();
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

long lastMsg = 0;
float temp = 0.0;
float hum = 0.0;
float diff = 1.0;

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 1000) {
    lastMsg = now;

    float newTemp = dataTemp();
    int newHum = dataHumidity();

      Serial.print("New temperature:");
      Serial.println(String(dht.readTemperature()).c_str());
      client.publish(temperature_topic, String(dht.readTemperature()).c_str(), true);
      Serial.print("New humidity:");
      Serial.println(String(dht.readHumidity()).c_str());
      client.publish(humidity_topic, String(dht.readHumidity()).c_str(), true);
  }
}


int dataHumidity() {
  int h = dht.readHumidity();
  return h;
}

float dataTemp(){
  float t = dht.readTemperature();
  return t;
}
