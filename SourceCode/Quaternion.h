#pragma once

struct Vector3D;

struct Quaternion
{
	double w, x, y, z;


	Quaternion(const double &_w = 1, const double &_x = 0, const double &_y = 0, const double &_z = 0) : w(_w), x(_x), y(_y), z(_z) {}

	~Quaternion() {}


	Quaternion operator + (const Quaternion &q) const;

	Quaternion operator - (const Quaternion &q) const;

	//vector multiplication of quaternions
	Quaternion operator * (const Quaternion &q) const;

	Quaternion operator * (const double &scale) const;

	Quaternion operator / (const double &divider) const;

	//vector multiplication of quaternion by vector
	Quaternion operator * (const Vector3D &vec) const;

	//element-wise multiplication
	Quaternion operator % (const Quaternion &q) const;

	//absolute value of quaternion
	double length();

	//quaternion normalization
	void normalize();

	//return of a three-dimensional vector from a quaternion
	Vector3D getVector();

	//quaternion inversion
	void invert();

	Quaternion getInverted();

	Quaternion getNormalized();


	bool operator == (const Quaternion &q) const;
	bool operator != (const Quaternion &q) const;


	template <typename T>
	friend Quaternion operator * (const T scale, const Quaternion &q);
};


template <typename T>
Quaternion operator * (const T scale, const Quaternion &q) {
	Quaternion _q;
	_q.x = q.w * scale;
	_q.x = q.x * scale;
	_q.y = q.y * scale;
	_q.z = q.z * scale;

	return _q;
}


//scalar product of quaternions
double dot(const Quaternion &_vec1, const Quaternion &_vec2);
