#include "Particle.h"

using Ogre::Vector3;

Particle::Particle(Ogre::SceneNode* sceneNode)
	: m_SceneNode(sceneNode)
{
	Reset();
}

void Particle::Reset(const Vector3& initialPosition)
{
	m_CurrentPosition = initialPosition;
	m_PreviousPosition = initialPosition; // TODO: Correctly initialize for Verlet solver
	m_CurrentVelocity = Vector3(0.0f, 0.0f, 0.0f);
	m_Mass = 1.0f;
	m_BouncingCoefficient = 1.0f;
	m_FrictionCoefficient = 0.5f;
	m_Lifetime = 5.0f;
	m_LifetimeLeft = 5.0f;
}

// TODO: Add more solvers
void Particle::Update(float dt, SolverMethod method)
{
	m_LifetimeLeft -= dt;
	if (m_LifetimeLeft <= 0.0f)
	{
		dt = -m_LifetimeLeft;
		m_LifetimeLeft += m_Lifetime;
		Reset();
	}

	Vector3 currentAcceleration = CurrentForce() / m_Mass;
	switch (method)
	{
	case SolverMethod::Euler:
	{
		m_PreviousPosition = m_CurrentPosition;
		m_CurrentPosition += m_CurrentVelocity * dt;
		m_CurrentVelocity += currentAcceleration * dt;
	} break;
	case SolverMethod::EulerSemi:
	{
		m_PreviousPosition = m_CurrentPosition;
		m_CurrentVelocity += currentAcceleration * dt;
		m_CurrentPosition += m_CurrentVelocity * dt;
	} break;
	case SolverMethod::Verlet:
	{
		OgreAssert(false);
		/*m_CurrentVelocity = m_CurrentPosition - m_PreviousPosition;
		m_PreviousPosition = m_CurrentPosition;
		m_CurrentPosition += m_CurrentVelocity;*/ // No need for timestep since it is already taken into account
	} break;
	}

	UpdateSceneNode();
}

void Particle::CheckAndResolveCollision(const Plane& plane)
{
	if (CheckCollision(plane))
	{
		ResolveCollision(plane);
		UpdateSceneNode();
	}
}

void Particle::CheckAndResolveCollision(const Sphere& sphere)
{
	float previousSign = sphere.DistanceToSurface(m_PreviousPosition);
	float currentSign = sphere.DistanceToSurface(m_CurrentPosition);
	if (previousSign * currentSign <= 0)
	{
		Vector3 contactPoint = sphere.ContactPoint(m_PreviousPosition, m_CurrentPosition);
		Plane normal = Plane(contactPoint - sphere.center, contactPoint);
		ResolveCollision(normal);
		UpdateSceneNode();
	}
}

Vector3 Particle::CurrentForce()
{
	return Vector3(0.0f, -5.0f, 0.0f);
}

bool Particle::CheckCollision(const Plane& plane)
{
	float previousSign = plane.normal.dotProduct(m_PreviousPosition) + plane.offset; // TODO: Consider refactoring this computation into a Plane function
	float currentSign = plane.normal.dotProduct(m_CurrentPosition) + plane.offset;
	return previousSign * currentSign <= 0;
}

void Particle::ResolveCollision(const Plane& plane)
{
	m_CurrentPosition = m_CurrentPosition - (1 + m_BouncingCoefficient) * (plane.offset + plane.normal.dotProduct(m_CurrentPosition)) * plane.normal;
	m_CurrentVelocity = m_CurrentVelocity - (1 + m_BouncingCoefficient) * plane.normal.dotProduct(m_CurrentVelocity) * plane.normal;

	Vector3 normalVelocity = plane.normal.dotProduct(m_CurrentVelocity) * plane.normal;
	Vector3 tangentVelocity = m_CurrentVelocity - normalVelocity;
	m_CurrentVelocity -= m_FrictionCoefficient * tangentVelocity;
}

void Particle::UpdateSceneNode()
{
	m_SceneNode->setPosition(m_CurrentPosition);
}
