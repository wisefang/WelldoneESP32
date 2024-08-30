#ifndef __06DEVICE_NO_H__
#define __06DEVICE_NO_H__
#include <Arduino.h>
#include <Preferences.h>

class DeviceNo
{
  
  public:
    DeviceNo(){     
    }
    ~DeviceNo(){      
      log_i("Close preferences device_no");
    }
    //get device_num_str
    String get_device_no(void){
      return _device_no;
    }
    //get device_num_uint 
    uint32_t get_device_num_uint(void){
      return _device_num_uint;
    }
    void save_serial_no(uint32_t num_uint)    {
      _preferences_device_no.begin("Device_no");
      _device_num_uint = num_uint;
      //_device_num_uint>=10000 ,_device_num_uint=1
      if(_device_num_uint>=10000){
        _device_num_uint = 1;
      }
      _preferences_device_no.putUInt("num_uint", _device_num_uint);          
      _device_no = _to_devie_no(_device_num_uint);
      _preferences_device_no.end();
    }
  static void SaveDeviceNum_static(void *arg,uint32_t num_uint)
    {
      DeviceNo *device_no = (DeviceNo *)arg;
      device_no->save_serial_no(num_uint);
    }
  private:
    Preferences _preferences_device_no;
    uint32_t _device_num_uint;
    String _device_no;//设备序列号，序号前加JFJZ
  public:
    void device_no_init(void){
      _preferences_device_no.begin("Device_no");
      _device_num_uint = _preferences_device_no.getUInt("num_uint", 1);
      //_device_num_uint>=10000 ,_device_num_uint=1
      if(_device_num_uint>=10000){
        _device_num_uint = 1;
        _preferences_device_no.putUInt("num_uint", _device_num_uint);
      }
      _device_no = _to_devie_no(_device_num_uint);
      _preferences_device_no.end();
    }
  private:
    String _to_devie_no(uint32_t num_uint){
      char buffer_No[10];
      sprintf(buffer_No,"%04d",num_uint);
      #ifndef Device_Name
      #define Device_Name "JFJZ"
      #endif            
      return Device_Name + String(buffer_No);
    }
};
#endif /* __F13_DEVICE_NO_H__ */



