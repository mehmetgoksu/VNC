#include "VNCServer.h"

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
						"VNC Server",
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