#pragma once

struct Quaternion;

struct Vector3D 
{
	double x, y, z;

	Vector3D(const double _x = 0.0);
	Vector3D(const double _x, const double _y, const double _z);
	~Vector3D();

	/* sum of vectors */
	Vector3D operator + (const Vector3D& vec) const;
	/* subtraction of vector */
	Vector3D operator - (const Vector3D& vec) const;
	/* cross of vectors */
	Vector3D operator * (const Vector3D& vec) const;
	/* scale vector */
	Vector3D operator * (double scaler) const;
	/* divide vector */
	Vector3D operator / (double divider) const ;
	/* multiply vector by quaternion */
	Quaternion operator * (const Quaternion& q) const ;
	/* multiply elements of two vectors */
	Vector3D operator % (const Vector3D& vec) const;

	/* comparison operators */
	bool operator == (const Vector3D& vec) const;
	bool operator != (const Vector3D& vec) const;

	/* normalize self */
	void normalize();
	/* invert self */
	void invert();
	/* return inverted vector */
	Vector3D getInverted() const;
	/* return normalized vector */
	Vector3D getNormalized() const;
	/* absolute value of the vector */
	double length() const;

	/* rotate this vector around another vector by angle */
	void rotateAroundVector(const Vector3D& vector, double angle);

	/* multiply quaternion by number */
	template <typename T>
	friend Vector3D operator * (T scale, const Vector3D& vec);
};

/* dot product of vectors */
double dot(const Vector3D& _vec1, const Vector3D& _vec2);
//like std::min, but for vector
Vector3D min(Vector3D vec, double min);
//like std::max, but for vector
Vector3D max(Vector3D vec, double max);
//like std::clamp, but for vector
Vector3D clamp(Vector3D vec, const double min, double max);

template <typename T>
Vector3D operator * (T scale, const Vector3D& vec)
{
	Vector3D _vec;
	_vec.x = vec.x * scale;
	_vec.y = vec.y * scale;
	_vec.z = vec.z * scale;

	return _vec;
}