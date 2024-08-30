/**********************************************************
 * @file    WelldoneEsp32.h
 * @author  Majunfang (Majf@Welldone-e.com)
 * @brief 
 * @version V1.0
 * @date    2024-08-24
 * 
 ***********************************************************/
#ifndef __WELLDONE_ESP32_H__
#define __WELLDONE_ESP32_H__
#include "01WdWifi.h"
class WelldoneEsp32:public WdWifi
{
    public:
        // 定义回调函数类型
        typedef void (*CallbackFunction_onSerialReceive)(const String&);
        void init(void){
            Serial.setTimeout(50);
            Serial.begin(115200);
            device_no_init();
            wifi_begin();
            
            xTaskCreate(taskFunction, "uart_receive_Task", 8*1024, this, 2, &_taskHandle);
        }
        static void taskFunction(void* pvParameters) {
        WelldoneEsp32* taskInstance = static_cast<WelldoneEsp32*>(pvParameters);
        taskInstance->_serial_receive_process(pvParameters);
    }
        void onSerialReceive(CallbackFunction_onSerialReceive callback){
            _callback = callback;
        }
    private:
        TaskHandle_t _taskHandle = NULL; 
        CallbackFunction_onSerialReceive _callback;
    private:
        void _serial_receive_process(void *pvParameters){
            HardwareSerial* com[3] = {&Serial, &Serial1 , &Serial2};
            for(;;){
                for(int i = 0; i < 3; i++){
                    if (com[i]->available() > 0) {
                        String data = com[i]->readStringUntil('\n');
                        parseJsonString(data,com[i]);                        
                    }
                }
                vTaskDelay(50 / portTICK_PERIOD_MS);                
            }
        }
};
extern WelldoneEsp32 WdESP32;
#endif /* __WELLDONE_ESP32_H__ */
