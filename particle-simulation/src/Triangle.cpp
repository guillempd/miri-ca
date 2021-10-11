#include "Triangle.h"

using Ogre::Vector3;

Triangle::Triangle(const Vector3& p0, const Vector3& p1, const Vector3& p2)
	: m_P0(p0)
	, m_P1(p1)
	, m_P2(p2)
	, m_Plane(nullptr, (p1-p0).crossProduct(p2-p0), p0)
{
}

bool Triangle::Contains(const Vector3& pointInPlane) const
{
	float a1 = Triangle(pointInPlane, m_P1, m_P2).Area();
	float a2 = Triangle(m_P0, pointInPlane, m_P2).Area();
	float a3 = Triangle(m_P0, m_P1, pointInPlane).Area();
	float a = Area();
	return a1 + a2 + a3 - a > 0.0f;
}

// NOTE: Not really area, should be multiplied by 1/2. Works for the point-inside-triangle-test
float Triangle::Area() const
{
	return (m_P1-m_P0).crossProduct(m_P2-m_P0).length();
}
