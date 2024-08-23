#ifndef __RESET_H__
#define __RESET_H__
#include <Arduino.h>
class Reset
{
  public:
    Reset():_resetDelayTime_ms(2000){}
    void reset() {
      _resetDelayTime_ms = 2000;//default 2s
      begin();
    }
    void reset(uint32_t resetDelayTime_ms){
      _resetDelayTime_ms = resetDelayTime_ms;//set reset delay time
      begin();
    }
    void begin(void)
    {
      xTaskCreate(taskFunction, "reset_Task", 1*1024, this, 1, &_taskHandle);
    }
    static void taskFunction(void* pvParameters) 
    {
        Reset* taskInstance = static_cast<Reset*>(pvParameters);
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

