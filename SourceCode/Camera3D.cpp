#include "Camera3D.h"


Camera3D::Camera3D(const Vector3D &_position, const double &_fov) {
	width = 1280;
	height = 720;

	position = _position;
	setFOV(_fov);
}



Vector3D Camera3D::getRay(double _width, double _height) {
	//camera direction vector
	Vector3D ray = localAxisX;

	//to interval [-1; 1]
	_width = _width - width / 2;
	_width = -2 * _width / width;

	_height = _height - height / 2;
	_height = -2 * _height / height;


	ray = ray + localAxisY * _width * tan(verticalFOV / 2 * 3.14 / 360) * horizontalFOV / verticalFOV;
	ray = ray + localAxisZ * _height * tan(verticalFOV / 2 * 3.14 / 360);

	ray.normalize();

	return ray;
}



void Camera3D::setFOV(const double &_fov) {
	//vertical fov
	verticalFOV = _fov;

	//horizontal fov calculation
	horizontalFOV = width * verticalFOV / height;
}



double Camera3D::getFOV() {
	return verticalFOV;
}



