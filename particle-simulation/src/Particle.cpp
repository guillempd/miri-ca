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
	if (CheckCollision(plane))
	{
		ResolveCollision(plane, properties);
		UpdateSceneNode();
	}
}

void Particle::CheckAndResolveCollision(const Sphere& sphere, const PhysicalProperties& properties)
{
	if (CheckCollision(sphere))
	{
		ResolveCollision(sphere, properties);
		UpdateSceneNode();
	}
}

Vector3 Particle::CurrentForce(const PhysicalProperties &properties)
{
	return properties.mass * properties.gravity;
}

bool Particle::CheckCollision(const Plane& plane)
{
	float previousSignedDistance = plane.SignedDistance(m_PreviousPosition);
	float currentSignedDistance = plane.SignedDistance(m_CurrentPosition);
	return previousSignedDistance * currentSignedDistance <= 0;
}

bool Particle::CheckCollision(const Sphere& sphere)
{
	float previousSignedDistance = sphere.DistanceToSurface(m_PreviousPosition);
	float currentSignedDistance = sphere.DistanceToSurface(m_CurrentPosition);
	return previousSignedDistance * currentSignedDistance <= 0;
}

void Particle::ResolveCollision(const Plane& plane, const PhysicalProperties& properties)
{
	m_CurrentPosition = m_CurrentPosition - (1 + properties.bouncingCoefficient) * (plane.GetOffset() + plane.GetNormal().dotProduct(m_CurrentPosition)) * plane.GetNormal();
	m_CurrentVelocity = m_CurrentVelocity - (1 + properties.bouncingCoefficient) * plane.GetNormal().dotProduct(m_CurrentVelocity) * plane.GetNormal();

	Vector3 normalVelocity = plane.GetNormal().dotProduct(m_CurrentVelocity) * plane.GetNormal();
	Vector3 tangentVelocity = m_CurrentVelocity - normalVelocity;
	m_CurrentVelocity -= properties.frictionCoefficient * tangentVelocity;
}

void Particle::ResolveCollision(const Sphere& sphere, const PhysicalProperties& properties)
{
	Vector3 contactPoint = sphere.ContactPoint(m_PreviousPosition, m_CurrentPosition);
	Plane normal = Plane(nullptr, contactPoint - sphere.center, contactPoint);
	ResolveCollision(normal, properties);
}

void Particle::UpdateSceneNode()
{
	m_SceneNode->setPosition(m_CurrentPosition);
}
