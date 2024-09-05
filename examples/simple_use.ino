#include <Arduino.h>
#include "WelldoneESP32.h"


void handleOtherCommand_com(const JsonObject& json,HardwareSerial* com) {
    com->println("Handling other command fro com:");
    serializeJson(json, Serial);
}

void handleOtherCommand_tcp(const JsonObject& json,AsyncClient* client) {
    Serial.println("Handling other command from tcp:");
    serializeJson(json, Serial);
    if(client)
    client->write("Handling other command from client");  
}
volatile size_t sent_bytes = 0, received_bytes = 0;
void onReceiveFunction(const String &data) {
  // This is a callback function that will be activated on UART RX events
  Serial.print("Received data: ");
    Serial.println(data);
}
void setup() {
  
  // put your setup code here, to run once:

  WdESP32.onOtherCMD_tcp(handleOtherCommand_tcp);
  WdESP32.onOtherCMD_com(handleOtherCommand_com);
  WdESP32.begin();  
  
}
void loop() {

  
}

