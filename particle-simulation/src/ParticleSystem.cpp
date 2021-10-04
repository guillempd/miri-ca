#include "ParticleSystem.h"

using Ogre::Vector3;

ParticleSystem::ParticleSystem()
	: ParticleSystem(Vector3(0.0f, 0.0f, 0.0f))
{
}

ParticleSystem::ParticleSystem(const Vector3& particlePosition)
	: m_ParticlePosition(particlePosition)
	, m_ParticleVelocity(0.0f, 0.0f, 0.0f)
	, m_Gravity(0.0f, -9.8f, 0.0f)
	, m_ParticleMass(1.0f)
{
}

void ParticleSystem::Update(float timeStep)
{
	Vector3 particleAcceleration = ComputeForce() / m_ParticleMass;
	m_ParticlePosition += m_ParticleVelocity * timeStep;
	m_ParticleVelocity += particleAcceleration * timeStep;
}

Vector3 ParticleSystem::ComputeForce()
{
	return m_Gravity;
}
