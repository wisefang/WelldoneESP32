#ifndef __05WD_JSON_H__
#define __05WD_JSON_H__
#include <Arduino.h>
#include <ArduinoJson.h>
#include <AsyncUDP.h>
#include "06DeviceNo.h"
#include "06WdWifiSet.h"
#include "06BuildTime.h"
#include "06HttpOTA.h"
#include "06WdReset.h"
#include <AsyncTCP.h>
// 回调函数类型定义

typedef void (*CommandCallback_com)(const JsonObject&,HardwareSerial*);
typedef void (*CommandCallback_tcp)(const JsonObject&,AsyncClient*);
typedef void (*CommandCallback_udp)(const JsonObject&,AsyncUDPPacket);

typedef void (*CallbackFunction_notJson_com)(const String&,HardwareSerial*);
typedef void (*CallbackFunction_notJson_tcp)(const String&,AsyncClient*);
typedef void (*CallbackFunction_notJson_udp)(const String&,AsyncUDPPacket);
class WdJson:public DeviceNo,public WdWifiSet,public BuildTime,public HttpOTA,public WdReset
{
  public:
    WdJson():_onOtherCMDCallback_com(nullptr), _onOtherCMDCallback_tcp(nullptr){};
    //生成注册字符串
    String generateRegisterJsonString(void);    
  public:
    bool isUartConnected[3] = {false,false,false};
    bool isTcpConnected = false;
  public:  
    //命令执行结果字符串生成
    template<typename T>
    String getJsonString(String cmdCode, T result){
      JsonDocument output_doc;       
      String outputString = "";
#ifndef Device_Code
#define Device_Code 50001
#endif 
      output_doc["DeviceType"]=Device_Code;
      output_doc["DeviceNo"]=get_device_no();
      output_doc["CmdCode"]=cmdCode;
      JsonObject Body = output_doc["Body"].to<JsonObject>();
      Body["Result"] = result;
      serializeJson(output_doc, outputString);
      return outputString;
    }
    //解析Json字符串，执行通用指令,com为串口,client为tcp客户端
    void onOtherCMD_com(CommandCallback_com callback) {
      _onOtherCMDCallback_com = callback;
    }
    void onOtherCMD_tcp(CommandCallback_tcp callback) {
      _onOtherCMDCallback_tcp = callback;    
    }    
    void onOtherCMD_udp(CommandCallback_udp callback) {
      _onOtherCMDCallback_udp = callback;    
    }
    //解析非json字符串
    void onNotJson_com(CallbackFunction_notJson_com callback) {
      _onNotJsonCallback_com = callback;    
    }
    void onNotJson_tcp(CallbackFunction_notJson_tcp callback) {
      _onNotJsonCallback_tcp = callback;    
    }
    void onNotJson_udp(CallbackFunction_notJson_udp callback) {
      _onNotJsonCallback_udp = callback;    
    }
    //解析Json字符串，执行通用指令
    void parseJsonString(String jsonString,AsyncClient* client)
    {
      _deserial_string(jsonString,client);     
    }
    void parseJsonString(String jsonString,HardwareSerial* com)
    {
      _deserial_string(jsonString,com);
    } 
    void parseJsonString(String jsonString,AsyncUDPPacket packet)
    {
      _deserial_string(jsonString,packet);
    } 
    static void parseJsonString_static(void *arg,String jsonString,HardwareSerial* com)
    {
      WdJson *wdJson = (WdJson *)arg;
      wdJson->parseJsonString(jsonString,com);
    }
  private:
    CommandCallback_com _onOtherCMDCallback_com;
    CommandCallback_tcp _onOtherCMDCallback_tcp;
    CommandCallback_udp _onOtherCMDCallback_udp;
    CallbackFunction_notJson_com _onNotJsonCallback_com;
    CallbackFunction_notJson_tcp _onNotJsonCallback_tcp;
    CallbackFunction_notJson_udp _onNotJsonCallback_udp; 
    const char* CMD_SETUP = "SETUP";
    const char* CMD_REGISTER = "REGISTER";
    const char* CMD_OK = "OK";
    const char* CMD_FAIL_SSID_PWD = "FAIL_SSID_PWD";
    const char* CMD_OTHER = "OtherCmd";
  private:
    void _deserial_string(String jsonString,AsyncClient* client);
    void _deserial_string(String jsonString,HardwareSerial* com);
    void _deserial_string(String jsonString,AsyncUDPPacket packet);
    String _handleGeneralCommand(const JsonObject& json);   
    
};

#endif /* __WD_JSON_H__ */

