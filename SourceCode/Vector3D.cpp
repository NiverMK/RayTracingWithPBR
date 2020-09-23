#include <algorithm>
#include <cmath>
#include "Vector3D.h"
#include "Quaternion.h"
#include "Constants.h"

Vector3D::Vector3D(const double _x)
	: x(_x)
	, y(_x)
	, z(_x)
{
}

Vector3D::Vector3D(const double _x, const double _y, const double _z)
	: x(_x)
	, y(_y)
	, z(_z)
{
}

Vector3D::~Vector3D()
{
}

Vector3D Vector3D::operator + (const Vector3D& vec) const
{
	Vector3D _vec;
	_vec.x = x + vec.x;
	_vec.y = y + vec.y;
	_vec.z = z + vec.z;

	return _vec;
}

Vector3D Vector3D::operator - (const Vector3D& vec) const
{
	Vector3D _vec;
	_vec.x = x - vec.x;
	_vec.y = y - vec.y;
	_vec.z = z - vec.z;

	return _vec;
}

Vector3D Vector3D::operator * (const Vector3D& vec) const
{
	Vector3D _vec;
	_vec.x = y * vec.z - z * vec.y;
	_vec.y = z * vec.x - x * vec.z;
	_vec.z = x * vec.y - y * vec.x;

	return _vec;
}

Vector3D Vector3D::operator * (double scaler) const
{
	Vector3D vec;
	vec.x = x * scaler;
	vec.y = y * scaler;
	vec.z = z * scaler;

	return vec;
}

Vector3D Vector3D::operator / (double divider) const
{
	Vector3D vec;
	vec.x = x / divider;
	vec.y = y / divider;
	vec.z = z / divider;

	return vec;
}

Vector3D Vector3D::operator % (const Vector3D& vec) const
{
	Vector3D _vec;

	_vec.x = x * vec.x;
	_vec.y = y * vec.y;
	_vec.z = z * vec.z;

	return _vec;
}

bool Vector3D::operator == (const Vector3D& vec) const
{
	if (std::abs(x - vec.x) > DBL_EPSILON)
		return false;

	if (std::abs(y - vec.y) > DBL_EPSILON)
		return false;

	if (std::abs(z - vec.z) > DBL_EPSILON)
		return false;

	return true;
}

bool Vector3D::operator != (const Vector3D& vec) const
{
	return (x != vec.x) || (y != vec.y) || (z != vec.z);
}

void Vector3D::normalize()
{
	*this = *this / this->length();
}

void Vector3D::invert()
{
	x = -x;
	y = -y; 
	z = -z;
}

Vector3D Vector3D::getInverted() const
{
	return Vector3D(-x, -y, -z);
}

Vector3D Vector3D::getNormalized() const
{
	Vector3D vec = *this;
	vec = vec / vec.length();

	return vec;
}

double Vector3D::length() const
{
	return sqrt(x * x + y * y + z * z);
}

Quaternion Vector3D::operator * (const Quaternion& q) const
{
	Quaternion _q;

	_q.w = - x * q.x - y * q.y - z * q.z;
	_q.x = x * q.w + y * q.z - z * q.y;
	_q.y = y * q.w + z * q.x - x * q.z;
	_q.z = z * q.w + x * q.y - y * q.x;

	return _q;
}

void Vector3D::rotateAroundVector(const Vector3D& vector, double angle)
{
	angle *= DEGREES_TO_RADS;
	double sinA = std::sin(angle / 2.0);

	/* rotation quaternions */
	Quaternion rotor(std::cos(angle / 2.0), vector.x * sinA, vector.y * sinA, vector.z * sinA);
	Quaternion rotor_inv = rotor.getInverted();

	/* rotate this vector */
	*this = (rotor * *this * rotor_inv).getVector();
}

double dot(const Vector3D& _vec1, const Vector3D& _vec2)
{
	return _vec1.x * _vec2.x + _vec1.y * _vec2.y + _vec1.z * _vec2.z;
}

Vector3D min(Vector3D vec, double min)
{
	if (vec.x < min)
		vec.x = min;

	if (vec.y < min)
		vec.y = min;

	if (vec.z < min)
		vec.z = min;

	return vec;
}

Vector3D max(Vector3D vec, double max)
{
	if (vec.x > max)
		vec.x = max;

	if (vec.y > max)
		vec.y = max;

	if (vec.z > max)
		vec.z = max;

	return vec;
}

Vector3D clamp(Vector3D vec, double min, double max)
{
	if (vec.x > max)
		vec.x = max;
	else if (vec.x < min)
		vec.x = min;

	if (vec.y > max)
		vec.y = max;
	else if (vec.y < min)
		vec.y = min;

	if (vec.z > max)
		vec.z = max;
	else if (vec.z < min)
		vec.z = min;

	return vec;
}
