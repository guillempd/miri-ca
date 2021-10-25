#pragma once

#include "Particle.h"
#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"

#include <Ogre.h>
#include <OgreCameraMan.h>

#include <vector>
#include <random>

class Scene
{
	using Vector3 = Ogre::Vector3;
public:
	Scene(std::vector<Ogre::MaterialPtr>& materials, Ogre::MeshPtr planeMesh);
	virtual ~Scene(); // = default (?)
	void Setup(Ogre::SceneManager* sceneManager, Ogre::RenderWindow* renderWindow);
	virtual void Update(float dt);
	OgreBites::CameraMan* GetCameraMan() { return m_CameraMan; }
protected:
	void SetupCamera(Ogre::RenderWindow* renderWindow);
	void SetupLighting();
	virtual void SetupEntities() = 0;
	void CreateParticle();
	void CreatePlane(const Vector3& normal, const Vector3& pointInPlane);
	void CreateSphere(const Vector3& center, float radius);
	void CreateTriangle(const Vector3& p0, const Vector3& p1, const Vector3& p2);
	void CheckPlanes(Particle& particle, float dt);
	void CheckSpheres(Particle& particle, float dt);
	void CheckTriangles(Particle& particle, float dt); // TODO: Consider making these a template Check<Collidable>
protected:
	// Scene elements
	std::vector<Particle> m_Particles;
	std::vector<Plane> m_Planes;
	std::vector<Sphere> m_Spheres;
	std::vector<Triangle> m_Triangles;
	Ogre::Camera* m_Camera;
	// Random materials for particles
	std::vector<Ogre::MaterialPtr>& m_Materials;
	std::mt19937 m_Rng;
	std::uniform_int_distribution<int> m_UniformMaterialIndex;
	// Resources
	Ogre::MeshPtr m_PlaneMesh;
	// Particles properties management
	Particle::Properties m_ParticlesPhysicalProperties;
	// Ogre stuff
	Ogre::SceneManager* m_SceneManager;
	OgreBites::CameraMan* m_CameraMan;
	Particle::SolverMethod m_SolverMethod;
	Particle::GenerationType m_GenerationType;
};

