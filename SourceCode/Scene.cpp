#include "Scene.h"
#include "Figure.h"
#include "PointLight.h"
#include "Sphere.h"
#include "Box.h"
#include "Constants.h"
#include "MaterialFunctions.h"

Scene::Scene() 
{
}

Scene::~Scene() 
{
	for (auto object : figures) 
	{
		delete object;
	}

	for (auto light : lights) 
	{
		delete light;
	}
}

void setupScene(Scene* scene)
{
	/* setup main camera */
	scene->camera.setPosition(Vector3D(-1.0, -7.0, 0.0));
	scene->camera.width = WINDOW_WIDTH;
	scene->camera.height = WINDOW_HEIGHT;
	scene->camera.setFOV(70);
	scene->camera.rotateAroundVector(Vector3D(0.0, 0.0, 1.0), 23.0);

	/* create point lights */
	PointLight* light1 = new PointLight(20.0, 30.0);
	light1->setPosition(Vector3D(8.0, 5.0, 2.0));
	scene->lights.push_back(light1);

	PointLight* light2 = new PointLight(10.0, 30.0);
	light2->setPosition(Vector3D(0.0, -5.0, -1.0));
	scene->lights.push_back(light2);

	Material material;

	/* create 3d figures */
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Sphere* sphere = new Sphere(Vector3D(14.0, 6.0 - j * 4.0, 3.0 - i * 3.0), 1.0);

			material.albedo = Vector3D(1.0, 1.0, 1.0);
			material.roughness = 0.2 * j;
			material.metalness = 1.0 - 0.5 * i;

			sphere->setMaterial(material);

			scene->figures.push_back(sphere);
		}
	}

	Box* room = new Box(Vector3D(5.0, 0.0, 0.0), 40.0, 20.0, 10.0);

	material.isDoubleSided = true;

	material.albedo = Vector3D(1.0);
	material.roughness = 0.2;
	material.metalness = 0.5;
	material.materialFunction = &traceryMaterialFunction;
	room->faces[0].setMaterial(material);

	material.albedo = Vector3D(1.0);
	material.roughness = 1.0;
	material.metalness = 1.0;
	material.materialFunction = &checkerMaterialFunction;
	room->faces[1].setMaterial(material);

	material.albedo = Vector3D(0.0, 0.0, 1.0);
	material.roughness = 0.14;
	material.metalness = 0.6;
	material.materialFunction = &baseMaterialFunction;
	room->faces[2].setMaterial(material);

	material.albedo = Vector3D(1.0, 0.0, 0.0);
	material.roughness = 0.14;
	material.metalness = 0.75;
	material.materialFunction = &baseMaterialFunction;
	room->faces[3].setMaterial(material);

	material.albedo = Vector3D(1.0);
	material.roughness = 0.15;
	material.metalness = 0.25;
	material.materialFunction = &rainbowMaterialFunction;
	room->faces[4].setMaterial(material);

	material.albedo = Vector3D(1.0);
	material.roughness = 0.1;
	material.metalness = 1.0;
	material.materialFunction = &baseMaterialFunction;
	room->faces[5].setMaterial(material);

	scene->figures.push_back(room);
}