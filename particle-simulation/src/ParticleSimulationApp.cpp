#include "ParticleSimulationApp.h"

#include "BoxScene.h"

#include <imgui.h>

#include <Ogre.h>
#include <OgreOverlay.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayManager.h>
#include <OgreImGuiOverlay.h>

#include <vector>

ParticleSimulationApp::ParticleSimulationApp()
    : OgreBites::ApplicationContext("ParticleSimulationApp")
    , m_SceneType(SceneType::BOX)
{
    // TODO: Make a switch according to m_SceneType
    m_Scene = new BoxScene();
}

// TODO: Correctly delete al resources that are generated
void ParticleSimulationApp::setup()
{
    OgreBites::ApplicationContext::setup();

    m_SceneManager = getRoot()->createSceneManager();

    m_SceneManager->addRenderQueueListener(getOverlaySystem());
    m_ImGuiOverlay = new Ogre::ImGuiOverlay();
    m_ImGuiInputListener = new OgreBites::ImGuiInputListener();
    Ogre::OverlayManager::getSingleton().addOverlay(m_ImGuiOverlay);
    m_ImGuiOverlay->show();

    Ogre::RTShader::ShaderGenerator::getSingleton().addSceneManager(m_SceneManager);

    m_Scene->Setup(m_SceneManager, getRenderWindow());

    std::vector<InputListener*> chain = {m_ImGuiInputListener, this, m_Scene->GetCameraMan()};
    OgreBites::InputListenerChain *inputListenerChain = new OgreBites::InputListenerChain(chain);
    addInputListener(inputListenerChain);
}

void ParticleSimulationApp::shutdown()
{
    OgreBites::ApplicationContext::shutdown();
    // delete m_ImGuiOverlay; // TODO: OverlayManager::removeOverlay(m_ImGuiOverlay) (?)
    delete m_ImGuiInputListener;
}

bool ParticleSimulationApp::keyPressed(const OgreBites::KeyboardEvent& event)
{
    if (event.keysym.sym == OgreBites::SDLK_ESCAPE)
    {
        getRoot()->queueEndRendering();
        return true;
    }
	else return false;
}

bool ParticleSimulationApp::frameStarted(const Ogre::FrameEvent& event)
{
    OgreBites::ApplicationContext::frameStarted(event);
    Ogre::ImGuiOverlay::NewFrame();
    CreateSceneSelectionInterface();
    m_Scene->Update(event.timeSinceLastFrame);
    return true; // TODO: What should this return value be (?)
}

bool ParticleSimulationApp::frameEnded(const Ogre::FrameEvent& event)
{
    OgreBites::ApplicationContext::frameEnded(event);
    ImGui::EndFrame();
    return true; // TODO: What should this return value be (?)
}

void ParticleSimulationApp::CreateSceneSelectionInterface()
{
    SceneType previousSceneType = m_SceneType;
    if (ImGui::Begin("Select Scene"))
    {
        ImGui::RadioButton("Box Scene", reinterpret_cast<int*>(&m_SceneType), static_cast<int>(SceneType::BOX));
        ImGui::RadioButton("Rope Scene", reinterpret_cast<int*>(&m_SceneType), static_cast<int>(SceneType::ROPE));
    }
    ImGui::End();

    if (m_SceneType != previousSceneType)
    {
        // TODO: Switch to new scene type (delete and create)
    }
}
