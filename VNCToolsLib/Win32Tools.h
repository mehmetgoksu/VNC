#ifndef __WIN32TOOLS_H__
# define __WIN32TOOLS_H__

#define KILOBYTE	(1024LL)
#define MEGABYTE	(KILOBYTE * 1024)
#define GIGABYTE	(MEGABYTE * 1024)
#define TERABYTE	(GIGABYTE * 1024)

void		SendKeyDown(WORD key);
void		SendKeyUp(WORD key);
void		SendKeyPress(WORD key);
void		ForceRepaint(HWND hWnd);
void		ResizeClientWindow(HWND hWnd, int width, int height);
void		GetMousePosition(int *x, int *y);
void		SetMousePosition(int x, int y);
void		SetMouseLeftClick();
void		SetMouseLeftClickDown();
void		SetMouseLeftClickUp();

#endif