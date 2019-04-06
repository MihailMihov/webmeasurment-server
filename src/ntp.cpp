#include "ntp.hpp"

constexpr const char server_name[] = "time.google.com";
constexpr const int8_t time_zone = 3;
constexpr const uint16_t udp_port = 8888;
constexpr const uint8_t packet_size = 48;
uint8_t packet_buffer[packet_size];
IPAddress server_ip;
WiFiUDP UDP;

namespace ntp {

void setup() {
    UDP.begin(udp_port);
    setSyncProvider(get_time);
    setSyncInterval(10);
}

void send_packet() {
    UDP.beginPacket(server_ip, 123);
    UDP.write(packet_buffer, packet_size);
    UDP.endPacket();
}

void find_server() {
    WiFi.hostByName(server_name, server_ip);
}

void construct_packet() {

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

time_t get_time() {

    while(UDP.parsePacket() > 0);

    find_server();
    construct_packet();
    send_packet();

    uint64_t begin_wait = millis();

    while (millis() - begin_wait < 1500) {

        uint8_t size = UDP.parsePacket();

        if (size >= packet_size) {

            UDP.read(packet_buffer, packet_size);
            uint32_t secs_since_1900;

            secs_since_1900  = static_cast<uint32_t>(packet_buffer[40] << 24);
            secs_since_1900 |= static_cast<uint32_t>(packet_buffer[41] << 16);
            secs_since_1900 |= static_cast<uint32_t>(packet_buffer[42] << 8);
            secs_since_1900 |= static_cast<uint32_t>(packet_buffer[43]);

            return secs_since_1900 - 2208988800UL + time_zone * 3600;

        }

    }

    return 0; // No response

}

} // namespace ntp
