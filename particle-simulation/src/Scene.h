#pragma once

#include "Particle.h"
#include "Plane.h"
#include "Sphere.h"

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
private:
	std::vector<Particle> m_Particles;
	Plane m_Plane;
	Sphere m_Sphere;
	int m_NumParticles;
	int m_NumActiveParticles;
	float m_Lifetime;
	float m_ElapsedTime;
	Ogre::SceneManager* m_SceneManager;
	OgreBites::CameraMan* m_CameraMan;
	Particle::SolverMethod m_SolverMethod;
};
