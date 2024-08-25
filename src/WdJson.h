#ifndef __WD_JSON_H__
#define __WD_JSON_H__
#include <Arduino.h>
#include <ArduinoJson.h>
#include "DeviceNo.h"
#include "WdWifiSet.h"
// 回调函数类型定义
typedef void (*CommandCallback)(const JsonObject&);
class WdJson:public DeviceNo,public WdWifiSet
{
  public:
    WdJson():onOtherCMDCallback(nullptr) {};
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
    void onOtherCMD(CommandCallback callback) {
      onOtherCMDCallback = callback;
    }
    //解析Json字符串，执行通用指令
    void parseJsonString(String jsonString)
    {
      // 创建 JSON 文档
      JsonDocument  doc;
      // 解析 JSON 字符串
      DeserializationError error = deserializeJson(doc, jsonString);
      // 检查解析是否成功
      if (error)
      {
        log_e("DeserializationError: %s", error.c_str());
        return;
      }
       // 获取 JSON 对象
      JsonObject json = doc.as<JsonObject>();

      // 处理命令
      _handleCommand(json);          
    } 
    static void parseJsonString_static(void *arg,String jsonString)
    {
      WdJson *wdJson = (WdJson *)arg;
      wdJson->parseJsonString(jsonString);
    }
  private:
    CommandCallback onOtherCMDCallback;
    void _handleCommand(const JsonObject& json) {
        if (json.containsKey("CmdCode")) {
            String command = json["CmdCode"].as<String>();

            if (command == "SETUP") {
                if (json.containsKey("Body") && json["Body"].containsKey("SSID") && json["Body"].containsKey("PWD")) {
                    String ssid = json["Body"]["SSID"].as<String>();
                    String pwd = json["Body"]["PWD"].as<String>();
                    save_wifi_set(ssid, pwd);
                    log_i("SSID: %s, PWD: %s", ssid.c_str(), pwd.c_str());                                        
                }
            } else if (command == "noset") {
                Serial.println("Handling noset command");
            } else {
                log_i("Handling other command");
                if (onOtherCMDCallback) {
                    onOtherCMDCallback(json);
                } else {
                    log_w("onOtherCMDCallback is null");
                }
            }
        } else {
            log_i("CmdCode not found");
            if (onOtherCMDCallback) {
                onOtherCMDCallback(json);
            } else {
                log_w("onOtherCMDCallback is null");
            }
        }
    }//end _handleCommand
};

#endif /* __WD_JSON_H__ */

