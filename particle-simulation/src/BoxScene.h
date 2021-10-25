#pragma once

#include "Scene.h"

#include <Ogre.h>

#include <vector>
#include <random>

class BoxScene : public Scene
{
	using Vector3 = Ogre::Vector3;
public:
	enum class GenerationType { Random, Cascade, Fountain };
public:
	BoxScene(std::vector<Ogre::MaterialPtr>& materials, Ogre::MeshPtr planeMesh);
	~BoxScene() = default;
	void Update(float dt) override;
private:
	void SetupEntities() override;
	void CreateInterface();
	void ResetParticle(Particle& particle);
private:
	int m_NumParticles;
	int m_NumActiveParticles;
	float m_ElapsedTime;
	GenerationType m_GenerationType;
	std::uniform_real_distribution<float> m_UniformFloat;
};
