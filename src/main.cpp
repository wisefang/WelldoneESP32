#include <Arduino.h>
#include "WelldoneESP32.h"
void handleOtherCommand(const JsonObject& json) {
    Serial.println("Handling other command:");
    serializeJson(json, Serial);
    Serial.println();
}
volatile size_t sent_bytes = 0, received_bytes = 0;
void onReceiveFunction(const String &data) {
  // This is a callback function that will be activated on UART RX events
  Serial.print("Received data: ");
    Serial.println(data);
}
void setup() {
  // put your setup code here, to run once:
  WdESP32.onOtherCMD(handleOtherCommand);
  WdESP32.onSerialReceive(onReceiveFunction);
  WdESP32.init();  
  
  // uint32_t device_num_uint = WdESP32.get_device_num_uint();
  // //serial out: device_num_uint:15
  // Serial.printf("device_num_uint:%d\n",device_num_uint);
  // String device_no = WdESP32.get_device_no();
  // //serial out:device_num_str:TDR0015
  // Serial.printf("device_num_str:%s\n",device_no.c_str());
  // device_num_uint ++;
  // WdESP32.save_device_num_uint(device_num_uint);
  // String Result_Str="hello world";
  // String JsonString_HeartBeat= WdESP32.getJsonString("HeartBeat",Result_Str);
  // //serial out:{"DeviceType":50001,"CmdCode":"HeartBeat","Body":{"Result":"hello world"}}
  // Serial.printf("%s\n",JsonString_HeartBeat.c_str());
  // int Result_int=1;
  // String JsonString_HeartBeat2= WdESP32.getJsonString("HeartBeat",Result_int);
  // //serial out:{"DeviceType":50001,"CmdCode":"HeartBeat","Body":{"Result":1}}
  // Serial.printf("%s\n",JsonString_HeartBeat2.c_str());
  // String error_JsonString = "{\"error\":333}\"";
  // WdESP32.parseJsonString(error_JsonString);
  // String right_JsonString = "{\"CmdCode\":222}";
  // WdESP32.parseJsonString(right_JsonString);
  
if(0)//code not to be executed,only for examples
{
  WdESP32.reset();//esp32 will reset after 2000ms
  WdESP32.reset(2500);//esp32 will reset after 2500ms
  WdESP32.httpOTA("http://192.168.1.100:8080/update/firmware.bin");//update firmware from http
}
}
void loop() {
  // put your main code here, to run repeatedly:
}

