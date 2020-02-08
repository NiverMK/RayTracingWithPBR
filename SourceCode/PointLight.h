#pragma once

#include "Vector3D.h"

class PointLight {
private:
	double intensity;
public:
	Vector3D position;
	Vector3D colour;

	double radius;

	PointLight(const double &_intensity = 1.0, const double &_radius = 1.0);
	~PointLight() {}

	void setIntensity(const double &_intensity) { intensity = _intensity; }

	double getIntensity(const Vector3D &pointPos);
};

