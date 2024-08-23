#ifndef __HTTP_OTA_H__
#define __HTTP_OTA_H__
#include <HTTPUpdate.h>
class HttpOTA
{
  public:
    void httpOTA(String ota_url);
  private:
    String _ota_url;
  private:
    t_httpUpdate_return _updateBin(String ota_url);
    static void _update_started(void);
    static void _update_finished();
    static void _update_progress(int cur, int total);
    static void _update_error(int err);
};

#endif /* __F19_HTTP_OTA_H__ */
