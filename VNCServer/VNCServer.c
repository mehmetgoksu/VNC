#include "VNCServer.h"

void CALLBACK f(HWND hwnd, UINT uMsg, UINT timerId, DWORD dwTime)
{
	BYTE	*bytes;
	BYTE	*compressed;
	LONG	len;
	LONG	compressedLen;

	UNREFERENCED_PARAMETER(hwnd);
	UNREFERENCED_PARAMETER(dwTime);
	UNREFERENCED_PARAMETER(timerId);
	UNREFERENCED_PARAMETER(uMsg);
	bytes = GetScreenBitmap(&len);
	compressed = CompressData(bytes, len, &compressedLen);
	GlobalFree(bytes);
	/*
		Current protocol (to improve ^^):
		   
		Step 1) We send the compressed bitmap size
		Step 2) We send the compressed bitmap 
	*/
	WriteInt32ToSocket(cfg.ClientSocket, (int)compressedLen);
	WriteBytesToSocket(cfg.ClientSocket, compressed, compressedLen);
	free(compressed);
}

int WINAPI			WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASSEX		wClass;
	HWND			hWnd;
	MSG				msg;

	// eliminate compiler warning when a var / reference will never be used
	UNREFERENCED_PARAMETER(hPrevInst);
	UNREFERENCED_PARAMETER(lpCmdLine);
	initConfig(&cfg); // cfg is a global struct
	SecureZeroMemory(&msg, sizeof(msg));
	SecureZeroMemory(&wClass, sizeof(wClass));
	wClass.cbSize = sizeof(wClass);
	wClass.hbrBackground = (HBRUSH)COLOR_GRADIENTINACTIVECAPTION;
	wClass.hInstance = hInst;
	wClass.lpfnWndProc = (WNDPROC)WinProc;
	wClass.lpszClassName = "Window Class";
	wClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx(&wClass);
	hWnd = CreateWindowEx(0,
						wClass.lpszClassName,
						"My VNC Server",
						WS_OVERLAPPEDWINDOW,
						0,
						0,
						320,
						240,
						NULL,
						NULL,
						hInst,
						NULL);
	ShowWindow(hWnd, nShowCmd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (EXIT_SUCCESS);
}

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
			switch(LOWORD(wParam))
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
							SetTimer(hWnd, TIMER_ID, TIMER_INTERVAL_MS,(TIMERPROC) &f);
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
