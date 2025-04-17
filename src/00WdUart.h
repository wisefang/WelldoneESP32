#ifndef __00WD_UART_H__
#define __00WD_UART_H__
#include "01WdWifi.h"
// 定义回调函数类型
typedef void (*CallbackFunction_onSerialReceive)(const String&);
class WdUart : public WdWifi
{
public:
  WdUart();
  void uart_init();
  void uart_begin(void){
    uart_init();              
    xTaskCreate(taskFunction, "uart_receive_Task", 8*1024, this, 2, &_taskHandle);
  }
  static void taskFunction(void* pvParameters) {
    WdUart* taskInstance = static_cast<WdUart*>(pvParameters);
    taskInstance->_serial_receive_process(pvParameters);
  }
  void onSerialReceive(CallbackFunction_onSerialReceive callback){
    _callback = callback;
  }
private:
  void _serial_receive_process(void* pvParameters);
private:
  TaskHandle_t _taskHandle = NULL; 
  CallbackFunction_onSerialReceive _callback;
  uint32_t _serial_baud_rate; // 串口波特率
  uint32_t _serial1_baud_rate; // 串口1波特率
  int8_t _serial1_rx_pin; // 串口1接收引脚
  int8_t _serial1_tx_pin; // 串口1发送引脚
  uint32_t _serial1_config; // 串口1配置
  uint32_t _serial2_baud_rate; // 串口2波特率
  int8_t _serial2_rx_pin; // 串口2接收引脚
  int8_t _serial2_tx_pin; // 串口2发送引脚
  uint32_t _serial2_config; // 串口2配置
  bool _serial_in_use[3]; // 串口是否正在使用    
};

#endif /* __00WD_UART_H__ */
