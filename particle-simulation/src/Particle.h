#pragma once

#include <OgreVector3.h>

class Particle
{
	using Vector3 = Ogre::Vector3;
public:
	Particle();
	Particle(const Vector3 &initialPosition);
	void Update(float dt);
	Vector3 GetPosition() { return m_CurrentPosition; };
private:
	bool PlaneCollision();
	void ResolvePlaneCollision(); // TODO: Consider merging this and the previous function into one
	Vector3 CurrentForce();
private:
	Vector3 m_CurrentPosition;
	Vector3 m_PreviousPosition;
	Vector3 m_CurrentVelocity;
	float m_Mass;
	float m_BouncingCoefficient;
	float m_FrictionCoefficient;
};

