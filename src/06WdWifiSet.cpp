#include "06WdWifiSet.h"
/**********************************************************
 * @brief Construct a new Wd Wifi Set:: Wd Wifi Set object
 * 
 ***********************************************************/
WdWifiSet::WdWifiSet()
    : _default_station_ssid(StationSsid ? StationSsid : "Welldone"),
      _default_station_pwd(StationPwd ? StationPwd : "12345678"),
      _default_ap_ssid(ApSsid ? ApSsid : "Welldone"),
      _default_ap_pwd(ApPwd ? ApPwd : "12345678") {
}
/**********************************************************
 * @brief Destroy the Wd Wifi Set:: Wd Wifi Set object
 * 
 ***********************************************************/
WdWifiSet::~WdWifiSet() {
  log_i("WdWifiSet::~WdWifiSet()");
}
/**********************************************************
 * @brief get wifi setting
 * 
 * @param key 
 * @param default_value 
 * @return String 
 ***********************************************************/
String WdWifiSet::_get_wifi_setting(const char* key, String default_value) {
    if(!_preferences_wifiset.begin("WdWifiSet",false)){
        return default_value;
    };
    String value = _preferences_wifiset.getString(key, default_value);
    _preferences_wifiset.end();
    return value;
}
/**********************************************************
 * @brief set wifi setting
 * 
 * @param key
 * @param value
 * @return bool true if success
 ***********************************************************/
bool WdWifiSet::_set_wifi_setting(const char* key, String value) {
    if(!_preferences_wifiset.begin("WdWifiSet", false)){
      return false;
    }
    bool result = _preferences_wifiset.putString(key, value);
    _preferences_wifiset.end();
    return result;
}
/**********************************************************
 * @brief save station setting
 * 
 * @param ssid
 * @param pwd
 * @return bool true if success
 ***********************************************************/
bool WdWifiSet::save_station_setting(String ssid, String pwd) {
    return _set_wifi_setting("StationSsid", ssid) && _set_wifi_setting("StationPwd", pwd);
}
/**********************************************************
 * @brief save ap setting
 * 
 ***********************************************************/
bool WdWifiSet::save_ap_setting(String ssid, String pwd) {
    return _set_wifi_setting("ApSsid", ssid) && _set_wifi_setting("ApPwd", pwd);
}
/**********************************************************
 * @brief get station ssid
 * 
 ***********************************************************/
String WdWifiSet::get_station_ssid() {
  return _get_wifi_setting("StationSsid", _default_station_ssid);
}
/**********************************************************
 * @brief get ap ssid
 * 
 ***********************************************************/
String WdWifiSet::get_ap_ssid() {
  return _get_wifi_setting("ApSsid", _default_ap_ssid);
}
/**********************************************************
 * @brief get station pwd
 * 
 ***********************************************************/
String WdWifiSet::get_station_pwd() {
  return _get_wifi_setting("StationPwd", _default_station_pwd);
}
/**********************************************************
 * @brief get ap pwd
 * 
 ***********************************************************/
String WdWifiSet::get_ap_pwd() {
  return _get_wifi_setting("ApPwd", _default_ap_pwd);
}