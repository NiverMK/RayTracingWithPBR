#include "Box.h"

Box::Box(const Vector3D& _position, double size)
	: Figure(_position)
	, width(size)
	, height(size)
	, length(size)
{
	/* create a box from planes */
	setSizeOfPlanes();
	movePlanes();

	/* rotate planes to set normal's direction outside from cube */
	faces[1].rotateAroundVector(localAxisX, 180);
	faces[2].rotateAroundVector(localAxisX, -90);
	faces[3].rotateAroundVector(localAxisX, 90);
	faces[4].rotateAroundVector(localAxisY, 90);
	faces[5].rotateAroundVector(localAxisY, -90);
}

Box::Box(const Vector3D& _position, double _width, double _length, double _height)
	: Figure(_position)
	, width(_width)
	, height(_height)
	, length(_length)
{
	/* create a box from planes */
	setSizeOfPlanes();
	movePlanes();

	/* rotate planes to set normal's direction outside from box */
	faces[1].rotateAroundVector(localAxisX, 180);
	faces[2].rotateAroundVector(localAxisX, -90);
	faces[3].rotateAroundVector(localAxisX, 90);
	faces[4].rotateAroundVector(localAxisY, 90);
	faces[5].rotateAroundVector(localAxisY, -90);
}

FigureData Box::isIntersectsRay(const Vector3D& origin, const Vector3D& direction)
{
	FigureData output;

	for (int i = 0; i < 6; i++)
	{
		/* search for intersections with all cube's planes */
		FigureData data = this->faces[i].isIntersectsRay(origin, direction);

		if (data.scaler < output.scaler)
		{
			output = data;
		}
	}
	
	return output;
}

void Box::setPosition(const Vector3D& _position)
{
	position = _position;

	movePlanes();
}

void Box::rotateAroundLocalX(double angle)
{
	Object::rotateAroundLocalX(angle);
	movePlanes();
}

void Box::rotateAroundLocalY(double angle)
{
	Object::rotateAroundLocalY(angle);
	movePlanes();
}

void Box::rotateAroundLocalZ(double angle)
{
	Object::rotateAroundLocalZ(angle);
	movePlanes();
}

void Box::rotateAroundVector(const Vector3D& vector, double angle)
{
	/* rotate local vectors */
	Object::rotateAroundVector(vector, angle);

	/* rotate planes */
	for (int i = 0; i < 6; i++)
	{
		faces[i].rotateAroundVector(vector, angle);
	}

	movePlanes();
}

void Box::setSizeOfPlanes()
{
	faces[0].width = width;
	faces[0].length = length;
	faces[1].width = width;
	faces[1].length = length;

	faces[2].width = width;
	faces[2].length = height;
	faces[3].width = width;
	faces[3].length = height;

	faces[4].width = height;
	faces[4].length = length;
	faces[5].width = height;
	faces[5].length = length;
}

void Box::movePlanes()
{
	faces[0].setPosition(position + localAxisZ * height / 2.0);
	faces[1].setPosition(position - localAxisZ * height / 2.0);

	faces[2].setPosition(position + localAxisY * length / 2.0);
	faces[3].setPosition(position - localAxisY * length / 2.0);

	faces[4].setPosition(position + localAxisX * width / 2.0);
	faces[5].setPosition(position - localAxisX * width / 2.0);
}

void Box::setMaterial(const Material& _material)
{
	material = _material;

	for (int i = 0; i < 6; i++)
	{
		faces[i].setMaterial(material);
	}
}