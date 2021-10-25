#pragma once

#include "Scene.h"

#include <Ogre.h>

#include <vector>

class BoxScene : public Scene
{
	using Vector3 = Ogre::Vector3;
public:
	BoxScene(std::vector<Ogre::MaterialPtr>& materials, Ogre::MeshPtr planeMesh);
	~BoxScene() = default;
	void Update(float dt) override;
private:
	void SetupEntities() override;
	void CreateInterface();
private:
	int m_NumParticles;
	int m_NumActiveParticles;
	float m_ElapsedTime;
};
