#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <DHT.h>
#include "WiFi.h"

#define SSID "Anuj's Galaxy M31"
#define PASSWORD "VadaPavGr8"

#define MQTT_PORT 1883
#define MQTT_BROKER_ADDRESS "mqtt.eclipseprojects.io"

// random(0xFFFF) generates a value between 0 and 65535
// HEX converts it to hexadecimal value
const String ID = "ESP32-" + String(random(0xFFFF), HEX);

char tempTopic[50];

// WiFiClient object handles TCP/IP communication
WiFiClient wifiClient;
// PubSubClient object handles the MQTT protocol but needs wifiClient to send/receive data
PubSubClient client(wifiClient);

// Create an object of the DHT11 by specifying the type and pin
DHT dht(18, DHT11);

void reconnect(){
  
  while(!client.connected()){
    Serial.println("Attempting MQTT connection.....");
    // Attempt to connect
    if(client.connect(ID.c_str(), NULL, NULL)){
      Serial.println("Connected to the MQTT broker!");
      client.loop();
    } else{
      // Prints the state number of the faliure 
      Serial.print("Connection failed... rc = "); Serial.print(client.state());
      Serial.println("Retrying in 5 seconds");
      delay(5000);
    }
  }
}

void setup(){

  Serial.begin(9600);

  dht.begin();
  // Set WiFi to station mode
  WiFi.mode(WIFI_STA);
  // Disconnect from any pre-existing connections
  WiFi.disconnect();

  // Defining tempTopic
  sprintf(tempTopic, "%s/temp", ID.c_str());

  // Connect to the WiFi
  Serial.println("*****************************************************************");
  Serial.print("Attempting to connect to the WiFi network : "); Serial.println(SSID);
  WiFi.begin(SSID, PASSWORD);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Wifi Connection successful!");

  // Connecting to the MQTT server
  client.setServer(MQTT_BROKER_ADDRESS, MQTT_PORT);
  client.setKeepAlive(60);   // Send a ping every 60 seconds
  reconnect();  // Initial connection
}

void loop(){

  if(!client.connected()){
    reconnect();
  }

  client.loop();

  // Take the temperature and humidity readings
  float temp = dht.readTemperature();
  float humid = dht.readHumidity();

  //Serial.print("Temperature : "); Serial.print(temp, 1); Serial.println(" C");
  //Serial.print("Humidity : "); Serial.print(humid, 1); Serial.println(" %");

  client.publish(tempTopic, String(temp).c_str(), true);
  Serial.println("Message Published!");
  delay(10000);
}


