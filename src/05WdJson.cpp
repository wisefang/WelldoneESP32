/**********************************************************
 * @file    WdJson.cpp
 * @author  Majunfang (Majf@Welldone-e.com)
 * @brief 
 * @version V1.0
 * @date    2024-08-24
 * 
 * @copyright  Copyright © 2024 Huidong All Rights Reserved
 * 
 ***********************************************************/
#include "05WdJson.h"
#include "ArduinoJson.h"
/**********************************************************
 * @brief 解析tcp数据，处理通用命令，初始化回调函数
 * 
 * @param jsonString 
 * @param client 
 ***********************************************************/
void WdJson::_deserial_string(String jsonString,AsyncClient* client)
{
  JsonDocument  doc;
  DeserializationError error = deserializeJson(doc, jsonString);
  if (error){
    log_e("DeserializationError: %s", error.c_str());
    String err_str = getJsonString("PARSING_FAILED",error.c_str());
    client->write(err_str.c_str());  
  }else{
    JsonObject json = doc.as<JsonObject>();
    String CmdReturn=_handleGeneralCommand(json);
    log_i("CmdReturn: %s", CmdReturn.c_str());
    if (CmdReturn == "OtherCmd"){
      if (_onOtherCMDCallback_tcp) {
        _onOtherCMDCallback_tcp(json,client);
      } else {
        log_w("onOtherCMDCallback_tcp is null");
      }
    }
    else{
      client->write(CmdReturn.c_str());
    }
  }
} 
/**********************************************************
 * @brief 解析com数据，处理通用命令，初始化回调函数
 * 
 * @param jsonString 
 * @param com 
 ***********************************************************/
void WdJson::_deserial_string(String jsonString,HardwareSerial* com)
{
  JsonDocument  doc;
  DeserializationError error = deserializeJson(doc, jsonString);
  if (error){
    log_e("DeserializationError: %s", error.c_str());
    String err_str = getJsonString("PARSING_FAILED",error.c_str());
    com->write(err_str.c_str());  
  }else{
    JsonObject json = doc.as<JsonObject>();
    String CmdReturn=_handleGeneralCommand(json);
    log_i("CmdReturn: %s", CmdReturn.c_str());
    if (CmdReturn == "OtherCmd"){
      if (_onOtherCMDCallback_com) {
        _onOtherCMDCallback_com(json, com);
      } else {
        log_w("onOtherCMDCallback_com is null");
      }
    }
    else{
      com->write(CmdReturn.c_str());
    }
  }
}
/**********************************************************
 * @brief 处理通用的指令，其他指令返回OtherCmd调用回调函数
 * 
 * @param json 
 * @return String 
 ***********************************************************/
String WdJson::_handleGeneralCommand(const JsonObject& json){
  String _return_str = "";
  if (json.containsKey("CmdCode")) {
    String command = json["CmdCode"].as<String>();
    if (command == "SETUP") {
      if (json.containsKey("Body")) {
        //wifi setup
        if (json["Body"].containsKey("SSID") && json["Body"].containsKey("PWD")) {
          String ssid = json["Body"]["SSID"].as<String>();
          String pwd = json["Body"]["PWD"].as<String>();
          if(save_station_setting(ssid, pwd)){
            _return_str = getJsonString("SETUP", "OK");
          } else{
            _return_str = getJsonString("SETUP", "FAIL_SSID_PWD");
          }          
        }
        //SerialNo setup
        else if (json["Body"].containsKey("SerialNo")) {  
          uint32_t serialNo = json["Body"]["SerialNo"].as<uint32_t>();
          save_serial_no(serialNo);
          _return_str = getJsonString("SETUP", "OK");
        } 
        //HttpOTA
        else if (json["Body"].containsKey("HttpOTA")) {
          String url = json["Body"]["HttpOTA"].as<String>();
          httpOTA(url);
          _return_str = getJsonString("SETUP", "OK");
        }
        //rest
        else if (json["Body"].containsKey("Reset")) {
          uint32_t rest_delayms = json["Body"]["Reset"].as<uint32_t>();
          wdreset(rest_delayms);
          _return_str = getJsonString("SETUP", "OK");
        } else{ _return_str = "OtherCmd";}
      } else{ _return_str = "OtherCmd";} 
    } else{ _return_str = "OtherCmd";}
  } else{ _return_str = "OtherCmd";}
  return _return_str;
}
