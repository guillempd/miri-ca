#include "Scene.h"

#include <imgui.h>

#include <limits>

using Ogre::Vector3;

Scene::Scene(std::vector<Ogre::MaterialPtr>& materials, Ogre::MeshPtr planeMesh)
	: m_Particles()
	, m_Planes()
	, m_Spheres()
	, m_Triangles()
	, m_Camera(nullptr)
	, m_Materials(materials)
	, m_Rng()
	, m_UniformMaterialIndex(0, m_Materials.size()-1)
	, m_PlaneMesh(planeMesh)
	, m_ParticlesProperties{Vector3(0.0f, -9.8f, 0.0f), 0.01f, 0.5f, 0.5f, Particle::SolverMethod::Verlet}
	, m_SceneManager(nullptr)
	, m_CameraMan(nullptr)
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
	if (m_SceneManager->hasCamera("Camera"))
	{
		m_Camera = m_SceneManager->getCamera("Camera");
	}
	else
	{
		m_Camera = m_SceneManager->createCamera("Camera");
		renderWindow->addViewport(m_Camera);
	}

	m_Camera->setNearClipDistance(0.1f);
	m_Camera->setAutoAspectRatio(true);

	Ogre::SceneNode* cameraNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
	cameraNode->attachObject(m_Camera);
	cameraNode->setPosition(0, 0, 5);
	cameraNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);

	m_CameraMan = new OgreBites::CameraMan(cameraNode);
	m_CameraMan->setStyle(OgreBites::CS_ORBIT);
	m_CameraMan->setTarget(m_SceneManager->getRootSceneNode());
	m_CameraMan->setTopSpeed(10.0f); // TODO: Set appropriately
}

Particle& Scene::CreateParticle()
{
	Ogre::Entity* particleEntity = m_SceneManager->createEntity("sphere.mesh");
	particleEntity->setMaterial(m_Materials[m_UniformMaterialIndex(m_Rng)]);

	Ogre::SceneNode* particleEntityNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
	particleEntityNode->attachObject(particleEntity);
	particleEntityNode->setScale(0.0002f, 0.0002f, 0.0002f); // Set particles radius to be 1/100th of cube's side

	m_Particles.emplace_back(particleEntityNode);
	return m_Particles.back();
}

Plane& Scene::CreatePlane(const Vector3 &normal, const Vector3& pointInPlane)
{
	Ogre::Entity* planeEntity = m_SceneManager->createEntity(m_PlaneMesh);
	Ogre::SceneNode* planeEntityNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
	planeEntityNode->attachObject(planeEntity);
	m_Planes.emplace_back(planeEntityNode, normal, pointInPlane);
	return m_Planes.back();
}

Sphere& Scene::CreateSphere(const Vector3& center, float radius)
{
	Ogre::Entity* sphereEntity = m_SceneManager->createEntity("sphere.mesh");
	Ogre::SceneNode* sphereEntityNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
	sphereEntityNode->attachObject(sphereEntity);
	sphereEntityNode->setScale(0.01f, 0.01f, 0.01f);
	m_Spheres.emplace_back(sphereEntityNode, center, radius);
	return m_Spheres.back();
}

Triangle& Scene::CreateTriangle(const Vector3& p0, const Vector3& p1, const Vector3& p2)
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
	return m_Triangles.back();
}

// TODO: Destroy SceneNode's (?)
// TODO: Do this in shutdown (?)
Scene::~Scene()
{
	m_Camera->detachFromParent();
	m_SceneManager->destroyAllLights();
	m_SceneManager->destroyAllEntities();
	m_SceneManager->destroyAllManualObjects();
	delete m_CameraMan;
}

void Scene::CheckPlanes(Particle& particle, float dt)
{
	for (const Plane& plane : m_Planes)
	{
		particle.CheckAndResolveCollision(plane, m_ParticlesProperties, dt);
	}
}

void Scene::CheckSpheres(Particle& particle, float dt)
{
	for (const Sphere& sphere : m_Spheres)
	{
		particle.CheckAndResolveCollision(sphere, m_ParticlesProperties, dt);
	}
}

void Scene::CheckTriangles(Particle& particle, float dt)
{
	for (const Triangle& triangle : m_Triangles)
	{
		particle.CheckAndResolveCollision(triangle, m_ParticlesProperties, dt);
	}
}

void Scene::TogglePauseState()
{
	m_Paused = !m_Paused;
}

void Scene::ResetSimulation()
{
	ResetScene();
	SetupEntities();
}

// TODO: Maybe move this to another method not named update
void Scene::Update(float dt)
{
	ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp;

	ImGui::ShowMetricsWindow(); // For fps counter

	if (ImGui::Begin("Scene Controls"))
	{
		if (ImGui::Button("Pause/Resume Simulation")) TogglePauseState();
		if (ImGui::Button("Reset Simulation")) ResetSimulation();

		ImGui::Separator();

		ImGui::Text("Solver Method");
		ImGui::RadioButton("Euler Original", reinterpret_cast<int*>(&m_ParticlesProperties.method), static_cast<int>(Particle::SolverMethod::Euler));
		ImGui::RadioButton("Euler Semi Implicit", reinterpret_cast<int*>(&m_ParticlesProperties.method), static_cast<int>(Particle::SolverMethod::EulerSemi));
		ImGui::RadioButton("Verlet", reinterpret_cast<int*>(&m_ParticlesProperties.method), static_cast<int>(Particle::SolverMethod::Verlet));

		ImGui::Separator();

		ImGui::Text("Physical Properties");
		ImGui::DragFloat("Mass", &m_ParticlesProperties.mass, 0.01f, 0.01f, std::numeric_limits<float>::max(), "%.3f", flags);
		ImGui::SliderFloat("Bouncing Coefficient", &m_ParticlesProperties.bouncingCoefficient, 0.0f, 1.0f, "%.3f", flags);
		ImGui::SliderFloat("Friction Coefficient", &m_ParticlesProperties.frictionCoefficient, 0.0f, 1.0f, "%.3f", flags);
		ImGui::DragFloat3("Gravity", m_ParticlesProperties.gravity.ptr(), 0.05f, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), "%.3f", flags);
	}
	ImGui::End();
}

void Scene::ResetScene()
{
	m_SceneManager->destroyAllEntities();
	m_SceneManager->destroyAllManualObjects();
	m_Particles.clear();
	m_Planes.clear();
	m_Spheres.clear();
	m_Triangles.clear();
}
