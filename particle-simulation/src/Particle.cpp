#include "Particle.h"

#include "Plane.h"

using Ogre::Vector3;

Particle::Particle()
	: Particle(Vector3(0.0f, 0.0f, 0.0f))
{}

Particle::Particle(const Vector3 &initialPosition)
	: m_CurrentPosition(initialPosition)
	, m_PreviousPosition(initialPosition) // TODO: Correctly initialize for Verlet solver
	, m_CurrentVelocity(0.0f, 0.0f, 0.0f)
	, m_Mass(1.0f)
{}

// TODO: Add more solvers
void Particle::Update(float dt)
{
	m_PreviousPosition = m_CurrentPosition;

	Vector3 currentAcceleration = CurrentForce() / m_Mass;
	m_CurrentPosition += m_CurrentVelocity * dt;
	m_CurrentVelocity += currentAcceleration * dt;

	if (PlaneCollision()) ResolvePlaneCollision();
}


bool Particle::PlaneCollision()
{
	Plane floor = { Vector3(0.0f, 1.0f, 0.0f), 0.0f };

	float previousSign = floor.normal.dotProduct(m_PreviousPosition) + floor.offset; // TODO: Consider refactoring this computation into a Plane function
	float currentSign = floor.normal.dotProduct(m_CurrentPosition) + floor.offset;
	return previousSign * currentSign < 0;
}

void Particle::ResolvePlaneCollision()
{
	Plane floor = { Vector3(0.0f, 1.0f, 0.0f), 0.0f };
	m_CurrentPosition = m_CurrentPosition - 2 * (floor.offset + floor.normal.dotProduct(m_CurrentPosition)) * floor.normal;
	m_CurrentVelocity = m_CurrentVelocity - 2 * floor.normal.dotProduct(m_CurrentVelocity) * floor.normal;
}

Vector3 Particle::CurrentForce()
{
	return Vector3(0.0f, -9.8f, 0.0f);
}
