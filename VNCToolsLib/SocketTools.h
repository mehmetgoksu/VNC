#ifndef __SOCKETTOOLS_H__
# define __SOCKETTOOLS_H__

SOCKET	CreateClientSocketWithAsyncReading(const char *host_name, u_short port, HWND hWnd, u_int msg);
SOCKET	CreateServerSocketWithAsyncReading(u_short port, HWND hWnd, u_int msg);
SOCKET	AcceptClientConnection(SOCKET s);
void	CloseSocketAndCleanup(SOCKET Socket);
void	ReadBytesFromSocket(SOCKET Socket, BYTE *buf, int *pos, int toRead);
int		ReadInt32FromSocket(SOCKET Socket, int *val);
void	WriteBytesToSocket(SOCKET Socket, BYTE *buf, LONG toWrite);
int		WriteInt32ToSocket(SOCKET Socket, int val);

#endif