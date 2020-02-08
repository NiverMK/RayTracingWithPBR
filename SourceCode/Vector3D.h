#pragma once

struct Quaternion;


struct Vector3D {

	double x, y, z;

	Vector3D(const double &_x = 0) : x(_x), y(_x), z(_x) {}
	Vector3D(const double &_x, const double &_y, const double &_z) : x(_x), y(_y), z(_z) {}
	
	~Vector3D() {}


	Vector3D operator + (const Vector3D &vec) const;

	Vector3D operator - (const Vector3D &vec) const;

	//vector multiplication
	Vector3D operator * (const Vector3D &vec) const;

	Vector3D operator * (const double &scale) const;

	Vector3D operator / (const double &scale) const ;


	//vector multiplication of vector by quaternion
	Quaternion operator * (const Quaternion &q) const ;

	//element-wise multiplication
	Vector3D operator % (const Vector3D &vec) const;

	//absolute value of vector
	double length();

	//vector normalization
	void normalize();

	//vector inversion
	void invert();

	Vector3D getInverted();

	Vector3D getNormalized();

	//rotate vector around another vector
	void rotateAroundVector(const Vector3D &vector, double angle);

	bool operator == (const Vector3D &vec) const;
	bool operator != (const Vector3D &vec) const;


	template <typename T>
	friend Vector3D operator * (const T scale, const Vector3D &vec);
};

template <typename T>
Vector3D operator * (const T scale, const Vector3D &vec) {
	Vector3D _vec;
	_vec.x = vec.x * scale;
	_vec.y = vec.y * scale;
	_vec.z = vec.z * scale;

	return _vec;
}


//scalar product of vectors
double dot(const Vector3D &_vec1, const Vector3D &_vec2);

Vector3D min(const Vector3D &_vec, const double &min);

Vector3D max(const Vector3D &_vec, const double &max);

Vector3D clamp(const Vector3D &_vec, const double &min, const double &max);
