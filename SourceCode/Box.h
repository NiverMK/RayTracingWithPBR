#pragma once

#include "Figure.h"
#include "Plane.h"

class Box : public Figure
{
public:
	/*	0,1 - OZ
		2,3 - OY
		4,5 - OX */
	Plane faces[6];

	Box(const Vector3D& _position = Vector3D(0.0), double size = 1.0);
	Box(const Vector3D& _position, double _width, double _length, double _height);
	~Box() {}

	/* find intersection point of ray and object. 
	returns pointer to object, normal vector and scaler for ray to calculate point's coordinates */
	FigureData isIntersectsRay(const Vector3D& origin, const Vector3D& direction);
	/* set new position */
	void setPosition(const Vector3D& _position);
	/* rotate object around vector by angle */

	/* rotate around local axes */
	void rotateAroundLocalX(double angle);
	void rotateAroundLocalY(double angle);
	void rotateAroundLocalZ(double angle);

	void rotateAroundVector(const Vector3D& vector, double angle);
	/* set one material to all planes */
	void setMaterial(const Material& _material);

protected:
	double width;
	double height;
	double length;

	/* set size of cube's planes */
	void setSizeOfPlanes();
	/* calculate new centers of planes */
	void movePlanes();
};

