#pragma once

#include <Ogre.h>

class Sphere
{
	using Vector3 = Ogre::Vector3;
public:
	Sphere(Ogre::SceneNode* sceneNode, const Vector3& center, float radius);
	float SignedDistance(const Vector3& point) const;
	Vector3 ContactPoint(const Vector3& origin, const Vector3& destiny) const;
	const Vector3& GetCenter() const { return m_Center; }
	float GetRadius() const { return m_Radius; }
private:
	Ogre::SceneNode* m_SceneNode;
	Vector3 m_Center;
	float m_Radius;
};
