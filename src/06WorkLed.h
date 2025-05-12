#ifndef __06WORK_LED_H__
#define __06WORK_LED_H__
#include <Arduino.h>

class WorkLed {
public:
    WorkLed() {
#if (Use_WorkLed==true)
      // 初始化LED灯引脚和闪烁时间
#ifdef WorkLedPin
      _work_led_pin = WorkLedPin;
#else
      _work_led_pin = 2; // 默认引脚
#endif
      pinMode(_work_led_pin, OUTPUT); // 设置引脚为输出模式
      digitalWrite(_work_led_pin, LOW); // 初始化LED灯为熄灭状态
#ifdef WorkLedTime
      _work_led_time = WorkLedTime;
#else
      _work_led_time = 500; // 默认闪烁时间为500ms
#endif
#endif
    }
    // 创建一个任务，用于控制LED灯的闪烁
    void workled_blink(void){
#if (Use_WorkLed==true)
      // 创建一个任务，任务函数为taskFunction，任务名为workled_blink_Task，任务堆栈大小为1KB，任务优先级为1，任务句柄为_taskHandle
      xTaskCreate(taskFunction, "workled_blink_Task", 1*1024, this, 1, &_taskHandle);
#endif    
    }
    static void taskFunction(void* pvParameters) {
        WorkLed* taskInstance = static_cast<WorkLed*>(pvParameters);
        taskInstance->_work_led_blink(pvParameters);
    }
private:
  void _work_led_blink(void* pvParameters) {
    for (;;) {
      //翻转LED灯的状态
      digitalWrite(_work_led_pin, !digitalRead(_work_led_pin));
      //延时
      vTaskDelay( _work_led_time / portTICK_PERIOD_MS);
    }
  }  

private:
  int _work_led_pin;
  uint32_t _work_led_time = 500; // LED闪烁时间
  TaskHandle_t _taskHandle = NULL; // 任务句柄
};

#endif /* __06WORK_LED_H__ */
