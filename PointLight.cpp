#include "PointLight.h"


PointLight::PointLight(const double &_intensity, const double &_radius) {
	colour = Vector3D(1, 1, 1);
	intensity = _intensity;
	radius = _radius;
}


double PointLight::getIntensity(const Vector3D &pointPos) {

	Vector3D distanceVec = position - pointPos;
	//distance between point and light source
	double distance = distanceVec.length();

	if (distance > radius) {
		return 0;
	} else {
		double scaler = (radius - distance) / radius;
		return intensity * scaler * scaler;
	}
}