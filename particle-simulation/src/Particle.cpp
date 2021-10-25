#include "Particle.h"

using Ogre::Vector3;

Particle::Particle(Ogre::SceneNode* sceneNode)
	: m_SceneNode(sceneNode)
{
	m_SceneNode->setVisible(false);
}

void Particle::Set(const Vector3& position, const Vector3& velocity)
{
	m_CurrentPosition = position;
	m_CurrentVelocity = velocity;
	m_CurrentForce = Vector3::ZERO;
	m_SceneNode->setVisible(true); // TODO: Move this to some other place
	CorrectPreviousPosition(1.0f / 144.0f);
	UpdateSceneNode();
}

void Particle::CorrectPreviousPosition(float dt)
{
	m_CorrectedPreviousPosition = m_CurrentPosition - m_CurrentVelocity * dt;
}

void Particle::UpdateSceneNode()
{
	m_SceneNode->setPosition(m_CurrentPosition);
}

void Particle::AddForce(const Vector3& force)
{
	m_CurrentForce += force;
}

void Particle::Update(const Properties& properties, float dt)
{
	m_PreviousPosition = m_CorrectedPreviousPosition;
	Vector3 currentAcceleration = CurrentForce(properties) / properties.mass;
	switch (properties.method)
	{
	case SolverMethod::Euler:
	{
		SavePreviousPosition();
		m_CurrentPosition += m_CurrentVelocity * dt;
		m_CurrentVelocity += currentAcceleration * dt;
	} break;
	case SolverMethod::EulerSemi:
	{
		SavePreviousPosition();
		m_CurrentVelocity += currentAcceleration * dt;
		m_CurrentPosition += m_CurrentVelocity * dt;
	} break;
	case SolverMethod::Verlet:
	{
		Vector3 positionDelta = m_CurrentPosition - m_PreviousPosition;
		SavePreviousPosition();
		m_CurrentPosition += positionDelta + currentAcceleration * dt * dt;
		m_CurrentVelocity = (m_CurrentPosition - m_PreviousPosition) / dt;
	} break;
	}
	UpdateSceneNode();
}

Vector3 Particle::CurrentForce(const Properties& properties)
{
	Vector3 currentForce = m_CurrentForce;
	m_CurrentForce = Vector3::ZERO; // NOTE: All the forces accumulated are removed when force is consumed
	return currentForce + properties.mass * properties.gravity;
}

void Particle::SavePreviousPosition()
{
	m_PreviousPosition = m_CurrentPosition;
	m_CorrectedPreviousPosition = m_PreviousPosition;
}

void Particle::CheckAndResolveCollision(const Plane& plane, const Properties& properties, float dt)
{
	if (CheckCollision(plane))
	{
		ResolveCollision(plane, properties, dt);
		UpdateSceneNode();
	}
}

void Particle::CheckAndResolveCollision(const Sphere& sphere, const Properties& properties, float dt)
{
	if (CheckCollision(sphere))
	{
		ResolveCollision(sphere, properties, dt);
		UpdateSceneNode();
	}
}

void Particle::CheckAndResolveCollision(const Triangle& triangle, const Properties& properties, float dt)
{
	if (CheckCollision(triangle))
	{
		ResolveCollision(triangle, properties, dt);
		UpdateSceneNode();
	}
}

bool Particle::CheckCollision(const Plane& plane) const
{
	float previousSignedDistance = plane.SignedDistance(m_PreviousPosition);
	float currentSignedDistance = plane.SignedDistance(m_CurrentPosition);
	return previousSignedDistance * currentSignedDistance <= 0;
}

bool Particle::CheckCollision(const Sphere& sphere) const
{
	float previousSignedDistance = sphere.SignedDistance(m_PreviousPosition);
	float currentSignedDistance = sphere.SignedDistance(m_CurrentPosition);
	return previousSignedDistance * currentSignedDistance <= 0;
}

bool Particle::CheckCollision(const Triangle& triangle) const
{
	const Plane& plane = triangle.GetPlane();
	if (!CheckCollision(plane)) return false;

	Vector3 contactPoint = plane.ContactPoint(m_PreviousPosition, m_CurrentPosition);
	return triangle.Contains(contactPoint);
}

void Particle::ResolveCollision(const Plane& plane, const Properties& properties, float dt)
{
	m_CurrentPosition = m_CurrentPosition - (1 + properties.bouncingCoefficient) * (plane.GetOffset() + plane.GetNormal().dotProduct(m_CurrentPosition)) * plane.GetNormal();
	m_CurrentVelocity = m_CurrentVelocity - (1 + properties.bouncingCoefficient) * plane.GetNormal().dotProduct(m_CurrentVelocity) * plane.GetNormal();

	Vector3 normalVelocity = plane.GetNormal().dotProduct(m_CurrentVelocity) * plane.GetNormal();
	Vector3 tangentVelocity = m_CurrentVelocity - normalVelocity;
	m_CurrentVelocity -= properties.frictionCoefficient * tangentVelocity;

	CorrectPreviousPosition(dt);
}

void Particle::ResolveCollision(const Sphere& sphere, const Properties& properties, float dt)
{
	Vector3 contactPoint = sphere.ContactPoint(m_CorrectedPreviousPosition, m_CurrentPosition);
	Plane normal = Plane(nullptr, contactPoint - sphere.GetCenter(), contactPoint);
	ResolveCollision(normal, properties, dt);
}

void Particle::ResolveCollision(const Triangle& triangle, const Properties& properties, float dt)
{
	ResolveCollision(triangle.GetPlane(), properties, dt);
}
