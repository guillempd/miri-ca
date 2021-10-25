#include "RopeScene.h"

#include <imgui.h>

using Ogre::Vector3;

RopeScene::RopeScene(std::vector<Ogre::MaterialPtr>& materials, Ogre::MeshPtr planeMesh)
	: Scene(materials, planeMesh)
	, m_ElasticityK(1.0f)
	, m_DampingK(1.0f)
	, m_RestLength(0.1f)
{
}

// TODO: Handle initialization of the particles tha form the rope
void RopeScene::Update(float dt)
{
	Scene::Update(dt);

	CreateInterface();

	int numParticles = m_Particles.size();

	// Compute forces
	for (int i = 1; i < numParticles; ++i)
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
	for (int i = 1; i < numParticles; ++i)
	{
		Particle& particle = m_Particles[i];

		particle.UpdatePosition(dt, m_SolverMethod, m_ParticlesPhysicalProperties);
		CheckPlanes(particle, dt);
		CheckSpheres(particle, dt);
		CheckTriangles(particle, dt);
	}
}

void RopeScene::SetupEntities()
{
	constexpr int numParticles = 10;
	m_Particles.reserve(numParticles);
	for (int i = 0; i < numParticles; ++i)
	{
		CreateParticle();
		Vector3 initialPosition = Vector3(static_cast<float>(i)/numParticles, 0.75f, 0.0f);
		m_Particles[i].SetPosition(initialPosition);
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
	CreateSphere(Vector3(0.0f, -0.5f, 0.0f), 0.25f);
}

void RopeScene::CreateInterface()
{
	ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp;

	if (ImGui::Begin("Spring Settings"))
	{
		ImGui::DragFloat("Elasticity Constant", &m_ElasticityK, 1.0f, 0.0f, std::numeric_limits<float>::max(),"%.3f", flags);
		ImGui::DragFloat("Damping Constant", &m_DampingK, 1.0f, 0.0f, std::numeric_limits<float>::max(), "%.3f", flags);
		ImGui::DragFloat("RestLength", &m_RestLength, 0.01f, 0.0f, 0.25f, "%.3f", flags);
	}
	ImGui::End();
}
