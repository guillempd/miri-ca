#include "ParticleSystem.h"

#include "Plane.h"

using Ogre::Vector3;

ParticleSystem::ParticleSystem()
	: ParticleSystem(Vector3(0.0f, 3.0f, 0.0f))
{
}

ParticleSystem::ParticleSystem(const Vector3& particlePosition)
	: m_CurrentParticlePosition(particlePosition)
	, m_PreviousParticlePosition(particlePosition) // TODO: Correctly initialize for Verlet solver
	, m_ParticleVelocity(0.0f, 0.0f, 0.0f)
	, m_Gravity(0.0f, -9.8f, 0.0f)
	, m_ParticleMass(1.0f)
{
}

// TODO: Add other solvers
void ParticleSystem::Update(float timeStep)
{
	m_PreviousParticlePosition = m_CurrentParticlePosition;

	Vector3 particleAcceleration = ComputeForce() / m_ParticleMass;
	m_CurrentParticlePosition += m_ParticleVelocity * timeStep;
	m_ParticleVelocity += particleAcceleration * timeStep;

	if (PlaneCollision()) ResolvePlaneCollision();
}

Vector3 ParticleSystem::ComputeForce()
{
	return m_Gravity;
}

bool ParticleSystem::PlaneCollision()
{
	Plane floor = {Vector3(0.0f, 1.0f, 0.0f), 0.0f};

	float previousSign = floor.normal.dotProduct(m_PreviousParticlePosition) + floor.offset; // TODO: Consider refactoring this computation into Plane function
	float currentSign = floor.normal.dotProduct(m_CurrentParticlePosition) + floor.offset;
	return previousSign * currentSign < 0;
}

// TODO: Add bounciness coefficient
void ParticleSystem::ResolvePlaneCollision()
{
	Plane floor = {Vector3(0.0f, 1.0f, 0.0f), 0.0f};
	m_CurrentParticlePosition = m_CurrentParticlePosition - 2 * (floor.offset + floor.normal.dotProduct(m_CurrentParticlePosition)) * floor.normal;
	m_ParticleVelocity = m_ParticleVelocity - 2 * floor.normal.dotProduct(m_ParticleVelocity) * floor.normal;
}
