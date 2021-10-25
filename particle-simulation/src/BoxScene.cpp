#include "BoxScene.h"

#include "imgui.h"

using Ogre::Vector3;

BoxScene::BoxScene(std::vector<Ogre::MaterialPtr>& materials, Ogre::MeshPtr planeMesh)
	: Scene(materials, planeMesh)
	, m_NumParticles(100)
	, m_NumActiveParticles(0)
	, m_ElapsedTime(0.0f)
{
}

void BoxScene::Update(float dt)
{
	Scene::Update(dt);

	CreateInterface();

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
			particle.Reset(m_GenerationType, m_ParticlesPhysicalProperties.lifetime);
		}
		particle.UpdatePosition(actualDt, m_SolverMethod, m_ParticlesPhysicalProperties);
		CheckPlanes(particle, actualDt);
		CheckSpheres(particle, actualDt);
		CheckTriangles(particle, actualDt);
	}
}

void BoxScene::SetupEntities()
{
	m_Particles.reserve(m_NumParticles);
	for (int i = 0; i < m_NumParticles; ++i)
		CreateParticle();

	m_Planes.reserve(6);
	CreatePlane(Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f)); // FLOOR
	CreatePlane(Vector3(0.0f, -1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)); // CEILING
	CreatePlane(Vector3(1.0f, 0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f)); // LEFT
	CreatePlane(Vector3(-1.0f, 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f)); // RIGHT
	CreatePlane(Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f)); // BACK
	CreatePlane(Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 0.0f, 1.0f)); // FRONT

	m_Spheres.reserve(1);
	CreateSphere(Vector3(0.0f, -0.50f, 0.0f), 0.25f);

	m_Triangles.reserve(2);
	CreateTriangle(Vector3(0.5f, 0.0f, 0.0f), Vector3(-0.5f, 0.1f, 0.5f), Vector3(-0.5f, 0.2f, -0.5f));
	// CreateTriangle(Vector3(-1.0f, -1.0f, -1.0f), Vector3(-1.0f, -1.0f, 1.0f), Vector3(1.0f, -1.0f, -1.0f)); // Redundant triangle for testing coplanar collisions
}

void BoxScene::CreateInterface()
{
	if(ImGui::Begin("Generation Method"))
	{
		ImGui::RadioButton("Random", reinterpret_cast<int*>(&m_GenerationType), static_cast<int>(Particle::GenerationType::Random));
		ImGui::RadioButton("Cascade", reinterpret_cast<int*>(&m_GenerationType), static_cast<int>(Particle::GenerationType::Cascade));
		ImGui::RadioButton("Fountain", reinterpret_cast<int*>(&m_GenerationType), static_cast<int>(Particle::GenerationType::Fountain));
	}
	ImGui::End();
}
