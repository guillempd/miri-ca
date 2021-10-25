#pragma once

#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"

#include <Ogre.h>

#include <random>

class Particle
{
	using Vector3 = Ogre::Vector3;
public:
	enum class SolverMethod {Euler, EulerSemi, Verlet};
	enum class GenerationType {Random, Cascade, Fountain};
	struct PhysicalProperties
	{
		Vector3 gravity;
		float mass;
		float bouncingCoefficient;
		float frictionCoefficient;
		float lifetime;
	};
public:
	Particle(Ogre::SceneNode* sceneNode);
	void Reset(GenerationType generationType, float lifetime);
	float UpdateLifetime(float dt);
	void UpdatePosition(float dt, SolverMethod method, const PhysicalProperties& properties);
	void CheckAndResolveCollision(const Plane &plane, const PhysicalProperties& properties, float dt);
	void CheckAndResolveCollision(const Sphere &sphere, const PhysicalProperties& properties, float dt);
	void CheckAndResolveCollision(const Triangle& triangle, const PhysicalProperties& properties, float dt);
	void AddForce(const Vector3& force);
	Vector3 GetPosition() const { return m_CurrentPosition; }
	Vector3 GetVelocity() const { return m_CurrentVelocity; }
	void SetPosition(const Vector3& position) {
		m_SceneNode->setVisible(true);
		m_CurrentForce = Vector3(0.0f, 0.0f, 0.0f);
		m_CurrentVelocity = Vector3(0.0f, 0.0f, 0.0f);
		m_CurrentPosition = position;
		CorrectPreviousPosition(1.0f / 144.0f);
		UpdateSceneNode();
	} // TODO: Better do this
	void SetFixed(bool fixed) { m_Fixed = fixed; }
private:
	Vector3 CurrentForce(const PhysicalProperties& properties);
	bool CheckCollision(const Plane& plane) const;
	bool CheckCollision(const Sphere& sphere) const;
	bool CheckCollision(const Triangle& triangle) const;
	void ResolveCollision(const Plane& plane, const PhysicalProperties& properties, float dt);
	void ResolveCollision(const Sphere& sphere, const PhysicalProperties& properties, float dt);
	void ResolveCollision(const Triangle& triangle, const PhysicalProperties& properties, float dt);
	void UpdateSceneNode();
	void CorrectPreviousPosition(float dt);
	void SavePreviousPosition();
private:
	Ogre::SceneNode* m_SceneNode;
	Vector3 m_CurrentPosition;
	Vector3 m_PreviousPosition;
	Vector3 m_CurrentVelocity;
	Vector3 m_CorrectedPreviousPosition;
	Vector3 m_CurrentForce;
	float m_LifetimeLeft;
	bool m_Fixed;
	static std::minstd_rand s_Rng;
	static std::uniform_real_distribution<float> s_UniformFloat;
};
