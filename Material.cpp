#include "Material.h"


Material::Material() { 

	albedo = Vector3D(0.5); 
	F0 = Vector3D(0.04); 
	metalness = 0.0; 
	roughness = 0.5; 

	isDoubleSided = false; 

	shader.material = this; 
}



Material::Material(const Material &_material) {
	albedo = _material.albedo;
	F0 = _material.F0;
	metalness = _material.metalness;
	roughness = _material.roughness;
	isDoubleSided = _material.isDoubleSided;
}



Material& Material::operator= (const Material &_material) {
	albedo = _material.albedo;
	F0 = _material.F0;
	metalness = _material.metalness;
	roughness = _material.roughness;
	isDoubleSided = _material.isDoubleSided;

	return *this;
}



void Material::setAlbedo(const Vector3D &_albedo) {
	albedo = _albedo;


	F0 = clamp(albedo * metalness, 0.04, 1.0);
}



void Material::setMetalness(const double &_metalness) {
	metalness = (std::clamp)(_metalness, 0.0, 1.0);

	F0 = clamp(albedo * metalness, 0.04, 1.0);
}



void Material::setRoughness(const double &_roughness) {
	roughness = (std::clamp)(_roughness, 0.01, 1.0);
}