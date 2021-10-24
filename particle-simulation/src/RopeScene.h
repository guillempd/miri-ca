#pragma once

#include "Scene.h"

#include <Ogre.h>

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
	float m_ElasticityK;
	float m_RestLength;
};
