#include "OTA.hpp"

void OTA::handlers::start() {

    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
    } else { // U_SPIFFS
        type = "filesystem";
    }
    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);

}

void OTA::handlers::end() { Serial.println("\nEnd"); }

void OTA::handlers::progress(unsigned int progress, unsigned int total) { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); }

void OTA::handlers::error(ota_error_t error) {

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

}

void OTA::setup() {

    auto startHandle = &handlers::start;
    auto endHandle = &handlers::end;
    auto progressHandle = &handlers::progress;
    auto errorHandle = &handlers::error;

    ArduinoOTA.setPort(8266);
    ArduinoOTA.setPassword("admin");

    ArduinoOTA.onStart(startHandle);
    ArduinoOTA.onEnd(endHandle);
    ArduinoOTA.onProgress(progressHandle);
    ArduinoOTA.onError(errorHandle);

    ArduinoOTA.begin();

}
