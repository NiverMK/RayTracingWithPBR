#include "Material.h"
#include "Constants.h"
#include "MaterialFunctions.h"

Material::Material()
{ 
	albedo = Vector3D(1.0); 
	metalness = 1.0; 
	roughness = 1.0; 
	isDoubleSided = false;
	materialFunction = &baseMaterialFunction;
}

Material::~Material()
{
}

Material::Material(const Material &_material)
{
	albedo = _material.albedo;
	metalness = _material.metalness;
	roughness = _material.roughness;
	isDoubleSided = _material.isDoubleSided;
	materialFunction = _material.materialFunction;
}

Material& Material::operator = (const Material& _material)
{
	albedo = _material.albedo;
	metalness = _material.metalness;
	roughness = _material.roughness;
	isDoubleSided = _material.isDoubleSided;
	materialFunction = _material.materialFunction;

	return *this;
}

MaterialData Material::getParameters(double u, double v) const
{
	MaterialData materialData = materialFunction(u, v);

	materialData.albedo = materialData.albedo % albedo;
	materialData.metalness = materialData.metalness * metalness;
	materialData.roughness = std::clamp(materialData.roughness * roughness, ROUGHNESS_MIN, 1.0);
	materialData.Fresnel = clamp(materialData.albedo * materialData.metalness, FRESNEL_MIN, 1.0);

	return materialData;
}