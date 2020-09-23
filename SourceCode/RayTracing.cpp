// RayTracing.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <ctime>
#include "Camera3D.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "PointLight.h"
#include "Render.h"
#include "Scene.h"
#include "WindowClass.h"
#include "Constants.h"

double lastTime = 0;	//time on last iteration
double deltaTime = 0;	//time on current iteration

int wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	/* create console */
	if (AllocConsole())
	{
		FILE* dummyFile;
		freopen_s(&dummyFile, "CONOUT$", "wt", stdout);
		freopen_s(&dummyFile, "CONIN$", "rt", stdin);
		freopen_s(&dummyFile, "nul", "w", stderr);
		SetConsoleTitle(L"Debug Console");
		std::ios::sync_with_stdio(1);
	}

	std::cout << "Window Size = " << WINDOW_WIDTH << "x" << WINDOW_HEIGHT << std::endl;
	std::cout << "Render Resolution = " << WINDOW_WIDTH << "x" << WINDOW_HEIGHT << std::endl;
	std::cout << "Threads number = " << std::thread::hardware_concurrency() << std::endl;

	/* create main window */
	WindowClass mainWindow(WINDOW_WIDTH, WINDOW_HEIGHT);

	if (!mainWindow.createWindow(hInstance, nCmdShow))
	{
		/* window was not created */
		return 1;
	}

	/* messages received by window */
	MSG msg;

	/* create and setup scene */
	Scene *mainScene = new Scene;
	setupScene(mainScene);

	/* pointer to RGB pixels (created and will be deleted in Render class) */
	Vector3D* pixelsRGB = nullptr;

	/* create render */
	Render* render = new Render(WINDOW_WIDTH, WINDOW_HEIGHT);
	render->renderFrame(*mainScene);

	bool isRenderingFrame = true;

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);

			/* window was closed */
			if (msg.message == WM_QUIT)
			{
				/* break while cycle */
				break;		
			}
			
			/* send messsage to window event handler */
			auto result = DispatchMessage(&msg);

			/* left mouse button was pressed */
			if (result == 2)
			{
				render->stopRendering();
				isRenderingFrame = false;
				render->renderFastFrame(*mainScene);
			}
			/* left mouse button was released */
			if (result == 3)
			{
				render->stopRendering();

				lastTime = clock();
				isRenderingFrame = true;
				render->renderFrame(*mainScene);
			}
		}

		if (mainWindow.isLMouseButtonPressed)
		{
			pixelsRGB = render->isRendered();

			if (pixelsRGB)
			{
				mainWindow.updateCursor();

				/* rotate camera */
				mainScene->camera.rotateAroundVector(Vector3D(0,0,1), - static_cast<double>(mainWindow.cursorPos_delta.x) / 10.0);
				mainScene->camera.rotateAroundVector(mainScene->camera.localAxisY, static_cast<double>(mainWindow.cursorPos_delta.y) / 10.0);

				/* draw frame in window */
				mainWindow.DrawBitmap(pixelsRGB, 0, 0);
				/* start new frame renderingn*/
				render->renderFastFrame(*mainScene);
			}
		}
		else
		{
			if (isRenderingFrame)
			{
				if (render->isRendered())
				{
					isRenderingFrame = false;
					deltaTime = (clock() - lastTime) / 1000.0;
					std::cout << "Render Time = " << deltaTime << std::endl;
				}

				pixelsRGB = render->pixelsRGB;
				/* draw frame in window */
				mainWindow.DrawBitmap(pixelsRGB, 0, 0);
			}
		}
	}

	delete render;
	delete mainScene;

	return 0;
}
