#pragma once

#include <Ogre.h>
#include <OgreApplicationContext.h>

class ParticleSimulationApp
	: public OgreBites::ApplicationContext
	, public OgreBites::InputListener
{
public:
	ParticleSimulationApp();
	void setup() override;
	bool keyPressed(const OgreBites::KeyboardEvent& event) override;
private:
	void SetupLighting(Ogre::SceneManager *sceneManager);
	void SetupCamera(Ogre::SceneManager *sceneManager);
	void SetupMeshes(Ogre::SceneManager *sceneManager);
};

