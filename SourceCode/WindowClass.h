#pragma once

#include "Vector3D.h"

#include <windows.h>


class WindowClass {

public:

	WindowClass(const unsigned int &width, const unsigned int &height);
	~WindowClass();

	HWND hMainWnd;		//main window

	//size of window
	unsigned int window_width;
	unsigned int window_height;

	POINT cursorPos_current;	//current position of cursor
	POINT cursorPos_previous;	//last position
	POINT cursorPos_delta;		//difference between current and last cursor positions

	bool isLMouseButtonPressed;	//is left mouse button pressed

	bool createWindow(HINSTANCE hInst, int nCmdShow);

	void DrawBitmap(Vector3D *pixels_rgb, int xStart, int yStart);

	void updateCursor();

protected:

	unsigned char *pixels_bgra;	

	HBITMAP hBitmap;

	//window event handler
	static LRESULT CALLBACK MainWinProc(HWND, UINT, WPARAM, LPARAM);

	void leftMouseButtonDown();
	void leftMouseButtonUp();

	//from RGB to BRGA
	void RGBtoBRGA(Vector3D *pixels_rgb, unsigned char *_pixels_bgra);
};
