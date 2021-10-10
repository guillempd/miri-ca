#include "Particle.h"

using Ogre::Vector3;

Particle::Particle(Ogre::SceneNode* sceneNode) // TODO: Pass in a GenerationType
	: m_SceneNode(sceneNode)
{
	m_LifetimeLeft = 0.0f;
	m_SceneNode->setVisible(false);
}

// TODO: Randomize
void Particle::Reset(GenerationType generationType, float lifetime)
{
	m_SceneNode->setVisible(true);
	m_LifetimeLeft += lifetime; // So it takes into account the small part of lifetime it had left
	switch (generationType)
	{
	case GenerationType::Cascade:
	{
		m_CurrentPosition = Vector3(0.0f, 10.0f, 0.0f);
		m_CurrentVelocity = Vector3(0.0f, 0.0f, 0.0f);
	} break;
	case GenerationType::Fountain:
	{
		m_CurrentPosition = Vector3(0.0f, 0.0f, 0.0f);
		m_CurrentVelocity = Vector3(0.0f, 10.0f, 0.0f);
	} break;
	}
	m_PreviousPosition = m_CurrentPosition; // TODO: Correctly initialize for Verlet solver
}

float Particle::UpdateLifetime(float dt)
{
	m_LifetimeLeft -= dt;
	if (m_LifetimeLeft <= 0.0f) dt = m_LifetimeLeft;
	return dt;
}

void Particle::UpdatePosition(float dt, SolverMethod method, const PhysicalProperties& properties)
{
	Vector3 currentAcceleration = CurrentForce(properties) / properties.mass;
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

void Particle::CheckAndResolveCollision(const Plane& plane, const PhysicalProperties& properties)
{
	if (CheckCollision(plane, properties))
	{
		ResolveCollision(plane, properties);
		UpdateSceneNode();
	}
}

void Particle::CheckAndResolveCollision(const Sphere& sphere, const PhysicalProperties& properties)
{
	float previousSign = sphere.DistanceToSurface(m_PreviousPosition);
	float currentSign = sphere.DistanceToSurface(m_CurrentPosition);
	if (previousSign * currentSign <= 0)
	{
		Vector3 contactPoint = sphere.ContactPoint(m_PreviousPosition, m_CurrentPosition);
		Plane normal = Plane(contactPoint - sphere.center, contactPoint);
		ResolveCollision(normal, properties);
		UpdateSceneNode();
	}
}

Vector3 Particle::CurrentForce(const PhysicalProperties &properties)
{
	return properties.mass * properties.gravity;
}

bool Particle::CheckCollision(const Plane& plane, const PhysicalProperties& properties)
{
	float previousSign = plane.normal.dotProduct(m_PreviousPosition) + plane.offset; // TODO: Consider refactoring this computation into a Plane function
	float currentSign = plane.normal.dotProduct(m_CurrentPosition) + plane.offset;
	return previousSign * currentSign <= 0;
}

void Particle::ResolveCollision(const Plane& plane, const PhysicalProperties& properties)
{
	m_CurrentPosition = m_CurrentPosition - (1 + properties.bouncingCoefficient) * (plane.offset + plane.normal.dotProduct(m_CurrentPosition)) * plane.normal;
	m_CurrentVelocity = m_CurrentVelocity - (1 + properties.bouncingCoefficient) * plane.normal.dotProduct(m_CurrentVelocity) * plane.normal;

	Vector3 normalVelocity = plane.normal.dotProduct(m_CurrentVelocity) * plane.normal;
	Vector3 tangentVelocity = m_CurrentVelocity - normalVelocity;
	m_CurrentVelocity -= properties.frictionCoefficient * tangentVelocity;
}

void Particle::UpdateSceneNode()
{
	m_SceneNode->setPosition(m_CurrentPosition);
}
