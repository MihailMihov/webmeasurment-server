#pragma once

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

namespace ntp {

    void setup();
    void sendPacket();
    void findServer();
    void constructPacket();
    time_t getTime();

}