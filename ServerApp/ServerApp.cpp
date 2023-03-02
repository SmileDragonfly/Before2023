// ServerApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
    std::cout << "Starting server...\n";
	// Server side step:
	// 1.WSAStartup
	// 2.Create soket
	// 3.Bind socket
	// 4.Listenning
	// 5.Accept
	// 6.Communication client - server.......
	// 7.WSACleanUp

	// 1. WSAStartup: Load winsock library
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// 2. Create socket
	// 2.1. AF_INET: IPv4
	// 2.2. SOCK_STREAM: TCP/IP
	//		SOCK_DGRAM: UDP/IP
	// 2.3. IPPROTO_TCP: TCP
	//		IPPROTO_UDP: UDP
	SOCKET udpSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// 3. Bind socket
	SOCKADDR_IN udpAddress;
	int port = 9900;
	udpAddress.sin_family = AF_INET;
	udpAddress.sin_port = htons(port);
	udpAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	sizeof(USHORT);
	sizeof(IN_ADDR);
	int iResult = bind(udpSocket, (SOCKADDR*)&udpAddress, sizeof(udpAddress));

	if (iResult == SOCKET_ERROR) {
		std::cout << "bind failed with error: " << WSAGetLastError() << std::endl;
		closesocket(udpSocket);
		WSACleanup();
		return 1;
	}

	// 4. Listenning
	if (listen(udpSocket, 5) == SOCKET_ERROR)
	{
		std::cout << "socket_error" << std::endl;
		int errorIndex = WSAGetLastError();
		std::cout << "error_index = " << errorIndex << std::endl;
	}

	// 5. Accept a new connection when one arrives
	SOCKADDR_IN ClientAddress;
	int clientAddressLength = sizeof(SOCKADDR_IN);
	SOCKET newUdpSocket;

	while (1)
	{
		newUdpSocket = accept(udpSocket, (SOCKADDR*)&ClientAddress, &clientAddressLength);
		if (newUdpSocket == INVALID_SOCKET)
		{
			std::cout << "accept failed with error: " << WSAGetLastError() << std::endl;
			continue;
		}
		std::cout << "Client connected" << std::endl;
		break;
	}
	
	// 6.Communication Server-Client
	while (1)
	{
		char buff[100];
		int iRet = recv(newUdpSocket, (char*)buff, sizeof(buff), 0);
		if (iRet == SOCKET_ERROR)
		{
			std::cout << "Server cant receive data" << std::endl;
			continue;
		}
		buff[iRet] = '\0';
		std::cout << buff << std::endl;
	}

	// Close socket
	closesocket(newUdpSocket);
	closesocket(udpSocket);

	// 7. WsaCleanUp
	WSACleanup();
}
