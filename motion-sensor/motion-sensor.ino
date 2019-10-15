#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "config.h"

const char* ssid = "BYU-WiFi";
const char* password = BYU_WIFI_PASSWORD;

void setup(){
  Serial.begin(115200);
  Serial.println("----------------------------------");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected to ");
  Serial.print(ssid);
  Serial.print(", IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop(){
  
}
