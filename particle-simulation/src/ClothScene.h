#pragma once

#include "Scene.h"
#include "Spring.h"

#include <Ogre.h>

#include <vector>

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
	std::vector<Spring> m_StretchSprings;
	std::vector<Spring> m_ShearSprings;
	std::vector<Spring> m_BendSprings;
	Spring::Properties m_StretchProperties;
	Spring::Properties m_ShearProperties;
	Spring::Properties m_BendProperties;
};

