#include "Cube.h"


Cube::Cube() {
	width = 1.0; 
	height = 1.0; 
	length = 1.0;

	//set size of planes
	setSizeOfPlanes();
	//move planes to the necessary positions
	movePlanes();

	//rotate planes
	faces[1].rotateAroundVector(localAxisX, 180);
	faces[2].rotateAroundVector(localAxisX, -90);
	faces[3].rotateAroundVector(localAxisX, 90);
	faces[4].rotateAroundVector(localAxisY, 90);
	faces[5].rotateAroundVector(localAxisY, -90);
}



Cube::Cube(const Vector3D &_position, const double &_width, const double &_length, const double &_height) {
	position = _position;
	width = _width;
	height = _height;
	length = _length;

	//set size of planes
	setSizeOfPlanes();
	//move planes to the necessary positions
	movePlanes();

	//rotate planes
	faces[1].rotateAroundVector(localAxisX, 180);
	faces[2].rotateAroundVector(localAxisX, -90);
	faces[3].rotateAroundVector(localAxisX, 90);
	faces[4].rotateAroundVector(localAxisY, 90);
	faces[5].rotateAroundVector(localAxisY, -90);
}



std::tuple<Object3D *, Vector3D, double> Cube::isIntersectsRay(const Vector3D &origin, const Vector3D &direction) {

	double t = DBL_MAX;		//ray multiplier = max double value

	Object3D *pointer;	//object pointer

	Vector3D normal;	//normal vector

	for (int i = 0; i < 6; i++) {
		auto [_pointer, _normal, _t] = this->faces[i].isIntersectsRay(origin, direction);

		if (_t < t && _t > 0) {	
			pointer = _pointer;
			normal = _normal;
			t = _t;
		}
	}

	//if multiplier != DBL_MAX, intersection point was found
	if (t != DBL_MAX) {
		return { pointer, normal, t };
	}
	else {
		//there is no intersection point
		return { NULL, normal, -1.0 };
	}
}



void Cube::setMaterial(const Material &_material) {
	for (int i = 0; i < 6; i++) {
		faces[i].setMaterial(_material);
	}
}



void Cube::setPosition(const Vector3D &_position) {
	position = _position;

	movePlanes();
}



void Cube::rotateAroundVector(const Vector3D &vector, double angle) {

	Object3D::rotateAroundVector(vector, angle);

	for (int i = 0; i < 6; i++) {
		faces[i].rotateAroundVector(vector, angle);
	}

	movePlanes();
}



void Cube::setSizeOfPlanes() {
	faces[0].setWidth(width);
	faces[0].setLength(length);
	faces[1].setWidth(width);
	faces[1].setLength(length);

	faces[2].setWidth(width);
	faces[2].setLength(height);
	faces[3].setWidth(width);
	faces[3].setLength(height);

	faces[4].setWidth(height);
	faces[4].setLength(length);
	faces[5].setWidth(height);
	faces[5].setLength(length);
}



void Cube::movePlanes() {
	faces[0].setPosition(position + localAxisZ * height / 2.0);
	faces[1].setPosition(position - localAxisZ * height / 2.0);

	faces[2].setPosition(position + localAxisY * length / 2.0);
	faces[3].setPosition(position - localAxisY * length / 2.0);

	faces[4].setPosition(position + localAxisX * width / 2.0);
	faces[5].setPosition(position - localAxisX * width / 2.0);
}
