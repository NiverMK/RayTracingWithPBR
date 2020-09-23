#include <algorithm>
#include <cmath>
#include "Shader.h"
#include "PointLight.h"
#include "Constants.h"
#include "ReturnData.h"

Shader::Shader()
{
}

Shader::~Shader()
{
}

Vector3D Shader::calculateBlinnPhongLight(const Vector3D& pointPos, const Vector3D& viewVec, const Vector3D& normalVec, const std::vector<PointLight*>& lights, const MaterialData& materialData) const
{
	Vector3D colour(0.0);

	Vector3D lightVec;		/* from point on object to light source */
	Vector3D medianVec;		/* vector between vector from point to camera and from point to light source */

	for (const PointLight* light : lights)
	{
		lightVec = (light->getPosition() - pointPos);
		medianVec = viewVec + lightVec;
		lightVec.normalize();
		medianVec.normalize();

		Vector3D L = Illumination(pointPos, normalVec, light);
		Vector3D F = FresnelSchlick(medianVec, viewVec, materialData.Fresnel, materialData.roughness);
		double D = TrowbridgeReitzGGX(normalVec, medianVec, materialData.roughness);
		double G = SmithGGX_Direct(normalVec, lightVec, viewVec, materialData.roughness);

		Vector3D Kd = (Vector3D(1.0) - F) * (1.0 - materialData.metalness);
		colour = colour + (Kd % materialData.albedo / PI + F * D * G / (4 * std::max(dot(viewVec, normalVec), 0.0) * std::max(dot(lightVec, normalVec), 0.0) + 0.001)) % L;
	}

	return colour;
}

Vector3D Shader::calculateDiffuseLight(const Vector3D& pointPos, const Vector3D& normalVec, const std::vector<PointLight*>& lights, const MaterialData& materialData) const
{
	Vector3D colour(0.0);

	for (const PointLight* light : lights)
	{
		Vector3D L = Illumination(pointPos, normalVec, light);
		colour = colour + materialData.albedo % L;
	}

	return colour;
}

Vector3D Shader::calculateReflection(const Vector3D& viewVec, const Vector3D& normalVec, const Vector3D& reflectVec, const Vector3D& L, const MaterialData& materialData) const
{
	Vector3D F = FresnelSchlick(normalVec, viewVec, materialData.Fresnel, materialData.roughness);
	/* gives white edges of metal speheres
	double G = SmithGGX_Indirect(normalVec, reflectVec, viewVec, materialData.roughness);
	*/
	double G = SmithGGX_Direct(normalVec, reflectVec, viewVec, materialData.roughness);
	Vector3D Kd = (Vector3D(1.0) - F) * (1.0 - materialData.metalness);

	Vector3D  colour = (Kd % materialData.albedo + F * G / (4 * std::max(dot(viewVec, normalVec), 0.0) * std::max(dot(reflectVec, normalVec), 0.0) + 0.001)) % L;

	return colour;
}

Vector3D Shader::Illumination(const Vector3D& pointPos, const Vector3D& objNormal, const PointLight* light) const
{
	Vector3D vecToLight = (light->getPosition() - pointPos);
	vecToLight.normalize();
	double cosAlpha = std::max(0.0, dot(objNormal, vecToLight));

	Vector3D illumination = light->colour * light->getIntensity(pointPos) * cosAlpha;

	return illumination;
}

Vector3D Shader::FresnelSchlick(const Vector3D& medianVec, const Vector3D& viewVec, const Vector3D& F0, double roughness) const
{
	double cosAlpha = std::max(dot(medianVec, viewVec), 0.0);

	return F0 + (max(F0, 1.0 - roughness) - F0) * std::pow(1.0 - cosAlpha, 5);
}

double Shader::TrowbridgeReitzGGX(const Vector3D& normalVec, const Vector3D& medianVec, double roughness) const
{
	double alpha = roughness * roughness;
	double cosBeta = dot(normalVec, medianVec);
	double NDF = alpha / (PI * (cosBeta * cosBeta * (alpha - 1.0) + 1.0) * (cosBeta * cosBeta * (alpha - 1.0) + 1.0));

	return NDF;
}

double Shader::SchlickGGX_Direct(double cosAlpha, double roughness) const
{
	double k = std::pow((roughness + 1.0), 2) / 8;

	return cosAlpha / (cosAlpha * (1.0 - k) + k);
}

double Shader::SmithGGX_Direct(const Vector3D &normalVec, const Vector3D &viewVec, const Vector3D &lightVec, double roughness) const
{
	double cosAlpha = std::max(dot(normalVec, viewVec), 0.0);
	double cosBeta = std::max(dot(normalVec, lightVec), 0.0);

	return SchlickGGX_Direct(cosAlpha, roughness) * SchlickGGX_Direct(cosBeta, roughness);
}

double Shader::SchlickGGX_Indirect(const double cosAlpha, const double roughness) const
{
	double k = roughness * roughness / 2;

	return cosAlpha / (cosAlpha * (1.0 - k) + k);
}

double Shader::SmithGGX_Indirect(const Vector3D& normalVec, const Vector3D& viewVec, const Vector3D& lightVec, double roughness) const
{
	double cosAlpha = std::max(dot(normalVec, viewVec), 0.0);
	double cosBeta = std::max(dot(normalVec, lightVec), 0.0);

	return SchlickGGX_Indirect(cosAlpha, roughness) * SchlickGGX_Indirect(cosBeta, roughness);
}