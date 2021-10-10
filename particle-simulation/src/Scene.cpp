#include "Scene.h"

#include <imgui.h>

using Ogre::Vector3;

Scene::Scene()
	: m_Particles()
	, m_Plane(Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f)) // Floor
	, m_Sphere{Vector3(0.0f, 0.0f, 0.0f), 3.0f}
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
	int numParticles = 10;
	m_Particles.reserve(numParticles);
	for (int i = 0; i < numParticles; ++i)
	{
		Ogre::Entity* particleEntity = m_SceneManager->createEntity("sphere.mesh");
		particleEntity->setMaterial(Ogre::MaterialManager::getSingleton().getDefaultMaterial());

		Ogre::SceneNode* particleEntityNode = m_SceneManager->getRootSceneNode()->createChildSceneNode(); // TODO: Name these (?)
		particleEntityNode->attachObject(particleEntity);
		particleEntityNode->setScale(0.01f, 0.01f, 0.01f);

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
	if (ImGui::Begin("Solver Method"))
	{
		ImGui::RadioButton("Euler Original", reinterpret_cast<int*>(&m_SolverMethod), static_cast<int>(Particle::SolverMethod::Euler));
		ImGui::RadioButton("Euler Semi Implicit", reinterpret_cast<int*>(&m_SolverMethod), static_cast<int>(Particle::SolverMethod::EulerSemi));
		ImGui::RadioButton("Verlet", reinterpret_cast<int*>(&m_SolverMethod), static_cast<int>(Particle::SolverMethod::Verlet));
	}
	ImGui::End();

	for (Particle& particle : m_Particles)
	{
		particle.Update(dt, m_SolverMethod);
		particle.CheckAndResolveCollision(m_Plane);
		particle.CheckAndResolveCollision(m_Sphere);
	}

}
