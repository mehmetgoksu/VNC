#include "VNCServer.h"

HWND	InitHWnd(WNDCLASSEX wClass, HINSTANCE hInst)
{
	return (CreateWindowEx(0,
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
		NULL));
}