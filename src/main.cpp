#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <FS.h>

#include "ntp.hpp"
#include "ota.hpp"

double_t measure(bool write_result);
constexpr double_t duration_to_cm (uint32_t duration);
void init_wifi(const char* ssid, const char* psk);
void init_webserver(ESP8266WebServer& server);

constexpr const char* ssid = "HomeWLBill";
constexpr const char* psk = "2468013579";
constexpr const uint16_t web_server_port = 80;
constexpr const uint8_t signal_pin = 5;
uint32_t prev_millis = 0;

ESP8266WebServer server(web_server_port);
FSInfo fs_info;

void setup() {

  Serial.begin(9600);

  init_wifi(ssid, psk);
  init_webserver(server);

  SPIFFS.begin();
  ntp::setup();
  ota::setup();

}

void loop() {

  if (millis() - prev_millis > 300000) {
    measure(true);
    prev_millis = millis();
  }

  yield();
  now();
  server.handleClient();
  ArduinoOTA.handle();
  SPIFFS.info(fs_info);
}

double_t measure(bool write_result) {

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

  distance = 195.f - duration_to_cm(duration);

  if(write_result) {
    File measurment_file = SPIFFS.open("/data.txt", "a");
    time_t time_now = now();
    measurment_file.printf("%d/%d/%d %d:%d:%d, %.2f\n", day(time_now), month(time_now), year(time_now), hour(time_now), minute(time_now), second(time_now), distance);
    measurment_file.close();
  }
    
  return distance;

}

constexpr double_t duration_to_cm (uint32_t duration) {
  return static_cast<double_t>(duration * 34 / 2000.f);
}

void init_wifi(const char* ssid, const char* psk) {

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, psk);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void init_webserver(ESP8266WebServer& server) {

  server.on("/", [&]{
    File file = SPIFFS.open("index.htm", "r");
    if(!file)
    {
      server.send(200, "text/plain", "File open failed");
    }
    else {
      server.streamFile(file, "text/html");
    }
    file.close();
  });
  server.on("/measurments", [&]{
    File file = SPIFFS.open("/data.txt", "r");
    if(!file)
    {
      server.send(200, "text/plain", "File open failed");
    }
    else {
      server.streamFile(file, "text/plain");
    }
    file.close();
  });
  server.on("/now", [&]{
    server.send(200, "text/plain", String(measure(false)));
  });
  server.on("/info", [&]{
    String str;
    str += String(fs_info.blockSize) + '\n';
    str += String(fs_info.pageSize) + '\n';
    str += String(fs_info.totalBytes) + '\n';
    str += String(fs_info.usedBytes) + '\n';
    server.send(200, "text/plain", str);
  });
  server.on("/millis", [&]{
    String str = "millis: " + String(millis()) + '\n' + "prev_millis: " + String(prev_millis);
    server.send(200, "text/plain", str);
  });
  server.on("/time", [&]{
    time_t time_now = now();
    String str = String(day(time_now)) + '/' + String(month(time_now)) + '/' + String(year(time_now)) + ' ' + String(hour(time_now)) + ':' + String(minute(time_now)) + ':' + String(second(time_now));
    server.send(200, "text/plain", str);
  });
  server.on("/files", []{
      
  });
  server.on("/restart", []{
    SPIFFS.end();
    ESP.restart();
  });
  server.on("/format", []{
    SPIFFS.end();
    SPIFFS.format();
    ESP.restart();
  });

  server.begin();
}