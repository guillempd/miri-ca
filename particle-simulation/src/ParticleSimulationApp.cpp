#include "ParticleSimulationApp.h"

#include <Ogre.h>
#include <OgreOverlay.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayManager.h>
#include <OgreImGuiOverlay.h>

#include <vector>

// TODO: Cleanup this includes

ParticleSimulationApp::ParticleSimulationApp()
	: OgreBites::ApplicationContext("ParticleSimulationApp")
    , m_ParticleSystem()
{
}

// TODO: Correctly delete al resources that are generated
void ParticleSimulationApp::setup()
{
    OgreBites::ApplicationContext::setup();

    m_SceneManager = getRoot()->createSceneManager();
    Ogre::RTShader::ShaderGenerator* shaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shaderGenerator->addSceneManager(m_SceneManager);
    SetupLighting();
    SetupCamera();
    SetupMeshes();

    CreateOverlay();

    std::vector<InputListener*> chain = {m_ImGui, this, m_CameraMan};
    OgreBites::InputListenerChain *inputListenerChain = new OgreBites::InputListenerChain(chain);
    addInputListener(inputListenerChain);
}

void ParticleSimulationApp::shutdown()
{
    OgreBites::ApplicationContext::shutdown();
    m_SceneManager->destroyLight(m_Light);
    m_SceneManager->destroyCamera(m_Camera);
    m_SceneManager->destroyEntity(m_Sinbad);
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
    m_ParticleSystem.Update(event.timeSinceLastFrame);
    m_SceneManager->getSceneNode("Sinbad")->setPosition(m_ParticleSystem.GetParticlePosition()); // TODO: Manage better SceneNode's so no lookups have to be done // TODO: Update scene based on the data from m_ParticleSystem
    return true;
}

bool ParticleSimulationApp::frameEnded(const Ogre::FrameEvent& event)
{
    OgreBites::ApplicationContext::frameEnded(event);
    ImGui::EndFrame();
    return true; // TODO: What should this return value be (?)
}

void ParticleSimulationApp::SetupLighting()
{
    m_SceneManager->setAmbientLight(Ogre::ColourValue(0.1f, 0.1f, 0.1f));

    m_Light = m_SceneManager->createLight("Light");
    m_Light->setDiffuseColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
    m_Light->setSpecularColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
    Ogre::SceneNode *lightNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(0, 10, 15);
    lightNode->attachObject(m_Light);
}

void ParticleSimulationApp::SetupCamera()
{
    m_Camera = m_SceneManager->createCamera("Camera");
    m_Camera->setNearClipDistance(5);
    m_Camera->setAutoAspectRatio(true);

    Ogre::SceneNode *cameraNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
    cameraNode->setPosition(0, 0, 15);
    cameraNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);
    cameraNode->attachObject(m_Camera);

    m_CameraMan = new OgreBites::CameraMan(cameraNode);
    m_CameraMan->setStyle(OgreBites::CS_ORBIT);
    m_CameraMan->setTarget(m_SceneManager->getRootSceneNode());
    m_CameraMan->setTopSpeed(10.0f);

    getRenderWindow()->addViewport(m_Camera);
}

void ParticleSimulationApp::SetupMeshes()
{
    m_Sinbad = m_SceneManager->createEntity("Sinbad.mesh");
    Ogre::SceneNode *sinbadNode = m_SceneManager->getRootSceneNode()->createChildSceneNode("Sinbad");
    sinbadNode->attachObject(m_Sinbad);
}

void ParticleSimulationApp::CreateOverlay()
{
    // Add the overlay system to listen for render events
    Ogre::OverlaySystem *overlaySystem = getOverlaySystem();
    m_SceneManager->addRenderQueueListener(overlaySystem);

    Ogre::ImGuiOverlay* imguiOverlay = new Ogre::ImGuiOverlay();
    Ogre::OverlayManager::getSingletonPtr()->addOverlay(imguiOverlay);
    imguiOverlay->show();

    m_ImGui = new OgreBites::ImGuiInputListener();
}
