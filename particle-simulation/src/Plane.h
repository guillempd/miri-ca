#pragma once

#include <Ogre.h>

struct Plane
{
	using Vector3 = Ogre::Vector3;
	Vector3 normal;
	float offset;

	Plane(const Vector3& direction, const Vector3& pointInPlane) // TODO: Check this code for changing the parameter direction to normal
		: normal(direction.normalisedCopy())
		, offset(-normal.dotProduct(pointInPlane))
	{
	}
};
