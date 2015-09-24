#include "VNCServer.h"

void CALLBACK FrameBuff(HWND hwnd, UINT uMsg, UINT timerId, DWORD dwTime)
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