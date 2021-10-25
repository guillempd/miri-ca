#include "ParticleSimulationApp.h"

#include "BoxScene.h"
#include "RopeScene.h"

#include <imgui.h>

#include <Ogre.h>
#include <OgreOverlay.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayManager.h>
#include <OgreImGuiOverlay.h>

#include <vector>

using Ogre::Vector3;

ParticleSimulationApp::ParticleSimulationApp()
    : OgreBites::ApplicationContext("ParticleSimulationApp")
    , m_SceneType(SceneType::ROPE)
    , m_Scene(nullptr)
    , m_InputListenerChain(nullptr)
{
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

    CreateResources();
    CreateScene();
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

void ParticleSimulationApp::CreateResources()
{
    m_PlaneMesh = Ogre::MeshManager::getSingleton().createPlane("Plane", "General", Ogre::Plane(Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 0.0f, 0.0f)), 2, 2);

    m_Materials.reserve(10);

    Ogre::MaterialPtr material;

    material = Ogre::MaterialManager::getSingleton().create("Color1", "General");
    material->setDiffuse(228.0f / 255.0f, 26.0f / 255.0f, 28.0f / 255.0f, 1.0f);
    material->setAmbient(228.0f / 255.0f, 26.0f / 255.0f, 28.0f / 255.0f);
    material->setSpecular(0.9f, 0.9f, 0.9f, 1.0f);
    // material->setShininess(4.0f);
    m_Materials.push_back(material);

    material = Ogre::MaterialManager::getSingleton().create("Color2", "General");
    material->setDiffuse(55.0f / 255.0f, 126.0f / 255.0f, 184.0f / 255.0f, 1.0f);
    material->setAmbient(55.0f / 255.0f, 126.0f / 255.0f, 184.0f / 255.0f);
    material->setSpecular(0.9f, 0.9f, 0.9f, 1.0f);
    // material->setShininess(4.0f);
    m_Materials.push_back(material);

    material = Ogre::MaterialManager::getSingleton().create("Color3", "General");
    material->setDiffuse(77.0f / 255.0f, 175.0f / 255.0f, 74.0f / 255.0f, 1.0f);
    material->setAmbient(77.0f / 255.0f, 175.0f / 255.0f, 74.0f / 255.0f);
    material->setSpecular(0.9f, 0.9f, 0.9f, 1.0f);
    // material->setShininess(4.0f);
    m_Materials.push_back(material);

    material = Ogre::MaterialManager::getSingleton().create("Color4", "General");
    material->setDiffuse(152.0f / 255.0f, 78.0f / 255.0f, 163.0f / 255.0f, 1.0f);
    material->setAmbient(152.0f / 255.0f, 78.0f / 255.0f, 163.0f / 255.0f);
    material->setSpecular(0.9f, 0.9f, 0.9f, 1.0f);
    // material->setShininess(4.0f);
    m_Materials.push_back(material);

    material = Ogre::MaterialManager::getSingleton().create("Color5", "General");
    material->setDiffuse(255.0f / 255.0f, 127.0f / 255.0f, 0.0f / 255.0f, 1.0f);
    material->setAmbient(255.0f / 255.0f, 127.0f / 255.0f, 0.0f / 255.0f);
    material->setSpecular(0.9f, 0.9f, 0.9f, 1.0f);
    // material->setShininess(4.0f);
    m_Materials.push_back(material);

    material = Ogre::MaterialManager::getSingleton().create("Color6", "General");
    material->setDiffuse(255.0f / 255.0f, 255.0f / 255.0f, 51.0f / 255.0f, 1.0f);
    material->setAmbient(255.0f / 255.0f, 255.0f / 255.0f, 51.0f / 255.0f);
    material->setSpecular(0.9f, 0.9f, 0.9f, 1.0f);
    // material->setShininess(4.0f);
    m_Materials.push_back(material);

    material = Ogre::MaterialManager::getSingleton().create("Color7", "General");
    material->setDiffuse(166.0f / 255.0f, 86.0f / 255.0f, 40.0f / 255.0f, 1.0f);
    material->setAmbient(166.0f / 255.0f, 86.0f / 255.0f, 40.0f / 255.0f);
    material->setSpecular(0.9f, 0.9f, 0.9f, 1.0f);
    // material->setShininess(4.0f);
    m_Materials.push_back(material);

    material = Ogre::MaterialManager::getSingleton().create("Color8", "General");
    material->setDiffuse(247.0f / 255.0f, 129.0f / 255.0f, 191.0f / 255.0f, 1.0f);
    material->setAmbient(247.0f / 255.0f, 129.0f / 255.0f, 191.0f / 255.0f);
    material->setSpecular(0.9f, 0.9f, 0.9f, 1.0f);
    // material->setShininess(4.0f);
    m_Materials.push_back(material);

    material = Ogre::MaterialManager::getSingleton().create("Color9", "General");
    material->setDiffuse(153.0f / 255.0f, 153.0f / 255.0f, 153.0f / 255.0f, 1.0f);
    material->setAmbient(153.0f / 255.0f, 153.0f / 255.0f, 153.0f / 255.0f);
    material->setSpecular(0.9f, 0.9f, 0.9f, 1.0f);
    // material->setShininess(4.0f);
    m_Materials.push_back(material);
}

void ParticleSimulationApp::CreateScene()
{
    switch (m_SceneType)
    {
    case SceneType::BOX:
    {
        m_Scene = new BoxScene(m_Materials, m_PlaneMesh);
    } break;
    case SceneType::ROPE:
    {
        m_Scene = new RopeScene(m_Materials, m_PlaneMesh);
    } break;
    }
    m_Scene->Setup(m_SceneManager, getRenderWindow());

    if (m_InputListenerChain) removeInputListener(m_InputListenerChain);

    std::vector<InputListener*> chain = { m_ImGuiInputListener, this, m_Scene->GetCameraMan() };
    m_InputListenerChain = new OgreBites::InputListenerChain(chain);
    addInputListener(m_InputListenerChain);
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
        delete m_Scene;
        CreateScene();
    }
}
