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
  String _return_str = CMD_OTHER;

  if (!json.containsKey("CmdCode")) {
    return _return_str;
  }

  String command = json["CmdCode"].as<String>();
  if (command != CMD_SETUP) {
    return _return_str;
  }

  if (!json.containsKey("Body")) {
    return _return_str;
  }

  const JsonObject& body = json["Body"];

  if (body.containsKey("SSID") && body.containsKey("PWD")) {
    String ssid = body["SSID"].as<String>();
    String pwd = body["PWD"].as<String>();
    if (save_station_setting(ssid, pwd)) {
      _return_str = getJsonString(CMD_SETUP, CMD_OK);
    } else {
      _return_str = getJsonString(CMD_SETUP, CMD_FAIL_SSID_PWD);
    }
  } else if (body.containsKey("SerialNo")) {
    if (!body["SerialNo"].is<uint32_t>()) {
      return getJsonString(CMD_SETUP, "INVALID_SERIALNO");
    }
    uint32_t serialNo = body["SerialNo"].as<uint32_t>();
    save_serial_no(serialNo);
    _return_str = getJsonString(CMD_SETUP, CMD_OK);
  } else if (body.containsKey("HttpOTA")) {
    String url = body["HttpOTA"].as<String>();
    httpOTA(url);
    _return_str = getJsonString(CMD_SETUP, CMD_OK);
  } else if (body.containsKey("Reset")) {
    if (!body["Reset"].is<uint32_t>()) {
      return getJsonString(CMD_SETUP, "INVALID_RESET_DELAY");
    }
    uint32_t rest_delayms = body["Reset"].as<uint32_t>();
    wdreset(rest_delayms);
    _return_str = getJsonString(CMD_SETUP, CMD_OK);
  }
  return _return_str;
}
