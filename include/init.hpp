#pragma once

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
    File file = SPIFFS.open("/measurments", "r");
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
  server.on("/restart", []{
    ESP.restart();
  });

  server.begin();
}

void time() {
  setSyncProvider(ntp::getTime);
  setSyncInterval(30);
}

void fs() {
  SPIFFS.begin();
  SPIFFS.info(fs_info);
}

void ntp() {
  ntp::setup();
} 

void ota() {
  ota::setup();
}

} // namespace init