#include "main.h"

int message_change_number = 0;

//################ Overriding Private onmessage Function ######################
// Sending 'successful' message  when there is messge from client side
// on private channel
void MyWebsocketCPP::PrivateOnmessage(
    std::shared_ptr<WsServer::Connection> connection, std::string msg) {
  // std::cout << "[Override Func][Private][Message Recieved] " << msg
  //           << std::endl;
  if (msg == "Hand Shake") {
    this->PrivateSend(connection, "connection established successfully");
  } else {
    this->PrivateSend(connection, "successful");
  }
}

//############### Overriding Broadcast onmessage Function ######################
// Changing the broadcast message when there is message from client side
// on broadcast channel
void MyWebsocketCPP::BroadcastOnmessage(
    std::shared_ptr<WsServer::Connection> connection, std::string msg) {
  // std::cout << "[Override Func][Broadcast][Message Recieved] " << msg
  //           << std::endl;
}

void *MyWebsocketCPP::broadcast_msg(void *c) {
  while (((MyWebsocketCPP *)c)->update_broadcast_messages_status) {
    message_change_number++;
    std::string set_message =
        "Message No: " + std::to_string(message_change_number);
    ((MyWebsocketCPP *)c)->SetBroadcastMessage(set_message);
    usleep(1000000);
    // std::cout << "BM No: " << message_change_number << std::endl;
  }
}

void MyWebsocketCPP::update_broadcast_messages() {
  std::cout << "[INFO] Started Thread to update broadcast messages"
            << std::endl;
  pthread_create(&broadcast_msg_thread, NULL, MyWebsocketCPP::broadcast_msg,
                 this);
}

int main(int argc, char const *argv[]) {
  // Create obj with server setup and start server
  MyWebsocketCPP wa(8000, true, true, 1000);
  wa.StartServer();

  // For NO terminal outputs of message logs from api
  wa.terminal_ouputs = false;

  // changing broadcast message
  sleep(1);
  wa.SetBroadcastMessage("Changed Message");
  wa.update_broadcast_messages();

  pthread_join(wa.server_thread, NULL);
  pthread_join(wa.broadcast_msg_thread, NULL);

  return 0;
}
