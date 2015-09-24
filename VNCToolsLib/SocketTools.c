#include <winsock2.h>
#include <Windows.h>
#include <stdlib.h>
#include "SocketTools.h"

/*
  TODO: Error testing
*/

#pragma comment(lib,"ws2_32.lib")

SOCKET					CreateClientSocketWithAsyncReading(const char *host_name, u_short port, HWND hWnd, u_int msg)
{
	struct hostent		*host;
	SOCKADDR_IN			SockAddr;
	WSADATA				WsaDat;
	SOCKET				Socket;

	WSAStartup(MAKEWORD(2,2), &WsaDat);
	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	WSAAsyncSelect(Socket, hWnd, msg, (FD_CLOSE | FD_READ));
	host = gethostbyname(host_name);			
	SockAddr.sin_port = htons(port);
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
	connect(Socket, (LPSOCKADDR)(&SockAddr), sizeof(SockAddr));
	return (Socket);
}

SOCKET					CreateServerSocketWithAsyncReading(u_short port, HWND hWnd, u_int msg)
{
	SOCKADDR_IN			SockAddr;
	WSADATA				WsaDat;
	SOCKET				Socket;

	WSAStartup(MAKEWORD(2,2), &WsaDat);
	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SockAddr.sin_port = htons(port);
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(Socket, (LPSOCKADDR)&SockAddr, sizeof(SockAddr));
	WSAAsyncSelect(Socket, hWnd, msg, (FD_CLOSE | FD_ACCEPT | FD_READ));
	listen(Socket, 1);
	return (Socket);
}

SOCKET				AcceptClientConnection(SOCKET s)
{
	SOCKET			Socket;
	SOCKADDR		client;
	int				size;

	size = sizeof(client);
	Socket = accept(s, &client, &size);
	return (Socket);
}

void					CloseSocketAndCleanup(SOCKET Socket)
{
	shutdown(Socket, SD_BOTH);
	closesocket(Socket);
	WSACleanup();
}

void					ReadBytesFromSocket(SOCKET Socket, BYTE *buf, int *pos, int toRead)
{
	int					inDataLength;

	while (*pos < toRead)
	{
		inDataLength = recv(Socket, ((char *)buf) + *pos, toRead - *pos, 0);
		if (inDataLength <= 0)
			break;
		*pos += inDataLength;
	}
}

int					ReadInt32FromSocket(SOCKET Socket, int *val)
{
	return (recv(Socket, (char *)val, sizeof(*val), 0));
}

void					WriteBytesToSocket(SOCKET Socket, BYTE *buf, LONG toWrite)
{
	LONG				sent;
	int					write;

	sent = 0;
	while (sent < toWrite)
	{
		write = send(Socket, ((char *)buf) + sent, toWrite - sent, 0);
		if (write <= 0)
			break;
		sent += write;
	}
}

int					WriteInt32ToSocket(SOCKET Socket, int val)
{
	return (send(Socket, (const char *)&val, sizeof(val), 0));
}
