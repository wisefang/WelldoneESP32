#ifndef __01WD_WIFI_H__
#define __01WD_WIFI_H__
#include <Arduino.h>
#include <WiFi.h>
#include "02WdTCP.h"
class WdWifi:public WdTCP
{
  public:
    WdWifi();
    void wifi_begin() 
    {
#if WifiMode==1
      _wifi_init_asStaion();
#elif WifiMode==2
      _wifi_init_asAP();
#endif
      // xTaskCreate(taskFunction_wifi, "Wifi_Status_Task", 4*1024, this, 1, &_taskHandle);
    }
    static void taskFunction_wifi(void* pvParameters) {
        WdWifi* taskInstance = static_cast<WdWifi*>(pvParameters);
        taskInstance->_wifi_status();
    }
  public:
    bool Flag_isConnected;
    bool Flag_isSet;
  private:
    static WdWifi * _instance_wdwifi;
    String _wifi_ssid;  
    String _wifi_pwd;   
    bool _isWifiConnected;
    TaskHandle_t _taskHandle = NULL;
  private:
    void _wifi_init_asStaion(void);
    void _wifi_init_asAP(void);
    static void _wifi_event(WiFiEvent_t event);
    static void _wifi_gotIP_static(WiFiEvent_t event, WiFiEventInfo_t info);
    void _wifi_gotIP(WiFiEvent_t event, WiFiEventInfo_t info);
    static void _wifi_disconnected_static(WiFiEvent_t event, WiFiEventInfo_t info);
    void _wifi_disconnected(WiFiEvent_t event, WiFiEventInfo_t info);
    static void _wifi_ap_stationConnected_static(WiFiEvent_t event, WiFiEventInfo_t info);
    void _wifi_ap_stationConnected(WiFiEvent_t event, WiFiEventInfo_t info);
    static void _wifi_ap_stationDisconnected_static(WiFiEvent_t event, WiFiEventInfo_t info);
    void _wifi_ap_stationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);
    //检查wifi连接状态
    void _wifi_status(void){
      for (;;) {
        if (WiFi.status() != WL_CONNECTED) {
          if (_isWifiConnected) {
            _isWifiConnected = false;
            log_w("Disconnected from WiFi");
          }
          log_i("Connecting to %s......",_wifi_ssid);
          // WiFi.begin(_wifi_ssid.c_str(), _wifi_pwd.c_str());
        } else if (!_isWifiConnected) {
            _isWifiConnected = true;
            log_i("Connected to the WiFi network");
            log_i("IP Address: %s", WiFi.localIP().toString().c_str());
            // Start OTA service and TCP service
            otaBegin();
            as_client_begin(serverIP,REMOTE_TCP_SERVER_PORT);
            as_server_begin(LOCAL_TCP_SERVER_PORT);
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS);
      }
    }    
};
#endif /* __F03_WIFI_H__ */
