#include <DHT.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "";
const char* password = "";
const char* mqttServer = "driver.cloudmqtt.com";
const int   mqttPort = 18960;
const char* mqttUser = "";
const char* mqttPassword = "";
const char* tempTopic = "firo-esp/temperature";
const char* humiTopic = "firo-esp/humidity";

 
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);
  Serial.println(F("Fridge IoT!")); 
  dht.begin();

  WiFi.begin(ssid, password); // Connect to the network
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network!");

  client.setServer(mqttServer, mqttPort);
  //client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
  
}

void loop() {
  delay(2000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(F(" Humidity: "));
  Serial.print(h);
  Serial.println(F("%"));

  client.publish(tempTopic, String(t).c_str());
  client.publish(humiTopic, String(h).c_str());
  
}
