
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHTesp.h"

#define DHT11_PIN 2 // NodeMcu D4 pin

const char* ssid     = "UPC3499437";
const char* password = "wuNzAp3dhh7t";

const char* mqttServer = "cloud.hackcube.org";
const int mqttPort = 11883;
const char* mqttUser = "YourMqttUser";
const char* mqttPassword = "YourMqttUserPassword";

DHTesp dht;
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
    }
  Serial.println("Connected to the WiFi network");
   
  client.setServer(mqttServer, mqttPort);
 
  connectToMqtt();
 
  client.publish("esp/test", "Hello from ESP8266");
  dht.setup(DHT11_PIN, DHTesp::DHT11);
}

void connectToMqtt(){
    while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client")) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
}

void loop() {
  GetDhtValues();
  if(!client.connected()){
    connectToMqtt();
  }
}

void GetDhtValues() {
  
  float wilgotnosc = dht.getHumidity();
  float temperatura = dht.getTemperature();
  if (dht.getStatusString() == "OK") {
     //Pobranie informacji o wilgotnosci
    char charBuf[50];
    Serial.print(wilgotnosc, 1);
    String(wilgotnosc).toCharArray(charBuf, 50);
    client.publish("testcube/humidity", charBuf);
    Serial.print("\t");
    Serial.print("%RH ");
    Serial.print("\t\t");
    //Pobranie informacji o temperaturze
    
    Serial.print(temperatura, 1);
    String(temperatura).toCharArray(charBuf, 50);
    client.publish("testcube/temp", charBuf);
    Serial.print("\t");
    Serial.println("*C");
  }
  delay(dht.getMinimumSamplingPeriod());
}



                                        
