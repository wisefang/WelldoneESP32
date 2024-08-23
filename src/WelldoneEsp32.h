/**********************************************************
 * @file    WelldoneEsp32.h
 * @author  Majunfang (Majf@Welldone-e.com)
 * @brief 
 * @version V1.0
 * @date    2024-08-24
 * 
 * This work is licensed under a BSD style license. See
 * http://www.mathertel.de/License.aspx
 * 
 ***********************************************************/
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
