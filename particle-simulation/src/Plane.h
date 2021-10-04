#pragma once

#include <OgreVector3.h>

struct Plane
{
	using Vector3 = Ogre::Vector3;
	Vector3 normal;
	float offset;
};