# WebSocket C++ Library

WebSocket is used to start server and client communications in C++ Programs. It uses RFC6455 - WebSocket Protocol.

## Features

	* Easy Setup
	* Flexible and Customizable
	* Cross-platform compatable
	* Supports secure WebSockets (TLS), IPv6, and explicit proxies.
	* Easy BroadCasting to all connected clients
	* Private and Secure Channel

## Details

Library provides WebsocketCPP class which can be inherited and can over-ride functions to perform the required tasks.

	* Can start and stop server 
	* Can set the port of server
	* Can enable and disable creation of broadcast and private channels.
	* Can Over-ride - Onopen, Onmessage, Onerror, Onclose methods
	* Can enable and disable terminal ouputs
	* Can change the broadcast message using function call

the below functions are used to control the servwer

	* StartServer()
	* StopServer()

### Broadcast Channel

The Channel is used to send same data to all clients connected to server, The below functions need to be over-ride to define its functionality

	* BroadcastOnopen(..) - called when the connection to server is established from a client
	* BroadcastOnmessage(..) - called when message received from any client
	* BroadcastOnclose(..) - called when the connection is closed by any client (doesn't effect broadcasting to other clients)
	* BroadcastOnerror(..) - called when there is an error (doesn't effect broadcasting to other clients)

the below functions are used to control broadcasting channel

	* StartBroadcast() - To start broadcasting ( by default - if broadcast channel is enabled it starts broadcasting)
	* StopBroadcast() - To stop broadcasting
	* ExitBroadcast() - To exit the broadcasting channel
	* SetBroadcastMessage(..) - To set broadcasting message

### Private Channel

The Channel is used to communicate to client connected to server privately, the below functions need to be over-ride to define its functionality

	* PrivateOnopen(..)- called when the connection to server is established from a client
	* PrivateOnmessage(..) - called when message received from client
	* PrivateOnclose(..)  - called when the connection is closed by the client
	* PrivateOnerror(..) - called when there is an error

the below functions are used to control private channel

	* PrivateSend(..) - To send the data to client

## Usage 

### Examples

	#### Basic Client(JS) Server(Cpp) Setup
	
	* Setup: *
	```
		cd <path>/Basic_Client_JS_Server_Cpp_Setup/Server/build/
		cmake ..
		make
	```
	* Run :*
	```
		cd <path>/Basic_Client_JS_Server_Cpp_Setup/Server/bin/
		sudo ./websocket
	```

## Upcoming Features

	* JSON message format
	* Logger support
	* Signal Handler