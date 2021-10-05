#include "Particle.h"

using Ogre::Vector3;

Particle::Particle()
	: Particle(Vector3(0.0f, 0.0f, 0.0f))
{}

Particle::Particle(const Vector3 &initialPosition)
	: m_CurrentPosition(initialPosition)
	, m_PreviousPosition(initialPosition) // TODO: Correctly initialize for Verlet solver
	, m_CurrentVelocity(5.0f, 0.0f, 0.0f)
	, m_Mass(1.0f)
	, m_BouncingCoefficient(0.5f)
	, m_FrictionCoefficient(0.5f)
{}

// TODO: Add more solvers
void Particle::Update(float dt)
{
	m_PreviousPosition = m_CurrentPosition;

	Vector3 currentAcceleration = CurrentForce() / m_Mass;
	m_CurrentPosition += m_CurrentVelocity * dt;
	m_CurrentVelocity += currentAcceleration * dt;
}

void Particle::CheckAndResolveCollision(const Plane& plane)
{
	float previousSign = plane.normal.dotProduct(m_PreviousPosition) + plane.offset; // TODO: Consider refactoring this computation into a Plane function
	float currentSign = plane.normal.dotProduct(m_CurrentPosition) + plane.offset;
	bool collision = previousSign * currentSign < 0;
	if (!collision) return;

	m_CurrentPosition = m_CurrentPosition - (1 + m_BouncingCoefficient) * (plane.offset + plane.normal.dotProduct(m_CurrentPosition)) * plane.normal;
	m_CurrentVelocity = m_CurrentVelocity - (1 + m_BouncingCoefficient) * plane.normal.dotProduct(m_CurrentVelocity) * plane.normal;

	Vector3 normalVelocity = plane.normal.dotProduct(m_CurrentVelocity) * plane.normal;
	Vector3 tangentVelocity = m_CurrentVelocity - normalVelocity;
	m_CurrentVelocity -= m_FrictionCoefficient * tangentVelocity;
}

Vector3 Particle::CurrentForce()
{
	return Vector3(0.0f, -9.8f, 0.0f);
}
