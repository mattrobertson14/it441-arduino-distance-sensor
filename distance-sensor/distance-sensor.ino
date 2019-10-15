#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "config.h"

ESP8266WiFiMulti wifiMulti;
WiFiClient client;
HTTPClient http;
const char* ssid1 = HOME_WIFI_SSID;
const char* passwd1 = HOME_WIFI_PASSWORD;
const char* ssid2 = MATT_HOTSPOT_SSID;
const char* passwd2 = MATT_HOTSPOT_PASSWORD;

const int trigPin = D7;
const int echoPin = D6;

long distance, duration;
String state = "green";

void connectToWifi(){
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(ssid1, passwd1);
  wifiMulti.addAP(ssid2, passwd2);
  
  Serial.print("\nConnecting");
  
  while (wifiMulti.run() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected to ");
  Serial.print(WiFi.SSID());
  Serial.print(", IP Address: ");
  Serial.println(WiFi.localIP());
}

void getRequest(String req_uri){
  String full_uri = "http://172.20.10.2"+req_uri;
  Serial.print("[HTTP] Requesting ");
  Serial.println(full_uri);
  if (http.begin(client, full_uri)){
    int httpCode = http.GET();

    if (httpCode > 0){
      if (httpCode == HTTP_CODE_OK){
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  } else {
    Serial.println("[HTTP] Unable to connect");
  }

  
}

void setup(){
  Serial.begin(115200);
  connectToWifi();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.println(distance);

  if (distance >= 25 && state != "green"){
    state = "green";
    getRequest("/api/set-green");
  } else if (25 > distance && distance >= 15 && state != "yellow"){
    state = "yellow";
    getRequest("/api/set-yellow");
  } else if (15 > distance && distance >= 5 && state != "red"){
    state = "red";
    getRequest("/api/set-red");
  } else if (distance < 5 && state != "cycle") {
    state = "cycle";
    getRequest("/api/cycle");
  }
}
