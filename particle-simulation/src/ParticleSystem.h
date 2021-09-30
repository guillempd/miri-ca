#pragma once

#include <OgreVector3.h>

class ParticleSystem
{
public:
	ParticleSystem();
	ParticleSystem(const Ogre::Vector3 &particlePosition);
	Ogre::Vector3 GetParticlePosition() { return m_ParticlePosition; }
	void Update(float timeStep);
private:
	Ogre::Vector3 m_ParticlePosition; // TODO: Add more particles
};

