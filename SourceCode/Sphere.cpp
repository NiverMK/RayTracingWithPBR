#include "Sphere.h"

Sphere::Sphere(double _radius)
	: radius(_radius)
{
}

Sphere::Sphere(const Vector3D& _position, double _radius)
	: Figure(_position)
	, radius(_radius)
{
}

Sphere::~Sphere()
{
}

FigureData Sphere::isIntersectsRay(const Vector3D& origin, const Vector3D& direction)
{
	/* Solving the quadratic equation
	(origin + direction * scale - position)^2 = radius^2 */

	/* distance between ray's origin and sphere's center */
	Vector3D distance = position - origin;

	/* components of the quadratic equation */
	double A = dot(direction, direction);
	double B = -2.0 * dot(distance, direction);
	double C = dot(distance, distance) - radius * radius;

	double D = B * B - 4.0 * dot(direction, direction) * C;

	/* if discriminant < 0, then there are no intersection points */
	if (D < 0.0)
	{
		return FigureData();
	}

	double sqrtD = sqrt(D);

	/* smallest of roots */
	double length = (-B - sqrtD) / (2.0 * A);

	/* if length < 0, then this sphere is behind the camera, or camera is inside the sphere */
	if (length < 0.0)
	{
		length = (-B + sqrtD) / (2.0 * A);

		/* if length < 0, then sphere is behind the camera */
		if (length < 0.0)
		{
			return FigureData();
		}
	}

	/* normal vector */
	Vector3D normal = (origin + direction * length) - position;
	normal.normalize();

	FigureData data = { this, normal, length, 0.0, 0.0 };

	/* if figure is facing the camera, then return figure's data */
	return isFacingRay(data, direction);
}
