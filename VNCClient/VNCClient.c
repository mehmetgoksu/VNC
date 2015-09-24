#include <winsock2.h>
#include <windows.h>
#include "SocketTools.h"
#include "IOTools.h"
#include "BMPTools.h"
#include "Win32Tools.h"
#include "VNCClient.h"
#include "CompressionTools.h"

/* TODO: Only one static variable is allowed. Do structure :D */

static SOCKET Socket = 0;
static BYTE *buf = NULL;
static BYTE *bitmapBuf = NULL;
static int toRead = -1;
static int pos = 0;
static HBITMAP hBitmap  = NULL;
static LONG	lastWidth = -1;
static LONG	lastHeight = -1;

/* TODO: There is too much function in this file */

void				OnKeyDown(WPARAM key)
{
	/*TODO: get keyboard events */
	UNREFERENCED_PARAMETER(key);
}

void				OnKeyUp(WPARAM key)
{
	/*TODO: get keyboard events */
	UNREFERENCED_PARAMETER(key);
}

void				ResizeHandling(HWND hWnd, LONG width, LONG height)
{
	/* If it's the first time or if the new bitmap size is different from previous... */
	if (lastHeight <= 0 || lastWidth <= 0 || lastHeight != height || lastWidth != width)
	{
		ResizeClientWindow(hWnd, width, height);
		lastWidth = width;
		lastHeight = height;
	}
}

void				InitClient()
{
	/* I think there is some untested return value :) */

	buf = (BYTE *)malloc(SOCKET_READER_CACHE_SIZE);
	bitmapBuf = (BYTE *)malloc(BITMAP_BUFFER_SIZE);
}

void				ClearClient()
{
	free(bitmapBuf);
	free(buf);
}

void				SocketHandling(HWND hWnd, LPARAM lParam)
{
	LONG			uncompressedLen;
	LONG			width;
	LONG			height;

	if (WSAGETSELECTERROR(lParam))
	{	
		MessageBox(hWnd, "Connection to server failed", "Error", MB_OK | MB_ICONERROR);
		DestroyWindow(hWnd);
		return;
	}
	switch (WSAGETSELECTEVENT(lParam))
	{
		case FD_WRITE:
			{
			}
			break;
		case FD_READ:
			{
				/* TODO: Improve the protocol ^^ */
				if (toRead < 0)
				{
					/* Step 1) We receive the compressed bitmap size */
					ReadInt32FromSocket(Socket, &toRead);
				}
				/* Step 2) We receive the compressed bitmap by chunks which means that FD_READ is called multiple time for each image */
				ReadBytesFromSocket(Socket, buf, &pos, toRead);
				/* If we received all the data */
				if (pos == toRead)
				{
					/* We destroy the previous bitmap */
					if (hBitmap != NULL)
					{
						DeleteObject(hBitmap);
						hBitmap = NULL;
					}
					/* We extract and remplace the bitmap */
					UncompressData(buf, toRead, bitmapBuf, &uncompressedLen);
					hBitmap = GetBitmapFromBytes(bitmapBuf, &width, &height);
					/* Resize the window if necessary */
					ResizeHandling(hWnd, width, height);
					/* Force WM_PAINT to be send */
					ForceRepaint(hWnd);
					/* We are ready for another image */
					toRead = -1;
					pos = 0;
				}
			}
			break;
		case FD_CLOSE:
			{
				MessageBox(hWnd, "Connection closed by the server", "Warning", MB_OK | MB_ICONWARNING);
				DestroyWindow(hWnd);
			}
			break;
		default:
			break;
	}
}

int WINAPI			WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASSEX		wClass;
	HWND			hWnd;
	MSG				msg;

	UNREFERENCED_PARAMETER(hPrevInst);
	UNREFERENCED_PARAMETER(lpCmdLine);
	InitClient();
	SecureZeroMemory(&msg, sizeof(msg));
	SecureZeroMemory(&wClass, sizeof(wClass));
	wClass.cbSize = sizeof(wClass);
	wClass.hbrBackground = (HBRUSH)BLACK_BRUSH;
	wClass.hInstance = hInst;
	wClass.lpfnWndProc = (WNDPROC)WinProc;
	wClass.lpszClassName = "Window Class";
	wClass.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	RegisterClassEx(&wClass);
	hWnd = CreateWindowEx(0,
						wClass.lpszClassName,
						"My VNC Client",
						WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
						0,
						0,
						320,
						240,
						NULL,
						NULL,
						hInst,
						NULL);
	ShowWindow(hWnd, nShowCmd);
	EnableWindow(hWnd, TRUE);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (msg.message == WM_KEYDOWN)
			OnKeyDown(msg.wParam);
		else if (msg.message == WM_KEYUP)
			OnKeyUp(msg.wParam);
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (EXIT_SUCCESS);
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
			{
				Socket = CreateClientSocketWithAsyncReading("127.0.0.1", VNC_PORT, hWnd, WM_SOCKET);
				if (Socket == 0)
				{
					MessageBox(hWnd, "Connection to server failed", "Error", MB_OK | MB_ICONERROR);
					DestroyWindow(hWnd);
				}
			}
			break;
		case WM_ERASEBKGND:
			/* Performance Tricks: Don't paint the background if we already paint a bitmap on the window */
			if (hBitmap != NULL)
				return (1);
			break;
		case WM_PAINT:
			{
				/* We paint the bitmap */
				if (hBitmap != NULL)
					PaintBitmap(hWnd, hBitmap);
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
				ClearClient();
				PostQuitMessage(0);
				if (Socket != 0)
					CloseSocketAndCleanup(Socket);
				return (0);
			}
			break;
		case WM_SOCKET:
			{
				SocketHandling(hWnd, lParam);
				break;
			}
		default:
			break;
		}
	return (DefWindowProc(hWnd, msg, wParam, lParam));
}
