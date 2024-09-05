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
        WelldoneEsp32();
        ~WelldoneEsp32();
        // 定义回调函数类型
        typedef void (*CallbackFunction_onSerialReceive)(const String&);
        void init(void);
        void begin(void){
            init();
                      
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
        uint32_t _serial_baud_rate; // 串口波特率
    private:
        void _serial_receive_process(void *pvParameters) {
            HardwareSerial* com[3] = {&Serial, &Serial1, &Serial2};
            for (;;) {
                for (int i = 0; i < 3; i++) {
                    if (com[i]->available() > 0) {
                        char buffer[256];                        
                        // 确保读取不会超出缓冲区大小
                        int bytesRead = com[i]->readBytesUntil('\n', buffer, sizeof(buffer));
                        if ((bytesRead >= min_command_length) && (bytesRead <= 255)) {
                            String data(buffer);                            
                            parseJsonString(data, com[i]);                                
                        } else {
                            // 命令字符串太短或太长
                            log_e("Invalid command length: %d", bytesRead);
                        }
                    }
                }
                vTaskDelay(50 / portTICK_PERIOD_MS);
            }
        }
};
extern WelldoneEsp32 WdESP32;
#endif /* __WELLDONE_ESP32_H__ */
