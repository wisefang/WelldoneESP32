/**********************************************************
 * @file    WelldoneEsp32.cpp
 * @author  Majunfang (Majf@Welldone-e.com)
 * @brief 
 * @version V1.0
 * @date    2024-08-23
 * 
 ***********************************************************/
#include "WelldoneEsp32.h"

WelldoneEsp32 WdESP32;
/**********************************************************
 * @brief Construct a new Welldone Esp 3 2:: Welldone Esp 3 2 object
 * 
 ***********************************************************/
WelldoneEsp32::WelldoneEsp32(){

}
/**********************************************************
 * @brief Destroy the Welldone Esp 3 2:: Welldone Esp 3 2 object
 * 
 ***********************************************************/
WelldoneEsp32::~WelldoneEsp32()
{
}
void WelldoneEsp32::init(void){    
  device_no_init();
  workled_blink();
  wduart_begin();
#if (UseWifi)
  wifi_begin();  
#endif
}