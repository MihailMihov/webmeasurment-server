#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <FS.h>
#include <TimeLib.h>

#include <string>

#include "NTP.hpp"
#include "OTA.hpp"

const char *ssid = "HomeWLBill";
const char *password = "2468013579";

static const unsigned int webServerPort = 80;

const byte signalPin = 5;

unsigned long prevMillis = 0;

void setupWiFi();
void setupServer();
void setupTime();
void setupSPIFFS();

void handleRoot();
void handleMeasurments();
void handleNow();

int measure(bool writeResult);

ESP8266WebServer Server(webServerPort);

void setup() {

  Serial.begin(9600);

  setupWiFi();
  setupServer();
  NTP::setup();
  OTA::setup();
  setupTime();
  setupSPIFFS();
  prevMillis = millis();
}

void loop() {

  if (millis() - prevMillis > 300000)
    measure(true);
  Server.handleClient();
  ArduinoOTA.handle();
}

void setupWiFi() {

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void setupServer() {

  Server.on("/", handleRoot);
  Server.on("/measurments", handleMeasurments);
  Server.on("/now", [](){
    Server.send(200, "text/plain", String(measure(false)));
  });
  Server.on("/restart", [](){
    ESP.restart();
  });
  Server.begin();
}

void setupTime() {

  setSyncProvider(NTP::getTime);
  setSyncInterval(30);
}

void setupSPIFFS() { SPIFFS.begin(); }

void handleRoot() {

  File file = SPIFFS.open("/index.htm", "r");
  Server.streamFile(file, "text/html");
  file.close();

}

void handleMeasurments() {

  File file = SPIFFS.open("/measurments.txt", "r");
  Server.streamFile(file, "text/plain");
  file.close();

}

int measure(bool writeResult) {

  long duration;
  int distance;

  pinMode(signalPin, OUTPUT);
  digitalWrite(signalPin, LOW);
  digitalWrite(signalPin, HIGH);
  delayMicroseconds(20);
  digitalWrite(signalPin, LOW);

  pinMode(signalPin, INPUT);
  duration = pulseIn(signalPin, HIGH);

  yield();

  distance = (float)duration * 34 / 2000.0;

  if(writeResult) {
    File measurmentFile = SPIFFS.open("/measurments", "a");
    measurmentFile.printf("%lu,%d\n", now(), distance);
    measurmentFile.close();
    return 0;
  }
  else {
    return distance;
  }

}
