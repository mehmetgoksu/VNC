#include <Windows.h>
#include <WinGDI.h>
#include <stdlib.h>
#include "BMPTools.h"

/*
  TODO: Error testing
*/

void					PaintBitmap(HWND hWnd, HBITMAP hBitmap)
{
	PAINTSTRUCT 		ps;
	HGDIOBJ 			oldBitmap;
	BITMAP 				bitmap;
	HDC 				hdc;
	HDC 				hdcMem;

	hdc = BeginPaint(hWnd, &ps);
	hdcMem = CreateCompatibleDC(hdc);
	oldBitmap = SelectObject(hdcMem, hBitmap);
	GetObject(hBitmap, sizeof(bitmap), &bitmap);
	BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, oldBitmap);
	DeleteDC(hdcMem);
    EndPaint(hWnd, &ps);
}

HBITMAP					GetBitmapFromBytes(BYTE *bytes, LONG *width, LONG *height)
{
    HBITMAP				ret;
    BITMAPINFO			*info;
    BITMAPFILEHEADER	*header;
	HDC					hDC;

	hDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	header = (BITMAPFILEHEADER *)bytes;
	info = (BITMAPINFO*)(bytes + sizeof(BITMAPFILEHEADER));
	if (width != NULL);
		*width = info->bmiHeader.biWidth;
	if (height != NULL);
		*height = info->bmiHeader.biHeight;
	ret = CreateDIBitmap(hDC, &(info->bmiHeader), CBM_INIT, bytes + header->bfOffBits, info, DIB_RGB_COLORS);
	DeleteDC(hDC);
	return (ret);
}

BYTE					*HBitmap2Bytes(HBITMAP hBitmap, LONG *len)
{
	LPBITMAPINFOHEADER	lpbi;
	BITMAPFILEHEADER	bmfHdr;
	BITMAPINFOHEADER	bi;
	BITMAP				Bitmap;
	LPBYTE				p;
	HANDLE				hDib;
	HANDLE				hPal;
	HANDLE				hOldPal;
	DWORD				dwBmBitsSize;
	WORD				wBitCount;
	HDC					hDC;
	int					iBits;

/*
		 ▲
		▲ ▲
*/
	hOldPal = NULL;
	hDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	wBitCount = (iBits <= 1 ? 1 : iBits <= 4 ? 4 : iBits <= 8 ? 8 : 24);
	GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);
	SecureZeroMemory(&bi, sizeof(bi));
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;
	hDib = GlobalAlloc(GHND, dwBmBitsSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	memcpy(lpbi, &bi, sizeof(bi));
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{ 
		hDC = GetDC(NULL);
		hOldPal = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER), (BITMAPINFO *)lpbi, DIB_RGB_COLORS);
	if (hOldPal != NULL)
	{
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
		ReleaseDC(NULL, hDC);
	}
	SecureZeroMemory(&bmfHdr, sizeof(bmfHdr));
	bmfHdr.bfType = 0x4D42; // "BM"
	*len = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwBmBitsSize;
	bmfHdr.bfSize = *len;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);
	p = (LPBYTE)GlobalAlloc(GMEM_FIXED, *len);
	memcpy(p, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));
	memcpy(p + sizeof(BITMAPFILEHEADER), (LPSTR)lpbi, *len - sizeof(BITMAPFILEHEADER));
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	return (p);
}

void			CaptureCursor(HDC hdc, int screenWidth, int screenHeight)
{
	ICONINFOEXW	info;
	CURSORINFO	cursor;
	BITMAP		bmpCursor;
	RECT		rcWnd;
	int			x;
	int			y;

	cursor.cbSize = sizeof(cursor);
	GetCursorInfo(&cursor);
	if (cursor.flags == CURSOR_SHOWING)
	{
		rcWnd.top = 0;
		rcWnd.left = 0;
		rcWnd.right = screenWidth;
		rcWnd.bottom = screenHeight;
		info.cbSize = sizeof(info);
		GetIconInfoExW(cursor.hCursor, &info);
		x = cursor.ptScreenPos.x - rcWnd.left - info.xHotspot;
		y = cursor.ptScreenPos.y - rcWnd.top  - info.yHotspot;
		SecureZeroMemory(&bmpCursor, sizeof(bmpCursor));
		GetObject(info.hbmColor, sizeof(bmpCursor), &bmpCursor);
		DrawIconEx(hdc, x, y, cursor.hCursor, bmpCursor.bmWidth, bmpCursor.bmHeight, 0, NULL, DI_NORMAL);
	}
}

BYTE 			*GetScreenBitmap(LONG *len)
{
	HBITMAP		hBitmap;
	BYTE		*p;
	HDC			hdcScreen;
	HDC			hdc;
	int			height;
	int			width;

	hdcScreen = GetDC(NULL);
	hdc = CreateCompatibleDC(hdcScreen);
	width = GetDeviceCaps(hdcScreen, HORZRES);
	height = GetDeviceCaps(hdcScreen, VERTRES);
	hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
	SelectObject(hdc, hBitmap);
	BitBlt(hdc, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);
	CaptureCursor(hdc, width, height);
	p = HBitmap2Bytes(hBitmap, len);
	DeleteDC(hdc);
	DeleteObject(hBitmap);
	ReleaseDC(NULL, hdcScreen);
	return (p);
}
