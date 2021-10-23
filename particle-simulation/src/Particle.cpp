#include "Particle.h"

using Ogre::Vector3;
std::minstd_rand Particle::s_Rng = std::minstd_rand();
std::uniform_real_distribution<float> Particle::s_UniformFloat = std::uniform_real_distribution<float>(-1.0f, 1.0f);

Particle::Particle(Ogre::SceneNode* sceneNode)
	: m_SceneNode(sceneNode)
{
	m_LifetimeLeft = 0.0f;
	m_SceneNode->setVisible(false);
}

void Particle::Reset(GenerationType generationType, float lifetime)
{
	m_SceneNode->setVisible(true);
	m_LifetimeLeft += lifetime; // So it takes into account the small part of lifetime it had left
	switch (generationType)
	{
	case GenerationType::Random:
	{
		m_CurrentPosition = Vector3(s_UniformFloat(s_Rng), s_UniformFloat(s_Rng), s_UniformFloat(s_Rng));
		m_CurrentVelocity = Vector3(s_UniformFloat(s_Rng), s_UniformFloat(s_Rng), s_UniformFloat(s_Rng));
	} break;
	case GenerationType::Cascade:
	{
		m_CurrentPosition = Vector3(0.0f, 0.9f, 0.0f);
		m_CurrentVelocity = Vector3(s_UniformFloat(s_Rng), 0.0f, s_UniformFloat(s_Rng));
	} break;
	case GenerationType::Fountain:
	{
		m_CurrentPosition = Vector3(0.0f, -0.9f, 0.0f);
		m_CurrentVelocity = Vector3(s_UniformFloat(s_Rng), 5.0f, s_UniformFloat(s_Rng));
	} break;
	}
	CorrectPreviousPosition(1.0f/60.0f); // NOTE: This is only "valid" for 60Hz and Vsync on
}

float Particle::UpdateLifetime(float dt)
{
	m_LifetimeLeft -= dt;
	if (m_LifetimeLeft <= 0.0f) dt = m_LifetimeLeft;
	return dt;
}

void Particle::UpdatePosition(float dt, SolverMethod method, const PhysicalProperties& properties)
{
	m_PreviousPosition = m_CorrectedPreviousPosition;
	Vector3 currentAcceleration = CurrentForce(properties) / properties.mass;
	switch (method)
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

void Particle::SavePreviousPosition()
{
	m_PreviousPosition = m_CurrentPosition;
	m_CorrectedPreviousPosition = m_PreviousPosition;
}

void Particle::CheckAndResolveCollision(const Plane& plane, const PhysicalProperties& properties, float dt)
{
	if (CheckCollision(plane))
	{
		ResolveCollision(plane, properties, dt);
		UpdateSceneNode();
	}
}

void Particle::CheckAndResolveCollision(const Sphere& sphere, const PhysicalProperties& properties, float dt)
{
	if (CheckCollision(sphere))
	{
		ResolveCollision(sphere, properties, dt);
		UpdateSceneNode();
	}
}

void Particle::CheckAndResolveCollision(const Triangle& triangle, const PhysicalProperties& properties, float dt)
{
	if (CheckCollision(triangle))
	{
		ResolveCollision(triangle, properties, dt);
		UpdateSceneNode();
	}
}

Vector3 Particle::CurrentForce(const PhysicalProperties &properties) const
{
	return properties.mass * properties.gravity;
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

void Particle::ResolveCollision(const Plane& plane, const PhysicalProperties& properties, float dt)
{
	m_CurrentPosition = m_CurrentPosition - (1 + properties.bouncingCoefficient) * (plane.GetOffset() + plane.GetNormal().dotProduct(m_CurrentPosition)) * plane.GetNormal();
	m_CurrentVelocity = m_CurrentVelocity - (1 + properties.bouncingCoefficient) * plane.GetNormal().dotProduct(m_CurrentVelocity) * plane.GetNormal();

	Vector3 normalVelocity = plane.GetNormal().dotProduct(m_CurrentVelocity) * plane.GetNormal();
	Vector3 tangentVelocity = m_CurrentVelocity - normalVelocity;
	m_CurrentVelocity -= properties.frictionCoefficient * tangentVelocity;

	CorrectPreviousPosition(dt);
}

void Particle::ResolveCollision(const Sphere& sphere, const PhysicalProperties& properties, float dt)
{
	Vector3 contactPoint = sphere.ContactPoint(m_CorrectedPreviousPosition, m_CurrentPosition);
	Plane normal = Plane(nullptr, contactPoint - sphere.GetCenter(), contactPoint);
	ResolveCollision(normal, properties, dt);
}

void Particle::ResolveCollision(const Triangle& triangle, const PhysicalProperties& properties, float dt)
{
	ResolveCollision(triangle.GetPlane(), properties, dt);
}

void Particle::UpdateSceneNode()
{
	m_SceneNode->setPosition(m_CurrentPosition);
}

void Particle::CorrectPreviousPosition(float dt)
{
	m_CorrectedPreviousPosition = m_CurrentPosition - m_CurrentVelocity * dt;
}
