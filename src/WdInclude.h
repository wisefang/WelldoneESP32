#ifndef __WD_INCLUDE_H__
#define __WD_INCLUDE_H__
#include <Arduino.h>
#include <WiFi.h>
#include <ElegantOTA.h>
//define WelldoneESP32 specific build flags here
//define Serial baud rate,not defined default is 115200
#ifndef SerialBaudRate
#define SerialBaudRate 115200
#endif 
//define use or not wifi,default is true
#ifndef UseWifi
#define UseWifi true
#endif 
//WifiMode, 1=WIFI_STA or 2=WIFI_AP,default is 1 WIFI_STA
#ifndef WifiMode
#define WifiMode 1
#endif 
//define default wifi ssid and password
#ifndef StationSsid 
#define StationSsid "Wifi_SSID"
#endif 

#ifndef StationPwd
#define StationPwd "Wifi_PASSWD"
#endif 

#ifndef ApSsid
#define ApSsid "WdEsp32"
#endif 

#ifndef ApPwd
#define ApPwd ""
#endif 

#ifndef WdHostName
#define WdHostName "WdEsp32"
#endif 
//define use or not tcp client,default is true
#ifndef UseTcpClient
#define UseTcpClient true
#endif 

#ifndef RemoteIP
#define RemoteIP "192.168.16.222"
#endif 

#ifndef RemotePort
#define RemotePort 1234
#endif 
//define use or not tcp server,default is true
#ifndef UseTcpServer
#define UseTcpServer true
#endif 

#ifndef LocalPort
#define LocalPort 5001
#endif 
#endif /* __WD_INCLUDE_H__ */
