#include "ota.hpp"

namespace ota {

void setup() {

    ArduinoOTA.setPort(8266);
    ArduinoOTA.setPassword("admin");

    ArduinoOTA.onStart([](){
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
        } else { // U_SPIFFS
            type = "filesystem";
        }
        SPIFFS.end();
        Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]{
        Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](uint32_t progress, uint32_t total){
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error){
        Serial.printf("Error[%u]: ", error);
        switch(error) {
            case OTA_AUTH_ERROR:
                Serial.println("Auth Failed");
            break;
            case OTA_BEGIN_ERROR:
                Serial.println("Begin Failed");
            break;
            case OTA_CONNECT_ERROR:
                Serial.println("Connect Failed");
            break;
            case OTA_RECEIVE_ERROR:
                Serial.println("Receive Failed");
            break;
            case OTA_END_ERROR:
                Serial.println("End Failed");
            break;
        }
    });

    ArduinoOTA.begin(false);

}

} // namespace ota