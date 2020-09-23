#pragma once

#include <vector>
#include "Camera3D.h"

class Figure;
class PointLight;

class Scene
{
public:
	Scene();
	~Scene();

	Camera3D camera;
	std::vector<Figure*> figures;
	std::vector<PointLight*> lights;
};

void setupScene(Scene* scene);
