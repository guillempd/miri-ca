#include "Scene.h"

#include <imgui.h>

using Ogre::Vector3;

Scene::Scene()
	: m_Particles()
	, m_Planes()
	, m_Spheres()
	, m_ParticlesPhysicalProperties{Vector3(0.0f, -9.8f, 0.0f), 1.0f, 0.5f, 0.5f, 5.0f}
	, m_NumParticles(100)
	, m_NumActiveParticles(0)
	, m_ElapsedTime(0.0f)
{
}

void Scene::Setup(Ogre::SceneManager* sceneManager, Ogre::RenderWindow* renderWindow)
{
	m_SceneManager = sceneManager;
	SetupCamera(renderWindow);
	SetupLighting();
	SetupEntities();
}

void Scene::SetupLighting()
{
	m_SceneManager->setAmbientLight(Ogre::ColourValue(0.1f, 0.1f, 0.1f));
	
	Ogre::Light* redDirectionalLight = m_SceneManager->createLight(Ogre::Light::LightTypes::LT_DIRECTIONAL);
	redDirectionalLight->setDiffuseColour(Ogre::ColourValue(0.2f, 0.0f, 0.0f));
	redDirectionalLight->setSpecularColour(Ogre::ColourValue(0.2f, 0.0f, 0.0f));

	Ogre::SceneNode* redDirectionalLightNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
	redDirectionalLightNode->attachObject(redDirectionalLight);
	redDirectionalLightNode->setDirection(1, 0, 0); // TODO: Set appropiately

	Ogre::Light* blueDirectionalLight = m_SceneManager->createLight(Ogre::Light::LightTypes::LT_DIRECTIONAL);
	blueDirectionalLight->setDiffuseColour(Ogre::ColourValue(0.0f, 0.0f, 0.2f));
	blueDirectionalLight->setSpecularColour(Ogre::ColourValue(0.0f, 0.0f, 0.2f));

	Ogre::SceneNode* blueDirectionalLightNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
	blueDirectionalLightNode->attachObject(blueDirectionalLight);
	blueDirectionalLightNode->setDirection(-1, 0, 0);


	Ogre::Light* pointLight = m_SceneManager->createLight(Ogre::Light::LightTypes::LT_POINT);
	pointLight->setDiffuseColour(Ogre::ColourValue(0.3f, 0.3f, 0.3f));
	pointLight->setSpecularColour(Ogre::ColourValue(0.3f, 0.3f, 0.3f));
	m_CameraMan->getCamera()->attachObject(pointLight);
}

void Scene::SetupCamera(Ogre::RenderWindow* renderWindow)
{
	Ogre::Camera* camera = m_SceneManager->createCamera("Camera");
	camera->setNearClipDistance(1.0f); // TODO: Set this according to CameraMan orbit distance
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

	Ogre::Entity* planeEntity;
	Ogre::SceneNode* planeEntityNode;
	Ogre::MeshPtr meshPtr = Ogre::MeshManager::getSingleton().createPlane("Plane", "General", Ogre::Plane(Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f)), 2, 2);
	m_Planes.reserve(6);

	// FLOOR
	planeEntity = m_SceneManager->createEntity(meshPtr);
	planeEntityNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
	planeEntityNode->attachObject(planeEntity);
	m_Planes.emplace_back(planeEntityNode, Vector3(0.0f, -1.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f));

	// CEILING
	planeEntity = m_SceneManager->createEntity(meshPtr);
	planeEntityNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
	planeEntityNode->attachObject(planeEntity);
	m_Planes.emplace_back(planeEntityNode, Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));

	// LEFT FACE
	planeEntity = m_SceneManager->createEntity(meshPtr);
	planeEntityNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
	planeEntityNode->attachObject(planeEntity);
	m_Planes.emplace_back(planeEntityNode, Vector3(-1.0f, 0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f));

	// RIGHT FACE
	planeEntity = m_SceneManager->createEntity(meshPtr);
	planeEntityNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
	planeEntityNode->attachObject(planeEntity);
	m_Planes.emplace_back(planeEntityNode, Vector3(1.0f, 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f));

	// FRONT FACE
	planeEntity = m_SceneManager->createEntity(meshPtr);
	planeEntityNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
	planeEntityNode->attachObject(planeEntity);
	m_Planes.emplace_back(planeEntityNode, Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f));

	// BACK FACE
	planeEntity = m_SceneManager->createEntity(meshPtr);
	planeEntityNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
	planeEntityNode->attachObject(planeEntity);
	m_Planes.emplace_back(planeEntityNode, Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f));

	m_Spheres.reserve(1);
	Ogre::Entity* sphereEntity = m_SceneManager->createEntity("sphere.mesh");
	Ogre::SceneNode* sphereEntityNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
	sphereEntityNode->attachObject(sphereEntity);
	sphereEntityNode->setScale(0.01f, 0.01f, 0.01f);
	m_Spheres.emplace_back(sphereEntityNode, Vector3(0.0f, 0.0f, 0.0f), 0.5f);
}

// TODO: Destroy SceneNode's (?)
// TODO: Do this in shutdown
Scene::~Scene()
{
	// m_SceneManager->destroyAllLights();
	// m_SceneManager->destroyAllCameras();
	// m_SceneManager->destroyAllEntities();
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

	m_ElapsedTime += dt;
	m_NumActiveParticles = static_cast<int>(m_NumParticles * (m_ElapsedTime / m_ParticlesPhysicalProperties.lifetime));
	m_NumActiveParticles = (m_NumActiveParticles <= m_NumParticles ? m_NumActiveParticles : m_NumParticles);

	for (int i = 0; i < m_NumActiveParticles; ++i)
	{
		Particle& particle = m_Particles[i];

		float actualDt = particle.UpdateLifetime(dt);
		if (actualDt <= 0.0f)
		{
			actualDt = -actualDt;
			particle.Reset(Particle::GenerationType::Random, m_ParticlesPhysicalProperties.lifetime);
		}
		particle.UpdatePosition(actualDt, m_SolverMethod, m_ParticlesPhysicalProperties);
		CheckPlanes(particle);
		CheckSpheres(particle);
		// CheckTriangles(particle);
	}

}

void Scene::CheckPlanes(Particle& particle)
{
	for (const Plane& plane : m_Planes)
	{
		particle.CheckAndResolveCollision(plane, m_ParticlesPhysicalProperties);
	}
}

void Scene::CheckSpheres(Particle& particle)
{
	for (const Sphere& sphere : m_Spheres)
	{
		particle.CheckAndResolveCollision(sphere, m_ParticlesPhysicalProperties);
	}
}
