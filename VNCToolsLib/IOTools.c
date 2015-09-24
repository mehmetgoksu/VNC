#include <Windows.h>
#include <Strsafe.h>
#include <stdlib.h>
#include <stdio.h>
#include "IOTools.h"

/*
  TODO: Error testing
*/

void		WriteAllBytes(LPTSTR path, LPBYTE p, LONG len)
{
	HANDLE	hFile;
	DWORD	dwBytesWritten;

	hFile = CreateFile(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hFile, (LPCVOID)p, (DWORD)len, &dwBytesWritten, NULL);
	CloseHandle(hFile);
}

void		DisplayError(LPTSTR lpszFunction) 
{ 
	LPVOID	lpMsgBuf;
	LPVOID	lpDisplayBuf;
	DWORD	dw;

	dw = GetLastError(); 
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
					FORMAT_MESSAGE_FROM_SYSTEM |
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					dw,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					(LPTSTR) &lpMsgBuf,
					0, 
					NULL);

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40/*FATAL ERROR: ...*/) * sizeof(TCHAR));
	if (FAILED(StringCchPrintf((LPTSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / sizeof(TCHAR), TEXT("%s failed with error code %d as follows:\n%s"), lpszFunction, dw, lpMsgBuf)))
		printf("FATAL ERROR: Unable to output error code.\n");
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, "Error", MB_OK | MB_ICONERROR);
	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}
