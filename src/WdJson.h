#ifndef __WD_JSON_H__
#define __WD_JSON_H__
#include <Arduino.h>
#include <ArduinoJson.h>
#include "DeviceNo.h"
#include "WdWifi.h"

class WdJson:public DeviceNo,public WdWifi
{
  public:
    template<typename T>
    String getJsonString(String cmdCode, T result);
    //判断字符串是不是json格式，是返回true，不是返回false
    bool isJson(String jsonString)    {
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, jsonString);
      if (error)
      {
        return false;
      }
      return true;
    }
    //解析Json字符串，执行通用指令
    String parseJsonString(String jsonString)
    {
      JsonDocument  doc;
      String returnStr = "";
      DeserializationError error = deserializeJson(doc, jsonString);
      if (error)
      {
        log_e("DeserializationError: %s", error.c_str());
        returnStr = "PARSING_FAILED";
      }
      else
      {
        //general cmd SETUP CmdCode=SETUP
        if (doc.containsKey("CmdCode"))
        {
          String cmdCode = doc["CmdCode"];
          if (cmdCode == "SETUP")
          {
            //SETUP WIFI ["Body"]["SSID"],["Body"]["PWD"]
            if (doc.containsKey("Body"))
            {
              JsonObject body = doc["Body"];
              if (body.containsKey("SSID") && body.containsKey("PWD"))
              {
                String ssid = body["SSID"];
                String pwd = body["PWD"];
                save_wifi_set(ssid, pwd);
                returnStr = getJsonString("SETUP", 0);                
              }
              else
              {
                returnStr = "PARSING_FAILED";
              }
            }
            else
            {
              returnStr = "PARSING_FAILED";
            }
          }
          else
          {
            returnStr = "PARSING_FAILED";
          }
        }
        else
        {
          returnStr = "Not Found CmdCode";
        }
      
      }
      return returnStr;
    }
    // void   setJsonString(String jsonString);
    // void   setJsonString(String jsonString, String key, String value);
    // void   setJsonString(String jsonString, String key, int value);
    // void   setJsonString(String jsonString, String key, float value);
    // void   setJsonString(String jsonString, String key, bool value);
    // void   setJsonString(String jsonString, String key, const char *value);
    // void   setJsonString(String jsonString, String key, JsonArray value);
    // void   setJsonString(String jsonString, String key, JsonObject value);
    // void   setJsonString(String jsonString, String key, JsonVariant value);
    // void   setJsonString(String jsonString, String key, JsonArray value, int index, String subKey, String subValue);
    // void   setJsonString(String jsonString, String key, JsonArray value, int index, String subKey, int subValue);
    // void   setJsonString(String jsonString, String key, JsonArray value, int index, String subKey, float subValue);
    // void   setJsonString(String jsonString, String key, JsonArray value, int index, String subKey, bool subValue);
    // void   setJsonString(String jsonString, String key, JsonArray value, int index, String subKey, const char *subValue);
    // void   setJsonString(String jsonString, String key, JsonArray value, int index, String subKey, JsonArray subValue);
    // void   setJsonString(String jsonString, String key, JsonArray value, int index, String subKey, JsonObject subValue);
    // void   setJsonString(String jsonString, String key, JsonArray value, int index, String subKey, JsonVariant subValue);
    // void   setJsonString(String jsonString, String key, JsonObject value, String subKey, String subValue);
    // void   setJsonString(String jsonString, String key, JsonObject value, String subKey, int subValue);
    // void   setJsonString(String jsonString, String key, JsonObject value, String subKey, float subValue);
    // void   setJsonString(String jsonString, String key, JsonObject value, String subKey, bool subValue);
    // void   setJsonString(String jsonString, String key, JsonObject value, String subKey, const char *subValue);
    // void   setJsonString(String jsonString, String key, JsonObject value, String subKey, JsonArray subValue);
    // void   setJsonString(String jsonString, String key, JsonObject value, String subKey, JsonObject subValue);
    // void   setJsonString(String jsonString, String key, JsonObject value, String subKey, JsonVariant subValue);
    // String getJsonString(String jsonString, String key);
    // String getJsonString(String jsonString, String key, int index, String subKey);
    // String getJsonString(String jsonString, String key, int index, String subKey, int subIndex, String subSubKey);
    // String getJsonString(String jsonString, String key, int index, String subKey, int subIndex, String subSubKey, int subSubIndex, String subSubSubKey);
    // String getJsonString(String jsonString, String key, String subKey);
 
    

};
template<typename T1>
String WdJson::getJsonString(String cmdCode, T1 result)
{
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

#endif /* __WD_JSON_H__ */

