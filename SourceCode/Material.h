#pragma once

#include <algorithm>
#include "Vector3D.h"
#include "Shader.h"
#include "ReturnData.h"

class Material
{
public:
	/* [0.0 ... 1.0] */
	Vector3D albedo;
	double metalness;
	double roughness;

	Material();
	~Material();

	Material(const Material& _material);

	Material& operator = (const Material& _material);

	MaterialData getParameters(double u = 0.0, double v = 0.0) const;

	Shader shader;
	bool isDoubleSided;
	MaterialData(*materialFunction)(double u, double v) = nullptr;
};