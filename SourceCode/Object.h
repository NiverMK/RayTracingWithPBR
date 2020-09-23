#pragma once

#include "Quaternion.h"
#include "Vector3D.h"

class Object
{
public:
	/* local axes */
	Vector3D localAxisX;
	Vector3D localAxisY;
	Vector3D localAxisZ;

	Object(const Vector3D& _position = Vector3D(0.0));
	virtual ~Object();

	/* rotate around local axes */
	virtual void rotateAroundLocalX(double angle);
	virtual void rotateAroundLocalY(double angle);
	virtual void rotateAroundLocalZ(double angle);

	/* rotate object around vector by angle */
	virtual void rotateAroundVector(const Vector3D& vector, double angle);

	virtual void setPosition(const Vector3D& _position);
	virtual const Vector3D& getPosition() const;

protected:
	/* position */
	Vector3D position;
};

