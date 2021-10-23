#pragma once

#include "Scene.h"

#include <Ogre.h>

class BoxScene : public Scene
{
	using Vector3 = Ogre::Vector3;
public:
	BoxScene() = default;
	~BoxScene() = default;
	void Update(float dt) override;
private:
	void SetupEntities() override;
	void CreateInterface();
};
