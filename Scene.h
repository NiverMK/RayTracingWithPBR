#pragma once

#include <vector>

#include "Object3D.h"
#include "Camera3D.h"
#include "PointLight.h"

class Scene
{
public:
	Camera3D camera;
	std::vector<Object3D *> objects;
	std::vector<PointLight *> lights;

	Scene() {}
	~Scene();
};

