#pragma once

#include "Particle.h"
#include "Plane.h"

#include <Ogre.h>

class ParticleSystem
{
	using Vector3 = Ogre::Vector3;
public:
	ParticleSystem();
	ParticleSystem(const Vector3 &particlePosition);
	Vector3 GetParticlePosition() { return m_Particle.GetPosition(); }
	void Update(float dt);
private:
	// TODO: Add MORE of these
	Particle m_Particle;
	Plane m_Plane;
};

