#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
}

ParticleSystem::ParticleSystem(const Ogre::Vector3& particlePosition)
	: m_ParticlePosition(particlePosition)
{
}

// TODO: Correctly implement this update
void ParticleSystem::Update(float timeStep)
{
	m_ParticlePosition += Ogre::Vector3(-0.01f, 0.0f, 0.0f) * timeStep;
}
