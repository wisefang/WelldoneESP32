#ifndef __WD_WIFI_H__
#define __WD_WIFI_H__
#include <Arduino.h>
#include <WiFi.h>

#include "WdTCP.h"
#include "WdJson.h"
class WdWifi:public WdTCP
{
  public:
    
    // void startAP(void);
    void wifi_begin() 
    {
      _wifi_init_asStaion();
      xTaskCreate(taskFunction_wifi, "Wifi_Status_Task", 4*1024, this, 1, &_taskHandle);
    }
    static void taskFunction_wifi(void* pvParameters) {
        WdWifi* taskInstance = static_cast<WdWifi*>(pvParameters);
        taskInstance->_wifi_status();
    }
  public:
    bool Flag_isConnected;
    bool Flag_isSet;
  private:
    String _wifi_ssid;  
    String _wifi_pwd;   
    bool _isWifiConnected;
    TaskHandle_t _taskHandle = NULL;
  private:
    //初始化wifi参数并连接wifi
    void _wifi_init_asStaion(void){
      
      _wifi_ssid = get_wifi_ssid();
      _wifi_pwd = get_wifi_pwd();
      
      WiFi.mode(WIFI_STA);
      WiFi.setAutoReconnect(true);
      WiFi.setHostname("CAL");
      WiFi.setSleep(false);
      WiFi.begin(_wifi_ssid.c_str(), _wifi_pwd.c_str());
      log_i("try to Connect to %s ......",_wifi_ssid);

      _isWifiConnected = false;
      
    }
    //检查wifi连接状态
    void _wifi_status(void){
      for (;;) {
        if (WiFi.status() != WL_CONNECTED) {
          if (_isWifiConnected) {
            _isWifiConnected = false;
            log_i("Disconnected from WiFi");
          }
          log_i("Connecting to WiFi...");
          // WiFi.begin(_wifi_ssid.c_str(), _wifi_pwd.c_str());
        } else if (!_isWifiConnected) {
            _isWifiConnected = true;
            log_i("Connected to the WiFi network");
            log_i("IP Address: %s", WiFi.localIP().toString().c_str());
            // Start OTA service and TCP service
            // f12_Pagehtml.start_ota_server();
            as_client_begin(serverIP,REMOTE_TCP_SERVER_PORT);
            as_server_begin(LOCAL_TCP_SERVER_PORT);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
      }
    }    
};
#endif /* __F03_WIFI_H__ */
