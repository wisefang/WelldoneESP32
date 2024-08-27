#include "WdOTA.h"
#include "ESPmDNS.h"
#include "Wdindex.h"
AsyncWebServer server(80);
const char *hostname = "WdEsp32";
void WdOTA::otaBegin() {
  if (!MDNS.begin(hostname)) {
    Serial.println("Error setting up mDNS responder!");
  } else {
    Serial.printf("Access at http://%s.local\n", hostname);
  }
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String index = index_html;
    index.replace ("%web_title%", hostname);
    index.replace("%deviceNo%", "0001");
   
    request->send(200, "text/html", index);
  });

  ElegantOTA.begin(&server, "admin", "12345");    // Start ElegantOTA
  // ElegantOTA callbacks
  // ElegantOTA.onStart(onOTAStart);
  // ElegantOTA.onProgress(onOTAProgress);
  // ElegantOTA.onEnd(onOTAEnd);

  server.begin();  
  
}