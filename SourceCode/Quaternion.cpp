#include <algorithm>
#include "Quaternion.h"
#include "Vector3D.h"
#include <math.h>

Quaternion::Quaternion(const double _w, const double _x, const double _y, const double _z)
	: w(_w)
	, x(_x)
	, y(_y)
	, z(_z) 
{
}

Quaternion::~Quaternion()
{
}

Quaternion Quaternion::operator + (const Quaternion& q) const
{
	Quaternion _q;
	_q.w = w + q.w;
	_q.x = x + q.x;
	_q.y = y + q.y;
	_q.z = z + q.z;

	return _q;
}

Quaternion Quaternion::operator - (const Quaternion& q) const
{
	Quaternion _q;
	_q.w = w - q.w;
	_q.x = x - q.x;
	_q.y = y - q.y;
	_q.z = z - q.z;

	return _q;
}

Quaternion Quaternion::operator * (const Quaternion& q) const
{
	Quaternion _q;
	_q.w = w * q.w - x * q.x - y * q.y - z * q.z;
	_q.x = w * q.x + x * q.w + y * q.z - z * q.y;
	_q.y = w * q.y + y * q.w + z * q.x - x * q.z;
	_q.z = w * q.z + z * q.w + x * q.y - y * q.x;

	return _q;
}

Quaternion Quaternion::operator * (double scaler) const
{
	Quaternion _q;
	_q.w = w * scaler;
	_q.x = x * scaler;
	_q.y = y * scaler;
	_q.z = z * scaler;

	return _q;
}

Quaternion Quaternion::operator / (double divider) const
{
	Quaternion _q;
	_q.w = w / divider;
	_q.x = x / divider;
	_q.y = y / divider;
	_q.z = z / divider;

	return _q;
}

Quaternion Quaternion::operator % (const Quaternion& q) const
{
	return Quaternion(w * q.w, x * q.x, y * q.y, z * q.z);
}

bool Quaternion::operator == (const Quaternion& q) const
{
	if (std::abs(w - q.w) > DBL_EPSILON)
		return false;

	if (std::abs(x - q.x) > DBL_EPSILON)
		return false;

	if (std::abs(y - q.y) > DBL_EPSILON)
		return false;

	if (std::abs(z - q.z) > DBL_EPSILON)
		return false;

	return true;
}

bool Quaternion::operator != (const Quaternion& q) const
{
	return !(*this == q);
}

Quaternion Quaternion::operator * (const Vector3D& vec) const
{
	Quaternion _q;
	_q.w = - x * vec.x - y * vec.y - z * vec.z;
	_q.x = w * vec.x + y * vec.z - z * vec.y;
	_q.y = w * vec.y + z * vec.x - x * vec.z;
	_q.z = w * vec.z + x * vec.y - y * vec.x;

	return _q;
}

void Quaternion::normalize()
{
	*this = *this / length();
}

void Quaternion::invert()
{
	x = -x;
	y = -y;
	z = -z;
}

Quaternion Quaternion::getInverted() const
{
	return Quaternion(w, -x, -y, -z);
}

Quaternion Quaternion::getNormalized() const
{
	Quaternion q = *this;
	q = q / q.length();

	return q;
}

Vector3D Quaternion::getVector() const
{
	return Vector3D(x, y, z);
}

double Quaternion::length() const
{
	return sqrt(w * w + x * x + y * y + z * z);
}

double dot(const Quaternion& q1, const Quaternion& q2)
{
	return q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
}