/**********************************************************
 * @file    01WdWifi.cpp
 * @author  Majunfang (Majf@Welldone-e.com)
 * @brief 
 * @version V1.0
 * @date    2024-09-01
 * 
 * @copyright  Copyright © 2024 Huidong All Rights Reserved
 * 
 ***********************************************************/
#include "01WdWifi.h"

WdWifi* WdWifi::_instance_wdwifi = NULL;
/**********************************************************
 * @brief Construct a new Wd Wifi:: Wd Wifi object
 * 
 ***********************************************************/
WdWifi::WdWifi(){
  _instance_wdwifi = this;
#if (isDHCP==false)
#ifdef local_IP
  if(_local_IP.fromString(local_IP) == false){
    _local_IP = IPAddress(192, 168, 2, 100);
  }  
#else
  _local_IP = IPAddress(192, 168, 2, 100);
#endif
#ifdef gateway_IP
  if(_gateway_IP.fromString(gateway_IP) == false){
    _gateway_IP = IPAddress(192, 168, 2, 1);
  }
#else
  _gateway_IP = IPAddress(192, 168, 2, 1);
#endif 
#ifdef subnet_IP
  if(_subnet_IP.fromString(subnet_IP) == false){
    _subnet_IP = IPAddress(255, 255, 255, 0);
  }
#else
  _subnet_IP = IPAddress(255, 255, 255, 0);
#endif
#ifdef primaryDNS_IP
  if(_primaryDNS_IP.fromString(primaryDNS_IP) == false){
    _primaryDNS_IP = IPAddress(8, 8, 8, 8);
  }
#else
  _primaryDNS_IP = IPAddress(8, 8, 8, 8);
#endif
#ifdef secondaryDNS_IP
  if(_secondaryDNS_IP.fromString(secondaryDNS_IP) == false){
    _secondaryDNS_IP = IPAddress(8, 8, 4, 4);
  }
#else
  _secondaryDNS_IP = IPAddress(8, 8, 4, 4);
#endif
#endif
}
/**********************************************************
 * @brief wifi init as station
 * 
 ***********************************************************/
void WdWifi::_wifi_init_asStaion(){
  // 删除旧配置
  WiFi.disconnect(true);
  
  WiFi.onEvent(_wifi_event);
  WiFi.onEvent(_wifi_gotIP_static, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(_wifi_disconnected_static, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  
  _wifi_ssid = get_station_ssid();
  _wifi_pwd = get_station_pwd();
  
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.setHostname("CAL");
  WiFi.setSleep(false);//关闭省电模式，降低ping延迟
#if (isDHCP==false)
  WiFi.config(_local_IP, _gateway_IP, _subnet_IP, _primaryDNS_IP, _secondaryDNS_IP);
#endif  
  WiFi.begin(_wifi_ssid.c_str(), _wifi_pwd.c_str());
  log_i("try to Connect to %s ......",_wifi_ssid);

  wifi_isConnected = false;
}
/**********************************************************
 * @brief wifi init as ap
 * 
 ***********************************************************/
void WdWifi::_wifi_init_asAP(){
  // 删除旧配置
  WiFi.disconnect(true);

  WiFi.onEvent(_wifi_event);
  WiFi.onEvent(_wifi_ap_stationConnected_static, WiFiEvent_t::ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED);
  WiFi.onEvent(_wifi_ap_stationDisconnected_static, WiFiEvent_t::ARDUINO_EVENT_WIFI_AP_STADISCONNECTED);

  _wifi_ssid = get_ap_ssid();
  _wifi_pwd = get_ap_pwd();

  WiFi.mode(WIFI_AP);
  WiFi.setAutoReconnect(true);
  WiFi.setHostname("CAL");
  WiFi.setSleep(false);
  if(!WiFi.softAP(_wifi_ssid.c_str(), _wifi_pwd.c_str())){
    log_e("AP %s Start Failed",_wifi_ssid);
  }
  else{
    log_i("AP %s Started",_wifi_ssid);
  }  
  wifi_isConnected = false;
  otaBegin();
#if UseTcpClient == true
  as_client_begin();
#endif
#if UseTcpServer == true
  as_server_begin();
#endif
#if UseUdpServer == true
  as_udpServer_begin();
#endif
}
/**********************************************************
 * @brief wifi got ip
 * 
 * @param event 
 * @param info 
 ***********************************************************/
void WdWifi::_wifi_gotIP_static(WiFiEvent_t event, WiFiEventInfo_t info){
  _instance_wdwifi->_wifi_gotIP(event,info);
}
void WdWifi::_wifi_gotIP(WiFiEvent_t event, WiFiEventInfo_t info){
  wifi_isConnected = true;
  log_i("Connected to %s",_wifi_ssid);
  log_i("IP address: %s",WiFi.localIP().toString().c_str());
  //通过udp发送ip地址
  
  wdudp_log("IP address: %s",WiFi.localIP().toString().c_str());
  // Start OTA service and TCP service
  otaBegin();
#if UseTcpClient == true
  as_client_begin();
#endif
#if UseTcpServer == true
  as_server_begin();
#endif
#if UseUdpServer == true
  as_udpServer_begin();
#endif
}
/**********************************************************
 * @brief wifi ap station connected
 * 
 **********************************************************/
void WdWifi::_wifi_ap_stationConnected_static(WiFiEvent_t event, WiFiEventInfo_t info){
  _instance_wdwifi->_wifi_ap_stationConnected(event,info);
}

void WdWifi::_wifi_ap_stationConnected(WiFiEvent_t event, WiFiEventInfo_t info){
  log_i("Station connected.");
  wifi_isConnected = true;
}
/**********************************************************
 * @brief wifi ap station disconnected
 * 
 * @param event 
 * @param info 
 ***********************************************************/
void WdWifi::_wifi_ap_stationDisconnected_static(WiFiEvent_t event, WiFiEventInfo_t info){
  _instance_wdwifi->_wifi_ap_stationDisconnected(event,info);
}

void WdWifi::_wifi_ap_stationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info){
  log_i("Station disconnected.");
}
/**********************************************************
 * @brief wifi disconnected
 * 
 * @param event 
 * @param info 
 ***********************************************************/
void WdWifi::_wifi_disconnected_static(WiFiEvent_t event, WiFiEventInfo_t info){
  _instance_wdwifi->_wifi_disconnected(event,info);
}
void WdWifi::_wifi_disconnected(WiFiEvent_t event, WiFiEventInfo_t info){
  wifi_isConnected = false;
  log_i("try to Connect to %s ......",_wifi_ssid);
}
/**********************************************************
 * @brief wifi 事件处理函数
 * 
 * @param event 
 ***********************************************************/
void WdWifi::_wifi_event(WiFiEvent_t event){
  log_i("[WiFi-event] event: %d\n", event);
  switch(event){ 
    case ARDUINO_EVENT_WIFI_READY: 
        log_i("WiFi interface ready");
        break;
    case ARDUINO_EVENT_WIFI_SCAN_DONE:
        log_i("Completed scan for access points");
        break;
    case ARDUINO_EVENT_WIFI_STA_START:
        log_i("WiFi client started");
        break;
    case ARDUINO_EVENT_WIFI_STA_STOP:
        log_i("WiFi clients stopped");
        break;
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
        log_i("Connected to access point");
        break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        log_i("Disconnected from WiFi access point");
        break;
    case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
        log_i("Authentication mode of access point has changed");
        break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        log_i("Obtained IP address: %s", WiFi.localIP().toString().c_str());
        break;
    case ARDUINO_EVENT_WIFI_STA_LOST_IP:
        log_i("Lost IP address and IP address is reset to 0");
        break;
    case ARDUINO_EVENT_WPS_ER_SUCCESS:
        log_i("WiFi Protected Setup (WPS): succeeded in enrollee mode");
        break;
    case ARDUINO_EVENT_WPS_ER_FAILED:
        log_i("WiFi Protected Setup (WPS): failed in enrollee mode");
        break;
    case ARDUINO_EVENT_WPS_ER_TIMEOUT:
        log_i("WiFi Protected Setup (WPS): timeout in enrollee mode");
        break;
    case ARDUINO_EVENT_WPS_ER_PIN:
        log_i("WiFi Protected Setup (WPS): pin code in enrollee mode");
        break;
    case ARDUINO_EVENT_WIFI_AP_START:
        log_i("WiFi access point started");
        break;
    case ARDUINO_EVENT_WIFI_AP_STOP:
        log_i("WiFi access point  stopped");
        break;
    case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
        log_i("Client connected");
        break;
    case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
        log_i("Client disconnected");
        break;
    case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:
        log_i("Assigned IP address to client");
        break;
    case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:
        log_i("Received probe request");
        break;
    case ARDUINO_EVENT_WIFI_AP_GOT_IP6:
        log_i("IPv6 is preferred");
        break;
    case ARDUINO_EVENT_ETH_START:
        log_i("Ethernet started");
        break;
    case ARDUINO_EVENT_ETH_STOP:
        log_i("Ethernet stopped");
        break;
    case ARDUINO_EVENT_ETH_CONNECTED:
        log_i("Ethernet connected");
        break;
    case ARDUINO_EVENT_ETH_DISCONNECTED:
        log_i("Ethernet disconnected");
        break;
    case ARDUINO_EVENT_ETH_GOT_IP:
        log_i("Obtained IP address");
        break;
    default:
      break;
  }
}