#pragma once

#include <Ogre.h>

struct Plane
{
	using Vector3 = Ogre::Vector3;
	Vector3 normal;
	float offset;
};