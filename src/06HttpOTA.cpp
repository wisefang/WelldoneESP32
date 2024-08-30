/**********************************************************
 * @file    F19_HttpOTA.cpp
 * @author  Majunfang (Majf@Welldone-e.com)
 * @brief 
 * @version V1.0
 * @date    2024-08-23
 * 
 ***********************************************************/
#include "06HttpOTA.h"


/**********************************************************
 * @brief 
 * 
 ***********************************************************/
void HttpOTA::httpOTA(String ota_url)
{
  _ota_url = ota_url;
  t_httpUpdate_return ret = _updateBin(ota_url); //开始升级
  switch (ret)
  {
    case HTTP_UPDATE_FAILED: //当升级失败
        log_e("Update failed.");
        break;
    case HTTP_UPDATE_NO_UPDATES: //当无升级
        log_i("no Update.");
        break;
    case HTTP_UPDATE_OK: //当升级成功
        log_i("Update ok.");
        break;
    default:
        break;
  }
}
/**********************************************************
 * @brief updateBin 更新固件
 * 
 * @param ota_url 固件地址
 *
 * @return t_httpUpdate_return
 ***********************************************************/
t_httpUpdate_return HttpOTA::_updateBin(String ota_url)
{
  t_httpUpdate_return ret ;
  WiFiClient UpdateClient;
  httpUpdate.onStart(_update_started);     //当升级开始时
  httpUpdate.onEnd(_update_finished);      //当升级结束时
  httpUpdate.onProgress(_update_progress); //当升级中
  httpUpdate.onError(_update_error);       //当升级失败时
  Serial.println(ota_url);
  ret = httpUpdate.update(UpdateClient, ota_url);
  return ret;
}
/**********************************************************
 * @brief update started 升级开始
 * 
 ***********************************************************/
void HttpOTA::_update_started(void)
{
  log_i("Update started");
}
/**********************************************************
 * @brief update finished 升级结束
 * 
 ***********************************************************/
void HttpOTA::_update_finished(void)
{
  log_i("Update finished");
}
/**********************************************************
 * @brief update progress 升级进度
 * 
 * @param cur 
 * @param total 
 ***********************************************************/
void HttpOTA::_update_progress(int cur, int total)
{  
  log_i("Process at %d of %d bytes[%.1f%%]...\n", cur, total, cur * 100.0 / total);
}
/**********************************************************
 * @brief update error 升级失败
 * 
 ***********************************************************/
void HttpOTA::_update_error(int err)
{
  log_e("Error[%d] updating firmware!", err);
}