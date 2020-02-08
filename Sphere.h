#pragma once

#include "Object3D.h"

class Sphere : public Object3D {

protected:
	Material material;

public:
	double radius;

	Sphere(const double &_radius = 1) : radius(_radius) { }
	Sphere(const Vector3D &_position, const double &_radius = 1) : radius(_radius) { position = _position; }

	~Sphere() { }

	//function determines whether the ray crosses the object
	std::tuple<Object3D *, Vector3D, double> isIntersectsRay(const Vector3D &origin, const Vector3D &direction);

	//set material
	void setMaterial(const Material &_material) { material = _material; }

	//get material pointer
	Material *getMaterial() { return &material; }
};

