#include "../include/websocket.h"

void WebsocketCPP::StartServer() {
  if (!(this->server_endpoints_init_status)) {
    if (this->require_broadcast) {
      this->BroadcastEndpoint();
    }
    if (this->require_private) {
      this->PrivateEndpoint();
    }
    this->server_endpoints_init_status = true;
  }
  pthread_create(&server_thread, NULL, WebsocketCPP::server_handler, this);
  pthread_create(&broadcast_thread, NULL, WebsocketCPP::broadcast_data, this);
}

void *WebsocketCPP::server_handler(void *c) {
  std::cout << "[WebsocketCPP] Initiated Server" << std::endl;
  ((WebsocketCPP *)c)->server.start();
}

void WebsocketCPP::StopServer() {
  this->ExitBroadcast();
  this->server.stop();
}

void WebsocketCPP::BroadcastEndpoint() {
  auto &bc = server.endpoint["^/broadcast/?$"];

  bc.onmessage = [&](std::shared_ptr<WsServer::Connection> connection,
                     std::shared_ptr<WsServer::Message> message) {
    std::string msg = message->string();
    this->BroadcastOnmessage(connection, msg);

    if (this->terminal_ouputs) {
      std::cout << "[Broadcast] Message Recieved: " << msg
                << " from: " << (size_t)connection.get()
                << " Connections Count: " << this->bc_conn_list.size()
                << std::endl;
    }
  };

  bc.onopen = [&](std::shared_ptr<WsServer::Connection> connection) {
    this->BroadcastOnopen(connection);
    this->add_connection(connection, this->bc_conn_list);
    if (this->terminal_ouputs) {
      std::cout << "[Broadcast] Server: Opened connection "
                << (size_t)connection.get()
                << " Connections Count: " << this->bc_conn_list.size()
                << std::endl;
    }
  };

  bc.onclose = [&](std::shared_ptr<WsServer::Connection> connection, int status,
                   const std::string & /*reason*/) {
    this->BroadcastOnclose(connection);
    this->rm_connection(connection, this->bc_conn_list);
    if (this->terminal_ouputs) {
      std::cout << "[Broadcast] Server: Closed connection "
                << (size_t)connection.get() << " with status code " << status
                << " Connections Count: " << this->bc_conn_list.size()
                << std::endl;
    }
  };

  bc.onerror = [&](std::shared_ptr<WsServer::Connection> connection,
                   const boost::system::error_code &ec) {
    this->BroadcastOnerror(connection);
    this->rm_connection(connection, this->bc_conn_list);
    if (this->terminal_ouputs) {
      std::cout << "[Broadcast] Server: Error in connection "
                << (size_t)connection.get() << ". "
                << "Error: " << ec << ", error message: " << ec.message()
                << std::endl;
    }
  };
  std::cout << "[WebsocketCPP] Broadcast Endpoint Setup Completed" << std::endl;
}

void *WebsocketCPP::broadcast_data(void *c) {
  std::cout << "[WebsocketCPP] Started broadcasting messages" << std::endl;
  while (!((WebsocketCPP *)c)->exit_broadcast) {
    if (((WebsocketCPP *)c)->broadcast_data_status) {
      ((WebsocketCPP *)c)->set_message_lock = true;
      ((WebsocketCPP *)c)->SendBroadcastMessage();
      ((WebsocketCPP *)c)->set_message_lock = false;
    }
    usleep(((WebsocketCPP *)c)->broadcast_delay_in_micro_sec);
  }
  ((WebsocketCPP *)c)->exit_broadcast_status = true;
}

void WebsocketCPP::SendBroadcastMessage() {
  for (int i = 0; i < this->bc_conn_list.size(); i++) {
    while (!is_message_updated) {
    }
    this->set_message_lock = true;
    this->SendBroadcastMessage2Client(this->bc_conn_list.at(i));
    this->set_message_lock = false;
  }
}

void WebsocketCPP::SendBroadcastMessage2Client(_connection client) {
  auto message_str = this->broadcast_msg;
  auto send_stream = make_shared<WsServer::SendStream>();
  *send_stream << message_str;
  this->server.send(
      client.connection, send_stream, [](const boost::system::error_code &ec) {
        if (ec) {
          std::cout << "[Server Error][Broadcast] sending message. "
                    << "[Error] " << ec << ", [Error Message] " << ec.message()
                    << std::endl;
        }
      });
}

void WebsocketCPP::StartBroadcast() { this->broadcast_data_status = true; }
void WebsocketCPP::StopBroadcast() { this->broadcast_data_status = false; }
void WebsocketCPP::ExitBroadcast() {
  this->exit_broadcast = true;
  while (!(this->exit_broadcast_status)) {
    usleep(100000);
  }
  usleep(100000);
}
void WebsocketCPP::SetBroadcastMessage(std::string msg) {
  this->is_message_updated = false;
  while (this->set_message_lock) {
  }
  this->broadcast_msg = msg;
  this->is_message_updated = true;
}
void WebsocketCPP::BroadcastOnmessage(
    std::shared_ptr<WsServer::Connection> connection, std::string msg) {
  // overide the function to add a functionality
}
void WebsocketCPP::BroadcastOnopen(
    std::shared_ptr<WsServer::Connection> connection) {
  // overide the function to add a functionality
}
void WebsocketCPP::BroadcastOnclose(
    std::shared_ptr<WsServer::Connection> connection) {
  // overide the function to add a functionality
}
void WebsocketCPP::BroadcastOnerror(
    std::shared_ptr<WsServer::Connection> connection) {
  // overide the function to add a functionality
}

void WebsocketCPP::PrivateEndpoint() {
  auto &p = this->server.endpoint["^/private/?$"];

  p.onmessage = [&](std::shared_ptr<WsServer::Connection> connection,
                    std::shared_ptr<WsServer::Message> message) {

    std::string msg = message->string();
    this->PrivateOnmessage(connection, msg);

    if (this->terminal_ouputs) {
      std::cout << "[Private] Message Recieved: " << msg
                << " from: " << (size_t)connection.get()
                << " Connections Count: " << this->p_conn_list.size()
                << std::endl;
    }
  };

  p.onopen = [&](std::shared_ptr<WsServer::Connection> connection) {
    this->PrivateOnopen(connection);
    this->add_connection(connection, this->p_conn_list);
    if (this->terminal_ouputs) {
      std::cout << "[Private] Server: Opened connection "
                << (size_t)connection.get()
                << " Connections Count: " << this->p_conn_list.size()
                << std::endl;
    }
  };

  p.onclose = [&](std::shared_ptr<WsServer::Connection> connection, int status,
                  const std::string & /*reason*/) {
    this->PrivateOnclose(connection);
    this->rm_connection(connection, this->p_conn_list);
    if (this->terminal_ouputs) {
      std::cout << "[Private] Server: Closed connection "
                << (size_t)connection.get() << " with status code " << status
                << " Connections Count: " << this->p_conn_list.size()
                << std::endl;
    }
  };

  p.onerror = [&](std::shared_ptr<WsServer::Connection> connection,
                  const boost::system::error_code &ec) {
    this->PrivateOnerror(connection);
    this->rm_connection(connection, this->p_conn_list);
    if (this->terminal_ouputs) {
      std::cout << "[Private] Server: Error in connection "
                << (size_t)connection.get() << ". "
                << "Error: " << ec << ", error message: " << ec.message()
                << std::endl;
    }
  };
  std::cout << "[WebsocketCPP] Private Endpoint Setup Completed" << std::endl;
}

void WebsocketCPP::PrivateOnmessage(
    std::shared_ptr<WsServer::Connection> connection, std::string msg) {
  // overide the function to add a functionality
}
void WebsocketCPP::PrivateOnopen(
    std::shared_ptr<WsServer::Connection> connection) {
  // overide the function to add a functionality
}
void WebsocketCPP::PrivateOnclose(
    std::shared_ptr<WsServer::Connection> connection) {
  // overide the function to add a functionality
}
void WebsocketCPP::PrivateOnerror(
    std::shared_ptr<WsServer::Connection> connection) {
  // overide the function to add a functionality
}

void WebsocketCPP::PrivateSend(std::shared_ptr<WsServer::Connection> connection,
                               std::string msg) {
  auto message_str = msg;

  auto send_stream = make_shared<WsServer::SendStream>();
  *send_stream << message_str;

  server.send(connection, send_stream, [](const boost::system::error_code &ec) {
    if (ec) {
      std::cout << "[Server Error][Private] sending message. "
                << "[Error] " << ec << ", [Error Message] " << ec.message()
                << std::endl;
    }
  });
}

void WebsocketCPP::rm_connection(
    std::shared_ptr<WsServer::Connection> connection,
    std::deque<_connection> &list) {
  _connection conn;
  conn.connection = connection;
  conn.status = false;
  int conn_location = 0;
  for (int i = 0; i < list.size(); i++) {
    _connection tmp;
    tmp = list.at(i);
    conn_location = i;
    if (tmp.connection == conn.connection) {
      break;
    }
  }
  if (list.size() > 0) {
    list.erase(list.begin() + conn_location);
  }
}

void WebsocketCPP::add_connection(
    std::shared_ptr<WsServer::Connection> connection,
    std::deque<_connection> &list) {
  _connection conn;
  conn.connection = connection;
  conn.status = true;
  list.push_back(conn);
}

size_t GetConnectionID(std::shared_ptr<WsServer::Connection> connection) {
  return (size_t)connection.get();
}
