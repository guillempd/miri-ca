#pragma once

#include "Scene.h"

#include <Ogre.h>

class ClothScene : public Scene
{
	using Vector3 = Ogre::Vector3;
public:
	ClothScene(std::vector<Ogre::MaterialPtr>& materials, Ogre::MeshPtr planeMesh);
	~ClothScene() = default;
	void Update(float dt) override;
private:
	void SetupEntities() override;
	void CreateInterface();
private:
	int m_ClothDimension;
};

