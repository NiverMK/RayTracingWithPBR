#include <cmath>
#include "Camera3D.h"
#include "Constants.h"

Camera3D::Camera3D(const Vector3D& _position, double _fov)
	: Object(_position)
{
	width = 1280;
	height = 720;

	setFOV(_fov);
}

Camera3D::~Camera3D()
{
}

Vector3D Camera3D::getRay(size_t _width, size_t _height) const
{
	/* convert to interval [-1.0; 1.0] */
	double w = -2.0 * _width + width;
	w = w / width;

	double h = -2.0 * _height + height;
	h = h / height;

	Vector3D ray = localAxisX;

	ray = ray + localAxisY * w * std::tan(verticalFOV / 2 * PI / 360.0) * horizontalFOV / verticalFOV;
	ray = ray + localAxisZ * h * std::tan(verticalFOV / 2 * PI / 360.0);

	ray.normalize();

	return ray;
}

Vector3D Camera3D::getRayQuaternion(size_t _width, size_t _height) const
{
	Vector3D ray = localAxisX;

	/* convert to interval [-0.5; 0.5] */
	double w = _width - width / 2.0;
	w = w / width;

	double h = _height - height / 2.0;
	h = h / height;

	/* rotation angles */
	double angleZ = horizontalFOV * w;
	double angleY = verticalFOV * h;

	angleY *= DEGREES_TO_RADS;
	angleZ *= DEGREES_TO_RADS;

	Quaternion rotorY(std::cos(angleY / 2.0), std::sin(angleY / 2.0) * localAxisY.x, std::sin(angleY / 2.0) * localAxisY.y, std::sin(angleY / 2.0) * localAxisY.z);
	Quaternion rotorZ(std::cos(angleZ / 2.0), std::sin(angleZ / 2.0) * localAxisZ.x, std::sin(angleZ / 2.0) * localAxisZ.y, std::sin(angleZ / 2.0) * localAxisZ.z);
	
	/* rotate */
	ray = (rotorY * ray * rotorY.getInverted()).getVector();
	ray = (rotorZ * ray * rotorZ.getInverted()).getVector();

	return ray;
}

void Camera3D::setFOV(double _fov)
{
	verticalFOV = _fov;

	horizontalFOV = width * verticalFOV / height;
}

double Camera3D::getFOV() const
{
	return verticalFOV;
}
