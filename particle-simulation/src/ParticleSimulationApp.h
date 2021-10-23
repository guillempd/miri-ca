#pragma once

#include "Scene.h"

#include <Ogre.h>
#include <OgreApplicationContext.h>
#include <OgreImGuiInputListener.h>
#include <OgreImGuiOverlay.h>

#include <vector>

class ParticleSimulationApp
	: public OgreBites::ApplicationContext
	, public OgreBites::InputListener
{
public:
	enum class SceneType {BOX, ROPE};
public:
	ParticleSimulationApp();
	void setup() override;
	void shutdown() override;
	bool keyPressed(const OgreBites::KeyboardEvent& event) override;
	bool frameStarted(const Ogre::FrameEvent& event) override;
	bool frameEnded(const Ogre::FrameEvent& event) override;
private:
	void CreateResources();
	void CreateScene();
	void CreateSceneSelectionInterface();
private:
	SceneType m_SceneType;
	Scene* m_Scene;
	OgreBites::InputListenerChain* m_InputListenerChain;
	OgreBites::ImGuiInputListener* m_ImGuiInputListener;
	Ogre::SceneManager* m_SceneManager;
	Ogre::ImGuiOverlay* m_ImGuiOverlay;
	std::vector<Ogre::MaterialPtr> m_Materials;
	Ogre::MeshPtr m_PlaneMesh;
};

