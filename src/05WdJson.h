#ifndef __05WD_JSON_H__
#define __05WD_JSON_H__
#include <Arduino.h>
#include <ArduinoJson.h>
#include "06DeviceNo.h"
#include "06WdWifiSet.h"
#include "06BuildTime.h"
#include "06HttpOTA.h"
#include "06WdReset.h"
#include "AsyncTCP.h"
// 回调函数类型定义

typedef void (*CommandCallback_com)(const JsonObject&,HardwareSerial*);
typedef void (*CommandCallback_tcp)(const JsonObject&,AsyncClient*);
class WdJson:public DeviceNo,public WdWifiSet,public BuildTime,public HttpOTA,public WdReset
{
  public:
    WdJson():_onOtherCMDCallback_com(nullptr), _onOtherCMDCallback_tcp(nullptr){};
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
    
    //解析Json字符串，执行通用指令
    void parseJsonString(String jsonString,AsyncClient* client)
    {
      _deserial_string(jsonString,client);     
    }
    void parseJsonString(String jsonString,HardwareSerial* com)
    {
      _deserial_string(jsonString,com);
    } 
    static void parseJsonString_static(void *arg,String jsonString,HardwareSerial* com)
    {
      WdJson *wdJson = (WdJson *)arg;
      wdJson->parseJsonString(jsonString,com);
    }
  private:
    CommandCallback_com _onOtherCMDCallback_com;
    CommandCallback_tcp _onOtherCMDCallback_tcp; 
    
  private:
    void _deserial_string(String jsonString,AsyncClient* client);
    void _deserial_string(String jsonString,HardwareSerial* com);
    String _handleGeneralCommand(const JsonObject& json);   
    
};

#endif /* __WD_JSON_H__ */

