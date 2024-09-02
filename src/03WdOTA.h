#ifndef __03WD_OTA_H__
#define __03WD_OTA_H__
#include <Arduino.h>
#include <ElegantOTA.h>
#include <ESPAsyncWebServer.h>
#include "04Wdindex.h"
class WdOTA:public Wdindex
{
  public:
    WdOTA();
    void otaBegin();
    String MergeData(String flag);
    static void Config_Callback(AsyncWebServerRequest *request);
  private:
    static WdOTA *_instance_wdota;
    const char *_host_name;
};
#endif /* __WD_OTA_H__ */
