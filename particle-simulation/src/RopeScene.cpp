#include "RopeScene.h"

#include <imgui.h>

using Ogre::Vector3;

RopeScene::RopeScene(std::vector<Ogre::MaterialPtr>& materials, Ogre::MeshPtr planeMesh)
	: Scene(materials, planeMesh)
{
}
// TODO: Handle initialization of the particles tha form the rope
void RopeScene::Update(float dt)
{
	CreateInterface();

	int numParticles = m_Particles.size();

	// Compute forces TODO: Add damping term
	for (int i = 1; i < numParticles; ++i)
	{
		Particle& particle1 = m_Particles[i-1];
		Particle& particle2 = m_Particles[i];

		Vector3 direction = particle2.GetPosition() - particle1.GetPosition();
		float length = direction.length();
		direction.normalise();
		Vector3 force = m_ElasticityK * (length - m_RestLength)*direction;

		particle1.AddForce(force);
		particle2.AddForce(-force);
	}

	// Update particles
	for (int i = 0; i < numParticles; ++i)
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
		Vector3 initialPosition = Vector3(i / static_cast<float>(numParticles) * 2.0f - 1.0f, 0.75f, 0.0f);
		m_Particles[i].SetPosition(initialPosition);
	}

	m_Planes.reserve(6);
	CreatePlane(Vector3(0.0f, -1.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f));
	CreatePlane(Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
	CreatePlane(Vector3(-1.0f, 0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f));
	CreatePlane(Vector3(1.0f, 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f));
	CreatePlane(Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f));
	CreatePlane(Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f));

	m_Spheres.reserve(1);
	CreateSphere(Vector3(0.0f, -0.5f, 0.0f), 0.25f);
}

// TODO: Add interface similar to BoxScene
void RopeScene::CreateInterface()
{
	ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp;

	ImGui::ShowMetricsWindow(); // For fps counter

	if (ImGui::Begin("Settings"))
	{
		ImGui::Text("Solver Method");
		ImGui::RadioButton("Euler Original", reinterpret_cast<int*>(&m_SolverMethod), static_cast<int>(Particle::SolverMethod::Euler));
		ImGui::RadioButton("Euler Semi Implicit", reinterpret_cast<int*>(&m_SolverMethod), static_cast<int>(Particle::SolverMethod::EulerSemi));
		ImGui::RadioButton("Verlet", reinterpret_cast<int*>(&m_SolverMethod), static_cast<int>(Particle::SolverMethod::Verlet));

		ImGui::Separator();

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

	if (ImGui::Begin("Spring Settings"))
	{
		ImGui::DragFloat("Elasticity", &m_ElasticityK, 1.0f, 0.0f, std::numeric_limits<float>::max(),"%.3f", flags);
		ImGui::DragFloat("RestLength", &m_RestLength, 0.05f, 0.0f, 0.25f, "%.3f", flags);
	}
	ImGui::End();
}
