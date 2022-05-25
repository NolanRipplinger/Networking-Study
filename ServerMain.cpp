#include <iostream>
#include <string>
#include <sstream>
#include "NolanNetworking.h"

const int MAX_BUFFER_LENGTH = 4096, //Tutorial default was 4096
PORT = 1738,
MAX_DATE_LENGTH = 26;


int main() {

	char buf[MAX_BUFFER_LENGTH],
		timeBuffer[MAX_DATE_LENGTH];
	std::ostringstream ss;

	/* Initialize Winsock
	*
	*/
	WSADATA wsaData;
	WORD ver = MAKEWORD(2, 2);

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

	ZeroMemory(host, NI_MAXHOST);		//target, size
	ZeroMemory(service, NI_MAXSERV);	//target, size

	//calculate dateTime
	ZeroMemory(timeBuffer, MAX_DATE_LENGTH); //Zero buffer before using
	time_t result = time(NULL); //setting up time stuff
	ctime_s(timeBuffer, sizeof(timeBuffer), &result);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		ss << host << " has connected on port " << service << " - " << timeBuffer;
		std::cout << ss.str();
	}
	else //get ip of host if can't find name
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		ss << host << ": connected on port " << service << " - " << timeBuffer;
		std::cout << ss.str();
	}

	/* Close listening Socket
	*
	*/
	closesocket(listening);


	/* While: acceptand echo message back to cleitn
	*
	*/
	while (true)//Run server until disconnect
	{
		ZeroMemory(buf, MAX_BUFFER_LENGTH);
		ZeroMemory(timeBuffer, MAX_DATE_LENGTH); //Zero buffer before using
		ss.str("");
		ss.clear();


		//Wait for client to send data
		int bytesReceived = recv(clientSocket, buf, MAX_BUFFER_LENGTH, 0);

		result = time(NULL); //setting up time stuff
		ctime_s(timeBuffer, sizeof(timeBuffer), &result);

		if (bytesReceived == SOCKET_ERROR)
		{

			std::cerr << "Error in recv(), exiting" << std::endl;
			break;
		}

		if (bytesReceived == 0)
		{
			ss << host << " has disconnected - " << timeBuffer << std::endl;
			std::cout << ss.str();
			break;
		}

		/* Process Input
		* 
		* This will output the client's input
		* unless it's a newline (\n) or carriage return (\r)
		*/
		if (buf[0] != '\r' && buf[0] != '\n') //Doesn't print newline server side 
		{
			ss << host << " says: \"" << buf << "\" - " << timeBuffer;
			std::cout << ss.str();
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