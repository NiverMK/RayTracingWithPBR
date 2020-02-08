#include "Plane.h"


Plane::Plane() {
	width = 1;
	length = 1;
}



Plane::Plane(const Vector3D &_position, const double &_width, const double &_length) {
	width = _width;
	length = _length;

	position = _position;
}



std::tuple<Object3D *, Vector3D, double> Plane::isIntersectsRay(const Vector3D &origin, const Vector3D &direction) {
	/*Solving the equation for finding the intersection point of the plane and the ray. Cramer's rule.
	origin + direction * t = cornerPoint + localAxisX * u + localAxizY * v
	localAxisX * u + localAxizY * v - direction * t = origin - cornerPoint */

	//vector of free members
	Vector3D valueVec = origin - position;

	/*calculation of the determinant of a matrix consisting of coefficients for unknown u, v, t
	|localAxisX.x	localAxisY.x	-direction.x|
	|localAxisX.y	localAxisY.y	-direction.y|
	|localAxisX.z	localAxisY.z	-direction.z|
	*/
	double main_determinant = localAxisX.x * (localAxisY.y * (-direction.z) - localAxisY.z * (-direction.y))
		+ localAxisY.x * (localAxisX.z * (-direction.y) - localAxisX.y * (-direction.z))
		- direction.x * (localAxisX.y * localAxisY.z - localAxisX.z * localAxisY.y);


	/*calculation of the determinant of a matrix consisting of coefficients for unknown u, t and free members instead of coefficients for v
	|localAxisX.x	valueVec.x	-direction.x|
	|localAxisX.y	valueVec.y	-direction.y|
	|localAxisX.z	valueVec.z	-direction.z|
	*/
	double v_determinant = localAxisX.x * (valueVec.y * (-direction.z) - valueVec.z * (-direction.y))
		+ valueVec.x * (localAxisX.z * (-direction.y) - localAxisX.y * (-direction.z))
		- direction.x * (localAxisX.y * valueVec.z - localAxisX.z * valueVec.y);

	double v = v_determinant / main_determinant;

	if (v < -length / 2.0 || v > length / 2.0)
		return { NULL, Vector3D(), -1 };			//the intersection point with the plane is outside the plain

	/*calculation of the determinant of a matrix consisting of coefficients for unknown v, t and free members instead of coefficients for u
	|valueVec.x		localAxisY.x	-direction.x|
	|valueVec.y		localAxisY.y	-direction.y|
	|valueVec.z		localAxisY.z	-direction.z|
	*/
	double u_determinant = valueVec.x * (localAxisY.y * (-direction.z) - localAxisY.z * (-direction.y))
		+ localAxisY.x * (valueVec.z * (-direction.y) - valueVec.y * (-direction.z))
		- direction.x * (valueVec.y * localAxisY.z - valueVec.z * localAxisY.y);

	double u = u_determinant / main_determinant;

	if (u < -width / 2.0 || u > width / 2.0)
		return { NULL, Vector3D(), -1 };			//the intersection point with the plane is outside the plain

	/*calculation of the determinant of a matrix consisting of coefficients for unknown u, v and free members instead of coefficients for t
	|localAxisX.x	localAxisY.x	valueVec.x|
	|localAxisX.y	localAxisY.y	valueVec.y|
	|localAxisX.z	localAxisY.z	valueVec.z|
	*/
	double t_determinant = localAxisX.x * (localAxisY.y * valueVec.z - localAxisY.z * valueVec.y)
		+ localAxisY.x * (localAxisX.z * valueVec.y - localAxisX.y * valueVec.z)
		+ valueVec.x * (localAxisX.y * localAxisY.z - localAxisX.z * localAxisY.y);


	double t = t_determinant / main_determinant;

	//Intersection point exists
	return { this, localAxisZ, t };
}



void Plane::setWidth(const double &_width) {
	width = _width;
}



void Plane::setLength(const double &_length) {
	length = _length;
}