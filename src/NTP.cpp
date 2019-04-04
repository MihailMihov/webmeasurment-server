#include "NTP.hpp"

WiFiUDP UDP;

const char* serverName = "time.google.com";
const int packetSize = 48;
const unsigned int UDPport = 8888;

IPAddress serverIP;
byte packetBuffer[packetSize];
byte timeZone = 2;

void NTP::setup() { UDP.begin(UDPport); }

void NTP::sendPacket() {

    UDP.beginPacket(serverIP, 123);
    UDP.write(packetBuffer, packetSize);
    UDP.endPacket();

}

void NTP::findServer() {

    WiFi.hostByName(serverName, serverIP);

}

void NTP::constructPacket() {

    memset(packetBuffer, 0, packetSize);

    packetBuffer[0] = 0b11100011;
    packetBuffer[1] = 0;
    packetBuffer[2] = 6;
    packetBuffer[3] = 0xEC;

    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;

}

time_t NTP::getTime() {

    while(UDP.parsePacket() > 0) ;

    sendPacket();

    unsigned long long beginWait = millis();

    while (millis() - beginWait < 1500) {

        int size = UDP.parsePacket();

        if (size >= packetSize) {

            UDP.read(packetBuffer, packetSize);
            unsigned long secsSince1900;

            secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
            secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
            secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
            secsSince1900 |= (unsigned long)packetBuffer[43];

            return secsSince1900 - 2208988800UL + timeZone * 3600;

        }

    }

    return 0; // No response

}
