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
#ifdef SerialBaudRate
  _serial_baud_rate = SerialBaudRate;
#else
  _serial_baud_rate = 115200;
#endif  
}
/**********************************************************
 * @brief Destroy the Welldone Esp 3 2:: Welldone Esp 3 2 object
 * 
 ***********************************************************/
WelldoneEsp32::~WelldoneEsp32()
{
}
void WelldoneEsp32::init(void){
  Serial.setTimeout(50);
  Serial.begin(_serial_baud_rate);
  device_no_init();
#if (UseWifi)
  wifi_begin();  
#endif
}