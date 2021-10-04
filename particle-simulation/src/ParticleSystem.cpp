#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
}

ParticleSystem::ParticleSystem(const Vector3& particlePosition)
	: m_ParticlePosition(particlePosition)
{
}

// TODO: Correctly implement this update
void ParticleSystem::Update(float timeStep)
{
	m_ParticlePosition += Vector3(-0.1f, 0.0f, 0.0f) * timeStep;
}
