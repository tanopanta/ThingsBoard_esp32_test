// https://github.com/thingsboard/ThingsBoard-Arduino-MQTT-SDK のライブラリを利用

#include "ThingsBoard.h"
#include <Arduino.h>
#include <WiFi.h>

#include "myconfig.h"

// See https://thingsboard.io/docs/getting-started-guides/helloworld/ 

// thingsboadインスタンスの初期化
WiFiClient espClient;
ThingsBoard tb(espClient);

// the Wifi radio's status
int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(115200);
  delay(10);
  WiFi.begin(ssid, password);
  InitWiFi();
}

void loop() {
  delay(1000);

  if (WiFi.status() != WL_CONNECTED) {
    reconnect();
  }

  if (!tb.connected()) {
    // Connect to the ThingsBoard
    Serial.print("Connecting to: ");
    Serial.print(address);
    Serial.print(" with token ");
    Serial.println(key);
    if (!tb.connect(address, key)) {
      Serial.println("Failed to connect");
      return;
    }
  }

  Serial.println("Sending data...");

  // Uploads new telemetry to ThingsBoard using MQTT. 
  // See https://thingsboard.io/docs/reference/mqtt-api/#telemetry-upload-api 
  // for more details

  // QoS 0 で送信（ノンブロッキング）
  tb.sendTelemetryInt("temperature", random(40));
  tb.sendTelemetryFloat("humidity", 1.2);
  
  tb.loop();
}

void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

void reconnect() {
  // Loop until we're reconnected
  status = WiFi.status();
  if ( status != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("Connected to AP");
  }
}
