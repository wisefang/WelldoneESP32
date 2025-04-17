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
#include "00WdUart.h"
class WelldoneEsp32:public WdUart
{
public:
    WelldoneEsp32();
    ~WelldoneEsp32();        
    void init(void);        
};
extern WelldoneEsp32 WdESP32;
#endif /* __WELLDONE_ESP32_H__ */
