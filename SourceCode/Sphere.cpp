#include "Sphere.h"



std::tuple<Object3D *, Vector3D, double> Sphere::isIntersectsRay(const Vector3D &origin, const Vector3D &direction) {

	/*solving the quadratic equation
	(origin + direction * scale - position)^2 = radius^2
	*/
	Vector3D distance = position - origin;		//distance between the beginning of the ray and the center of the sphere

	//parts of the quadratic equation
	double A = dot(direction, direction);
	double B = -2.0 * dot(distance, direction);
	double C = dot(distance, distance) - radius * radius;

	double D = B * B - 4.0 * dot(direction, direction) * C;		//discriminant of the quadratic equation

	if (D < 0) {		
		return { NULL, Vector3D(), -1 };	//no intersection points
	}


	double t = (-B - sqrt(D)) / (2.0 * A);	 //smallest of roots

	//if the factor is less than 0, then either the sphere is behind the camera, or the camera lies inside the sphere
	if(t < 0)
		t = (-B + sqrt(D)) / (2.0 * A);		//biggest of the roots

	Vector3D normal = ((origin + direction * t) - position);		//normal vector
	normal.normalize();

	
	return { this, normal, t };
}
