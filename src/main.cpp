#include <Arduino.h>
#include "WelldoneEsp32.h"


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
  WdESP32.onNotJson_com(handleNotJson_com);
  WdESP32.onNotJson_tcp(handleNotJson_tcp);
  WdESP32.onNotJson_udp(handleNotJson_udp);
  WdESP32.init();  
  
}
void loop() {

  
}

