#include <Arduino.h>
#include "WelldoneESP32.h"
#include "soc/rtc_wdt.h"     //设置看门狗用

void handleOtherCommand_com(const JsonObject& json,HardwareSerial* com) {
    com->println("Handling other command fro com:");
    
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
  // WdESP32.onOtherCMD(handleOtherCommand);
  WdESP32.onOtherCMD_tcp(handleOtherCommand_tcp);
  WdESP32.onOtherCMD_com(handleOtherCommand_com);
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
  WdESP32.wdreset();//esp32 will reset after 2000ms
  WdESP32.wdreset(2500);//esp32 will reset after 2500ms
  WdESP32.httpOTA("http://192.168.1.100:8080/update/firmware.bin");//update firmware from http
}
  rtc_wdt_protect_off();     //看门狗写保护关闭 关闭后可以喂狗
  //rtc_wdt_protect_on();    //看门狗写保护打开 打开后不能喂狗
  //rtc_wdt_disable();       //禁用看门狗
  rtc_wdt_enable();          //启用看门狗
  rtc_wdt_feed();            //喂狗
  rtc_wdt_set_time(RTC_WDT_STAGE0, 2500);     // 设置看门狗超时 5000ms.
}
void loop() {
  // put your main code here, to run repeatedly:
  rtc_wdt_feed();
  delay(1000);
}

