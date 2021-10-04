#pragma once

#include <OgreVector3.h>

class ParticleSystem
{
	using Vector3 = Ogre::Vector3;
public:
	ParticleSystem();
	ParticleSystem(const Vector3 &particlePosition);
	Vector3 GetParticlePosition() { return m_ParticlePosition; }
	void Update(float timeStep);
private:
	Vector3 m_ParticlePosition; // TODO: Add more particles
};

