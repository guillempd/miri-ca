#include "Scene.h"

#include <imgui.h>

using Ogre::Vector3;

Scene::Scene()
	: m_Particles()
	, m_Plane(Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f)) // Floor
	, m_Sphere{Vector3(0.0f, 0.0f, 0.0f), 3.0f}
	, m_ParticlesPhysicalProperties{Vector3(0.0f, -9.8f, 0.0f), 1.0f, 0.5f, 0.5f, 5.0f}
	, m_NumParticles(10)
	, m_NumActiveParticles(0)
	, m_ElapsedTime(0.0f)
{
}

void Scene::Setup(Ogre::SceneManager* sceneManager, Ogre::RenderWindow* renderWindow)
{
	m_SceneManager = sceneManager;
	SetupLighting();
	SetupCamera(renderWindow);
	SetupEntities();
}

void Scene::SetupLighting()
{
	m_SceneManager->setAmbientLight(Ogre::ColourValue(0.1f, 0.1f, 0.1f));
	
	Ogre::Light* directionalLight = m_SceneManager->createLight("DirectionalLight", Ogre::Light::LightTypes::LT_DIRECTIONAL);
	directionalLight->setDiffuseColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
	directionalLight->setSpecularColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

	Ogre::SceneNode* directionalLightNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
	directionalLightNode->attachObject(directionalLight);
	directionalLightNode->setDirection(0, 10, 15); // TODO: Set appropiately
}

void Scene::SetupCamera(Ogre::RenderWindow* renderWindow)
{
	Ogre::Camera* camera = m_SceneManager->createCamera("Camera");
	camera->setNearClipDistance(5.0f); // TODO: Set this according to CameraMan orbit distance
	camera->setAutoAspectRatio(true);

	Ogre::SceneNode* cameraNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
	cameraNode->attachObject(camera);
	cameraNode->setPosition(0, 0, 15);
	cameraNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);

	m_CameraMan = new OgreBites::CameraMan(cameraNode);
	m_CameraMan->setStyle(OgreBites::CS_ORBIT);
	m_CameraMan->setTarget(m_SceneManager->getRootSceneNode()); // TODO: Set this to the floor plane probably
	m_CameraMan->setTopSpeed(10.0f); // TODO: Set appropriately

	renderWindow->addViewport(camera);
}

void Scene::SetupEntities()
{
	m_Particles.reserve(m_NumParticles);
	for (int i = 0; i < m_NumParticles; ++i)
	{
		Ogre::Entity* particleEntity = m_SceneManager->createEntity("sphere.mesh");
		particleEntity->setMaterial(Ogre::MaterialManager::getSingleton().getDefaultMaterial());

		Ogre::SceneNode* particleEntityNode = m_SceneManager->getRootSceneNode()->createChildSceneNode(); // TODO: Name these (?)
		particleEntityNode->attachObject(particleEntity);
		particleEntityNode->setScale(0.001f, 0.001f, 0.001f);

		m_Particles.emplace_back(particleEntityNode);
	}
}

// TODO: Destroy SceneNode's (?)
Scene::~Scene()
{
	m_SceneManager->destroyAllLights();
	m_SceneManager->destroyAllCameras();
	m_SceneManager->destroyAllEntities();
	delete m_CameraMan;
}

void Scene::Update(float dt)
{
	if (ImGui::Begin("Settings"))
	{
		ImGui::Text("Solver Method");
		ImGui::RadioButton("Euler Original", reinterpret_cast<int*>(&m_SolverMethod), static_cast<int>(Particle::SolverMethod::Euler));
		ImGui::RadioButton("Euler Semi Implicit", reinterpret_cast<int*>(&m_SolverMethod), static_cast<int>(Particle::SolverMethod::EulerSemi));
		ImGui::RadioButton("Verlet", reinterpret_cast<int*>(&m_SolverMethod), static_cast<int>(Particle::SolverMethod::Verlet));

		ImGui::Separator();

		ImGui::Text("Particle Physical Properties");
		ImGui::SliderFloat("Mass", &m_ParticlesPhysicalProperties.mass, 0.1f, 10.0f);
		ImGui::SliderFloat("Bouncing Coefficient", &m_ParticlesPhysicalProperties.bouncingCoefficient, 0.1f, 1.0f);
		ImGui::SliderFloat("Friction Coefficient", &m_ParticlesPhysicalProperties.frictionCoefficient, 0.1f, 1.0f);
	}
	ImGui::End();

	// Update numActiveParticles, for now assume the lifetime is fixed
	m_ElapsedTime += dt;
	m_NumActiveParticles = m_NumParticles * (m_ElapsedTime / m_ParticlesPhysicalProperties.lifetime);
	m_NumActiveParticles = (m_NumActiveParticles <= m_NumParticles ? m_NumActiveParticles : m_NumParticles);

	for (int i = 0; i < m_NumActiveParticles; ++i)
	{
		Particle& particle = m_Particles[i];

		float actualDt = particle.UpdateLifetime(dt);
		if (actualDt <= 0.0f)
		{
			actualDt = -actualDt;
			particle.Reset(Particle::GenerationType::Cascade, m_ParticlesPhysicalProperties.lifetime);
		}
		particle.UpdatePosition(actualDt, m_SolverMethod, m_ParticlesPhysicalProperties);
		particle.CheckAndResolveCollision(m_Plane, m_ParticlesPhysicalProperties);
		particle.CheckAndResolveCollision(m_Sphere, m_ParticlesPhysicalProperties);
	}

}
