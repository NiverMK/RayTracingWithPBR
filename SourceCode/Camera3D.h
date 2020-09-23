#pragma once

#include "Object.h"

class Camera3D : public Object
{
public:
	/* render size */
	size_t width;
	size_t height;

	Camera3D(const Vector3D& _position = Vector3D(), double _fov = 60.0);
	~Camera3D();

	void setFOV(double _fov);
	double getFOV() const;

	/* get casted ray from camera */
	Vector3D getRay(size_t _width, size_t _height) const;
	/* experemental */
	Vector3D getRayQuaternion(size_t _width, size_t _height) const;

protected:
	double verticalFOV;
	double horizontalFOV;
};