#include "Scene.h"


Scene::~Scene() {
	for (auto object : objects) {
		delete object;
	}
	for (auto light : lights) {
		delete light;
	}
}
