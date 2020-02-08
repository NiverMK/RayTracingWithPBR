#include "WindowClass.h"
#include <tchar.h>



WindowClass::WindowClass(const unsigned int &width, const unsigned int &height) {
	window_width = width;
	window_height = height;
	
	pixels_bgra = new unsigned char[(size_t)window_width * (size_t)window_height * 4];
	for (unsigned int i = 0; i < window_width * window_height * 4; i++)
		pixels_bgra[i] = 0;

	hBitmap = CreateBitmap(window_width, window_height, 1, 32, pixels_bgra);
	
	isLMouseButtonPressed = 0;

	updateCursor();
}



WindowClass::~WindowClass() {
	DeleteObject(hBitmap);
	delete[] pixels_bgra;
}



bool WindowClass::createWindow(HINSTANCE hInst, int nCmdShow) {
	//create a window class
	WNDCLASS wc;
	wc.style = 0;									
	wc.lpfnWndProc = WindowClass::MainWinProc;		
	wc.cbClsExtra = 0;								
	wc.cbWndExtra = 0;								
	wc.hInstance = hInst;							
	wc.hIcon = NULL;								
	wc.hCursor = NULL;								
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	
	wc.lpszMenuName = NULL;							
	wc.lpszClassName = _T("Window Class");			

	if (!RegisterClass(&wc)) {		
		return FALSE;			//class was not registered		
	}

	hMainWnd = CreateWindow(				
		_T("Window Class"),				
		_T("Ray Tracing"),					
		WS_OVERLAPPEDWINDOW						
		^ WS_THICKFRAME						
		^ WS_MAXIMIZEBOX,						
		CW_USEDEFAULT,						
		CW_USEDEFAULT,							
		window_width + 4 * GetSystemMetrics(SM_CXFRAME),									//i don't know why, but we should multiply it by 4 
		window_height + 4 * GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYCAPTION),	
		NULL,									
		NULL,									
		hInst,									
		NULL);									

	if (!hMainWnd) {			
		return FALSE;		//window was not created	
	}


	/*add a pointer to the WindowClass class in the user data of the hMainWnd object.
	It will help in the static function MainWndProc to access the non-static functions and objects of the WindowClass class*/
	SetWindowLongPtr(hMainWnd, GWLP_USERDATA, (LONG_PTR)this);

	ShowWindow(hMainWnd, nCmdShow);
	UpdateWindow(hMainWnd);

	return 1;
}



void WindowClass::DrawBitmap(Vector3D *pixels_rgb, int xStart, int yStart) {

	RGBtoBRGA(pixels_rgb, pixels_bgra);

	//replace image in memory with new image
	SetBitmapBits(hBitmap, window_width * window_height * 4, pixels_bgra);


	HDC hdc = GetDC(hMainWnd);	//get device context

	BITMAP bm;				
	HDC hdcMem;				
	POINT ptSize, ptOrg;	

	hdcMem = CreateCompatibleDC(hdc);		

	SelectObject(hdcMem, hBitmap);			
											
	SetMapMode(hdcMem, GetMapMode(hdc));	
											

	GetObject(hBitmap, sizeof(BITMAP), (LPVOID)&bm);	

	ptSize.x = bm.bmWidth;					
	ptSize.y = bm.bmHeight;

	DPtoLP(hdc, &ptSize, 1);				
	ptOrg.x = 0;							
	ptOrg.y = 0;						
	DPtoLP(hdcMem, &ptOrg, 1);			


	BitBlt(hdc, xStart, yStart, ptSize.x, ptSize.y, hdcMem, ptOrg.x, ptOrg.y, SRCCOPY);

	DeleteDC(hdcMem);			
	DeleteDC(hdc);			
}



LRESULT CALLBACK WindowClass::MainWinProc(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	/*extract from the user data of the window hWnd a pointer to the class WindowClass*/
	WindowClass *winptr = (WindowClass *)GetWindowLongPtr(hw, GWLP_USERDATA);


	//window event handler
	switch (msg) {
	case WM_CREATE:					
		return 0;

	case WM_SIZE:					
		return 0;

	case WM_COMMAND:				
		return 0;

	case WM_LBUTTONDOWN:				//left mouse button was pressed
		winptr->leftMouseButtonDown();
		return 2;

	case WM_LBUTTONUP:					//left mouse button was released		
		winptr->leftMouseButtonUp();
		return 3;

	case WM_DESTROY:					//window was destroyed
		PostQuitMessage(WM_QUIT);		//send message to break while cicle in main fuction
		return 1;

	default:						
		return DefWindowProc(hw, msg, wp, lp);		
	}

	return DefWindowProc(hw, msg, wp, lp);
}



void WindowClass::updateCursor() {
	if (GetCursorPos(&cursorPos_current)) {
		//convert the coordinates of the cursor from global to local space relative to the hMainWnd window
		ScreenToClient(hMainWnd, &cursorPos_current);

		cursorPos_delta.x = cursorPos_current.x - cursorPos_previous.x;
		cursorPos_delta.y = cursorPos_current.y - cursorPos_previous.y;

		cursorPos_previous = cursorPos_current;
	}
}



void WindowClass::leftMouseButtonDown() {
	isLMouseButtonPressed = 1;
	updateCursor();

	/*Set the mouse capture.*/
	SetCapture(hMainWnd);
}



void WindowClass::leftMouseButtonUp() {
	isLMouseButtonPressed = 0;
	updateCursor();

	//free capture
	ReleaseCapture();
}



void WindowClass::RGBtoBRGA(Vector3D *pixels_rgb, unsigned char *_pixels_bgra) {


	for (unsigned int i = 0; i < window_width * window_height; i++) {
		_pixels_bgra[i * 4] = (unsigned char)(pixels_rgb[i].z * 255);
		_pixels_bgra[i * 4 + 1] = (unsigned char)(pixels_rgb[i].y * 255);
		_pixels_bgra[i * 4 + 2] = (unsigned char)(pixels_rgb[i].x * 255);
		_pixels_bgra[i * 4 + 3] = 255;
	}
}
