#include "pch.h"
#include "ServerThread.h"
#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib")

BOOL CServerThread::InitServerThread(INT nPort, TPROTOCOL protocol)
{
	BOOL bRet = FALSE;
	m_nPort = nPort;
	m_nProtocol = protocol;

	WSADATA wsaData;
	INT err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0)
	{
		AfxMessageBox(L"WSAStartup failed with error: %d", err);
		bRet = FALSE;
	}

	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET)
	{
		AfxMessageBox(L"socket function failed with error = %d", WSAGetLastError());
		WSACleanup();
		bRet = FALSE;
	}

	SOCKADDR_IN sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sockAddr.sin_port = htons(m_nPort);
	err = bind(listenSocket, (SOCKADDR*) &sockAddr, sizeof(sockAddr));
	if (err == SOCKET_ERROR)
	{
		AfxMessageBox(L"bind failed with error %u", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		bRet = FALSE;
	}

	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		AfxMessageBox(L"listen function failed with error: %d", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		bRet = FALSE;
	}
	else
	{
		if (WSAAsyncSelect(listenSocket, AfxGetApp()->GetMainWnd()->m_hWnd, WM_SOCKET, FD_ACCEPT | FD_CLOSE) != 0)
		{
			AfxMessageBox(L"WSAAsyncSelect function failed with error: %d", WSAGetLastError());
			closesocket(listenSocket);
			WSACleanup();
			bRet = FALSE;
		}
		else
		{
			bRet = TRUE;
		}
	}

	return bRet;
}
