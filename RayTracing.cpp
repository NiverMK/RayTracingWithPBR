#include <iostream>

#include "Camera3D.h"
#include "Sphere.h"
#include "Plane.h"
#include "Cube.h"

#include "PointLight.h"

#include "Render.h"

#include "Scene.h"

#include "WindowClass.h"

#include <ctime>

//window size
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

//render size
#define RENDER_WIDTH 1280
#define RENDER_HEIGHT 720

#define GAMMA 2.2

#define PI 3.1415926


double lastTime = 0;
double deltaTime = 0;

//scene setup
void setupScene(Scene *scene);


int wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	//console
	if (AllocConsole())
	{
		FILE *dummyFile;
		freopen_s(&dummyFile, "CONOUT$", "wt", stdout);
		freopen_s(&dummyFile, "CONIN$", "rt", stdin);
		freopen_s(&dummyFile, "nul", "w", stderr);
		SetConsoleTitle(L"Debug Console");
		std::ios::sync_with_stdio(1);
	}
	std::cout << "Window Size = " << WINDOW_WIDTH << "x" << WINDOW_HEIGHT << std::endl;
	std::cout << "Render Resolution = " << RENDER_WIDTH << "x" << RENDER_HEIGHT << std::endl;

	//main window
	WindowClass mainWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!mainWindow.createWindow(hInstance, nCmdShow))
		return 0;

	MSG msg;

	//scene creation and customization
	Scene *mainScene = new Scene;
	setupScene(mainScene);
	
	//pointer to rgb pixels array (Render)
	Vector3D *pixels_rgb = nullptr;

	//render
	Render *render = new Render(RENDER_WIDTH, RENDER_HEIGHT);
	render->gamma = GAMMA;
	render->renderFrame(*mainScene);

	bool isRenderingFrame = 1;

	while (1) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			
			TranslateMessage(&msg);

			//window was closed
			if (msg.message == WM_QUIT) {
				//break while cycle
				break;		
			}
			
			auto result = DispatchMessage(&msg);

			//left mouse button was pressed
			if (result == 2) {
				render->stopRendering();
				isRenderingFrame = 0;
				render->renderFastFrame(*mainScene);
			}

			//left mouse button was released
			if (result == 3) {
				render->stopRendering();

				lastTime = clock();
				isRenderingFrame = 1;
				render->renderFrame(*mainScene);
			}
		}


		if (mainWindow.isLMouseButtonPressed) {

			pixels_rgb = render->isRendered();

			if (pixels_rgb) {

				mainWindow.updateCursor();

				//rotate the camera
				mainScene->camera.rotateAroundVector(Vector3D(0,0,1), -(double)mainWindow.cursorPos_delta.x / 10);
				mainScene->camera.rotateAroundVector(mainScene->camera.localAxisY, (double)mainWindow.cursorPos_delta.y / 10);

				//draw frame in window
				mainWindow.DrawBitmap(pixels_rgb, 0, 0);
				//render new frame
				render->renderFastFrame(*mainScene);
			}

		} else {
			
			if (isRenderingFrame) {
				if (render->isRendered()) {
					isRenderingFrame = 0;
					deltaTime = (clock() - lastTime) / 1000.0;
					std::cout << "Render Time = " << deltaTime << std::endl;
				}

				pixels_rgb = render->pixels_rgb;
				//draw frame in window
				mainWindow.DrawBitmap(pixels_rgb, 0, 0);
			}
		}
	}


	delete render;

	delete mainScene;

	return 1;
}


void setupScene(Scene *scene) {

	//camera setup
	scene->camera.setPosition(Vector3D(0, -9, 0));
	scene->camera.width = RENDER_WIDTH;
	scene->camera.height = RENDER_HEIGHT;
	scene->camera.setFOV(70);
	scene->camera.rotateAroundVector(Vector3D(0, 0, 1), 30);


	//create point lights
	PointLight *light1 = new PointLight(20, 30);
	light1->position = Vector3D(8, 5, 2);
	scene->lights.push_back(light1);

	PointLight *light2 = new PointLight(10, 30);
	light2->position = Vector3D(0, -5, -1);
	scene->lights.push_back(light2);


	//create 3D objects
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			Sphere *sphere = new Sphere(Vector3D(14, 6 - j * 4.0, 3 - i * 3.0), 1);
			sphere->getMaterial()->setAlbedo(Vector3D(1, 1, 1));
			sphere->getMaterial()->setRoughness(0.1 * j);
			sphere->getMaterial()->setMetalness(1.0 - 0.5 * i);
			scene->objects.push_back(sphere);
		}
	}

	Cube *room = new Cube(Vector3D(5, 0, 0), 40, 20, 10);
	for (int i = 0; i < 6; i++) {
		room->faces[i].getMaterial()->isDoubleSided = true;
	}

	room->faces[0].getMaterial()->setAlbedo(Vector3D(1, 1, 1));
	room->faces[0].getMaterial()->setRoughness(0.2);
	room->faces[0].getMaterial()->setMetalness(0.5);

	room->faces[1].getMaterial()->setAlbedo(Vector3D(1, 1, 1));
	room->faces[1].getMaterial()->setRoughness(0.2);
	room->faces[1].getMaterial()->setMetalness(0.5);

	room->faces[2].getMaterial()->setAlbedo(Vector3D(1, 0, 0));
	room->faces[2].getMaterial()->setRoughness(0.14);
	room->faces[2].getMaterial()->setMetalness(0.75);

	room->faces[3].getMaterial()->setAlbedo(Vector3D(0, 0.5, 0));
	room->faces[3].getMaterial()->setRoughness(0.14);
	room->faces[3].getMaterial()->setMetalness(0.75);

	room->faces[4].getMaterial()->setAlbedo(Vector3D(0, 0, 0.75));
	room->faces[4].getMaterial()->setRoughness(0.14);
	room->faces[4].getMaterial()->setMetalness(0.75);

	room->faces[5].getMaterial()->setAlbedo(Vector3D(1, 1, 1));
	room->faces[5].getMaterial()->setRoughness(0.1);
	room->faces[5].getMaterial()->setMetalness(1);

	scene->objects.push_back(room);
}