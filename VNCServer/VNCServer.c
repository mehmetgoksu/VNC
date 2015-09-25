#include "VNCServer.h"

int WINAPI			WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASSEX		wClass;
	HWND			hWnd;
	MSG				msg;

	UNREFERENCED_PARAMETER(hPrevInst);
	UNREFERENCED_PARAMETER(lpCmdLine);
	initConfig(&cfg);
	SecureZeroMemory(&msg, sizeof(msg));
	SecureZeroMemory(&wClass, sizeof(wClass));
	InitWindowClass(&wClass, hInst, hWnd, msg);
	RegisterClassEx(&wClass);
	hWnd = InitHWnd(wClass, hInst);
	ShowWindow(hWnd, nShowCmd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (EXIT_SUCCESS);
}