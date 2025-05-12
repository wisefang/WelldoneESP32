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

// 定义回调函数类型，返回字符串
typedef const char* (*CommandCallback)(const JsonObject& command);
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
    //处理串口的其他json命令
    void handleOtherCommand_com(const JsonObject& json,HardwareSerial* com) {
      String _return_cmd = handleJsonCommand(json);
      if(com) com->println(_return_cmd);
    }
    //处理tcp的其他json命令
    void handleOtherCommand_tcp(const JsonObject& json,AsyncClient* client) {
      String _return_cmd = handleJsonCommand(json);
      if(client) client->write(_return_cmd.c_str());
    }
    //处理udp的其他json命令
    void handleOtherCommand_udp(const JsonObject& json,AsyncUDPPacket packet) {  
      String _return_cmd = handleJsonCommand(json);
      packet.println(_return_cmd.c_str());
    }
    //注册命令处理函数
    void registerCommand(const char* commandName, const char* (WdJson::*callback)(const JsonObject& command)) {
      if (commandCount < MAX_COMMANDS) {
          commandMappings[commandCount].commandName = commandName;
          commandMappings[commandCount].callback = callback;
          commandCount++;
      }
    }
    // 处理 JSON 命令的函数
    String handleJsonCommand(const JsonObject& json) {
      String _return_str = getJsonString("commanderror",1);       
      if (json["CmdCode"].is<const char*>()) {
        const char* commandName = json["CmdCode"];
        if (commandName) {
          for (int i = 0; i < commandCount; i++) {
              if (strcmp(commandName, commandMappings[i].commandName) == 0) {                
                  _return_str = (String)(this->*(commandMappings[i].callback))(json);
                  commandHandled = true;
                  break; // 找到匹配的命令后退出循环
              }
          }
          if (!commandHandled) {
            _return_str = getJsonString("unknownCommand",1);      
          }
        } else {
          _return_str = getJsonString("noCommand",1);
        } 
      }
      return _return_str;
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

    // 命令映射表
  struct CommandMapping {
    const char* commandName;
    const char* (WdJson::*callback)(const JsonObject& command);
  };

  // 最大命令数量
  static const int MAX_COMMANDS = 50;
  CommandMapping commandMappings[MAX_COMMANDS];
  int commandCount = 0;
  bool commandHandled = false;
    
};

#endif /* __WD_JSON_H__ */

