#include "BoxScene.h"

#include "imgui.h"

using Ogre::Vector3;

void BoxScene::Update(float dt)
{
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

	Ogre::MeshPtr meshPtr = Ogre::MeshManager::getSingleton().createPlane("Plane", "General", Ogre::Plane(Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f)), 2, 2);
	m_Planes.reserve(6);
	CreatePlane(Vector3(0.0f, -1.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), meshPtr);
	CreatePlane(Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), meshPtr);
	CreatePlane(Vector3(-1.0f, 0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f), meshPtr);
	CreatePlane(Vector3(1.0f, 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), meshPtr);
	CreatePlane(Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), meshPtr);
	CreatePlane(Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f), meshPtr);

	m_Spheres.reserve(1);
	CreateSphere(Vector3(0.0f, -0.50f, 0.0f), 0.25f);

	m_Triangles.reserve(2);
	CreateTriangle(Vector3(0.5f, 0.0f, 0.0f), Vector3(-0.5f, 0.1f, 0.5f), Vector3(-0.5f, 0.2f, -0.5f));
	// CreateTriangle(Vector3(-1.0f, -1.0f, -1.0f), Vector3(-1.0f, -1.0f, 1.0f), Vector3(1.0f, -1.0f, -1.0f)); // Redundant triangle for testing coplanar collisions
}

void BoxScene::CreateInterface()
{
	ImGui::ShowMetricsWindow(); // For fps counter

	if (ImGui::Begin("Settings"))
	{
		ImGui::Text("Solver Method");
		ImGui::RadioButton("Euler Original", reinterpret_cast<int*>(&m_SolverMethod), static_cast<int>(Particle::SolverMethod::Euler));
		ImGui::RadioButton("Euler Semi Implicit", reinterpret_cast<int*>(&m_SolverMethod), static_cast<int>(Particle::SolverMethod::EulerSemi));
		ImGui::RadioButton("Verlet", reinterpret_cast<int*>(&m_SolverMethod), static_cast<int>(Particle::SolverMethod::Verlet));

		ImGui::Separator();

		ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp;

		ImGui::Text("Particle Properties");
		ImGui::DragFloat("Mass", &m_ParticlesPhysicalProperties.mass, 0.05f, std::numeric_limits<float>::min(), std::numeric_limits<float>::max(), "%.3f", flags);
		ImGui::SliderFloat("Bouncing Coefficient", &m_ParticlesPhysicalProperties.bouncingCoefficient, 0.0f, 1.0f, "%.3f", flags);
		ImGui::SliderFloat("Friction Coefficient", &m_ParticlesPhysicalProperties.frictionCoefficient, 0.0f, 1.0f, "%.3f", flags);
		ImGui::DragFloat("Lifetime", &m_ParticlesPhysicalProperties.lifetime, 0.05f, 1.0f, std::numeric_limits<float>::max(), "%.3f", flags);
		ImGui::DragFloat3("Gravity", m_ParticlesPhysicalProperties.gravity.ptr(), 0.05f, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), "%.3f", flags);

		ImGui::Separator();

		ImGui::Text("Generation Method");
		ImGui::RadioButton("Random", reinterpret_cast<int*>(&m_GenerationType), static_cast<int>(Particle::GenerationType::Random));
		ImGui::RadioButton("Cascade", reinterpret_cast<int*>(&m_GenerationType), static_cast<int>(Particle::GenerationType::Cascade));
		ImGui::RadioButton("Fountain", reinterpret_cast<int*>(&m_GenerationType), static_cast<int>(Particle::GenerationType::Fountain));

		//ImGui::Separator();
		// TODO: Add controls for number of particles, probably should reset the scene after changing this
	}
	ImGui::End();
}
