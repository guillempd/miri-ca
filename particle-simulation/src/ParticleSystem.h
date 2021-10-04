#pragma once

#include "Particle.h"

#include <OgreVector3.h>

class ParticleSystem
{
	using Vector3 = Ogre::Vector3;
public:
	ParticleSystem();
	ParticleSystem(const Vector3 &particlePosition);
	Vector3 GetParticlePosition() { return m_Particle.GetPosition(); }
	void Update(float dt);
private:
	Particle m_Particle; // TODO: Add MORE particles
};

