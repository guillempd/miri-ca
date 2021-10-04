#pragma once

#include <OgreVector3.h>

class ParticleSystem
{
	using Vector3 = Ogre::Vector3;
public:
	ParticleSystem();
	ParticleSystem(const Vector3 &particlePosition);
	Vector3 GetParticlePosition() { return m_CurrentParticlePosition; }
	void Update(float timeStep);
private:
	Vector3 ComputeForce();
	bool PlaneCollision();
	void ResolvePlaneCollision();
private:
	Vector3 m_CurrentParticlePosition; // TODO: Add more particles
	Vector3 m_PreviousParticlePosition;
	Vector3 m_ParticleVelocity;
	Vector3 m_Gravity;
	float m_ParticleMass;
};

