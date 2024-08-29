#ifndef __06WD_WIFI_SET_H__
#define __06WD_WIFI_SET_H__
#include <Arduino.h>
#include <Preferences.h>
class WdWifiSet
{
  public:
    void save_wifi_set(String ssid, String pwd){
      _preferences_wifiset.begin("WdWifiSet");
      _preferences_wifiset.putString("wifi_ssid", ssid);
      _preferences_wifiset.putString("wifi_pwd", pwd);
      _wifi_ssid = _preferences_wifiset.getString("wifi_ssid", "MYJ");
      _wifi_pwd = _preferences_wifiset.getString("wifi_pwd", "123456");
      _preferences_wifiset.end();
      log_i("wifi set saved");
      log_i("wifi ssid: %s", _wifi_ssid.c_str());
      log_i("wifi pwd: %s", _wifi_pwd.c_str());

    }
    String get_wifi_ssid(void){
      _preferences_wifiset.begin("WdWifiSet",true);
      String ssid = _preferences_wifiset.getString("wifi_ssid", "MYJXF2");
      _preferences_wifiset.end();
      return ssid;
    }
    String get_wifi_pwd(void){
      _preferences_wifiset.begin("WdWifiSet",true);
      String pwd = _preferences_wifiset.getString("wifi_pwd", "1234567890");
      _preferences_wifiset.end();
      return pwd;
    }
  private:
    Preferences _preferences_wifiset;
    String _wifi_ssid;
    String _wifi_pwd;

};

#endif /* __WD_WIFI_SET_H__ */
