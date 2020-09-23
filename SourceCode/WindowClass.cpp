#include "WindowClass.h"
#include <tchar.h>
#include <iostream>

WindowClass::WindowClass(unsigned int width, unsigned int height)
	: window_width(width)
	, window_height(height)
{
	pixels_bgra = new unsigned char[static_cast<size_t>(window_width) * static_cast<size_t>(window_height) * 4];

	for (size_t i = 0; i < static_cast<size_t>(window_width) * static_cast<size_t>(window_height) * 4; i++)
		pixels_bgra[i] = 0;

	hBitmap = CreateBitmap(window_width, window_height, 1, 32, pixels_bgra);
	
	isLMouseButtonPressed = false;

	updateCursor();
}

WindowClass::~WindowClass() 
{
	DeleteObject(hBitmap);
	delete[] pixels_bgra;
}

bool WindowClass::createWindow(HINSTANCE hInst, int nCmdShow)
{
	WNDCLASS wc;
	wc.style = 0;									//default window behavior style
	wc.lpfnWndProc = WindowClass::MainWinProc;		//window event handler
	wc.cbClsExtra = 0;								//extra memory size in the system structure of class
	wc.cbWndExtra = 0;								//extra memory size in the system structure of window
	wc.hInstance = hInst;							//application descriptor
	wc.hIcon = NULL;								//window's icon
	wc.hCursor = NULL;								//cursor image
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	//window's background
	wc.lpszMenuName = NULL;							//menu is disabled
	wc.lpszClassName = _T("Window Class");			//class name of main window

	/* try to register window */
	if (!RegisterClass(&wc))
	{
		return false;
	}

	hMainWnd = CreateWindow(
		_T("Window Class"),						//class name
		_T("Ray Tracing"),						//window title
		WS_OVERLAPPEDWINDOW						//window behavior
		^ WS_THICKFRAME							//disabled ability to change window's size
		^ WS_MAXIMIZEBOX,						//disabled button to maximaze window
		CW_USEDEFAULT,							//horizontal window's position
		CW_USEDEFAULT,							//vertical windonw's position
		window_width + 4 * GetSystemMetrics(SM_CXFRAME),									//window width
		window_height + 4 * GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYCAPTION),	//window height
		NULL,									//parent window handler
		NULL,									//menu handler
		hInst,									//program instance descriptor
		NULL);									//no extra arguments		

	/* if window wasn't created */
	if (!hMainWnd)
	{
		return false;
	}

	/* store pointer of WindowClass object to user data
	it will help to get access from static function to non-static function of class */
	SetWindowLongPtr(hMainWnd, GWLP_USERDATA, (LONG_PTR)this);

	/* launch window with parameter nCmdShow */
	ShowWindow(hMainWnd, nCmdShow);
	UpdateWindow(hMainWnd);

	/* window was created */
	return true;
}

void WindowClass::DrawBitmap(Vector3D *pixels_rgb, int xStart, int yStart)
{
	RGBtoBRGA(pixels_rgb, pixels_bgra);
	/* set to hBitmap new image */
	SetBitmapBits(hBitmap, window_width * window_height * 4, pixels_bgra);

	/* get device context */
	HDC hdc = GetDC(hMainWnd);

	BITMAP bm;				//BITMAP structure to write information about image hBitmap
	HDC hdcMem;				//memory context
	POINT ptSize, ptOrg;	//coordinates

	/* create a memory device context compatible with the specified device*/
	hdcMem = CreateCompatibleDC(hdc);

	/* selects an object into the specified device context. 
	The new object replaces the previous object of the same type */
	SelectObject(hdcMem, hBitmap);

	/* set the mapping mode of the specified device context */
	SetMapMode(hdcMem, GetMapMode(hdc));

	/* retrieve information for graphics object bm */
	GetObject(hBitmap, sizeof(BITMAP), (LPVOID)&bm);

	/* get image size */
	ptSize.x = bm.bmWidth;
	ptSize.y = bm.bmHeight;

	/* start point */
	ptOrg.x = 0;
	ptOrg.y = 0;

	/* convert device coordinates into logical coordinates */
	DPtoLP(hdcMem, &ptOrg, 1);
	DPtoLP(hdc, &ptSize, 1);

	/* transfer color data corresponding to a rectangle of pixels from 
	the specified source device context into a destination device context */
	BitBlt(hdc, xStart, yStart, ptSize.x, ptSize.y, hdcMem, ptOrg.x, ptOrg.y, SRCCOPY);

	/* remove memory and device contexts */
	DeleteDC(hdcMem);
	DeleteDC(hdc);
}

LRESULT CALLBACK WindowClass::MainWinProc(HWND hw, UINT msg, WPARAM wp, LPARAM lp)
{
	/* get stored pointer of WindowClass object from user data to
	get access from static function to non-static function of class */
	WindowClass *winptr = (WindowClass *)GetWindowLongPtr(hw, GWLP_USERDATA);

	switch (msg)
	{
		case WM_CREATE:					//window was created
			return 0;
		case WM_SIZE:					//change window size
			return 0;
		case WM_COMMAND:				//message from menu command
			return 0;
		case WM_LBUTTONDOWN:			//left mouse button was pressed
			winptr->leftMouseButtonDown();
			return 2;
		case WM_LBUTTONUP:				//left mouse button was released
			winptr->leftMouseButtonUp();
			return 3;
		case WM_DESTROY:				//destroy window
			PostQuitMessage(WM_QUIT);	//send quit message to application
			return 1;
		default:
			return DefWindowProc(hw, msg, wp, lp);	//default message handling
	}
}

void WindowClass::updateCursor()
{
	if (GetCursorPos(&cursorPos_current))
	{
		/* get local cursor coordinates from global relative to window hMainWnd */
		ScreenToClient(hMainWnd, &cursorPos_current);

		cursorPos_delta.x = cursorPos_current.x - cursorPos_previous.x;
		cursorPos_delta.y = cursorPos_current.y - cursorPos_previous.y;

		cursorPos_previous = cursorPos_current;
	}
}

void WindowClass::leftMouseButtonDown()
{
	isLMouseButtonPressed = true;
	updateCursor();

	/* set mouse capture to process the situations, when mouse is outside the window */
	SetCapture(hMainWnd);
}

void WindowClass::leftMouseButtonUp()
{
	isLMouseButtonPressed = false;
	updateCursor();
	ReleaseCapture();
}

void WindowClass::RGBtoBRGA(Vector3D* pixels_rgb, unsigned char* _pixels_bgra)
{
	/* BGRA - DDB (Device Depenedent Bitmap) with 32 bits per pixel */
	for (size_t i = 0; i < static_cast<size_t>(window_width) * static_cast<size_t>(window_height); i++)
	{
		_pixels_bgra[i * 4] = static_cast<unsigned char>(pixels_rgb[i].z * 255);
		_pixels_bgra[i * 4 + 1] = static_cast<unsigned char>(pixels_rgb[i].y * 255);
		_pixels_bgra[i * 4 + 2] = static_cast<unsigned char>(pixels_rgb[i].x * 255);
		_pixels_bgra[i * 4 + 3] = 255;
	}
}