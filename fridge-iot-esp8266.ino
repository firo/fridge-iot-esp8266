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
const char* topic = "fridgeIoT";
const char* serial = "123-456-789";

 
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

  static char jsonData0[10] = "{\"s__c\":";
  static char jsonData1[10] = ", \"t__c\":";
  static char jsonData2[10] = ", \"h__c\":";
  static char jsonData5[10] = ", \"p__c\":";
  static char jsonData3[5] = "}";
  static char jsonData4[5] = "\"";
  char sensorValue[80];


  strcpy(sensorValue, jsonData0);
  strcat(sensorValue, jsonData4);
  strcat(sensorValue, serial);
  strcat(sensorValue, jsonData4);

  strcat(sensorValue, jsonData1);
  strcat(sensorValue, jsonData4);
  strcat(sensorValue, String(t).c_str());
  strcat(sensorValue, jsonData4);
  
  strcat(sensorValue, jsonData2);
  strcat(sensorValue, jsonData4);
  strcat(sensorValue, String(h).c_str());
  strcat(sensorValue, jsonData4);
  
  strcat(sensorValue, jsonData5);
  strcat(sensorValue, jsonData4);
  strcat(sensorValue, String(random(75, 90)).c_str());
  strcat(sensorValue, jsonData4);
  
  strcat(sensorValue, jsonData3);
  client.publish(topic, sensorValue);

  Serial.print(F("Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(F(" Humidity: "));
  Serial.print(h);
  Serial.println(F("%"));
  
}
