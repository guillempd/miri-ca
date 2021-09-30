#include "ParticleSimulationApp.h"

#include <OgreCameraMan.h>

ParticleSimulationApp::ParticleSimulationApp()
	: OgreBites::ApplicationContext("ParticleSimulationApp")
{
}

void ParticleSimulationApp::setup()
{
    OgreBites::ApplicationContext::setup();
    addInputListener(this);

    Ogre::SceneManager *sceneManager = getRoot()->createSceneManager();
    Ogre::RTShader::ShaderGenerator* shaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shaderGenerator->addSceneManager(sceneManager);
    SetupLighting(sceneManager);
    SetupCamera(sceneManager);
    SetupMeshes(sceneManager);
}

bool ParticleSimulationApp::keyPressed(const OgreBites::KeyboardEvent& event)
{
	if (event.keysym.sym == OgreBites::SDLK_ESCAPE) getRoot()->queueEndRendering();
	else return true;
}

void ParticleSimulationApp::SetupLighting(Ogre::SceneManager* sceneManager)
{
    sceneManager->setAmbientLight(Ogre::ColourValue(0.1f, 0.1f, 0.1f));

    Ogre::Light *light = sceneManager->createLight("Light");
    light->setDiffuseColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
    light->setSpecularColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
    Ogre::SceneNode *lightNode = sceneManager->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(0, 10, 15);
    lightNode->attachObject(light);
}

void ParticleSimulationApp::SetupCamera(Ogre::SceneManager* sceneManager)
{
    Ogre::Camera *camera = sceneManager->createCamera("Camera");
    camera->setNearClipDistance(5);
    camera->setAutoAspectRatio(true);

    Ogre::SceneNode *cameraNode = sceneManager->getRootSceneNode()->createChildSceneNode();
    cameraNode->setPosition(0, 0, 15);
    cameraNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);
    cameraNode->attachObject(camera);

    OgreBites::CameraMan *cameraMan = new OgreBites::CameraMan(cameraNode);
    cameraMan->setStyle(OgreBites::CS_FREELOOK);
    cameraMan->setTopSpeed(10.0f);
    addInputListener(cameraMan);

    getRenderWindow()->addViewport(camera);
}

void ParticleSimulationApp::SetupMeshes(Ogre::SceneManager* sceneManager)
{
    Ogre::Entity *sinbad = sceneManager->createEntity("Sinbad.mesh");
    Ogre::SceneNode *sinbadNode = sceneManager->getRootSceneNode()->createChildSceneNode();
    sinbadNode->attachObject(sinbad);
}
