#include "RopeScene.h"

#include <imgui.h>

using Ogre::Vector3;

RopeScene::RopeScene(std::vector<Ogre::MaterialPtr>& materials, Ogre::MeshPtr planeMesh)
	: Scene(materials, planeMesh)
	, m_ElasticityK(100.0f)
	, m_DampingK(0.25f)
	, m_NumParticles(50)
	, m_RestLength(0.5f/m_NumParticles)
{
}

void RopeScene::Update(float dt)
{
	Scene::Update(dt);

	CreateInterface();

	// Compute forces
	for (int i = 1; i < m_NumParticles; ++i)
	{
		Particle& particle1 = m_Particles[i-1];
		Particle& particle2 = m_Particles[i];

		Vector3 direction = particle2.GetPosition() - particle1.GetPosition();
		float length = direction.length();
		direction.normalise();

		float elasticTerm = m_ElasticityK * (length - m_RestLength);
		float dampingTerm = m_DampingK * (particle2.GetVelocity() - particle1.GetVelocity()).dotProduct(direction);
		Vector3 force = (elasticTerm + dampingTerm) * direction;

		particle1.AddForce(force);
		particle2.AddForce(-force);
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
	for (int i = 0; i < m_NumParticles; ++i)
	{
		Vector3 initialPosition = Vector3(static_cast<float>(i)/m_NumParticles, 0.75f, 0.0f);
		Particle& particle = CreateParticle();
		particle.Set(initialPosition, Vector3::ZERO);
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
		ImGui::DragFloat("Elasticity Constant", &m_ElasticityK, 0.01f, 0.0f, std::numeric_limits<float>::max(),"%.3f", flags);
		ImGui::DragFloat("Damping Constant", &m_DampingK, 0.01f, 0.0f, std::numeric_limits<float>::max(), "%.3f", flags);
		ImGui::DragFloat("RestLength", &m_RestLength, 0.001f, 0.0f, 0.1f, "%.3f", flags);
	}
	ImGui::End();
}
