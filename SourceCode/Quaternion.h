#pragma once

struct Vector3D;

struct Quaternion
{
	double w, x, y, z;

	Quaternion(const double _w = 1.0, const double _x = 0.0, const double _y = 0.0, const double _z = 0.0);
	~Quaternion();

	/* sum of quaternions */
	Quaternion operator + (const Quaternion& q) const;
	/* subtraction of quaternion */
	Quaternion operator - (const Quaternion& q) const;
	/* cross of quaternions */
	Quaternion operator * (const Quaternion& q) const;
	/* scale quaternion */
	Quaternion operator * (double scaler) const;
	/* divide quaternion */
	Quaternion operator / (double divider) const;
	/* multiply quaternion by vector */
	Quaternion operator * (const Vector3D& vec) const;
	/* multiply elements of two quaternions */
	Quaternion operator % (const Quaternion& q) const;

	/* comparison operators */
	bool operator == (const Quaternion& q) const;
	bool operator != (const Quaternion& q) const;

	/* normalize self */
	void normalize();
	/* invert self */
	void invert();
	/* return inverted quaternion */
	Quaternion getInverted() const;
	/* return normalized quaternion */
	Quaternion getNormalized() const;
	/* return Vector3D(x, y, z)*/
	Vector3D getVector() const;
	/* absolute value of the quaternion */
	double length() const;

	/* multiply quaternion by number */
	template <typename T>
	friend Quaternion operator * (T scale, const Quaternion& q);
};

/* dot product of quaternions */
double dot(const Quaternion& q1, const Quaternion& q2);

template <typename T>
Quaternion operator * (T scale, const Quaternion& q)
{
	Quaternion _q;
	_q.x = q.w * scale;
	_q.x = q.x * scale;
	_q.y = q.y * scale;
	_q.z = q.z * scale;

	return _q;
}