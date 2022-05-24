#include <stdio.h>
#include <iostream>
#include "NolanNetworking.h"

const int MAX_BUFFER_LENGTH = 4096; //Tutorial default was 4096
const int PORT = 1738; 


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
	hint.sin_port = htons(PORT);
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

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
	if (clientSocket == INVALID_SOCKET) 
	{
		std::cerr << "Can't accept socket, exiting" << std::endl;
		return 3;
	}

	char host[NI_MAXHOST];		//Client's remote name
	char service[NI_MAXSERV];	//Service (port) being connected on

	ZeroMemory(host, NI_MAXHOST);	//target, size
	ZeroMemory(service, NI_MAXHOST);	//target, size

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) 
	{
		std::cout << host << ": connected on port " << service << std::endl;
	}
	else //get ip of host if can't find name
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << ": connected on port " << service << std::endl;
	}

	/* Close listening Socket
	*
	*/ 
	closesocket(listening);


	/* While: acceptand echo message back to cleitn
	*
	*/
	char buf[MAX_BUFFER_LENGTH];

	while (true)//Run server forever 
	{
		ZeroMemory(buf, MAX_BUFFER_LENGTH);

		//Wait for client to send data
		int bytesReceived = recv(clientSocket, buf, MAX_BUFFER_LENGTH, 0);
		if (bytesReceived == SOCKET_ERROR) 
		{
			std::cerr << "Error in recv(), exiting" << std::endl;
			break;
		}

		if (bytesReceived == 0) 
		{
			std::cout << "client disconnected" << std::endl;
			break;
		}

		//Echo back to client
		send(clientSocket, buf, bytesReceived + 1, 0);//add for terminating string

	}

	
	/* Close socket
	*
	*/ 
	closesocket(clientSocket);

	/* Clean up winsock
	*
	*/
	WSACleanup();

	return 0;
}
