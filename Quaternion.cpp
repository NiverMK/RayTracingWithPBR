#include "Quaternion.h"
#include "Vector3D.h"
#include <math.h>

Quaternion Quaternion :: operator + (const Quaternion &q) const {
	Quaternion _q;
	_q.w = w + q.w;
	_q.x = x + q.x;
	_q.y = y + q.y;
	_q.z = z + q.z;

	return _q;
}


Quaternion Quaternion :: operator - (const Quaternion &q) const {
	Quaternion _q;
	_q.w = w - q.w;
	_q.x = x - q.x;
	_q.y = y - q.y;
	_q.z = z - q.z;

	return _q;
}


Quaternion Quaternion :: operator * (const Quaternion &q) const {
	Quaternion _q;
	_q.w = w * q.w - x * q.x - y * q.y - z * q.z;
	_q.x = w * q.x + x * q.w + y * q.z - z * q.y;
	_q.y = w * q.y + y * q.w + z * q.x - x * q.z;
	_q.z = w * q.z + z * q.w + x * q.y - y * q.x;

	return _q;
}


Quaternion Quaternion :: operator * (const double &scale) const {
	Quaternion _q;
	_q.w = w * scale;
	_q.x = x * scale;
	_q.y = y * scale;
	_q.z = z * scale;

	return _q;
}


Quaternion Quaternion :: operator / (const double &divider) const {
	Quaternion _q;
	_q.w = w / divider;
	_q.x = x / divider;
	_q.y = y / divider;
	_q.z = z / divider;

	return _q;
}


Quaternion Quaternion::operator % (const Quaternion &q) const {
	return Quaternion(w * q.w, x * q.x, y * q.y, z * q.z);
}


bool Quaternion::operator == (const Quaternion &q) const {
	return (w == q.w) && (x == q.x) && (y == q.y) && (z == q.z);
}


bool Quaternion::operator != (const Quaternion &q) const {
	return (w != q.w) || (x != q.x) || (y != q.y) || (z != q.z);
}


Quaternion Quaternion :: operator * (const Vector3D &vec) const {
	Quaternion _q;
	_q.w = - x * vec.x - y * vec.y - z * vec.z;
	_q.x = w * vec.x + y * vec.z - z * vec.y;
	_q.y = w * vec.y + z * vec.x - x * vec.z;
	_q.z = w * vec.z + x * vec.y - y * vec.x;

	return _q;
}


Vector3D Quaternion::getVector() {
	return Vector3D(x, y, z);
}


double Quaternion::length() {
	return sqrt(w * w + x * x + y * y + z * z);
}


void Quaternion::normalize() {
	*this = *this / this->length();
}

void Quaternion::invert() {
	x = -x;
	y = -y;
	z = -z;
}


Quaternion Quaternion::getInverted() {

	return Quaternion(w, -x, -y, -z);
}


Quaternion Quaternion::getNormalized() {
	Quaternion q = *this;
	q = q / q.length();
	return q;
}


double dot(const Quaternion &_q1, const Quaternion &_q2) {
	return _q1.w * _q2.w + _q1.x * _q2.x + _q1.y * _q2.y + _q1.z * _q2.z;
}