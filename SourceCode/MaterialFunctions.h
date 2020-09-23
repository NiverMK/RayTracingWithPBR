#pragma once

#include <cmath>
#include <algorithm>
#include "ReturnData.h"
#include "Constants.h"

inline MaterialData baseMaterialFunction(double u, double v)
{
	return { Vector3D(1.0), 1.0, 1.0, Vector3D(FRESNEL_MIN) };
}

inline MaterialData checkerMaterialFunction(double u, double v)
{
	MaterialData materialData(Vector3D(0.0), 1.0, 0.0);

	int32_t pointU = static_cast<int32_t>(u / 2.0);
	int32_t pointV = static_cast<int32_t>(v / 2.0);

	if (pointU % 2 == 0)
	{
		if (pointV % 2 != 0)
		{
			materialData.albedo = Vector3D(1.0);
			materialData.roughness = 1.0;
			materialData.metalness = 0.0;
		}
	}
	else
	{
		if (pointV % 2 == 0)
		{
			materialData.albedo = Vector3D(1.0);
			materialData.roughness = 1.0;
			materialData.metalness = 0.0;
		}
	}

	return materialData;
}

inline MaterialData traceryMaterialFunction(double u, double v)
{
	MaterialData materialData(Vector3D(1.0), 1.0, 1.0);

	double sinU = std::sin(4 * u);
	double sinV = std::sin(4 * v);
	double cosU = std::cos(2 * (4 * u + PI / 2.0));
	double cosV = std::cos(2 * (4 * v + PI / 2.0));

	double result = sinU - cosU + sinV - cosV;

	materialData.albedo = result > 0.9 ? Vector3D(0.25) : Vector3D(1.0);

	return materialData;
}

inline MaterialData rainbowMaterialFunction(double u, double v)
{
	MaterialData materialData(Vector3D(1.0), 1.0, 1.0);

	double diff = (u - v) / 2.0;
	double sinR = std::sin(diff);
	double sinG = std::sin(diff - PI * 0.6666);
	double sinB = std::sin(diff + PI * 0.6666);

	materialData.albedo = min(Vector3D(sinR, sinG, sinB), 0.0);

	return materialData;
}

inline MaterialData roughnessMaterialFunction(double u, double v)
{
	MaterialData materialData(Vector3D(1.0), 1.0, 1.0);
	materialData.roughness = 2 * std::sin(4 * (v + u)) / 2.0 + 0.5;
	
	return materialData;
}