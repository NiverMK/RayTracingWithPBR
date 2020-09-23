#include "Plane.h"

Plane::Plane()
{
}

Plane::Plane(const Vector3D &_position, double _width, double _length)
	: Figure(_position)
	, width(_width)
	, length(_length)
{
}

Plane::~Plane()
{
}

FigureData Plane::isIntersectsRay(const Vector3D& origin, const Vector3D& direction)
{
	/*
	Solving the equation to find intersetion point of the plane and the ray using Cramer's method
	from left - ray's points, from right - plane's points
	origin + direction * t = position + localAxisX * u + localAxizY * v 
	localAxisX * u + localAxizY * v - direction * t = origin - position
	*/

	/* vector of a free members */
	Vector3D valueVec = origin - position;

	/* calculate matrix determinant, coefficients of unknown u, v, t
	|localAxisX.x	localAxisY.x	-direction.x|
	|localAxisX.y	localAxisY.y	-direction.y|
	|localAxisX.z	localAxisY.z	-direction.z|
	*/
	double main_determinant = localAxisX.x * (localAxisY.y * (-direction.z) - localAxisY.z * (-direction.y))
		+ localAxisY.x * (localAxisX.z * (-direction.y) - localAxisX.y * (-direction.z))
		- direction.x * (localAxisX.y * localAxisY.z - localAxisX.z * localAxisY.y);

	/* calculate matrix determinant, coefficients of unknown u, t and free member of v
	|localAxisX.x	valueVec.x	-direction.x|
	|localAxisX.y	valueVec.y	-direction.y|
	|localAxisX.z	valueVec.z	-direction.z|
	*/
	double v_determinant = localAxisX.x * (valueVec.y * (-direction.z) - valueVec.z * (-direction.y))
		+ valueVec.x * (localAxisX.z * (-direction.y) - localAxisX.y * (-direction.z))
		- direction.x * (localAxisX.y * valueVec.z - localAxisX.z * valueVec.y);

	/* calculate coefficient V */
	double v = v_determinant / main_determinant;
	v += length / 2.0;

	/* if V is not inside [0; length], intersection point is outside the plane */
	if (v < 0.0 || v > length)
		return FigureData();

	/* calculate matrix determinant, coefficients of unknown v, t and free member of u
	|valueVec.x		localAxisY.x	-direction.x|
	|valueVec.y		localAxisY.y	-direction.y|
	|valueVec.z		localAxisY.z	-direction.z|
	*/
	double u_determinant = valueVec.x * (localAxisY.y * (-direction.z) - localAxisY.z * (-direction.y))
		+ localAxisY.x * (valueVec.z * (-direction.y) - valueVec.y * (-direction.z))
		- direction.x * (valueVec.y * localAxisY.z - valueVec.z * localAxisY.y);

	/* calculate coefficient U */
	double u = u_determinant / main_determinant;
	u += width / 2.0;

	/* if U is not inside [0; width], intersection point is outside the plane */
	if (u < 0.0 || u > width)
		return FigureData();

	/* calculate matrix determinant, coefficients of unknown u, v and free member of t
	|localAxisX.x	localAxisY.x	valueVec.x|
	|localAxisX.y	localAxisY.y	valueVec.y|
	|localAxisX.z	localAxisY.z	valueVec.z|
	*/
	double t_determinant = localAxisX.x * (localAxisY.y * valueVec.z - localAxisY.z * valueVec.y)
		+ localAxisY.x * (localAxisX.z * valueVec.y - localAxisX.y * valueVec.z)
		+ valueVec.x * (localAxisX.y * localAxisY.z - localAxisX.z * localAxisY.y);

	/* calculate scaler T - length of a ray vector */
	double t = t_determinant / main_determinant;

	if (t < 0.0)
	{
		/* intersection point is behind the camera */
		return FigureData();
	}

	FigureData data = { this, localAxisZ, t, u, v};

	/* if figure is facing the camera, then return figure's data */
	return isFacingRay(data, direction);
}
