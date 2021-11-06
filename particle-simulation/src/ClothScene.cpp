#include "ClothScene.h"

#include <imgui.h>

ClothScene::ClothScene(std::vector<Ogre::MaterialPtr>& materials, Ogre::MeshPtr planeMesh)
	: Scene(materials, planeMesh)
	, m_ClothDimension(10)
{
}

void ClothScene::Update(float dt)
{
	Scene::Update(dt);

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
	m_Particles.reserve(m_ClothDimension * m_ClothDimension);
	for (int i = 0; i < m_ClothDimension; ++i)
	{
		for (int j = 0; j < m_ClothDimension; ++j)
		{
			Vector3 initialPosition = Vector3(static_cast<float>(i)/m_ClothDimension, 0.75f, static_cast<float>(j)/m_ClothDimension);
			Particle &particle = CreateParticle();
			particle.Set(initialPosition, Vector3::ZERO);
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
