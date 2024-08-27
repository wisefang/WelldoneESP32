#ifndef __WD_OTA_H__
#define __WD_OTA_H__
#include <Arduino.h>
#include <ElegantOTA.h>
#include <ESPAsyncWebServer.h>
#include "Wdindex.h"
class WdOTA:public Wdindex
{
  public:
    void otaBegin();
};
#endif /* __WD_OTA_H__ */
