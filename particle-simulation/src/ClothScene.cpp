#include "ClothScene.h"

#include <imgui.h>

ClothScene::ClothScene(std::vector<Ogre::MaterialPtr>& materials, Ogre::MeshPtr planeMesh)
	: Scene(materials, planeMesh)
	, m_ClothDimension(10)
	, m_StretchSprings()
	, m_ShearSprings()
	, m_BendSprings()
	, m_StretchProperties{50.0f, 0.1f, 0.1f}
	, m_ShearProperties{0.0f, 0.0f, 0.2f}
	, m_BendProperties{50.0f, 0.1f, 0.2f}
	, m_InitialPositionOffset{0.0f, 0.0f}
{
	// TODO: Correctly initialize springs constants
}

void ClothScene::Update(float dt)
{
	Scene::Update(dt);

	CreateInterface();

	if (m_Paused) return;

	// Apply forces
	for (int i = 0; i < static_cast<int>(m_StretchSprings.size()); ++i)
		m_StretchSprings[i].ApplyForces(m_StretchProperties);

	for (int i = 0; i < static_cast<int>(m_ShearSprings.size()); ++i)
		m_ShearSprings[i].ApplyForces(m_ShearProperties);

	for (int i = 0; i < static_cast<int>(m_BendSprings.size()); ++i)
		m_BendSprings[i].ApplyForces(m_BendProperties);

	// Update particles
	for (int i = 0; i < m_ClothDimension; ++i)
	{
		for (int j = 0; j < m_ClothDimension; ++j)
		{
			Particle& particle = m_Particles[i*m_ClothDimension + j];

			particle.Update(m_ParticlesProperties, dt);
			CheckPlanes(particle, dt);
			CheckSpheres(particle, dt);
			CheckTriangles(particle, dt);
		}
	}
}

void ClothScene::SetupEntities()
{
	// TODO: Correctly reserve appropriate sizes
	m_Particles.reserve(m_ClothDimension * m_ClothDimension);
	m_StretchSprings.reserve(0);
	m_ShearSprings.reserve(0);
	m_BendSprings.reserve(0);
	std::vector<std::vector<Particle*>> particles = std::vector<std::vector<Particle*>>(m_ClothDimension, std::vector<Particle*>(m_ClothDimension));
	for (int i = 0; i < m_ClothDimension; ++i)
	{
		for (int j = 0; j < m_ClothDimension; ++j)
		{
			float centerOffset = static_cast<float>(m_ClothDimension - 1) / (2 * m_ClothDimension);
			Vector3 initialPosition = Vector3(static_cast<float>(i)/m_ClothDimension - centerOffset + m_InitialPositionOffset[0], 0.75f, static_cast<float>(j)/m_ClothDimension - centerOffset + m_InitialPositionOffset[1]);
			Particle &particle = CreateParticle();
			particle.Set(initialPosition, Vector3::ZERO);
			particles[i][j] = &particle;
		}
	}

	// Strecth springs

	// First column
	for (int i = 1; i < m_ClothDimension; ++i)
		m_StretchSprings.emplace_back(*particles[i][0], *particles[i-1][0]);

	// First row
	for (int j = 1; j < m_ClothDimension; ++j)
		m_StretchSprings.emplace_back(*particles[0][j], *particles[0][j-1]);

	// Rest
	for (int i = 1; i < m_ClothDimension; ++i)
	{
		for (int j = 1; j < m_ClothDimension; ++j)
		{
			m_StretchSprings.emplace_back(*particles[i][j], *particles[i][j-1]);
			m_StretchSprings.emplace_back(*particles[i][j], *particles[i-1][j]);
		}
	}

	// Shear springs
	for (int i = 1; i < m_ClothDimension - 1; ++i)
	{
		for (int j = 1; j < m_ClothDimension - 1; ++j)
		{
			m_ShearSprings.emplace_back(*particles[i][j], *particles[i-1][j-1]);
			m_ShearSprings.emplace_back(*particles[i][j], *particles[i-1][j+1]);
			m_ShearSprings.emplace_back(*particles[i][j], *particles[i+1][j-1]);
			m_ShearSprings.emplace_back(*particles[i][j], *particles[i+1][j+1]);
		}
	}

	// Bend springs

	// First column
	for (int i = 2; i < m_ClothDimension; ++i)
		m_BendSprings.emplace_back(*particles[i][0], *particles[i-2][0]);

	// First row
	for (int j = 2; j < m_ClothDimension; ++j)
		m_BendSprings.emplace_back(*particles[0][j], *particles[0][j-2]);

	// Rest
	for (int i = 2; i < m_ClothDimension; ++i)
	{
		for (int j = 2; j < m_ClothDimension; ++j)
		{
			m_BendSprings.emplace_back(*particles[i][j], *particles[i][j-2]);
			m_BendSprings.emplace_back(*particles[i][j], *particles[i-2][j]);
		}
	}

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

void ClothScene::CreateInterface()
{
	if (ImGui::Begin("Spring Properties"))
	{
		ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp;

		ImGui::DragFloat2("Initial Position Offset", m_InitialPositionOffset, 0.01f, -0.5f, 0.5f, "%.3f", flags);

		ImGui::Separator();

		ImGui::DragFloat("Stretch Stiffness Constant", &m_StretchProperties.stiffnessK, 0.01f, 0.0f, std::numeric_limits<float>::max(), "%.3f", flags);
		ImGui::DragFloat("Stretch Damping Constant", &m_StretchProperties.dampingK, 0.01f, 0.0f, std::numeric_limits<float>::max(), "%.3f", flags);
		ImGui::DragFloat("Stretch Rest Length", &m_StretchProperties.restLength, 0.001f, 0.0f, 0.5f, "%.3f", flags);

		ImGui::Separator();

		ImGui::DragFloat("Shear Stiffness Constant", &m_ShearProperties.stiffnessK, 0.01f, 0.0f, std::numeric_limits<float>::max(), "%.3f", flags);
		ImGui::DragFloat("Shear Damping Constant", &m_ShearProperties.dampingK, 0.01f, 0.0f, std::numeric_limits<float>::max(), "%.3f", flags);
		ImGui::DragFloat("Shear Rest Length", &m_ShearProperties.restLength, 0.001f, 0.0f, 0.5f, "%.3f", flags);

		ImGui::Separator();

		ImGui::DragFloat("Bend Stiffness Constant", &m_BendProperties.stiffnessK, 0.01f, 0.0f, std::numeric_limits<float>::max(), "%.3f", flags);
		ImGui::DragFloat("Bend Damping Constant", &m_BendProperties.dampingK, 0.01f, 0.0f, std::numeric_limits<float>::max(), "%.3f", flags);
		ImGui::DragFloat("Bend Rest Length", &m_BendProperties.restLength, 0.001f, 0.0f, 0.5f, "%.3f", flags);
	}
	ImGui::End();
}

void ClothScene::ResetScene()
{
	Scene::ResetScene();
	m_StretchSprings.clear();
	m_ShearSprings.clear();
	m_BendSprings.clear();
}
