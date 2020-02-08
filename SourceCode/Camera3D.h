#pragma once

#include <math.h>
#include "Object3D.h"


class Camera3D : public Object3D {
public:

	//render resolution
	uint32_t width;
	uint32_t height;


	Camera3D() { width = 1280; height = 720; setFOV(60); }
	Camera3D(const Vector3D &_position, const double &_fov);

	~Camera3D() {}

	//set vertical FOV
	void setFOV(const double &_fov);

	double getFOV();


	/*Returns the ray vector based on the pixel number*/
	Vector3D getRay(double _width, double _height);

protected:

	double verticalFOV;

	double horizontalFOV;
};
