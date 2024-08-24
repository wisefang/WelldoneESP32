#include "simple_use.h"
#include "WelldoneESP32.h"
void simple_use(void)
{
  WdESP32.reset();//esp32 will reset after 2000ms
  WdESP32.reset(2500);//esp32 will reset after 2500ms
  WdESP32.httpOTA("http://192.168.1.100:8080/update/firmware.bin");//update firmware from http
}
