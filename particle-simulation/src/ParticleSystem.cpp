#include "ParticleSystem.h"

#include <imgui.h>

using Ogre::Vector3;

ParticleSystem::ParticleSystem()
	: ParticleSystem(Vector3(-1.0f, 5.0f, -1.0f))
{
}

ParticleSystem::ParticleSystem(const Vector3& particlePosition)
	: m_Particle(particlePosition)
	, m_Plane(Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f)) // Floor
	, m_Sphere{Vector3(0.0f, 0.0f, 0.0f), 3.0f}
{
}

void ParticleSystem::Update(float dt)
{
	if (ImGui::Begin("Solver Method"))
	{
		ImGui::RadioButton("Euler Original", reinterpret_cast<int*>(&m_SolverMethod), static_cast<int>(Particle::SolverMethod::Euler));
		ImGui::RadioButton("Euler Semi Implicit", reinterpret_cast<int*>(&m_SolverMethod), static_cast<int>(Particle::SolverMethod::EulerSemi));
		ImGui::RadioButton("Verlet", reinterpret_cast<int*>(&m_SolverMethod), static_cast<int>(Particle::SolverMethod::Verlet));
	}
	ImGui::End();

	m_Particle.Update(dt, m_SolverMethod);
	m_Particle.CheckAndResolveCollision(m_Plane);
	m_Particle.CheckAndResolveCollision(m_Sphere);
}
