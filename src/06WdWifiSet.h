#ifndef __06WD_WIFI_SET_H__
#define __06WD_WIFI_SET_H__
#include <Arduino.h>
#include <Preferences.h>
class WdWifiSet
{
  public:
    WdWifiSet();
    ~WdWifiSet();
    String get_station_ssid();
    String get_station_pwd();
    String get_ap_ssid();
    String get_ap_pwd();
    bool save_station_setting(String ssid, String pwd);
    bool save_ap_setting(String ssid, String pwd);  
    
  private:
    Preferences _preferences_wifiset;
    String _wifi_ssid;
    String _wifi_pwd;
    String _default_station_ssid;
    String _default_station_pwd;
    String _default_ap_ssid;
    String _default_ap_pwd;
  private:
    String _get_wifi_setting(const char* key, String default_value);
    bool _set_wifi_setting(const char* key, String value);

};

#endif /* __WD_WIFI_SET_H__ */
