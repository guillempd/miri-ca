#pragma once

#include "Particle.h"
#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"

#include <Ogre.h>
#include <OgreCameraMan.h>

#include <vector>

class Scene
{
	using Vector3 = Ogre::Vector3;
public:
	Scene();
	~Scene();
	void Setup(Ogre::SceneManager* sceneManager, Ogre::RenderWindow* renderWindow);
	void Update(float dt);
	OgreBites::CameraMan* GetCameraMan() { return m_CameraMan; }
private:
	void SetupLighting();
	void SetupCamera(Ogre::RenderWindow* renderWindow);
	void SetupEntities();
	void CheckPlanes(Particle& particle, float dt);
	void CheckSpheres(Particle& particle, float dt);
	void CheckTriangles(Particle& particle, float dt); // TODO: Consider making these a template Check<Collidable>
private:
	std::vector<Particle> m_Particles;
	std::vector<Plane> m_Planes;
	std::vector<Sphere> m_Spheres;
	std::vector<Triangle> m_Triangles;
	Particle::PhysicalProperties m_ParticlesPhysicalProperties;
	int m_NumParticles;
	int m_NumActiveParticles;
	float m_ElapsedTime;
	Ogre::SceneManager* m_SceneManager;
	OgreBites::CameraMan* m_CameraMan;
	Particle::SolverMethod m_SolverMethod;
};

