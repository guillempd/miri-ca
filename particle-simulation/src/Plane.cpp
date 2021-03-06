#include "Plane.h"

using Ogre::Vector3;

Plane::Plane(Ogre::SceneNode* sceneNode, const Vector3& normal, const Vector3& pointInPlane)
	: m_SceneNode(sceneNode)
	, m_Normal(normal.normalisedCopy())
	, m_Position(pointInPlane)
	, m_Offset(-m_Normal.dotProduct(m_Position))
{
	if (m_SceneNode)
	{
		m_SceneNode->setPosition(m_Position);
		m_SceneNode->setDirection(m_Normal); // NOTE: This may cause the plane to not be oriented as we wish, for more control use setOrientation()
	}
}

float Plane::SignedDistance(const Vector3& point) const
{
	return m_Normal.dotProduct(point) + m_Offset;
}

Vector3 Plane::ContactPoint(const Vector3& origin, const Vector3& destiny) const
{
	Vector3 direction = destiny - origin;
	float lambda = -(m_Offset + m_Normal.dotProduct(origin)) / m_Normal.dotProduct(direction);
	return origin + lambda * direction;
}
