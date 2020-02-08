#pragma once

#include <vector>
#include <math.h>

#include "Quaternion.h"
#include "Vector3D.h"
#include "Material.h"


class Object3D
{
protected:
	//position in space
	Vector3D position;

public:

	//basis vectors
	Vector3D localAxisX;
	Vector3D localAxisY;
	Vector3D localAxisZ;

	Object3D();
	virtual ~Object3D() {}

	//set position
	virtual void setPosition(const Vector3D &_position) { position = _position; }

	//get position vector
	virtual Vector3D getPosition() { return position; }

	//rotate object around vector
	virtual void rotateAroundVector(const Vector3D &vector, double angle);

	/*Function determines whether the ray crosses the object.
	Returns a pointer to an object, a normal vector at the intersection point and a ray multiplier*/
	virtual std::tuple<Object3D*, Vector3D, double> isIntersectsRay(const Vector3D &origin, const Vector3D &direction) { return { NULL, Vector3D(), 0 }; }

	//set material
	virtual void setMaterial(const Material &_material) { }

	//get material pointer
	virtual Material *getMaterial() { return NULL; }
};

