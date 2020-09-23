#pragma once

#include "Figure.h"

class Sphere : public Figure
{
public:
	double radius;

	Sphere(double _radius = 1.0);
	Sphere(const Vector3D& _position, double _radius = 1.0);
	~Sphere();

	/* find intersection point of ray and object. 
	returns pointer to object, normal vector and scaler for ray to calculate point's coordinates */
	FigureData isIntersectsRay(const Vector3D& origin, const Vector3D& direction);
};