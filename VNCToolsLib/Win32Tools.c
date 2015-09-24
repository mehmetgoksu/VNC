#include <Windows.h>
#include <WinGDI.h>
#include <stdlib.h>
#include "Win32Tools.h"

/*
  TODO: Error testing
*/

void			SendKeyDown(WORD key)
{
	INPUT		ip;

	ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;
	ip.ki.wVk = key;
	ip.ki.dwFlags = 0;
	SendInput(1, &ip, sizeof(ip));
}

void			SendKeyUp(WORD key)
{
	INPUT		ip;

	ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;
	ip.ki.wVk = key;
	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(ip));
}

void			SendKeyPress(WORD key)
{
	SendKeyDown(key);
	SendKeyUp(key);
}

void			GetMousePosition(int *x, int *y)
{
	POINT		p;

	GetCursorPos(&p);
	*x = p.x;
	*y = p.y;
}

void			SetMouseLeftClick()
{
	SetMouseLeftClickDown();
	SetMouseLeftClickUp();
}

void			SetMouseLeftClickDown()
{
	POINT		p;

	GetCursorPos(&p);
	mouse_event(MOUSEEVENTF_LEFTDOWN, p.x, p.y, 0, 0);
}

void			SetMouseLeftClickUp()
{
	POINT		p;

	GetCursorPos(&p);
	mouse_event(MOUSEEVENTF_LEFTUP, p.x, p.y, 0, 0);
}


void			SetMousePosition(int x, int y)
{
	SetCursorPos(x, y);
}

void			ForceRepaint(HWND hWnd)
{
	RECT		rect;
	HDC			hdcScreen;

	hdcScreen = GetDC(NULL);
	rect.left = 0;
	rect.top = 0;
	rect.bottom = GetDeviceCaps(hdcScreen, VERTRES);
	rect.right = GetDeviceCaps(hdcScreen, HORZRES);
	ReleaseDC(NULL, hdcScreen);
	InvalidateRect(hWnd, &rect, TRUE);
}

void			ResizeClientWindow(HWND hWnd, int width, int height)
{
	POINT		ptDiff;
	RECT		rcClient;
	RECT		rcWind;

	GetClientRect(hWnd, &rcClient);
	GetWindowRect(hWnd, &rcWind);
	ptDiff.x = (rcWind.right - rcWind.left) - rcClient.right;
	ptDiff.y = (rcWind.bottom - rcWind.top) - rcClient.bottom;
	MoveWindow(hWnd, rcWind.left, rcWind.top, width + ptDiff.x, height + ptDiff.y, TRUE);
}
