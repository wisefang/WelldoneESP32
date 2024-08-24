#include <Arduino.h>
#include <WelldoneEsp32.h>


void setup() {
  // put your setup code here, to run once:
  WdESP32.reset();//esp32 will reset after 2000ms
  WdESP32.reset(2500);//esp32 will reset after 2500ms
  WdESP32.httpOTA("http://192.168.1.100:8080/update/firmware.bin");//update firmware from http
}

void loop() {
  // put your main code here, to run repeatedly:
}

