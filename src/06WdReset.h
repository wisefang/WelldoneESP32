/**********************************************************
 * @file    Reset.h
 * @author  Majunfang (Majf@Welldone-e.com)
 * @brief 
 * @version V1.0
 * @date    2024-08-24
 * 
 * 
 ***********************************************************/
#ifndef __06WD_RESET_H__
#define __06WD_RESET_H__
#include <Arduino.h>
class WdReset
{
  public:
    WdReset():_resetDelayTime_ms(2000){}
    //ESP32 reset,Default 2000ms,reset(uint32_t resetDelayTime_ms) 
    void wdreset() {
      _resetDelayTime_ms = 2000;//default 2s
      _begin();
    }
    void wdreset(uint32_t resetDelayTime_ms){
      _resetDelayTime_ms = resetDelayTime_ms;//set reset delay time
      _begin();
    }
  private:
    void _begin(void)
    {
      xTaskCreate(taskFunction, "reset_Task", 1*1024, this, 1, &_taskHandle);
    }
    static void taskFunction(void* pvParameters) 
    {
        WdReset* taskInstance = static_cast<WdReset*>(pvParameters);
        taskInstance->_delay_reset(pvParameters);
    }
  private:
    void _delay_reset(void *pvParameters)    {
      vTaskDelay(_resetDelayTime_ms / portTICK_PERIOD_MS);// delay
      ESP.restart();// reset
    }
  private:
    uint32_t _resetDelayTime_ms ;
    TaskHandle_t _taskHandle = NULL;
};
#endif /* __RESET_H__ */

