#pragma once

#include "ParticleSystem.h"

#include <Ogre.h>
#include <OgreApplicationContext.h>

class ParticleSimulationApp
	: public OgreBites::ApplicationContext
	, public OgreBites::InputListener
{
public:
	ParticleSimulationApp();
	void setup() override;
	void shutdown() override;
	bool keyPressed(const OgreBites::KeyboardEvent& event) override;
	bool frameEnded(const Ogre::FrameEvent& event) override;
private:
	void SetupLighting();
	void SetupCamera();
	void SetupMeshes();
private:
	ParticleSystem m_ParticleSystem;
	Ogre::SceneManager *m_SceneManager;
	Ogre::Light* m_Light;
	Ogre::Camera* m_Camera;
	Ogre::Entity* m_Sinbad;
};

