#include "NTP.hpp"

namespace NTP {

WiFiUDP UDP;

constexpr const char server_name[] = "time.google.com";
constexpr const uint8_t packet_size = 48;
constexpr const uint16_t udp_port = 8888;

IPAddress server_ip;
uint8_t packet_buffer[packet_size];
constexpr const int8_t time_zone = 2;

void setup() { UDP.begin(udp_port); }

void sendPacket() {

    UDP.beginPacket(server_ip, 123);
    UDP.write(packet_buffer, packet_size);
    UDP.endPacket();

}

void findServer() {

    WiFi.hostByName(server_name, server_ip);

}

void constructPacket() {

    memset(packet_buffer, 0, packet_size);

    packet_buffer[0] = 0b11100011;
    packet_buffer[1] = 0;
    packet_buffer[2] = 6;
    packet_buffer[3] = 0xEC;

    packet_buffer[12] = 49;
    packet_buffer[13] = 0x4E;
    packet_buffer[14] = 49;
    packet_buffer[15] = 52;

}

time_t getTime() {

    while(UDP.parsePacket() > 0);

    sendPacket();

    unsigned long long begin_wait = millis();

    while (millis() - begin_wait < 1500) {

        uint8_t size = UDP.parsePacket();

        if (size >= packet_size) {

            UDP.read(packet_buffer, packet_size);
            uint64_t secs_since_1900;

            secs_since_1900 =  static_cast<uint8_t>(packet_buffer[40] << 24);
            secs_since_1900 |= static_cast<uint8_t>(packet_buffer[41] << 16);
            secs_since_1900 |= static_cast<uint8_t>(packet_buffer[42] << 8);
            secs_since_1900 |= static_cast<uint8_t>(packet_buffer[43]);

            return secs_since_1900 - 2208988800UL + time_zone * 3600;

        }

    }

    return 0; // No response

}

} // namespace NTP
