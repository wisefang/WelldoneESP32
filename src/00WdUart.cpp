/**********************************************************
 * @file    00WdUart.cpp
 * @author  Majunfang (Majf@Welldone-e.com)
 * @brief 
 * @version V1.0
 * @date    2025-04-17
 * 
 * @copyright  Copyright © 2025 Huidong All Rights Reserved
 * 
 ***********************************************************/
#include "00WdUart.h"

/**********************************************************
 * @brief 串口参数初始化
 * 
 ***********************************************************/
void WdUart::_serial_parameter_init(){
#ifdef Serial_BaudRate
  _serial_baud_rate = Serial_BaudRate;
#else
  _serial_baud_rate = 115200;
#endif
#ifdef Serial1_BaudRate
  _serial1_baud_rate = Serial1_BaudRate;
#else
  _serial1_baud_rate = 115200;
#endif
#ifdef Serial1_Config
  _serial1_config = Serial1_Config;
#else
  _serial1_config = SERIAL_8N1;
#endif
#ifdef Serial1_Rx_Pin
  _serial1_rx_pin = Serial1_Rx_Pin;
#else
  _serial1_rx_pin = 9;
#endif
#ifdef Serial1_Tx_Pin
  _serial1_tx_pin = Serial1_Tx_Pin;
#else
  _serial1_tx_pin = 10;
#endif
#ifdef Serial2_Config
  _serial2_config = Serial2_Config;
#else
  _serial2_config = SERIAL_8N1;
#endif
#ifdef Serial2_Rx_Pin
  _serial2_rx_pin = Serial2_Rx_Pin;
#else
  _serial2_rx_pin = 16;
#endif
#ifdef Serial2_Tx_Pin 
  _serial2_tx_pin = Serial2_Tx_Pin;
#else
  _serial2_tx_pin = 17;
#endif

#ifdef Serial2_BaudRate
  _serial2_baud_rate = Serial2_BaudRate;
#else
  _serial2_baud_rate = 115200;
#endif
  //三个串口使用情况，串口0默认使用  
  _serial_in_use[0] = true;
#ifdef Use_Serial1
  _serial_in_use[1] = true;
#else
  _serial_in_use[1] = false;
#endif
#ifdef Use_Serial2
  _serial_in_use[2] = true;
#else
  _serial_in_use[2] = false;
#endif  
//   //开机发送注册信息
#if (Serial_Send_Register == true)
  _serial_send_register[0] = true;
#else
  _serial_send_register[0] = false;
#endif
#if (Serial1_Send_Register == true)
  _serial_send_register[1] = true;
#else
  _serial_send_register[1] = false;
#endif
#if (Serial2_Send_Register == true)
  _serial_send_register[2] = true;
#else
  _serial_send_register[2] = false;
#endif
}
/**********************************************************
 * @brief 00WdUart init() 初始化
 * 
 ************************************************************/
void WdUart::wduart_init(){
  // _serial_parameter_init();
  //Serial init
  Serial.setTimeout(50);
  Serial.begin(_serial_baud_rate);
  //Serial1 init 
  if (_serial_in_use[1]){
    Serial1.setTimeout(50);
    Serial1.begin(_serial1_baud_rate,_serial1_config, _serial1_rx_pin, _serial1_tx_pin);
  }
  //Serial2 init
  if (_serial_in_use[2]){
    Serial2.setTimeout(50);
    Serial2.begin(_serial2_baud_rate,_serial2_config, _serial2_rx_pin, _serial2_tx_pin);
  }
}
/**********************************************************
 * @brief 串口注册信息发送
 * 
 * @param pvParameters 
 ***********************************************************/
void  WdUart::_serial_register_process(void *pvParameters) {
  HardwareSerial* com[3] = {&Serial, &Serial1, &Serial2};
  for (;;) {
    for (int i = 0; i < 3; i++) {
      if ((_serial_in_use[i])&&(_serial_send_register[i]) && (!isUartConnected[i])) {
        com[i]->write(generateRegisterJsonString().c_str());
      }
    }
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}
/**********************************************************
 * @brief _serial_receive_process() 串口接收处理函数
 * 
 * @param pvParameters 
 ***********************************************************/
void  WdUart::_serial_receive_process(void *pvParameters) {
  HardwareSerial* com[3] = {&Serial, &Serial1, &Serial2};
  for (;;) {
    for (int i = 0; i < 3; i++) {
      if ((_serial_in_use[i])&&(com[i]->available() > 0)) {
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