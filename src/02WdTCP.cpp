#include "02WdTCP.h"
#include <AsyncTCP.h>
/**********************************************************
 * @brief static variables declaration
 * 
 ***********************************************************/
static AsyncClient* tcp_client = new AsyncClient;
static AsyncClient* esp_client = new AsyncClient;
static bool asClient_isConnected ;
static bool asServer_hasClient;
/**********************************************************
 * @brief Construct a new Wd T C P:: Wd T C P object
 * 
 ***********************************************************/
WdTCP::WdTCP(){
#ifdef RomotePort
  _remote_port = RomotePort;
#else
  _remote_port = 1234;
#endif
#ifdef RemoteIP
  if(_remote_ip.fromString(RemoteIP) == false){
    _remote_ip = IPAddress(192, 168, 1, 1);
  }  
#else
  _remote_ip = IPAddress(192, 168, 1, 1);
#endif
#ifdef LocalPort
  _local_port = LocalPort;
#else
  _local_port = 1234;
#endif
}
/**********************************************************
 * @brief as_client_begin
 * 
 * @param ip 
 * @param port 
 ***********************************************************/
void WdTCP::as_client_begin(IPAddress ip, uint16_t port)
{  
  log_i("client begin %s on port %d \n",ip.toString().c_str(), port);
  _esp32_as_client= new AsyncClient;
  _esp32_as_client->onData(&asClient_OnData, this);
	_esp32_as_client->onConnect(&asClient_OnConnected, _esp32_as_client);
  _esp32_as_client->onDisconnect(&asClient_OnDisconnected, _esp32_as_client);
	_esp32_as_client->connect(serverIP, port);
  _esp32_as_client->close();
}

void WdTCP::as_client_begin(void)
{  
  as_client_begin(_remote_ip, _remote_port);
}
/**********************************************************
 * @brief as_server_begin
 * 
 * @param port 
 ***********************************************************/
void WdTCP::as_server_begin(uint16_t port)
{
  log_i("server begin on port %d \n", port);
  _esp32_as_server = new AsyncServer(port);
  _esp32_as_server->onClient(&asServer_OnClient, this);
  _esp32_as_server->begin();
}

void WdTCP::as_server_begin(void)
{
  as_server_begin(_local_port);
}
/**********************************************************
 * @brief asyncClient OnConnected
 * 
 ***********************************************************/
void WdTCP::asClient_OnConnected(void *arg, AsyncClient *client)
{  
  log_i("client has been connected to %s on port %d \n", "HDDAC", REMOTE_TCP_SERVER_PORT); 
}
/**********************************************************
 * @brief asyncClient OnDisconnected
 * 
 ***********************************************************/
void WdTCP::asClient_OnDisconnected(void *arg, AsyncClient *client)
{
  log_w("client has been disconnected from %s on port %d \n", "HDDAC", REMOTE_TCP_SERVER_PORT);
  esp_client->free();
  esp_client->close();
  client->connect(serverIP, REMOTE_TCP_SERVER_PORT);
  client->close();
}
/**********************************************************
 * @brief asyncClient OnData
 * 
 ***********************************************************/
//包装函数，用于调用非静态函数
void WdTCP::asClient_OnData(void *arg, AsyncClient *client, void *data, size_t len)
{
  WdTCP *instance = static_cast<WdTCP*>(arg);
  if (instance) {
      instance->_asClientOnData(client, data, len);
  }else {
      log_e("Instance is null");
  }
}
//实际执行的非静态函数，用于处理接收到的数据
void WdTCP::_asClientOnData(AsyncClient* client, void* data, size_t len)
{
  if (!client) {
    log_e("Client is null");
    return;
  }
  if(len >= min_command_length)//if data is more than 10 bytes, it is a command
  {
    String data_from_server(static_cast<char*>(data)); 
    parseJsonString(data_from_server,client); // handle the command
    client->write(data_from_server.c_str());
  }
}
/**********************************************************
 * @brief asyncServer OnClient
 * 
 ***********************************************************/
void WdTCP::asServer_OnClient(void *arg, AsyncClient *client)
{
  WdTCP *instance = static_cast<WdTCP*>(arg);
  if (instance) {
      instance->_asServerOnClient(arg,client);
  }else {
      log_e("Instance is null");
  }
}
void WdTCP::_asServerOnClient(void *arg, AsyncClient *client)
{
  log_i("new client has been connected to server, ip: %s", client->remoteIP().toString().c_str());
  // asServer_hasClient = true;
  if (tcp_client->connected())
  {
    tcp_client->close(true);
  }
  tcp_client = client;
  tcp_client->onData(&asServer_OnData, this);
	tcp_client->onError(&asServer_OnError, NULL);
	tcp_client->onDisconnect(&asServer_OnDisconnected, NULL);
	tcp_client->onTimeout(&asServer_OnTimeout, NULL);
}
/**********************************************************
 * @brief asyncServer OnData
 * 
 ***********************************************************/
void WdTCP::asServer_OnData(void *arg, AsyncClient *client, void *data, size_t len)
{
  WdTCP *instance = static_cast<WdTCP*>(arg);
  if (instance) {
      instance->_asServerOnData(client, data, len);
  }else {
      log_e("Instance is null");
  }
}
void WdTCP::_asServerOnData(AsyncClient *client, void *data, size_t len)
{
  if (!client) {
    log_e("Client is null");
    return;
  }
  if (len >= min_command_length)
  {
    String data_from_client(static_cast<char*>(data));
    parseJsonString(data_from_client,client); // handle the command
  }  
}
/**********************************************************
 * @brief asyncServer OnError
 * 
 ***********************************************************/
void WdTCP::asServer_OnError(void *arg, AsyncClient *client, int8_t error)
{
  log_w("connection error %s from client %s \n", client->errorToString(error), client->remoteIP().toString().c_str());
}
/**********************************************************
 * @brief asyncServer OnTimeout
 *    
 * ***********************************************************/
void WdTCP::asServer_OnTimeout(void* arg, AsyncClient* client, uint32_t time)
{
  log_w("client ACK timeout ip: %s \n", client->remoteIP().toString().c_str());
}
/**********************************************************
 * @brief asyncServer OnDisconnected
 * 
 ***********************************************************/
void WdTCP::asServer_OnDisconnected(void *arg, AsyncClient *client)
{
  // asServer_hasClient = false;
  tcp_client->close(true);
  log_w("client %s disconnected \n", client->remoteIP().toString().c_str());
}
