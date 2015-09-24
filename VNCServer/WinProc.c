#include "VNCServer.h"

LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		cfg.Socket = CreateServerSocketWithAsyncReading(VNC_PORT, hWnd, WM_SOCKET);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		default:
			break;
		}
		break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		if (cfg.ClientSocket != 0)
			closesocket(cfg.ClientSocket);
		CloseSocketAndCleanup(cfg.Socket);
		return (0);
	}
	break;
	case WM_SOCKET:
	{
		if (WSAGETSELECTERROR(lParam))
		{
			if (cfg.ClientSocket != 0)
			{
				KillTimer(hWnd, TIMER_ID);
				closesocket(cfg.ClientSocket);
				cfg.ClientSocket = 0;
			}
			break;
		}
		switch (WSAGETSELECTEVENT(lParam))
		{
		case FD_READ:
		{
		}
		break;
		case FD_CLOSE:
		{
			closesocket((SOCKET)wParam);
		}
		break;
		case FD_WRITE:
		{
		}
		break;
		case FD_ACCEPT:
		{
			cfg.ClientSocket = AcceptClientConnection((SOCKET)wParam);
			SetTimer(hWnd, TIMER_ID, TIMER_INTERVAL_MS, (TIMERPROC)&FrameBuff);
		}
		break;
		default:
			break;
		}
	}
	default:
		break;
	}
	return (DefWindowProc(hWnd, msg, wParam, lParam));
}
