#ifndef __WD_INCLUDE_H__
#define __WD_INCLUDE_H__
#include <Arduino.h>
#include <WiFi.h>
#include <ElegantOTA.h>

#ifndef SerialBaudRate
#define SerialBaudRate 115200
#endif // !SerialBaudRate

#ifndef UseWifi
#define UseWifi true
#endif // !UseWifi

#ifndef WifiMode
#define WifiMode 1
#endif // !WifiMode

#ifndef StationSsid 
#define StationSsid "Wifi_SSID"
#endif // !StationSsid

#ifndef StationPwd
#define StationPwd "Wifi_PASSWD"
#endif // !StationPwd

#ifndef ApSsid
#define ApSsid "WdEsp32"
#endif // !ApSsid

 #ifndef ApPwd
 #define ApPwd ""
 #endif // !ApPwd

 #ifndef WdHostName
 #define WdHostName "WdEsp32"
 #endif // !WdHostName

 #ifndef UseTcpClient
 #define UseTcpClient true
 #endif // !UseTcpClient

 #ifndef RemoteIP
 #define RemoteIP "192.168.16.222"
 #endif // !RemoteIP
 
 #ifndef RemotePort
 #define RemotePort 1234
 #endif // !RemotePort
 
 #ifndef UseTcpServer
 #define UseTcpServer true
 #endif // !UseTcpServer
 
 #ifndef LocalPort
 #define LocalPort 1234
 #endif // !LocalPort
#endif /* __WD_INCLUDE_H__ */
