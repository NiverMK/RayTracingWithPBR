#pragma once

#include <algorithm>
#include "Vector3D.h"
#include "Constants.h"

class Figure;

struct FigureData
{
	FigureData(Figure* _figure = nullptr, const Vector3D& _normal = Vector3D(), double _scaler = DBL_MAX, double _u = 0.0, double _v = 0.0)
		: figure(_figure)
		, normal(_normal)
		, scaler(_scaler)
		, u(_u)
		, v(_v)
	{
	}

	Figure* figure;
	Vector3D normal;
	double scaler;
	double u;
	double v;
};

struct MaterialData
{
	MaterialData(const Vector3D& _albedo = Vector3D(1.0), double _metalness = 1.0, double _roughness = 1.0, const Vector3D& _Fresnel = Vector3D(FRESNEL_MIN))
		: albedo(_albedo)
		, metalness(_metalness)
		, roughness(_roughness)
		, Fresnel(_Fresnel)
	{
	}

	/* [0.0 ... 1.0] */
	Vector3D albedo;
	double metalness;
	double roughness;
	/* [0.04 ... 1.0] */
	Vector3D Fresnel;
};