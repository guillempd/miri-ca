#pragma once

#include <Ogre.h>

class Plane
{
	using Vector3 = Ogre::Vector3;
public:
	Plane(Ogre::SceneNode* sceneNode, const Vector3& normal, const Vector3& pointInPlane);
	float SignedDistance(const Vector3& point) const;
	const Vector3& GetNormal() const { return m_Normal; }
	const Vector3& GetPosition() const { return m_Position; }
	float GetOffset() const { return m_Offset; }
private:
	Ogre::SceneNode* m_SceneNode;
	Vector3 m_Normal;
	Vector3 m_Position;
	float m_Offset;
};
