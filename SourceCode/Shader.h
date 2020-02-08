#pragma once

#include "Vector3D.h"
#include "PointLight.h"

#include <algorithm>
#include <vector>

#define PI 3.1415926

class Material;

class Shader
{
public:
	Material *material;
	
	Shader() { material = NULL; }
	Shader(Material *_material) { material = _material; }
	~Shader() {}

	//calculation of direct illumination of an object by point lights
	Vector3D calculateBlinnPhongLight(const Vector3D &pointPos, const Vector3D &viewVec, const Vector3D &normalvec, std::vector<PointLight *> &lights);

	//Diffuse Lighting Calculation
	Vector3D calculateDiffuseLight(const Vector3D &pointPos, const Vector3D &normalvec, std::vector<PointLight *> &lights);

	//calculation of indirect illumination
	Vector3D calculateReflection(const Vector3D &viewVec, const Vector3D &normalVec, const Vector3D &medianVec, const Vector3D &reflectVec, const Vector3D &L);


private:

	//Diffuse Lighting
	Vector3D Illumination(const Vector3D &dotPos, const Vector3D &objNormal, PointLight &light);

	//Fresnel-Shleik approximation for finding the Fresnel coefficient
	Vector3D Fresnel_Schlick(const Vector3D &medianVec, const Vector3D &viewVec, const Vector3D &F0, const double &roughness);

	//Normal distribution function
	double Trowbridge_Reitz_GGX(const Vector3D &normalVec, const Vector3D &medianVec, const double &roughness);

	//Geometry Function - Smith Method
	double Smith_GGX(const Vector3D &normalVec, const Vector3D &viewVec, const Vector3D &lightVec, const double &roughness);
	
	//Geometry Function
	double Schlick_GGX(const double &cosAlpha, const double &roughness);
};
