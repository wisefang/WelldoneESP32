#include <Arduino.h>
#include "WelldoneEsp32.h"



// Forward declaration of JsonCommandHandler class
class JsonCommandHandler {
public:
    JsonCommandHandler() {
        
    }
    void init() {
      // Register command handlers
      WdESP32.registerCommand("sendPulse", [this](const JsonObject& command) {
          return handle_sendPulse_Command(command);
      });
      WdESP32.registerCommand("Set", [this](const JsonObject& command) {
          return handle_Set_Command(command);
      });
    }
    const char* handle_sendPulse_Command(const JsonObject& command);
    const char* handle_Set_Command(const JsonObject& command);
};
JsonCommandHandler Json_Command_Handler;
//json
const char* JsonCommandHandler::handle_sendPulse_Command(const JsonObject& command) {
  String _return_str = WdESP32.getJsonString("sendPulse",1); 
  char* _result = new char[_return_str.length() + 1];
  snprintf(_result, _return_str.length() + 1, "%s", _return_str.c_str());
  return _result;  
}

const char* JsonCommandHandler::handle_Set_Command(const JsonObject& command) {
  String _return_str = WdESP32.getJsonString("Set",1);  
  char* _result = new char[_return_str.length() + 1];
  snprintf(_result, _return_str.length() + 1, "%s", _return_str.c_str());
  return _result;
}
//not json
void handleNotJson_com(const String& data,HardwareSerial* com) {
    com->println("Handling not json fro com:");
    com->println(data);
}
void handleNotJson_tcp(const String& data,AsyncClient* client) {
    Serial.println("Handling not json from tcp:");
    Serial.println(data);
    if(client)
    client->write("Handling not json from client");  
}
void handleNotJson_udp(const String& data,AsyncUDPPacket packet) {
    Serial.println("Handling not json from udp:");
    Serial.println(data);
    packet.println("Handling not json from udp");  
}
// void onReceiveFunction(const String &data) {
//   // This is a callback function that will be activated on UART RX events
//   Serial.print("Received data: ");
//     Serial.println(data);
// }
void setup() {
  
  // put your setup code here, to run once:

//   WdESP32.onOtherCMD_tcp(handleOtherCommand_tcp);
//   WdESP32.onOtherCMD_com(handleOtherCommand_com);
//   WdESP32.onOtherCMD_udp(handleOtherCommand_udp);
  Json_Command_Handler.init();
  WdESP32.onNotJsonCMD(handleNotJson_com);
  WdESP32.onNotJsonCMD(handleNotJson_tcp);
  WdESP32.onNotJsonCMD(handleNotJson_udp);
  WdESP32.init();  
  
}
void loop() {
  // put your main code here, to run repeatedly:
}

