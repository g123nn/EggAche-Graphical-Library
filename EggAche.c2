//
// Implementation of EggAche Graphical Library
// By LJN-BOT Man, 2015
//

#include <windows.h>
#include <windowsx.h>
#pragma comment (lib, "Msimg32.lib")

#include "EggAche.h"

//========================================================================================
// Paper and Egg struct definiton
//========================================================================================

struct _Paper
{
	HDC hdc;
	HBITMAP hBitmap;
};

struct _Egg
{
	int x, y;
	int w, h;
	HDC hdc;
	HBITMAP hBitmap;
};

//========================================================================================
// Local Global vars
//========================================================================================

static HWND		g_hwnd;
static int		g_cxClient, g_cyClient;
static HANDLE	g_hEvent;
static int		g_iState;
static ONTIMER	fnTickArr[100];
static COLORREF	g_colorMask = RGB (0, 0, 201);  // Excluded in GetColor ()

//========================================================================================
// New Window and Message Loop
//========================================================================================

ONCREATE g_fnOnCreate;
ONCLOSE g_fnOnClose;
ONPAINT g_fnOnPaint;
ONRESIZE g_fnOnResize;
ONCLICK g_fnOnClick;
ONPRESS g_fnOnPress;

static void WINAPI NewWindow_Thread (const char *szCap);
static LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);

int HiWindow (const char *szCap, int width, int height)
{
	HANDLE		hThread;
	WNDCLASSW	wndclass;

	if (g_hwnd) return 0;

	wndclass.style			= CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc	= WndProc;
	wndclass.cbClsExtra		= 0;
	wndclass.cbWndExtra		= 0;
	wndclass.hInstance		= GetCurrentProcess ();
	wndclass.hIcon			= LoadIcon (NULL, IDI_APPLICATION);
	wndclass.hCursor		= LoadCursor (NULL, IDC_ARROW);
	wndclass.hbrBackground	= (HBRUSH) GetStockObject (WHITE_BRUSH);
	wndclass.lpszMenuName	= NULL;
	wndclass.lpszClassName	= L"LJN_WNDCLASS";

	if (!RegisterClassW (&wndclass)) return 0;

	//0 = Use Default in NewWindow_Thread ()
	g_cxClient = max (0, min (GetSystemMetrics (SM_CXSCREEN), width));
	g_cyClient = max (0, min (GetSystemMetrics (SM_CYSCREEN), height));

	g_hEvent = CreateEvent (NULL, FALSE, FALSE, NULL);
	if (!g_hEvent) return 0;

	hThread = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) NewWindow_Thread,
							(LPVOID) szCap, 0, NULL);
	if (!hThread)
	{
		CloseHandle (g_hEvent);
		return 0;
	}

	WaitForSingleObject (g_hEvent, INFINITE);
	CloseHandle (hThread);
	CloseHandle (g_hEvent);
	if (g_iState) return 0;

	return 1;
}

void WINAPI NewWindow_Thread (const char *szCap)
{
	MSG msg;

	g_hwnd = CreateWindowA ("LJN_WNDCLASS", szCap,
				WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME &~WS_MAXIMIZEBOX,
				CW_USEDEFAULT, CW_USEDEFAULT,
				g_cxClient ? g_cxClient : CW_USEDEFAULT,
				g_cyClient ? g_cyClient : CW_USEDEFAULT,
				NULL, NULL, GetCurrentProcess (), NULL);
	if (!g_hwnd) g_iState = 1;

	ShowWindow (g_hwnd, SW_NORMAL);
	UpdateWindow (g_hwnd);
	SetEvent (g_hEvent);

	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		g_hwnd = hwnd;
		if (g_fnOnCreate) g_fnOnCreate ();
		return 0;

	case WM_SIZE:
		g_cxClient = LOWORD (lParam);
		g_cyClient = HIWORD (lParam);
		if (g_fnOnResize) g_fnOnResize (g_cxClient, g_cyClient);
		return 0;

	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		if (g_fnOnClick)
			g_fnOnClick (GET_X_LPARAM (lParam),  GET_Y_LPARAM (lParam));
		return 0;

	case WM_CHAR:
		if (g_fnOnPress) g_fnOnPress ((char) wParam);
		return 0;

	case WM_TIMER:
		if (fnTickArr[(UINT) wParam]) fnTickArr[(UINT) wParam] ();
		return 0;

	case WM_PAINT:
		BeginPaint (hwnd, NULL);  //To validate the window

		if (g_fnOnPaint) g_fnOnPaint ();

		EndPaint (hwnd, NULL);
		return 0;

	case WM_DESTROY:
		if (g_fnOnClose) g_fnOnClose ();
		PostQuitMessage (0);
		return 0;
	}
	return DefWindowProc (hwnd, message, wParam, lParam);
}

//========================================================================================
// Message Box
//========================================================================================

void MsgBox (const char *szHint, const char *szCap)
{
	MessageBoxA (NULL, szHint, szCap, MB_OK);
}

//========================================================================================
// Timer Functions
//========================================================================================

int NewTimer (int id, int time, ONTIMER fnTick)
{
	if (!g_hwnd) return 0;

	if (id < 0 || id > 99) return 0;
	if (fnTickArr[id]) return 0;

	fnTickArr[id] = fnTick;
	if (!SetTimer (g_hwnd, id, time, NULL))
	{
		fnTickArr[id] = NULL;
		return 0;
	}

	return 1;
}

int FreeTimer (int id)
{
	if (!g_hwnd) return 0;
	if (id < 0 || id > 99) return 0;

	if (KillTimer (g_hwnd, id))
	{
		fnTickArr[id] = NULL;
		return 1;
	}
	else
		return 0;
}

//========================================================================================
// Paper Management
//========================================================================================

Paper NewPaper ()
{
	Paper	pNew;
	RECT	rect;
	HDC		hdcWnd;

	if (!g_hwnd) return 0;

	pNew = (Paper) malloc (sizeof (struct _Paper));
	if (!pNew) return NULL;

	hdcWnd = GetDC (g_hwnd);
	pNew->hdc = CreateCompatibleDC (hdcWnd);
	pNew->hBitmap = CreateCompatibleBitmap (hdcWnd, g_cxClient, g_cyClient);
	if (!pNew->hdc || !pNew->hBitmap)
	{
		if (pNew->hBitmap) DeleteObject (pNew->hBitmap);
		if (pNew->hdc) DeleteDC (pNew->hdc);
		free (pNew);
		ReleaseDC (g_hwnd, hdcWnd);
		return NULL;
	}
	SelectObject (pNew->hdc, pNew->hBitmap);

	rect.top = rect.left = 0;
	rect.right = g_cxClient;
	rect.bottom = g_cyClient;
	FillRect (pNew->hdc, &rect, (HBRUSH) GetStockObject (WHITE_BRUSH));

	SelectObject (pNew->hdc, (HBRUSH) GetStockObject (NULL_BRUSH));
	SelectObject (pNew->hdc, (HPEN) GetStockObject (BLACK_PEN));
	SetBkMode (pNew->hdc, TRANSPARENT);

	ReleaseDC (g_hwnd, hdcWnd);
	return pNew;
}

int DuplicatePaper (Paper pDst, Paper pSrc)
{
	if (!pDst || !pSrc) return 0;

	return BitBlt (pDst->hdc, 0, 0, g_cxClient, g_cyClient, pSrc->hdc, 0, 0, SRCCOPY);
}

int PrintPaper (Paper paper)
{
	HDC hdcWnd;

	if (!paper) return 0;

	hdcWnd = GetDC (g_hwnd);
	if (!hdcWnd) return 0;
	if (!BitBlt (hdcWnd, 0, 0, g_cxClient, g_cyClient, paper->hdc, 0, 0, SRCCOPY))
	{
		ReleaseDC (g_hwnd, hdcWnd);
		return 0;
	}

	ReleaseDC (g_hwnd, hdcWnd);
	return 1;
}

int FreePaper (Paper paper)
{
	HGDIOBJ hObj;

	if (!paper) return 0;

	hObj = SelectObject (paper->hdc, (HBRUSH) GetStockObject (NULL_BRUSH));
	if (hObj != GetStockObject (NULL_BRUSH))
		DeleteObject (hObj);

	hObj = SelectObject (paper->hdc, (HPEN) GetStockObject (BLACK_PEN));
	if (hObj != GetStockObject (BLACK_PEN) && hObj != GetStockObject (NULL_PEN))
		DeleteObject (hObj);

	DeleteObject (paper->hBitmap);
	DeleteDC (paper->hdc);
	free (paper);

	return 1;
}

//========================================================================================
// Egg Management
//========================================================================================

Egg EggNew (int x, int y, int width, int height)
{
	Egg		pNew;
	HBRUSH	hBrush;
	RECT	rect;
	HDC		hdcWnd;

	if (!g_hwnd) return NULL;

	pNew = (Egg) malloc (sizeof (struct _Egg));
	if (!pNew) return NULL;

	hdcWnd = GetDC (g_hwnd);
	pNew->hdc = CreateCompatibleDC (hdcWnd);
	pNew->hBitmap = CreateCompatibleBitmap (hdcWnd, width, height);
	pNew->x = max (INT_MIN, min (INT_MAX, x));
	pNew->y = max (INT_MIN, min (INT_MAX, y));

	if (!pNew->hdc || !pNew->hBitmap)
	{
		if (pNew->hBitmap) DeleteObject (pNew->hBitmap);
		if (pNew->hdc) DeleteDC (pNew->hdc);
		free (pNew);
		return NULL;
	}
	SelectObject (pNew->hdc, pNew->hBitmap);

	rect.left = rect.top = 0;
	rect.right = pNew->w = max (0, min (INT_MAX, width));
	rect.bottom = pNew->h = max (0, min (INT_MAX, height));
	hBrush = CreateSolidBrush (g_colorMask);
	FillRect (pNew->hdc, &rect, hBrush);

	SelectObject (pNew->hdc, (HBRUSH) GetStockObject (NULL_BRUSH));
	SelectObject (pNew->hdc, (HPEN) GetStockObject (BLACK_PEN));
	SetBkMode (pNew->hdc, TRANSPARENT);

	ReleaseDC (g_hwnd, hdcWnd);
	DeleteObject (hBrush);
	return pNew;
}

Egg EggFromBmp (int x, int y, int width, int height, const char *szPath)
{
	return EggFromBmpEx (x, y, width, height, 0, 0, 0, 0, szPath, -1);
}

Egg EggFromBmpEx (int x, int y, int width, int height,
				int xSrc, int ySrc, int wSrc, int hSrc,
				const char *szPath, int colorMask)
{
	Egg		pNew;
	HDC		hdcMemImag;
	HBITMAP	hBitmapImag;
	BITMAP	bitmap;
	int		cxBitmap, cyBitmap;

	hBitmapImag = LoadImageA (NULL, szPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (!hBitmapImag) return NULL;

	GetObject (hBitmapImag, sizeof (BITMAP), &bitmap);
	cxBitmap = width ? width : bitmap.bmWidth;
	cyBitmap = height ? height : bitmap.bmHeight;
	wSrc = wSrc ? wSrc : bitmap.bmWidth;
	hSrc = hSrc ? hSrc : bitmap.bmHeight;

	pNew = EggNew (x, y, cxBitmap, cyBitmap);
	if (!pNew)
	{
		DeleteObject (hBitmapImag);
		return NULL;
	}

	hdcMemImag  = CreateCompatibleDC (pNew->hdc);
	if (!hdcMemImag)
	{
		EggFree (pNew);
		DeleteObject (hBitmapImag);
		return NULL;
	}

	SelectObject (hdcMemImag, hBitmapImag);

	if (colorMask == -1)
	{
		if (!BitBlt (pNew->hdc, 0, 0, cxBitmap, cyBitmap,
						hdcMemImag, 0, 0, SRCCOPY))
		{
			EggFree (pNew);
			DeleteDC (hdcMemImag);
			DeleteObject (hBitmapImag);
			return NULL;
		}
	}
	else
	{
		colorMask = max (RGB (0, 0, 0), min (colorMask, RGB (255, 255, 255)));
		if (!TransparentBlt (pNew->hdc, 0, 0, cxBitmap, cyBitmap,
				hdcMemImag, xSrc, ySrc, wSrc, hSrc, colorMask))
		{
			EggFree (pNew);
			DeleteDC (hdcMemImag);
			DeleteObject (hBitmapImag);
			return NULL;
		}
	}

	DeleteDC (hdcMemImag);
	DeleteObject (hBitmapImag);
	return pNew;
}

Egg EggCopy (Egg egg, int x, int y)
{
	Egg pNew;

	if (!egg) return NULL;
	pNew = EggNew (x, y, egg->w, egg->h);
	if (!pNew) return NULL;

	pNew->x = max (INT_MIN, min (INT_MAX, x));
	pNew->y = max (INT_MIN, min (INT_MAX, y));

	if (!BitBlt (pNew->hdc, 0, 0, egg->w, egg->h, egg->hdc, 0, 0, SRCCOPY))
	{
		EggFree (pNew);
		return NULL;
	}

	return pNew;
}

int EggPaint (Paper paper, Egg egg)
{
	if (!paper || !egg) return 0;

	return TransparentBlt (paper->hdc, egg->x, egg->y, egg->w, egg->h,
				egg->hdc, 0, 0, egg->w, egg->h, g_colorMask);
}

int EggFree (Egg egg)
{
	HGDIOBJ hObj;

	if (!egg) return 0;

	hObj = SelectObject (egg->hdc, (HBRUSH) GetStockObject (NULL_BRUSH));
	if (hObj != GetStockObject (NULL_BRUSH))
		DeleteObject (hObj);
	hObj = SelectObject (egg->hdc, (HPEN) GetStockObject (BLACK_PEN));
	if (hObj != GetStockObject (BLACK_PEN) && hObj != GetStockObject (NULL_PEN))
		DeleteObject (hObj);

	DeleteObject (egg->hBitmap);
	DeleteDC (egg->hdc);
	free (egg);

	return 1;
}

void EggMove (Egg egg, int x, int y)
{
	if (!egg) return;

	egg->x = max (INT_MIN, min (INT_MAX, egg->x + x));
	egg->y = max (INT_MIN, min (INT_MAX, egg->y + y));
}

void EggPlace (Egg egg, int x, int y)
{
	if (!egg) return;

	egg->x = max (INT_MIN, min (INT_MAX, x));
	egg->y = max (INT_MIN, min (INT_MAX, y));
}

int EggGetX (Egg egg)
{
	if (!egg) return INT_MAX;
	return egg->x;
}

int EggGetY (Egg egg)
{
	if (!egg) return INT_MAX;
	return egg->y;
}

//========================================================================================
// Pen and Brush Customization
//========================================================================================

// Implementation

static int SetPen (HDC hdc, int width, int color, int fNULL)
{
	HPEN	hPen;
	HGDIOBJ	hObj;

	if (fNULL)
	{
		hObj = SelectObject (hdc, (HPEN) GetStockObject (NULL_PEN));
		if (hObj != GetStockObject (BLACK_PEN) && hObj != GetStockObject (NULL_PEN))
			DeleteObject (hObj);
		return 1;
	}

	hPen = CreatePen (PS_SOLID, max (0, width), (COLORREF) color);
	if (!hPen) return 0;

	hObj = SelectObject (hdc, hPen);
	if (hObj != GetStockObject (BLACK_PEN) && hObj != GetStockObject (NULL_PEN))
		DeleteObject (hObj);

	return 1;
}

static int SetBrush (HDC hdc, int color, int fNULL)
{
	HBRUSH	hBrush;
	HGDIOBJ	hObj;

	if (fNULL)
	{
		hObj = SelectObject (hdc, (HPEN) GetStockObject (NULL_BRUSH));
		if (hObj != GetStockObject (NULL_BRUSH))
			DeleteObject (hObj);
		return 1;
	}

	hBrush = CreateSolidBrush (color);
	if (!hBrush) return 0;

	hObj = SelectObject (hdc, hBrush);
	if (hObj != GetStockObject (NULL_BRUSH))
		DeleteObject (hObj);

	return 1;
}

//========================================================================================

int PSetPen (Paper paper, int width, int color, int fNULL)
{
	if (!paper) return 0;
	return SetPen (paper->hdc, width, color, fNULL);
}

int ESetPen (Egg egg, int width, int color, int fNULL)
{
	if (!egg) return 0;
	return SetPen (egg->hdc, width, color, fNULL);
}

int PSetBrush (Paper paper, int color, int fNULL)
{
	if (!paper) return 0;
	return SetBrush (paper->hdc, color, fNULL);
}

int ESetBrush (Egg egg, int color, int fNULL)
{
	if (!egg) return 0;
	return SetBrush (egg->hdc, color, fNULL);
}

int GetColor (int r, int g, int b)
{
	r = max (0, min (255, r));
	g = max (0, min (255, g));
	b = max (0, min (255, b));

	if (RGB (r, b, b) != g_colorMask)
		return (int) RGB (r, g, b);
	else
		return (int) RGB (r, g, b) + 1;
}

//========================================================================================
// Drawing Functions
//========================================================================================

// Implementation

static int DrawLine (HDC hdc, int xBeg, int yBeg, int xEnd, int yEnd)
{
	if (!MoveToEx (hdc, xBeg, yBeg, NULL)) return 0;
	if (!LineTo (hdc, xEnd, yEnd)) return 0;
	return 1;
}

static int DrawRect (HDC hdc, int xBeg, int yBeg, int xEnd, int yEnd)
{
	if (Rectangle (hdc, xBeg, yBeg, xEnd, yEnd)) return 1;
	else return 0;
}

static int DrawElps (HDC hdc, int xBeg, int yBeg, int xEnd, int yEnd)
{
	if (Ellipse (hdc, xBeg, yBeg, xEnd, yEnd)) return 1;
	else return 0;
}

static int DrawRdRt (HDC hdc, int xBeg, int yBeg, int xEnd, int yEnd, int wElps, int hElps)
{
	if (RoundRect (hdc, xBeg, yBeg, xEnd, yEnd, wElps, hElps)) return 1;
	else return 0;
}

static int DrawTxt (HDC hdc, int xBeg, int yBeg, const char *szText)
{
	if (TextOutA (hdc, xBeg, yBeg, szText, (int) strlen (szText)))
		return 1;
	else
		return 0;
}

//========================================================================================

// Draw Line
int PDrawLine (Paper paper, int xBeg, int yBeg, int xEnd, int yEnd)
{
	if (!paper) return 0;
	return DrawLine (paper->hdc, xBeg, yBeg, xEnd, yEnd);
}

int EDrawLine (Egg egg, int xBeg, int yBeg, int xEnd, int yEnd)
{
	if (!egg) return 0;
	return DrawLine (egg->hdc, xBeg, yBeg, xEnd, yEnd);
}

// Draw Rectangle
int PDrawRect (Paper paper, int xBeg, int yBeg, int xEnd, int yEnd)
{
	if (!paper) return 0;
	return DrawRect (paper->hdc, xBeg, yBeg, xEnd, yEnd);
}

int EDrawRect (Egg egg, int xBeg, int yBeg, int xEnd, int yEnd)
{
	if (!egg) return 0;
	return DrawRect (egg->hdc, xBeg, yBeg, xEnd, yEnd);
}

// Draw Ellipse
int PDrawElps (Paper paper, int xBeg, int yBeg, int xEnd, int yEnd)
{
	if (!paper) return 0;
	return DrawElps (paper->hdc, xBeg, yBeg, xEnd, yEnd);
}

int EDrawElps (Egg egg, int xBeg, int yBeg, int xEnd, int yEnd)
{
	if (!egg) return 0;
	return DrawElps (egg->hdc, xBeg, yBeg, xEnd, yEnd);
}

// Draw Round-Conner Rectangle
int PDrawRdRt (Paper paper, int xBeg, int yBeg, int xEnd, int yEnd,
			   int wElps, int hElps)
{
	if (!paper) return 0;
	return DrawRdRt (paper->hdc, xBeg, yBeg, xEnd, yEnd, wElps, hElps);
}

int EDrawRdRt (Egg egg, int xBeg, int yBeg, int xEnd, int yEnd,
			   int wElps, int hElps)
{
	if (!egg) return 0;
	return DrawRdRt (egg->hdc, xBeg, yBeg, xEnd, yEnd, wElps, hElps);
}

// Draw Text
int PDrawTxt (Paper paper, int xBeg, int yBeg, const char *szText)
{
	if (!paper) return 0;
	return DrawTxt (paper->hdc, xBeg, yBeg, szText);
}

int EDrawTxt (Egg egg, int xBeg, int yBeg, const char *szText)
{
	if (!egg) return 0;
	return DrawTxt (egg->hdc, xBeg, yBeg, szText);
}

// Draw Bitmap
int PDrawBitmap (Paper paper, int xBeg, int yBeg, int width, int height,
				 const char *szPath)
{
	HDC		hdcMemImag;
	HBITMAP	hBitmapImag;
	BITMAP	bitmap;
	int		cxBitmap, cyBitmap;

	if (!paper) return 0;

	hBitmapImag = LoadImageA (NULL, szPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (!hBitmapImag) return 0;

	GetObject (hBitmapImag, sizeof (BITMAP), &bitmap);
	cxBitmap = width ? min (bitmap.bmWidth, width) : bitmap.bmWidth;
	cyBitmap = height ? min (bitmap.bmHeight, height) : bitmap.bmHeight;

	hdcMemImag  = CreateCompatibleDC (paper->hdc);
	if (!hdcMemImag)
	{
		DeleteObject (hBitmapImag);
		return 0;
	}

	SelectObject (hdcMemImag, hBitmapImag);

	if (!BitBlt (paper->hdc, xBeg, yBeg, cxBitmap, cyBitmap, hdcMemImag, 0, 0, SRCCOPY))
	{
		DeleteDC (hdcMemImag);
		DeleteObject (hBitmapImag);
		return 0;
	}

	DeleteDC (hdcMemImag);
	DeleteObject (hBitmapImag);
	return 1;
}
