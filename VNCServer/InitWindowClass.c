#include "VNCServer.h"

void	InitWinClass(WNDCLASSEX *wClass, HINSTANCE hInst, HWND hWnd, MSG msg)
{
	wClass->cbSize = sizeof(wClass);
	wClass->hbrBackground = (HBRUSH)COLOR_GRADIENTINACTIVECAPTION;
	wClass->hInstance = hInst;
	wClass->lpfnWndProc = (WNDPROC)WinProc;
	wClass->lpszClassName = "Window Class";
	wClass->style = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx(wClass);
}