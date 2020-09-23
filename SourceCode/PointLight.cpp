#include "PointLight.h"

PointLight::PointLight(double _intensity, double _radius, double _Kc, double _Kl, double _Kq)
	: intensity(_intensity)
	, radius(_radius)
	, Kc(_Kc)
	, Kl(_Kl)
	, Kq(_Kq)
{
	colour = Vector3D(1.0);
}

PointLight::~PointLight()
{
}

void PointLight::setIntensity(double _intensity)
{
	intensity = _intensity;
}

double PointLight::getIntensity(const Vector3D& pointPos) const
{
	/* distance between source light and point */
	Vector3D distanceVec = position - pointPos;
	double distance = distanceVec.length();

	if (distance > radius) 
	{
		return 0.0;
	} 
	else 
	{
		double attenuation = 1.0 / (Kc + Kl * distance + Kq * distance * distance);

		return intensity * attenuation;
	}
}