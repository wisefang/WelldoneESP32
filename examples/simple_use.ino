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
  WdESP32.begin();  
  //watchdog
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