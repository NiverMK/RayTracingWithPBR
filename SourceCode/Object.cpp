#include <cmath>
#include "Object.h"
#include "Constants.h"

Object::Object(const Vector3D& _position)
	: position(_position)
{
	localAxisX.x = 1.0;
	localAxisY.y = 1.0;
	localAxisZ.z = 1.0;
}

Object::~Object()
{
}

void Object::rotateAroundVector(const Vector3D& vector, double angle)
{
	angle *= DEGREES_TO_RADS;
	double sinA = std::sin(angle / 2.0);

	/* rotation quaternion */
	Quaternion rotor(std::cos(angle / 2.0), vector.x * sinA, vector.y * sinA, vector.z * sinA);
	Quaternion rotor_inv = rotor.getInverted();

	/* rotate local axes */
	localAxisX = (rotor * localAxisX * rotor_inv).getVector();
	localAxisY = (rotor * localAxisY * rotor_inv).getVector();
	localAxisZ = (rotor * localAxisZ * rotor_inv).getVector();
}

void Object::rotateAroundLocalX(double angle)
{
	angle *= DEGREES_TO_RADS;
	double sinA = std::sin(angle / 2.0);

	/* rotation quaternion */
	Quaternion rotor(std::cos(angle / 2.0), localAxisX.x * sinA, localAxisX.y * sinA, localAxisX.z * sinA);
	Quaternion rotor_inv = rotor.getInverted();

	/* rotate local axes */
	localAxisY = (rotor * localAxisY * rotor_inv).getVector();
	localAxisZ = (rotor * localAxisZ * rotor_inv).getVector();
}

void Object::rotateAroundLocalY(double angle)
{
	angle *= DEGREES_TO_RADS;
	double sinA = std::sin(angle / 2.0);

	/* rotation quaternion */
	Quaternion rotor(std::cos(angle / 2.0), localAxisY.x * sinA, localAxisY.y * sinA, localAxisY.z * sinA);
	Quaternion rotor_inv = rotor.getInverted();

	/* rotate local axes */
	localAxisX = (rotor * localAxisX * rotor_inv).getVector();
	localAxisZ = (rotor * localAxisZ * rotor_inv).getVector();
}

void Object::rotateAroundLocalZ(double angle)
{
	angle *= DEGREES_TO_RADS;
	double sinA = std::sin(angle / 2.0);

	/* rotation quaternion */
	Quaternion rotor(std::cos(angle / 2.0), localAxisZ.x * sinA, localAxisZ.y * sinA, localAxisZ.z * sinA);
	Quaternion rotor_inv = rotor.getInverted();

	/* rotate local axes */
	localAxisX = (rotor * localAxisX * rotor_inv).getVector();
	localAxisY = (rotor * localAxisY * rotor_inv).getVector();
}

void Object::setPosition(const Vector3D& _position)
{
	position = _position;
}

const Vector3D& Object::getPosition() const
{
	return position;
}