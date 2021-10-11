#pragma once

#include "Plane.h"

#include <Ogre.h>

class Triangle
{
	using Vector3 = Ogre::Vector3;
public:
	Triangle(const Vector3& p0, const Vector3& p1, const Vector3& p2);
	bool Contains(const Vector3& pointInPlane) const;
	float Area() const;
	const Plane& GetPlane() const { return m_Plane; }
private:
	Vector3 m_P0, m_P1, m_P2;
	Plane m_Plane;
};

