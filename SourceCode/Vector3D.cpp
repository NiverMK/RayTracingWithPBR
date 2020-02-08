#include <math.h>
#include "Vector3D.h"
#include "Quaternion.h"

Vector3D Vector3D::operator + (const Vector3D &vec) const {
	Vector3D _vec;
	_vec.x = x + vec.x;
	_vec.y = y + vec.y;
	_vec.z = z + vec.z;

	return _vec;
}


Vector3D Vector3D::operator - (const Vector3D &vec) const {
	Vector3D _vec;
	_vec.x = x - vec.x;
	_vec.y = y - vec.y;
	_vec.z = z - vec.z;

	return _vec;
}


Vector3D Vector3D::operator * (const Vector3D &vec) const {
	Vector3D _vec;

	_vec.x = y * vec.z - z * vec.y;
	_vec.y = z * vec.x - x * vec.z;
	_vec.z = x * vec.y - y * vec.x;

	return _vec;
}


Vector3D Vector3D::operator * (const double &scale) const {
	Vector3D vec;
	vec.x = x * scale;
	vec.y = y * scale;
	vec.z = z * scale;

	return vec;
}


Vector3D Vector3D::operator / (const double &scale) const {
	Vector3D vec;
	vec.x = x / scale;
	vec.y = y / scale;
	vec.z = z / scale;

	return vec;
}


Vector3D Vector3D :: operator % (const Vector3D &vec) const {
	Vector3D _vec;

	_vec.x = x * vec.x;
	_vec.y = y * vec.y;
	_vec.z = z * vec.z;

	return _vec;
}


bool Vector3D::operator == (const Vector3D &vec) const {
	return (x == vec.x) && (y == vec.y) && (z == vec.z);
}


bool Vector3D::operator != (const Vector3D &vec) const {
	return (x != vec.x) || (y != vec.y) || (z != vec.z);
}


double Vector3D::length() {
	return sqrt(x * x + y * y + z * z);
}


void Vector3D::normalize() {
	*this = *this / this->length();
}


void Vector3D::invert() {
	x = -x;
	y = -y; 
	z = -z;
}


Vector3D Vector3D::getInverted() {
	return Vector3D(-x, -y, -z);
}


Vector3D Vector3D::getNormalized() {
	Vector3D vec = *this;
	vec = vec / vec.length();
	return vec;
}


Quaternion Vector3D::operator * (const Quaternion &q) const {
	Quaternion _q;

	_q.w = - x * q.x - y * q.y - z * q.z;
	_q.x = x * q.w + y * q.z - z * q.y;
	_q.y = y * q.w + z * q.x - x * q.z;
	_q.z = z * q.w + x * q.y - y * q.x;

	return _q;
}


void Vector3D::rotateAroundVector(const Vector3D &vector, double angle) {

	angle *= 3.1415926 / 180;
	double sinA = sin(angle / 2);

	//quaternions of rotation
	Quaternion rotor(cos(angle / 2), vector.x * sinA, vector.y * sinA, vector.z * sinA);
	Quaternion rotor_inv = rotor.getInverted();

	//rotation
	*this = (rotor * *this * rotor_inv).getVector();
}


Vector3D min(const Vector3D &vec, const double &min) {
	Vector3D _vec = vec;

	if (_vec.x < min)
		_vec.x = min;
	if (_vec.y < min)
		_vec.y = min;
	if (_vec.y z min)
		_vec.z = min;

	return _vec;
}


Vector3D max(const Vector3D &vec, const double &max) {
	Vector3D _vec = vec;

	if (_vec.x > max)
		_vec.x = max;
	if (_vec.y > max)
		_vec.y = max;
	if (_vec.z > max)
		_vec.z = max;

	return _vec;
}


Vector3D clamp(const Vector3D &vec, const double &min, const double &max) {
	Vector3D _vec = vec;

	if (_vec.x > max)
		_vec.x = max;
	if (_vec.y > max)
		_vec.y = max;
	if (_vec.z > max)
		_vec.z = max;

	if (_vec.x < min)
		_vec.x = min;
	if (_vec.y < min)
		_vec.y = min;
	if (_vec.z < min)
		_vec.z = min;

	return _vec;
}


double dot(const Vector3D &_vec1, const Vector3D &_vec2) {
	return _vec1.x * _vec2.x + _vec1.y * _vec2.y + _vec1.z * _vec2.z;
}


