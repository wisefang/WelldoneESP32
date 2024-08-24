#ifndef __WD_WIFI_H__
#define __WD_WIFI_H__
#include <Arduino.h>
#include <Preferences.h>
class WdWifi
{
  public:
    
    void init(void);
    void startAP(void);
    void begin() 
    {
      init();
      xTaskCreate(taskFunction, "Wifi_Status_Task", 2048, this, 1, &_taskHandle);
    }
    static void taskFunction(void* pvParameters) {
        WdWifi* taskInstance = static_cast<WdWifi*>(pvParameters);
        taskInstance->status(pvParameters);
    }
    void status(void *pvParameters);
  public:
    void save_wifi_set(String ssid, String pwd){
      _preferences_wifiset.begin("WdWifiSet");
      _preferences_wifiset.putString("wifi_ssid", ssid);
      _preferences_wifiset.putString("wifi_pwd", pwd);
      _preferences_wifiset.end();
    }
  public:
    bool Flag_isConnected;
    bool Flag_isSet;
  private:
    uint8_t _pin;
    Preferences _preferences_wifiset;
    String _wifi_ssid;  
    String _wifi_pwd;
  private:
    const int16_t Wifi_Status_Interval_ms = 1000;
    TaskHandle_t _taskHandle = NULL;
};
#endif /* __F03_WIFI_H__ */
