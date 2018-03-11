#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <deque>
#include <iostream>
#include "client_ws.hpp"
#include "server_ws.hpp"

#define DEFAULT_PORT 8000
#define DEFAULT_THREADS 1
#define DEFAULT_DELAY_IN_MICRO_SECS 500000

typedef SimpleWeb::SocketServer<SimpleWeb::WS> WsServer;

struct _connection {
  std::shared_ptr<WsServer::Connection> connection;
  bool status;
};

class WebsocketCPP {
 public:
  WebsocketCPP(
      unsigned int port = DEFAULT_PORT, bool require_broadcast = true,
      bool require_private = true,
      unsigned int broadcast_delay_in_micro_sec = DEFAULT_DELAY_IN_MICRO_SECS,
      size_t num_threads = DEFAULT_THREADS, size_t timeout_request = 5,
      size_t timeout_idle = 0)
      : server(DEFAULT_PORT, num_threads, timeout_request, timeout_idle) {
    this->port = port;
    this->broadcast_delay_in_micro_sec = broadcast_delay_in_micro_sec;
    this->require_broadcast = require_broadcast;
    this->require_private = require_private;
    if (port != DEFAULT_PORT) {
      this->server.config.port = port;
    }
    this->terminal_ouputs = true;
    this->broadcast_data_status = true;
    this->broadcast_msg = "MESSAGE NOT SET";
    this->exit_broadcast = false;
    this->exit_broadcast_status = false;
    this->server_endpoints_init_status = false;
    this->set_message_lock = false;
    this->is_message_updated = true;
  }

  WsServer server;
  void StartServer();
  void StopServer();

  // variables to control
  int port;
  int broadcast_delay_in_micro_sec;
  std::string broadcast_msg;
  std::deque<_connection> bc_conn_list;
  std::deque<_connection> p_conn_list;
  bool terminal_ouputs;
  bool require_broadcast;
  bool require_private;
  bool broadcast_data_status;
  bool exit_broadcast;
  bool exit_broadcast_status;
  bool server_endpoints_init_status;
  bool set_message_lock;
  bool is_message_updated;

  // functions
  void BroadcastEndpoint();
  //.... override these fucntions based on requirement.........
  virtual void BroadcastOnmessage(
      std::shared_ptr<WsServer::Connection> connection, std::string msg);
  virtual void BroadcastOnopen(
      std::shared_ptr<WsServer::Connection> connection);
  virtual void BroadcastOnclose(
      std::shared_ptr<WsServer::Connection> connection);
  virtual void BroadcastOnerror(
      std::shared_ptr<WsServer::Connection> connection);
  //......................................................
  void StopBroadcast();
  void StartBroadcast();
  void ExitBroadcast();
  void SetBroadcastMessage(std::string msg);
  void SendBroadcastMessage();

  void PrivateEndpoint();
  //........ override these fucntions based on requirement ........
  virtual void PrivateOnmessage(
      std::shared_ptr<WsServer::Connection> connection, std::string msg);
  virtual void PrivateOnopen(std::shared_ptr<WsServer::Connection> connection);
  virtual void PrivateOnclose(std::shared_ptr<WsServer::Connection> connection);
  virtual void PrivateOnerror(std::shared_ptr<WsServer::Connection> connection);
  //...............................................................
  void PrivateSend(std::shared_ptr<WsServer::Connection> connection,
                   std::string msg);

  pthread_t broadcast_thread;
  pthread_t server_thread;
  static void *broadcast_data(void *c);
  static void *server_handler(void *c);

  void rm_connection(std::shared_ptr<WsServer::Connection> connection,
                     std::deque<_connection> &list);
  void add_connection(std::shared_ptr<WsServer::Connection> connection,
                      std::deque<_connection> &list);
  size_t GetConnectionID(std::shared_ptr<WsServer::Connection> connection);

 private:
  void SendBroadcastMessage2Client(_connection client);
};

#endif  // TEST_H
