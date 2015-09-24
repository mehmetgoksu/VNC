#ifndef __VNCSERVER_H__
# define __VNCSERVER_H__

#pragma comment(lib,"ws2_32.lib")

# define WM_SOCKET			(104)
# define VNC_PORT			(5555)
# define TIMER_ID			(42)
# define TIMER_INTERVAL_MS	(300)

LRESULT CALLBACK WinProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

#endif