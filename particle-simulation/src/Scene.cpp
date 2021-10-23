#include "Scene.h"

#include <imgui.h>

#include <limits>

using Ogre::Vector3;

Scene::Scene()
	: m_Particles()
	, m_Planes()
	, m_Spheres()
	, m_Triangles()
	, m_Materials()
	, m_NumMaterials(9)
	, m_Rng()
	, m_UniformMaterialIndex(0, m_NumMaterials-1)
	, m_ParticlesPhysicalProperties{Vector3(0.0f, -9.8f, 0.0f), 1.0f, 0.5f, 0.5f, 5.0f}
	, m_NumParticles(100)
	, m_NumActiveParticles(0)
	, m_ElapsedTime(0.0f)
	, m_SceneManager(nullptr)
	, m_CameraMan(nullptr)
	, m_SolverMethod(Particle::SolverMethod::Euler)
	, m_GenerationType(Particle::GenerationType::Random)
{
}

void Scene::Setup(Ogre::SceneManager* sceneManager, Ogre::RenderWindow* renderWindow)
{
	m_SceneManager = sceneManager;
	SetupCamera(renderWindow);
	SetupLighting();
	SetupMaterials();
	SetupEntities();
}

void Scene::SetupLighting()
{
	m_SceneManager->setAmbientLight(Ogre::ColourValue(0.1f, 0.1f, 0.1f));

	Ogre::Light* directionalLight = m_SceneManager->createLight(Ogre::Light::LightTypes::LT_DIRECTIONAL);
	directionalLight->setDiffuseColour(0.5f, 0.5f, 0.5f);
	directionalLight->setSpecularColour(0.5f, 0.5f, 0.5f);

	Ogre::SceneNode* directionalLightNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
	directionalLightNode->attachObject(directionalLight);
	directionalLightNode->setDirection(-0.1f, -1.0f, 0.2f);

	Ogre::Light* pointLight = m_SceneManager->createLight(Ogre::Light::LightTypes::LT_POINT);
	pointLight->setDiffuseColour(Ogre::ColourValue(0.3f, 0.3f, 0.3f));
	pointLight->setSpecularColour(Ogre::ColourValue(0.3f, 0.3f, 0.3f));
	m_CameraMan->getCamera()->attachObject(pointLight);
}

void Scene::SetupCamera(Ogre::RenderWindow* renderWindow)
{
	Ogre::Camera* camera = m_SceneManager->createCamera("Camera");
	camera->setNearClipDistance(0.1f);
	camera->setAutoAspectRatio(true);

	Ogre::SceneNode* cameraNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
	cameraNode->attachObject(camera);
	cameraNode->setPosition(0, 0, 5);
	cameraNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);

	m_CameraMan = new OgreBites::CameraMan(cameraNode);
	m_CameraMan->setStyle(OgreBites::CS_ORBIT);
	m_CameraMan->setTarget(m_SceneManager->getRootSceneNode());
	m_CameraMan->setTopSpeed(10.0f); // TODO: Set appropriately

	renderWindow->addViewport(camera);
}

void Scene::SetupMaterials()
{
	m_Materials.reserve(m_NumMaterials);

	Ogre::MaterialPtr material;

	material = Ogre::MaterialManager::getSingleton().create("Color1", "General");
	material->setDiffuse(228.0f / 255.0f, 26.0f / 255.0f, 28.0f / 255.0f, 1.0f);
	material->setAmbient(228.0f / 255.0f, 26.0f / 255.0f, 28.0f / 255.0f);
	material->setSpecular(0.9f, 0.9f, 0.9f, 1.0f);
	// material->setShininess(4.0f);
	m_Materials.push_back(material);

	material = Ogre::MaterialManager::getSingleton().create("Color2", "General");
	material->setDiffuse(55.0f / 255.0f, 126.0f / 255.0f, 184.0f / 255.0f, 1.0f);
	material->setAmbient(55.0f / 255.0f, 126.0f / 255.0f, 184.0f / 255.0f);
	material->setSpecular(0.9f, 0.9f, 0.9f, 1.0f);
	// material->setShininess(4.0f);
	m_Materials.push_back(material);

	material = Ogre::MaterialManager::getSingleton().create("Color3", "General");
	material->setDiffuse(77.0f / 255.0f, 175.0f / 255.0f, 74.0f / 255.0f, 1.0f);
	material->setAmbient(77.0f / 255.0f, 175.0f / 255.0f, 74.0f / 255.0f);
	material->setSpecular(0.9f, 0.9f, 0.9f, 1.0f);
	// material->setShininess(4.0f);
	m_Materials.push_back(material);

	material = Ogre::MaterialManager::getSingleton().create("Color4", "General");
	material->setDiffuse(152.0f / 255.0f, 78.0f / 255.0f, 163.0f / 255.0f, 1.0f);
	material->setAmbient(152.0f / 255.0f, 78.0f / 255.0f, 163.0f / 255.0f);
	material->setSpecular(0.9f, 0.9f, 0.9f, 1.0f);
	// material->setShininess(4.0f);
	m_Materials.push_back(material);

	material = Ogre::MaterialManager::getSingleton().create("Color5", "General");
	material->setDiffuse(255.0f / 255.0f, 127.0f / 255.0f, 0.0f / 255.0f, 1.0f);
	material->setAmbient(255.0f / 255.0f, 127.0f / 255.0f, 0.0f / 255.0f);
	material->setSpecular(0.9f, 0.9f, 0.9f, 1.0f);
	// material->setShininess(4.0f);
	m_Materials.push_back(material);

	material = Ogre::MaterialManager::getSingleton().create("Color6", "General");
	material->setDiffuse(255.0f / 255.0f, 255.0f / 255.0f, 51.0f / 255.0f, 1.0f);
	material->setAmbient(255.0f / 255.0f, 255.0f / 255.0f, 51.0f / 255.0f);
	material->setSpecular(0.9f, 0.9f, 0.9f, 1.0f);
	// material->setShininess(4.0f);
	m_Materials.push_back(material);

	material = Ogre::MaterialManager::getSingleton().create("Color7", "General");
	material->setDiffuse(166.0f / 255.0f, 86.0f / 255.0f, 40.0f / 255.0f, 1.0f);
	material->setAmbient(166.0f / 255.0f, 86.0f / 255.0f, 40.0f / 255.0f);
	material->setSpecular(0.9f, 0.9f, 0.9f, 1.0f);
	// material->setShininess(4.0f);
	m_Materials.push_back(material);

	material = Ogre::MaterialManager::getSingleton().create("Color8", "General");
	material->setDiffuse(247.0f / 255.0f, 129.0f / 255.0f, 191.0f / 255.0f, 1.0f);
	material->setAmbient(247.0f / 255.0f, 129.0f / 255.0f, 191.0f / 255.0f);
	material->setSpecular(0.9f, 0.9f, 0.9f, 1.0f);
	// material->setShininess(4.0f);
	m_Materials.push_back(material);

	material = Ogre::MaterialManager::getSingleton().create("Color9", "General");
	material->setDiffuse(153.0f / 255.0f, 153.0f / 255.0f, 153.0f / 255.0f, 1.0f);
	material->setAmbient(153.0f / 255.0f, 153.0f / 255.0f, 153.0f / 255.0f);
	material->setSpecular(0.9f, 0.9f, 0.9f, 1.0f);
	// material->setShininess(4.0f);
	m_Materials.push_back(material);
}

void Scene::CreateParticle()
{
	Ogre::Entity* particleEntity = m_SceneManager->createEntity("sphere.mesh");
	particleEntity->setMaterial(m_Materials[m_UniformMaterialIndex(m_Rng)]);

	Ogre::SceneNode* particleEntityNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
	particleEntityNode->attachObject(particleEntity);
	particleEntityNode->setScale(0.0002f, 0.0002f, 0.0002f); // Set particles radius to be 1/100th of cube's side

	m_Particles.emplace_back(particleEntityNode);
}

void Scene::CreatePlane(const Vector3 &normal, const Vector3& pointInPlane, Ogre::MeshPtr meshPtr)
{
	Ogre::Entity* planeEntity = m_SceneManager->createEntity(meshPtr);
	Ogre::SceneNode* planeEntityNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
	planeEntityNode->attachObject(planeEntity);
	m_Planes.emplace_back(planeEntityNode, normal, pointInPlane);
}

void Scene::CreateSphere(const Vector3& center, float radius)
{
	Ogre::Entity* sphereEntity = m_SceneManager->createEntity("sphere.mesh");
	Ogre::SceneNode* sphereEntityNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
	sphereEntityNode->attachObject(sphereEntity);
	sphereEntityNode->setScale(0.01f, 0.01f, 0.01f);
	m_Spheres.emplace_back(sphereEntityNode, center, radius);
}

void Scene::CreateTriangle(const Vector3& p0, const Vector3& p1, const Vector3& p2)
{
	Ogre::ManualObject* manualObject = m_SceneManager->createManualObject();
	manualObject->begin(Ogre::MaterialManager::getSingleton().getDefaultMaterial());

	Vector3 normal = (p1-p0).crossProduct(p2-p0);

	manualObject->position(p0);
	manualObject->normal(normal);

	manualObject->position(p1);
	manualObject->normal(normal);

	manualObject->position(p2);
	manualObject->normal(normal);

	manualObject->triangle(0, 1, 2);
	manualObject->triangle(0, 2, 1);

	manualObject->end();

	m_SceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(manualObject);

	m_Triangles.emplace_back(p0, p1, p2);
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

void Scene::CheckPlanes(Particle& particle, float dt)
{
	for (const Plane& plane : m_Planes)
	{
		particle.CheckAndResolveCollision(plane, m_ParticlesPhysicalProperties, dt);
	}
}

void Scene::CheckSpheres(Particle& particle, float dt)
{
	for (const Sphere& sphere : m_Spheres)
	{
		particle.CheckAndResolveCollision(sphere, m_ParticlesPhysicalProperties, dt);
	}
}

void Scene::CheckTriangles(Particle& particle, float dt)
{
	for (const Triangle& triangle : m_Triangles)
	{
		particle.CheckAndResolveCollision(triangle, m_ParticlesPhysicalProperties, dt);
	}
}
