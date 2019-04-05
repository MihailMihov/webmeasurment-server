#pragma once

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

namespace ntp {

    void setup();
    void send_packet();
    void find_server();
    void construct_packet();
    time_t get_time();

}
