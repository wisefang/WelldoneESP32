#ifndef __WELLDONE_ESP32_H__
#define __WELLDONE_ESP32_H__
#include "HttpOTA.h"
#include "Reset.h"
class WelldoneEsp32:public HttpOTA,public Reset  
{
    public:
};
extern WelldoneEsp32 WdESP32;
#endif /* __WELLDONE_ESP32_H__ */
