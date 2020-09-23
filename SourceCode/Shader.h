#pragma once

#include <vector>
#include "Vector3D.h"

class PointLight;
struct MaterialData;

class Shader
{
public:
	Shader();
	~Shader();

	/* calculation of direct illumination of an object with point light sources */
	Vector3D calculateBlinnPhongLight(const Vector3D& pointPos, const Vector3D& viewVec, const Vector3D& normalvec, const std::vector<PointLight*>& lights, const MaterialData& materialData) const;
	/* calculation of diffuse illumination. used for reflected objects */
	Vector3D calculateDiffuseLight(const Vector3D& pointPos, const Vector3D& normalvec, const std::vector<PointLight*>& lights, const MaterialData& materialData) const;
	/* function for reflections calculation */
	Vector3D calculateReflection(const Vector3D& viewVec, const Vector3D& normalVec, const Vector3D& reflectVec, const Vector3D& L, const MaterialData& materialData) const;

protected:
	/* diffuse lighting */
	Vector3D Illumination(const Vector3D& dotPos, const Vector3D& objNormal, const PointLight* light) const;
	/* Fresnel-Schleick approximation for finding the Fresnel coefficient for indirect illumination */
	Vector3D FresnelSchlick(const Vector3D& medianVec, const Vector3D& viewVec, const Vector3D& F0, double roughness) const;
	/* normal distribution function */
	double TrowbridgeReitzGGX(const Vector3D& normalVec, const Vector3D& medianVec, double roughness) const;
	/* Geometry Function - Smith Method */
	double SmithGGX_Direct(const Vector3D& normalVec, const Vector3D& viewVec, const Vector3D& lightVec, double roughness) const;
	/* Geometry function for direct illumination */
	double SchlickGGX_Direct(double cosAlpha, double roughness) const;
	/* Geometry function for indirect illumination - Smith Method */
	double SmithGGX_Indirect(const Vector3D &normalVec, const Vector3D &viewVec, const Vector3D &lightVec, double roughness) const;
	/* Geometry function for indirect illumination */
	double SchlickGGX_Indirect(double cosAlpha, double roughness)const;
};