#pragma once

#include "Vector3D.h"
#include <windows.h>

class WindowClass
{
public:
	/* main window */
	HWND hMainWnd;

	/* window size */
	unsigned int window_width;
	unsigned int window_height;

	/* current, previous cursor position and difference between them */
	POINT cursorPos_current;
	POINT cursorPos_previous;
	POINT cursorPos_delta;

	bool isLMouseButtonPressed;

	WindowClass(unsigned int width, unsigned int height);
	~WindowClass();

	/* create window function */
	bool createWindow(HINSTANCE hInst, int nCmdShow);
	/* draw bitmap in window */
	void DrawBitmap(Vector3D* pixels_rgb, int xStart, int yStart);
	/* update cursor data */
	void updateCursor();

protected:
	/* pixels in bgra format */
	unsigned char* pixels_bgra;
	/* bitmap to draw in window */
	HBITMAP hBitmap;

	/* window event handler */
	static LRESULT CALLBACK MainWinProc(HWND, UINT, WPARAM, LPARAM);
	/* left mouse button click handler */
	void leftMouseButtonDown();
	void leftMouseButtonUp();
	/* convert from RGB to BRGA */
	void RGBtoBRGA(Vector3D* pixels_rgb, unsigned char* _pixels_bgra);
};