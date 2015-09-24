#ifndef __VNCCLIENT_H__
# define __VNCCLIENT_H__

#pragma comment(lib,"ws2_32.lib")

# define WM_SOCKET					(104)
# define VNC_PORT					(5555)
# define SOCKET_READER_CACHE_SIZE	(4 * MEGABYTE)
# define BITMAP_BUFFER_SIZE			(10 * MEGABYTE)

LRESULT CALLBACK WinProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

#endif