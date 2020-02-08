#include "Shader.h"
#include "Material.h"


Vector3D Shader::calculateBlinnPhongLight(const Vector3D &pointPos, const Vector3D &viewVec, const Vector3D &normalVec, std::vector<PointLight *> &lights) {

	Vector3D colour(0, 0, 0);
	size_t num = lights.size() + 1;				//num of point lights

	Vector3D lightVec;							//direction vector from intersection point to light source
	Vector3D medianVec;							//micronormal

	for (auto &light : lights) {
		lightVec = (light->position - pointPos);
		medianVec = viewVec + lightVec;
		lightVec.normalize();
		medianVec.normalize();

		Vector3D L = Illumination(pointPos, normalVec, *light);					//lighting
		Vector3D F = Fresnel_Schlick(medianVec, viewVec, material->getFresnel(), material->getRoughness());			//Fresnel equation result
		double D = Trowbridge_Reitz_GGX(normalVec, medianVec, material->getRoughness());		//normal distribution function result
		double G = Smith_GGX(normalVec, lightVec, viewVec, material->getRoughness());	//geometry function result

		Vector3D Kd = (Vector3D(1.0) - F) * (1.0 - material->getMetalness());	//light absorption coefficient

		//Cook-Torrens reflection equation
		colour = colour + (Kd % material->getAlbedo() / PI + F * D * G / (4 * (std::max)(dot(viewVec, normalVec), 0.0) * (std::max)(dot(lightVec, normalVec), 0.0) + 0.001)) % L;
	}

	colour = colour / (double)num;

	return colour;
}



Vector3D Shader::calculateDiffuseLight(const Vector3D &pointPos, const Vector3D &normalVec, std::vector<PointLight *> &lights) {

	Vector3D colour(0, 0, 0);
	size_t num = lights.size() + 1;			//num of point lights

	for (auto &light : lights) {
		Vector3D L = Illumination(pointPos, normalVec, *light);		//lighting

		colour = colour + material->getAlbedo() % L;
	}

	colour = colour / (double)num;

	return colour;
}



Vector3D Shader::calculateReflection(const Vector3D &viewVec, const Vector3D &normalVec, const Vector3D &medianVec, const Vector3D &reflectVec, const Vector3D &L) {

	Vector3D F = Fresnel_Schlick(normalVec, viewVec, material->getFresnel(), material->getRoughness());		//Fresnel equation result
	double G = Smith_GGX(normalVec, reflectVec, viewVec, material->getRoughness());					//geometry function result

	Vector3D Kd = (Vector3D(1.0) - F) * (1.0 - material->getMetalness());			//light absorption coefficient

	//Cook-Torrens reflection equation
	Vector3D  colour = (Kd % material->getAlbedo() / PI + F * G / (4 * (std::max)(dot(viewVec, normalVec), 0.0) * (std::max)(dot(reflectVec, normalVec), 0.0))) % L;

	return colour;
}



Vector3D Shader::Illumination(const Vector3D &pointPos, const Vector3D &objNormal, PointLight &light) {

	Vector3D vecToLight = (light.position - pointPos);		//direction vector from intersection point to light source
	vecToLight.normalize();
	double cosAlpha = (std::max)(0.0, dot(objNormal, vecToLight));

	Vector3D illum = light.colour * light.getIntensity(pointPos) * cosAlpha;	//lighting

	return illum;
}


Vector3D Shader::Fresnel_Schlick(const Vector3D &medianVec, const Vector3D &viewVec, const Vector3D &F0, const double &roughness) {
	double cosAlpha = (std::max)(dot(medianVec, viewVec), 0.0);

	return F0 + (max(F0, 1.0 - roughness) - F0) * pow(1.0 - cosAlpha, 5);
}



double Shader::Trowbridge_Reitz_GGX(const Vector3D &normalVec, const Vector3D &medianVec, const double &roughness) {
	double alpha = roughness * roughness;
	double cosBeta = dot(normalVec, medianVec);

	double NDF = alpha / (PI * (cosBeta * cosBeta * (alpha - 1.0) + 1.0) * (cosBeta * cosBeta * (alpha - 1.0) + 1.0));

	return NDF;
}



double Shader::Schlick_GGX(const double &cosAlpha, const double &roughness) {

	double k = pow((roughness + 1.0), 2) / 8;

	return cosAlpha / (cosAlpha * (1.0 - k) + k);
}


double Shader::Smith_GGX(const Vector3D &normalVec, const Vector3D &viewVec, const Vector3D &lightVec, const double &roughness) {
	double cosAlpha = (std::max)(dot(normalVec, viewVec), 0.0);
	double cosBeta = (std::max)(dot(normalVec, lightVec), 0.0);

	return Schlick_GGX(cosAlpha, roughness) * Schlick_GGX(cosBeta, roughness);
}