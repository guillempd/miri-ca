#include "ParticleSystem.h"

using Ogre::Vector3;

ParticleSystem::ParticleSystem()
	: ParticleSystem(Vector3(-1.0f, 5.0f, -1.0f))
{
}

ParticleSystem::ParticleSystem(const Vector3& particlePosition)
	: m_Particle(particlePosition)
	, m_Plane(Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f)) // Floor
	, m_Sphere{Vector3(0.0f, 0.0f, 0.0f), 3.0f}
{
}

void ParticleSystem::Update(float dt)
{
	m_Particle.Update(dt);
	m_Particle.CheckAndResolveCollision(m_Plane);
	m_Particle.CheckAndResolveCollision(m_Sphere);
}
