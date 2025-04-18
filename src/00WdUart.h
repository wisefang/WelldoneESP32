#ifndef __00WD_UART_H__
#define __00WD_UART_H__
#include "01WdWifi.h"
// 定义回调函数类型

class WdUart : public WdWifi
{
public:
  typedef void (*CallbackFunction_onSerialReceive)(const String&);
  void wduart_init();
  void wduart_begin(void){
    wduart_init();              
    xTaskCreate(taskFunction, "uart_receive_Task", 8*1024, this, 2, &_taskHandle);
    xTaskCreate(taskFunction1, "uart_register_Task", 4*1024, this, 3, &_taskHandle);
  }
  static void taskFunction(void* pvParameters) {
    WdUart* taskInstance = static_cast<WdUart*>(pvParameters);
    taskInstance->_serial_receive_process(pvParameters);
  }
  static void taskFunction1(void* pvParameters) {
    WdUart* taskInstance1 = static_cast<WdUart*>(pvParameters);
    taskInstance1->_serial_register_process(pvParameters);
  }
  void onSerialReceive(CallbackFunction_onSerialReceive callback){
    _callback = callback;
  }
private:
  void _serial_receive_process(void* pvParameters);
  void _serial_register_process(void* pvParameters);
  void _serial_parameter_init();
private:
  TaskHandle_t _taskHandle = NULL; 
  CallbackFunction_onSerialReceive _callback;
  uint32_t _serial_baud_rate = 115200; // 串口波特率
  uint32_t _serial1_baud_rate = 115200; // 串口1波特率
  int8_t _serial1_rx_pin = 4; // 串口1接收引脚
  int8_t _serial1_tx_pin = 16; // 串口1发送引脚
  uint32_t _serial1_config = SERIAL_8N1; // 串口1配置
  uint32_t _serial2_baud_rate = 115200; // 串口2波特率
  int8_t _serial2_rx_pin = 5; // 串口2接收引脚
  int8_t _serial2_tx_pin = 17; // 串口2发送引脚
  uint32_t _serial2_config = SERIAL_8N1; // 串口2配置
  bool _serial_in_use[3] = {true, false, false}; // 串口是否正在使用
  bool _serial_send_register[3] = {true, false, false};//开机串口是否发送注册信息    
};

#endif /* __00WD_UART_H__ */
