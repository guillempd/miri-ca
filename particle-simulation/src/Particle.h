#pragma once

#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"

#include <Ogre.h>

class Particle
{
	using Vector3 = Ogre::Vector3;
public:
	enum class SolverMethod {Euler, EulerSemi, Verlet};
	struct Properties
	{
		Vector3 gravity;
		float mass;
		float bouncingCoefficient;
		float frictionCoefficient;
		SolverMethod method;
	};
public:
	Particle(Ogre::SceneNode* sceneNode);
	void Set(const Vector3& position, const Vector3& velocity);
	void AddForce(const Vector3& force);
	void Update(const Properties& properties, float dt);
	void CheckAndResolveCollision(const Plane &plane, const Properties& properties, float dt);
	void CheckAndResolveCollision(const Sphere &sphere, const Properties& properties, float dt);
	void CheckAndResolveCollision(const Triangle& triangle, const Properties& properties, float dt);
	Vector3 GetPosition() const { return m_CurrentPosition; }
	Vector3 GetVelocity() const { return m_CurrentVelocity; }
private:
	Vector3 CurrentForce(const Properties& properties);
	bool CheckCollision(const Plane& plane) const;
	bool CheckCollision(const Sphere& sphere) const;
	bool CheckCollision(const Triangle& triangle) const;
	void ResolveCollision(const Plane& plane, const Properties& properties, float dt);
	void ResolveCollision(const Sphere& sphere, const Properties& properties, float dt);
	void ResolveCollision(const Triangle& triangle, const Properties& properties, float dt);
	void CorrectPreviousPosition(float dt);
	void SavePreviousPosition();
	void UpdateSceneNode();
private:
	Ogre::SceneNode* m_SceneNode;
	Vector3 m_CurrentPosition;
	Vector3 m_PreviousPosition;
	Vector3 m_CurrentVelocity;
	Vector3 m_CorrectedPreviousPosition;
	Vector3 m_CurrentForce;
};
