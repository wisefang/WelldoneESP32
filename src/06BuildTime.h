#ifndef __06BUILD_TIME_H__
#define __06BUILD_TIME_H__

#include <Arduino.h>
#include <TimeLib.h>
#include <Timezone.h>
class BuildTime
{
  public:
    String getBuildTime(void){
      TimeChangeRule CSTRule = {"CST", Last, Sun, Mar, 2, 480};    // 中国标准时间规则，东八区为UTC+8，没有夏令时
      Timezone myTZ(CSTRule);  // 创建Timezone对象
      String BuildTimeString;
      time_t unixTime = CURRENT_TIME;
      time_t localTime = myTZ.toLocal(unixTime); // 将Unix时间戳转换为本地时间戳
      tmElements_t tm; // 创建tmElements_t结构体变量，用于存储时间元素
      breakTime(localTime, tm); // 将localTime时间戳转换为时间元素
      BuildTimeString = "Build Time: "\
                        + String(tmYearToCalendar(tm.Year)) + "-" \
                        + _addLeadingZero(tm.Month) + "-" \
                        + _addLeadingZero(tm.Day) + " "\
                        + _addLeadingZero(tm.Hour) + ":"\
                        + _addLeadingZero(tm.Minute) + ":"\
                        + _addLeadingZero(tm.Second);
      return(BuildTimeString);
    }
    String getVersion(void){
      String VerString = "Firmware ";
      VerString+=SoftwareVer;
      VerString+=" of ";
      VerString+=HardwareVer;
      return(VerString);
    }
  private:
    String _addLeadingZero(int number){
      if (number < 10){
        return "0" + String(number);
      }else {
        return String(number);
      }
    }
};
#endif /* __F20_BUILD_TIME_H__ */
