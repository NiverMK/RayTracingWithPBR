#include "Object3D.h"

#define PI 3.1415926


Object3D::Object3D() {

	localAxisX.x = 1;

	localAxisY.y = 1;

	localAxisZ.z = 1;
}



void Object3D::rotateAroundVector(const Vector3D &vector, double angle) {

	angle *= PI / 180;
	double sinA = sin(angle / 2);

	//quaternions of rotation
	Quaternion rotor(cos(angle / 2), vector.x * sinA, vector.y * sinA, vector.z * sinA);
	Quaternion rotor_inv = rotor.getInverted();

	//rotate local axes
	localAxisX = (rotor * localAxisX * rotor_inv).getVector();
	localAxisY = (rotor * localAxisY * rotor_inv).getVector();
	localAxisZ = (rotor * localAxisZ * rotor_inv).getVector();
}
