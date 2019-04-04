#ifndef _OTA_hpp
#define _OTA_hpp

#include <ArduinoOTA.h>

namespace OTA {

namespace handlers {

void start();
void end();
void progress(unsigned int progress, unsigned int total);
void error(ota_error_t error);

} // namespace handlers

void setup();

} // namespace OTA

#endif
