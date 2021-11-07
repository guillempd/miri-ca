#include "BoxScene.h"

#include "imgui.h"

using Ogre::Vector3;

BoxScene::BoxScene(std::vector<Ogre::MaterialPtr>& materials, Ogre::MeshPtr planeMesh)
	: Scene(materials, planeMesh)
	, m_NumParticles(100)
	, m_NumActiveParticles(0)
	, m_ElapsedTime(0.0f)
	, m_GenerationType(GenerationType::Random)
	, m_UniformFloat(-1.0f, 1.0f)
	, m_ParticlesLifetimes(m_NumParticles, 0.0f)
{
}

void BoxScene::ResetParticle(Particle& particle)
{
	Vector3 initialPosition = Vector3::ZERO;
	Vector3 initialVelocity = Vector3::ZERO;
	switch (m_GenerationType)
	{
	case GenerationType::Random:
	{
		initialPosition = Vector3(m_UniformFloat(m_Rng), m_UniformFloat(m_Rng), m_UniformFloat(m_Rng));
		initialVelocity = Vector3(m_UniformFloat(m_Rng), m_UniformFloat(m_Rng), m_UniformFloat(m_Rng));
	} break;
	case GenerationType::Cascade:
	{
		initialPosition = Vector3(0.0f, 0.9f, 0.0f);
		initialVelocity = Vector3(m_UniformFloat(m_Rng), 0.0f, m_UniformFloat(m_Rng));
	} break;
	case GenerationType::Fountain:
	{
		initialPosition = Vector3(0.0f, -0.9f, 0.0f);
		initialVelocity = Vector3(m_UniformFloat(m_Rng), 5.0f, m_UniformFloat(m_Rng));
	} break;
	}
	particle.Set(initialPosition, initialVelocity);
}

void BoxScene::ResetScene()
{
	Scene::ResetScene();
	m_ElapsedTime = 0.0f;
	for (int i = 0; i < m_NumParticles; ++i)
		m_ParticlesLifetimes[i] = 0.0f;
}

void BoxScene::Update(float dt)
{
	Scene::Update(dt);

	CreateInterface();

	if (m_Paused) return;

	m_ElapsedTime += dt;
	m_NumActiveParticles = static_cast<int>(m_NumParticles * (m_ElapsedTime / s_ParticlesLifetime));
	m_NumActiveParticles = (m_NumActiveParticles <= m_NumParticles ? m_NumActiveParticles : m_NumParticles);

	for (int i = 0; i < m_NumActiveParticles; ++i)
	{
		Particle& particle = m_Particles[i];
		float& particleLifetime = m_ParticlesLifetimes[i];

		particleLifetime -= dt;
		if (particleLifetime <= 0.0f)
		{
			ResetParticle(particle);
			dt = -particleLifetime;
			particleLifetime += s_ParticlesLifetime;
		}

		particle.Update(m_ParticlesProperties, dt);

		CheckPlanes(particle, dt);
		CheckSpheres(particle, dt);
		CheckTriangles(particle, dt);
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
		ImGui::RadioButton("Random", reinterpret_cast<int*>(&m_GenerationType), static_cast<int>(GenerationType::Random));
		ImGui::RadioButton("Cascade", reinterpret_cast<int*>(&m_GenerationType), static_cast<int>(GenerationType::Cascade));
		ImGui::RadioButton("Fountain", reinterpret_cast<int*>(&m_GenerationType), static_cast<int>(GenerationType::Fountain));
	}
	ImGui::End();
}
