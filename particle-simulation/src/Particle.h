#pragma once

#include "Plane.h"

#include <Ogre.h>

class Particle
{
	using Vector3 = Ogre::Vector3;
public:
	Particle();
	Particle(const Vector3 &initialPosition);
	void Update(float dt);
	const Vector3 &GetPosition() const { return m_CurrentPosition; };
	void CheckAndResolveCollision(const Plane &plane);
private:
	Vector3 CurrentForce();
private:
	Vector3 m_CurrentPosition;
	Vector3 m_PreviousPosition;
	Vector3 m_CurrentVelocity;
	float m_Mass;
	float m_BouncingCoefficient;
	float m_FrictionCoefficient;
};

