#pragma once

#include "Scene.h"
#include "Spring.h"

#include <Ogre.h>

#include <vector>

class RopeScene : public Scene
{
	using Vector3 = Ogre::Vector3;
public:
	RopeScene(std::vector<Ogre::MaterialPtr>& materials, Ogre::MeshPtr planeMesh);
	~RopeScene() = default;
	void Update(float dt) override;
private:
	void SetupEntities() override;
	void CreateInterface();
private:
	int m_NumParticles;
	std::vector<Spring> m_Springs;
	Spring::Properties m_SpringProperties;
};
