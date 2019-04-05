#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <FS.h>

#include "../conf.hpp"
#include "ntp.hpp"
#include "ota.hpp"
#include "init.hpp"

double_t measure(bool writeResult);
constexpr double_t duration_to_cm (uint32_t duration);

constexpr const char* ssid = STASSID;
constexpr const char* psk = STAPSK;
constexpr const uint16_t web_server_port = 80;
constexpr const uint8_t signal_pin = 5;
uint32_t prev_millis = 0;

ESP8266WebServer server(web_server_port);
FSInfo fs_info;

void setup() {

  Serial.begin(9600);

  init_n::wifi(ssid, psk);
  init_n::fs();
  init_n::webserver(server);
  init_n::ntp();
  init_n::ota();
  init_n::time();

}

void loop() {

  if (millis() - prev_millis > 300000) {
    measure(true);
    prev_millis = millis();
  }

  yield();

  server.handleClient();
  ArduinoOTA.handle();
}

double_t measure(bool writeResult) {

  uint32_t duration;
  double_t distance;

  pinMode(signal_pin, OUTPUT);
  digitalWrite(signal_pin, LOW);
  digitalWrite(signal_pin, HIGH);
  delayMicroseconds(20);
  digitalWrite(signal_pin, LOW);

  pinMode(signal_pin, INPUT);
  duration = pulseIn(signal_pin, HIGH);

  yield();

  distance = duration_to_cm(duration);

  if(writeResult) {
    File measurment_file = SPIFFS.open("/measurments", "a");
    time_t time_now = now();
    measurment_file.printf("%d/%d/%d %d:%d:%d, %f\n", day(time_now), month(time_now), year(time_now), hour(time_now), minute(time_now), second(time_now), distance);
    measurment_file.close();
  }
    
  return distance;

}

constexpr double_t duration_to_cm (uint32_t duration) {
  return static_cast<double_t>(duration * 34 / 2000.f);
}