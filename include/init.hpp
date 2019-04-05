#pragma once

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <TimeLib.h>

#include "ntp.hpp"
#pragma once

#include "ota.hpp"

extern double_t measure(bool write_result);
extern FSInfo fs_info;
extern uint32_t prev_millis;

namespace init_n {

void wifi(const char* ssid, const char* psk);
void webserver(ESP8266WebServer& server);
void time();
void fs();
void ntp();
void ota();

} // namespace init_n