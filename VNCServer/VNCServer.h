#ifndef __VNCSERVER_H__
# define __VNCSERVER_H__

// Libraries
#include <winsock2.h>
#include <windows.h>
#include "SocketTools.h"
#include "BMPTools.h"
#include "Win32Tools.h"
#include "CompressionTools.h"

// Windows standard stuff
#pragma comment(lib,"ws2_32.lib")

// Pre-processor macros
# define WM_SOCKET			(104)  // WM = Windows Message (com protocol)
# define VNC_PORT			(5555) // Data transfer
# define TIMER_ID			(42)   // PID
# define TIMER_INTERVAL_MS	(300)  // Refresh rate

// Globale
typedef struct			s_config
{
	SOCKET Socket;
	SOCKET ClientSocket;
}						t_config;

extern t_config cfg;


// Functions prototypes
LRESULT CALLBACK	WinProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
void CALLBACK		FrameBuff(HWND hwnd, UINT uMsg, UINT timerId, DWORD dwTime);
LRESULT CALLBACK	WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void				InitConfig(t_config *cfg);


#endif