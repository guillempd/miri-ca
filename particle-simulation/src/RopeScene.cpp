#include "RopeScene.h"

#include <imgui.h>

using Ogre::Vector3;

RopeScene::RopeScene(std::vector<Ogre::MaterialPtr>& materials, Ogre::MeshPtr planeMesh)
	: Scene(materials, planeMesh)
	, m_NumParticles(50)
	, m_Springs()
	, m_SpringProperties{100.0f, 0.25f, 0.5f/ m_NumParticles}
{
}

void RopeScene::Update(float dt)
{
	Scene::Update(dt);

	CreateInterface();

	// Compute forces
	for (int i = 1; i < m_NumParticles; ++i)
	{
		m_Springs[i-1].ApplyForces(m_SpringProperties);
	}

	// Update particles except the first one (which is fixed)
	for (int i = 1; i < m_NumParticles; ++i)
	{
		Particle& particle = m_Particles[i];

		particle.Update(m_ParticlesProperties, dt);
		CheckPlanes(particle, dt);
		CheckSpheres(particle, dt);
		CheckTriangles(particle, dt);
	}
}

void RopeScene::SetupEntities()
{
	m_Particles.reserve(m_NumParticles);
	m_Springs.reserve(m_NumParticles - 1);

	Particle* previousParticle = &CreateParticle();
	previousParticle->Set(Vector3(0.0f, 0.75f, 0.0f), Vector3::ZERO);
	for (int i = 1; i < m_NumParticles; ++i)
	{
		Vector3 initialPosition = Vector3(static_cast<float>(i)/m_NumParticles, 0.75f, 0.0f);
		Particle* currentParticle = &CreateParticle();
		currentParticle->Set(initialPosition, Vector3::ZERO);

		m_Springs.emplace_back(*previousParticle, *currentParticle);
		previousParticle = currentParticle;
	}

	// TODO: Might consider making a function like CreateCornellBox or whatever to the scene
	m_Planes.reserve(6);
	CreatePlane(Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f)); // FLOOR
	CreatePlane(Vector3(0.0f, -1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)); // CEILING
	CreatePlane(Vector3(1.0f, 0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f)); // LEFT
	CreatePlane(Vector3(-1.0f, 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f)); // RIGHT
	CreatePlane(Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f)); // BACK
	CreatePlane(Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 0.0f, 1.0f)); // FRONT

	m_Spheres.reserve(1);
	CreateSphere(Vector3(0.0f, 0.0f, 0.0f), 0.5f);
}

void RopeScene::CreateInterface()
{
	ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp;

	if (ImGui::Begin("Spring Settings"))
	{
		ImGui::DragFloat("Elasticity Constant", &m_SpringProperties.stiffnessK, 0.01f, 0.0f, std::numeric_limits<float>::max(),"%.3f", flags);
		ImGui::DragFloat("Damping Constant", &m_SpringProperties.dampingK, 0.01f, 0.0f, std::numeric_limits<float>::max(), "%.3f", flags);
		ImGui::DragFloat("RestLength", &m_SpringProperties.restLength, 0.001f, 0.0f, 0.1f, "%.3f", flags);
	}
	ImGui::End();
}
