#include "ParticleSystem.h"

#include "Plane.h"

using Ogre::Vector3;

ParticleSystem::ParticleSystem()
	: ParticleSystem(Vector3(0.0f, 3.0f, 0.0f))
{
}

ParticleSystem::ParticleSystem(const Vector3& particlePosition)
	: m_Particle(particlePosition)
{
}

// TODO: Move the checking of collision with elements of the scene here (?)
void ParticleSystem::Update(float dt)
{
	m_Particle.Update(dt);
}
