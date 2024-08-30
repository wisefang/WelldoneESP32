#include "03WdOTA.h"
#include "ESPmDNS.h"
#include "ESPAsyncWebServer.h"

AsyncWebServer server(80);
const char *hostname = "WdEsp32";
WdOTA* WdOTA::_instance_wdota = NULL;
/**********************************************************
 * @brief otaBegin
 * 
 ***********************************************************/
void WdOTA::otaBegin() {
  if (!MDNS.begin(hostname)) {
    Serial.println("Error setting up mDNS responder!");
  } else {
    Serial.printf("Access at http://%s.local\n", hostname);
  }
  server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
    String index = index_html;
    index.replace ("%web_title%", hostname);
    index.replace("%deviceNo%", get_device_no());
    index.replace("%ChineseName%", ChineseName);
    index.replace("%SoftwareVer%",SoftwareVer);
    index.replace("%BuildTime%",getBuildTime());
    request->send(200, "text/html", index);
  });
  //setup页面
  server.on("/setup", HTTP_GET, [](AsyncWebServerRequest *request){
    String setup = setup_html;
    request->send(200, "text/html", setup); 
  });
  server.on("/No", HTTP_GET, [this](AsyncWebServerRequest *request){
    request->send(200, "text/plain", MergeData("No").c_str()); 
  });
  server.on("/set", HTTP_GET, Config_Callback);   // 绑定配置下发的处理函数
  ElegantOTA.begin(&server, "admin", "12345");    // Start ElegantOTA
  // ElegantOTA callbacks
  // ElegantOTA.onStart(onOTAStart);
  // ElegantOTA.onProgress(onOTAProgress);
  // ElegantOTA.onEnd(onOTAEnd);

  server.begin();  
  
}
/**********************************************************
 * @brief MergeData
 * 
 * @param flag 
 * @return String 
 ***********************************************************/
String WdOTA::MergeData(String flag){
  String dataBuffer = "<p>";
  if (flag == "No")
  {
    dataBuffer += get_device_no();
  }
  dataBuffer += "<br /></p>";
  return dataBuffer;
}
void WdOTA::Config_Callback(AsyncWebServerRequest *request)
{
  if( request->hasParam("DeviceNo"))
  {
    uint32_t HTTP_value = request->getParam("DeviceNo")->value().toInt();    // 获取下发的数据
    Serial.print("Receive DeviceNo: ");
    Serial.println(HTTP_value);
    _instance_wdota->save_serial_no(HTTP_value);
  }
  request->send(200, "text/plain", "OK"); // 发送接收成功标志符
}
