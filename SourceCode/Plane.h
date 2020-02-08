#pragma once

#include "Object3D.h"

class Plane : public Object3D {
protected:

	//cube size
	double width;
	double length;	

	Material material;

public:

	Plane();
	Plane(const Vector3D &_position, const double &_width = 1, const double &_length = 1);

	~Plane() { }

	void setWidth(const double &_width);
	void setLength(const double &_length);

	double getWidth() { return width; }
	double getLength() { return length; }

	//function determines whether the ray crosses the object
	std::tuple<Object3D *, Vector3D, double> isIntersectsRay(const Vector3D &origin, const Vector3D &direction);

	//set material
	void setMaterial(const Material &_material) { material = _material; }

	//get material pointer
	Material *getMaterial() { return &material; }
};

