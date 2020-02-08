#pragma once

#include <algorithm>

#include "Vector3D.h"
#include "Shader.h"

class Material
{
private:
	Vector3D albedo;	//[0...1]
	Vector3D F0;		//Fresnel coefficient [0...1]
	double metalness;	//[0...1]
	double roughness;	//[0...1]


public:
	//is material double-sided
	bool isDoubleSided;

	Shader shader;

	Material();
	~Material() {}

	//copy constructor
	Material(const Material &_material);
	//operator = overload
	Material &operator= (const Material &_material);


	void setAlbedo(const Vector3D &_albedo);
	void setMetalness(const double &_metalness);
	void setRoughness(const double &_roughness);

	const Vector3D &getAlbedo() { return albedo; }
	const Vector3D &getFresnel() { return F0; }
	const double &getMetalness() { return metalness; }
	const double &getRoughness() { return roughness; }
};