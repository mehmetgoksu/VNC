#ifndef __BMPTOOLS_H__
# define __BMPTOOLS_H__

void		PaintBitmap(HWND hWnd, HBITMAP hBitmap);
BYTE*		HBitmap2Bytes(HBITMAP hBitmap, LONG *len);
BYTE* 		GetScreenBitmap(LONG *len);
HBITMAP		GetBitmapFromBytes(BYTE *bytes, LONG *width, LONG *height);

#endif