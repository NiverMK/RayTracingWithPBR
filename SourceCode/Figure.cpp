#include "Figure.h"

Figure::Figure(const Vector3D& _position)
	: Object(_position)
{
}

Figure::~Figure()
{
}

void Figure::setMaterial(const Material& _material)
{
	material = _material;
}

const Material& Figure::getMaterial() const
{
	return material;
}

FigureData Figure::isFacingRay(FigureData& figureData, const Vector3D& ray)
{
	/* if figure is facing the ray */
	if (dot(ray.getInverted(), figureData.normal) > 0.0)
	{
		return figureData;
	}
	else if (material.isDoubleSided)
	{
		/* if figure has double-sided material */
		figureData.normal.invert();
		return figureData;
	}
	
	return FigureData();
}