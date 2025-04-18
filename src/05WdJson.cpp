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
 * @brief 生成注册json字符串
 * 
 * @return String 
 ***********************************************************/
String WdJson::generateRegisterJsonString(void){
  JsonDocument output_doc;       
  String outputString = "";
#ifndef Device_Code
#define Device_Code 50001
#endif 
  output_doc["DeviceType"]=Device_Code;
  output_doc["DeviceNo"]=get_device_no();
  output_doc["CmdCode"]="REGISTER";
  JsonObject body = output_doc["Body"].to<JsonObject>();
  body["Action"] = 0;
  body["HardwareVer"] = HardwareVer;
  body["SoftwareVer"] = SoftwareVer;
  serializeJson(output_doc, outputString);
  return outputString;
}
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
    if (CmdReturn == CMD_REGISTER){
      isTcpConnected = true;
    } else if (CmdReturn == "OtherCmd"){
      if (_onOtherCMDCallback_tcp) {
        _onOtherCMDCallback_tcp(json,client);
      } else {
        log_w("onOtherCMDCallback_tcp is null");
      }
    } else{
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
    if( CmdReturn == CMD_REGISTER){
      if (com == &Serial) {
        isUartConnected[0] = true;
      }
      if (com == &Serial1) {
        isUartConnected[1] = true;
      }
      if (com == &Serial2) {      
        isUartConnected[2] = true;
      }
      
    } else if (CmdReturn == CMD_OTHER){
      if (_onOtherCMDCallback_com) {
        _onOtherCMDCallback_com(json, com);
      } else {
        log_w("onOtherCMDCallback_com is null");
      }
    } else{
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
  
  String command = json["CmdCode"].as<String>();
  const JsonObject& body = json["Body"]; 
  if (command == CMD_SETUP) {   
    if ((body["SSID"].is<const char*>()) && (body["PWD"].is<const char*>())) {
      const char* ssid = body["SSID"];
      const char* pwd  = body["PWD"];
      log_i("ssid:%s, pwd:%s",ssid,pwd);
      if (save_station_setting(ssid, pwd)) {
        _return_str = getJsonString(CMD_SETUP, CMD_OK);
      } else {
        _return_str = getJsonString(CMD_SETUP, CMD_FAIL_SSID_PWD);
      }
    } else if (body["HttpOTA"].is<const char*>()) {   
      const char* url = body["HttpOTA"];
      log_i("url:%s", url); 
      httpOTA(url);
      _return_str = getJsonString(CMD_SETUP, CMD_OK);
    } else if (body["SerialNo"].is<uint32_t>()) { 
      uint32_t serialNo = body["SerialNo"].as<uint32_t>();
      log_i("serialNo:%d", serialNo);   
      save_serial_no(serialNo);
      _return_str = getJsonString(CMD_SETUP, CMD_OK);
    }  else if (body["Reset"].is<uint32_t>()) { 
      uint32_t rest_delayms = body["Reset"].as<uint32_t>();    
      log_i("rest_delayms:%d", rest_delayms);
      wdreset(rest_delayms);
      _return_str = getJsonString(CMD_SETUP, CMD_OK);
    }
  } else if (command == CMD_REGISTER) {
    _return_str = CMD_REGISTER;    
  }
  return _return_str;
}
