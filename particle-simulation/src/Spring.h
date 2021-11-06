#pragma once

#include "Particle.h"

#include <Ogre.h>

class Spring
{
	using Vector3 = Ogre::Vector3;
public:
	struct Properties
	{
		float stiffnessK;
		float dampingK;
		float restLength;
	};
public:
	Spring(Particle& particle1, Particle& particle2);
	void ApplyForces(const Properties& properties);
private:
	Particle& particle1;
	Particle& particle2;
};

