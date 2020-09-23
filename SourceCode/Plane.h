#pragma once

#include "Figure.h"

class Plane : public Figure
{
public:
	/* plane's width and length */
	double width = 1.0;
	double length = 1.0;

	Plane();
	Plane(const Vector3D& _position, double _width = 1.0, double _length = 1.0);
	~Plane();

	/* find intersection point of ray and object. 
	returns pointer to object, normal vector and scaler for ray to calculate point's coordinates */
	FigureData isIntersectsRay(const Vector3D& origin, const Vector3D& direction);
};
