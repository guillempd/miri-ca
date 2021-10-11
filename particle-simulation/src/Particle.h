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
	void CheckAndResolveCollision(const Plane &plane, const PhysicalProperties& properties);
	void CheckAndResolveCollision(const Sphere &sphere, const PhysicalProperties& properties);
	void CheckAndResolveCollision(const Triangle& triangle, const PhysicalProperties& properties);
private:
	Vector3 CurrentForce(const PhysicalProperties& properties);
	bool CheckCollision(const Plane& plane);
	bool CheckCollision(const Sphere& sphere);
	bool CheckCollision(const Triangle& triangle);
	void ResolveCollision(const Plane& plane, const PhysicalProperties& properties);
	void ResolveCollision(const Sphere& sphere, const PhysicalProperties& properties);
	void ResolveCollision(const Triangle& triangle, const PhysicalProperties& properties);
	void UpdateSceneNode();
private:
	Ogre::SceneNode* m_SceneNode;
	Vector3 m_CurrentPosition;
	Vector3 m_PreviousPosition;
	Vector3 m_CurrentVelocity;
	float m_LifetimeLeft;
	static std::minstd_rand s_Rng;
	static std::uniform_real_distribution<float> s_UniformFloat;
};

