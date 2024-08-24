/**********************************************************
 * @file    WelldoneEsp32.h
 * @author  Majunfang (Majf@Welldone-e.com)
 * @brief 
 * @version V1.0
 * @date    2024-08-24
 * 
 ***********************************************************/
#ifndef __WELLDONE_ESP32_H__
#define __WELLDONE_ESP32_H__
#include "HttpOTA.h"
#include "Reset.h"
#include "DeviceNo.h"
#include "WdJson.h"
class WelldoneEsp32:public HttpOTA,public Reset,public WdJson  
{
    public:
        void init(void){
            Serial.setTimeout(50);
            Serial.begin(115200);
            device_no_init();
        }
};
extern WelldoneEsp32 WdESP32;
#endif /* __WELLDONE_ESP32_H__ */
