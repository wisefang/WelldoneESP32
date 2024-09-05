#ifndef __02WD_TCP_H__
#define __02WD_TCP_H__
#include <Arduino.h>
#include <AsyncTCP.h>
#include "03WdOTA.h"


const static IPAddress serverIP(192,168,16,222);
const static int LOCAL_TCP_SERVER_PORT = 1234;
const static int REMOTE_TCP_SERVER_PORT = 1234;
const size_t min_command_length = 4;
class WdTCP: public WdOTA
{
  public:
    WdTCP();
    void as_client_begin(IPAddress ip, uint16_t port);
    void as_client_begin(void);
    void as_server_begin(uint16_t port);
    void as_server_begin(void);
  private:
    AsyncClient* _esp32_as_client;
    AsyncServer* _esp32_as_server;
    TaskHandle_t _taskHandle = NULL;
    uint16_t _local_port;
    uint16_t _remote_port;   
    IPAddress _remote_ip;
  private:
    static void asClient_OnConnected(void* arg, AsyncClient* client);
    static void asClient_OnDisconnected(void* arg, AsyncClient* client);
    static void asClient_OnData(void* arg, AsyncClient* client, void* data, size_t len);
    void _asClientOnData(AsyncClient* client, void* data, size_t len);
    static void asServer_OnClient(void *arg, AsyncClient *client);
    void _asServerOnClient(void *arg, AsyncClient *client);
    static void asServer_OnData(void* arg, AsyncClient* client, void *data, size_t len);
    void _asServerOnData(AsyncClient* client, void *data, size_t len);
    static void asServer_OnError(void* arg, AsyncClient* client, int8_t error);
    static void asServer_OnDisconnected(void* arg, AsyncClient* client);
    static void asServer_OnTimeout(void* arg, AsyncClient* client, uint32_t time);
};

#endif /* __WD_TCP_H__ */
