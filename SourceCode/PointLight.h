#pragma once

#include "Object.h"

class PointLight : public Object
{
public:
	Vector3D colour;

	PointLight(double _intensity = 1.0, double _radius = 1.0, double _Kc = 1.0, double _Kl = 0.09, double _Kq = 0.032);
	~PointLight();

	void setIntensity(double _intensity);
	double getIntensity(const Vector3D& pointPos) const;

protected:
	double intensity;
	double radius;
	double Kc = 1.0;
	double Kl = 0.09;
	double Kq = 0.032;
};
