#include <stdio.h>
#include <iostream>
#include "NolanNetworking.h"



int main() {

	/* Initialize Winsock
	* 
	*/
	WSADATA wsaData;
	WORD ver = MAKEWORD(2,2);

	int wsock = WSAStartup(ver, &wsaData);
	if (wsock != 0) 
	{
		std::cerr << "Can't initialize winsock, quitting" << std::endl;
		return 1;
	}


	/* Create socket
	* 
	* AF:
	*	AF_INET:	IPV4
	*	AF_INET6:	IPV6
	*	AF_BTH:		Bluetooth
	* 
	* Type:
	*	SOCK_STREAM:	TCP
	*			TCP is a "connection-oriented" protocol, in which
	*			data can be transmitted in both directions.
	*
	*	SOCK_DGRAM:		UDP
	*			UDP is a "connectionless" protocol with limited error checking
	*			and recovery, less overhead; Data sent continuously regardless
	*			of if there is a program listening
	* 
	*	SOCK_RAW:		Raw Socket
	*			Raw sockets contain no header; carries a payload
	* 
	* Protocol:
	*	0:
	* 
	*/
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket, exiting" << std::endl;
		return 2;
	}


	/* Bind the socket to ip and port
	*
	*/
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(1738);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; //Could also use inet_pton and bind to local loopback address

	bind(listening, (sockaddr*)&hint, sizeof(hint));


	/* Tell winsock that socket is for listening
	*
	*/ 
	listen(listening, SOMAXCONN);//max connections



	/* Wait for a connection
	*
	*/ 
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientsocket = accept(listening, (sockaddr*)&client, &clientSize);

	/* Close listening Socket
	*
	*/ 


	/* While: acceptand echo message back to cleitn
	*
	*/

	
	/* Close socket
	*
	*/ 

	/* Shutdown winsock
	*
	*/ 

	return 0;
}