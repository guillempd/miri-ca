#pragma once

#include "Scene.h"

#include <Ogre.h>
#include <OgreApplicationContext.h>
#include <OgreImGuiInputListener.h>
#include <OgreImGuiOverlay.h>

class ParticleSimulationApp
	: public OgreBites::ApplicationContext
	, public OgreBites::InputListener
{
public:
	ParticleSimulationApp();
	void setup() override;
	void shutdown() override;
	bool keyPressed(const OgreBites::KeyboardEvent& event) override;
	bool frameStarted(const Ogre::FrameEvent& event) override;
	bool frameEnded(const Ogre::FrameEvent& event) override;
private:
	Scene m_Scene;
	Ogre::SceneManager* m_SceneManager;
	OgreBites::ImGuiInputListener* m_ImGuiInputListener;
	Ogre::ImGuiOverlay* m_ImGuiOverlay;
};

