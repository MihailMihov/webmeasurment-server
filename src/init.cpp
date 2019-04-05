#include "init.hpp"

namespace init_n {

void wifi(const char* ssid, const char* psk) {

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, psk);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void webserver(ESP8266WebServer& server) {

  server.on("/", [&]{
    File file = SPIFFS.open("/index.htm", "r");
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
  server.on("/get_time", [&]{
    server.send(200, "text/plain", String(ntp::get_time()));
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

void time() {
  setSyncProvider(ntp::get_time);
  setSyncInterval(30);
}

void fs() {
  SPIFFS.begin();
}

void ntp() {
  ntp::setup();
} 

void ota() {
  ota::setup();
}

} // namespace init_n