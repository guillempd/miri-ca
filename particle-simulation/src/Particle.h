#pragma once

#include "Plane.h"
#include "Sphere.h"

#include <Ogre.h>

class Particle
{
	using Vector3 = Ogre::Vector3;
public:
	enum class SolverMethod {Euler, EulerSemi, Verlet};
	enum class GenerationType {Cascade, Fountain};
public:
	Particle(Ogre::SceneNode* sceneNode);
	void Reset(GenerationType generationType, float lifetime);
	float UpdateLifetime(float dt);
	void UpdatePosition(float dt, SolverMethod method);
	const Vector3 &GetPosition() const { return m_CurrentPosition; };
	void CheckAndResolveCollision(const Plane &plane);
	void CheckAndResolveCollision(const Sphere &sphere);
private:
	Vector3 CurrentForce();
	bool CheckCollision(const Plane& plane);
	void ResolveCollision(const Plane &plane);
	void UpdateSceneNode();
private:
	Ogre::SceneNode* m_SceneNode;
	Vector3 m_CurrentPosition;
	Vector3 m_PreviousPosition;
	Vector3 m_CurrentVelocity;
	float m_Mass;
	float m_BouncingCoefficient;
	float m_FrictionCoefficient;
	float m_LifetimeLeft;
};

