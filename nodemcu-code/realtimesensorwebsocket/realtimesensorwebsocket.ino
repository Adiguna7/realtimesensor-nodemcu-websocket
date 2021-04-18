#include "ESP8266WiFi.h"
#include <WebSocketClient.h>
#include <ArduinoJson.h>

#define triggerPin  16
#define echoPin     5

const char* ssid = "Bismillah";
const char* password = "sagsevennn";
char path[] = "/";
char host[] = "192.168.43.50";
//int port = 8080;

WebSocketClient webSocketClient;
WiFiClient client;

void setup(){
  Serial.begin(115200);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print("disconnect");
  }
  Serial.println("");
  Serial.println("Wifi Connected");
  Serial.println(WiFi.localIP());
  delay(5000);

  if(client.connect(host, 8080)){
    Serial.println("connected to tcp websocket");
  }
  else{
    Serial.println("Connection Failed to Websocket");
    while(1){
      
    }
  }

  webSocketClient.path = path;
  webSocketClient.host = host;
  if (webSocketClient.handshake(client)) {
    Serial.println("Handshake successful");
  } else {
    Serial.println("Handshake failed.");
    while(1) {
      // Hang on failure
    }  
  }
  
}

void loop(){
  // SENSOR ULTRASONIK 
  long duration, jarak;
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2); 
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  jarak = (duration/2) / 29.1;
  Serial.println("jarak :");
  Serial.print(jarak);
  Serial.println(" cm");

  // WIFI DAN WEBSOCKET  
  //  String data_json = "{\"device_type\":\"nodemcu\",\"data_sensor\":\"123\"}";
  StaticJsonDocument<200> data_json;
  data_json["device_type"] = "nodemcu";
  data_json["data_sensor"] = jarak;

  char data_json_send[100];
  size_t len = serializeJson(data_json, data_json_send);
  
  if(client.connected()){
    webSocketClient.sendData(data_json_send);
  }
  else{
    Serial.println("client disconnected");
    while(1){
      
    }
  }
  delay(1000);
}
