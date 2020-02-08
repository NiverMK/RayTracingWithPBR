#pragma once

#include "Object3D.h"
#include "Plane.h"


class Cube : public Object3D{

protected:
	//cube size
	double width;
	double height;
	double length;

	//sets the sizes of the cube's planes
	void setSizeOfPlanes();

	//move planes. Need to use this func after changing position or angle of the cube 
	void movePlanes();

public:

	/*cube's planes
	0,1 - OZ
	2,3 - OY
	4,5 - OX */
	Plane faces[6];
	

	Cube();
	Cube(const Vector3D &_position, const double &_width, const double &_length, const double &_height);
	~Cube() {}

	//function determines whether the ray crosses the object
	std::tuple<Object3D *, Vector3D, double> isIntersectsRay(const Vector3D &origin, const Vector3D &direction);
	
	//change position
	void setPosition(const Vector3D &_position);

	//rotation of an object around a vector "vector" by an angle "angle"
	void rotateAroundVector(const Vector3D &vector, double angle);

	//set one material for all planes
	void setMaterial(const Material &_material);
};

