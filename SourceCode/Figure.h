#pragma once

#include "Object.h"
#include "ReturnData.h"
#include "Material.h"

class Figure : public Object
{
public:
	Figure(const Vector3D& _position = Vector3D(0.0));
	virtual ~Figure();

	/* find intersection point of ray and object. 
	returns pointer to object, normal vector and scaler for ray to calculate point's coordinates */
	virtual FigureData isIntersectsRay(const Vector3D& origin, const Vector3D& direction) = 0;

	virtual void setMaterial(const Material& _material);
	virtual const Material& getMaterial() const;

protected:
	Material material;

	/* angle between casted ray and normal vector should be less than 90 degrees */
	FigureData isFacingRay(FigureData& figureData, const Vector3D& ray);
};