#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <iostream>
#include "../../../include/websocket.h"

class MyWebsocketCPP : public WebsocketCPP {
 public:
  MyWebsocketCPP(unsigned int port = 8000, bool require_broadcast = true,
                 bool require_private = true,
                 unsigned int broadcast_delay_in_micro_sec = 500000)
      : WebsocketCPP(port, require_broadcast, require_private,
                     broadcast_delay_in_micro_sec) {
    update_broadcast_messages_status = true;
  }
  void BroadcastOnmessage(std::shared_ptr<WsServer::Connection> connection,
                          std::string msg);

  void PrivateOnmessage(std::shared_ptr<WsServer::Connection> connection,
                        std::string msg);
  pthread_t broadcast_msg_thread;
  static void *broadcast_msg(void *c);
  void update_broadcast_messages();

 private:
  bool update_broadcast_messages_status;
};

#endif  // TEST_H
