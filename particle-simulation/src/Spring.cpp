#include "Spring.h"

Spring::Spring(Particle& particle1, Particle& particle2)
	: particle1(particle1)
	, particle2(particle2)
{
}

void Spring::ApplyForces(const Properties& properties)
{
	Vector3 direction = particle2.GetPosition() - particle1.GetPosition();
	float length = direction.length();
	direction.normalise();

	float elasticTerm = properties.stiffnessK * (length - properties.restLength);
	float dampingTerm = properties.dampingK * (particle2.GetVelocity() - particle1.GetVelocity()).dotProduct(direction);
	Vector3 force = (elasticTerm + dampingTerm) * direction;

	particle1.AddForce(force);
	particle2.AddForce(-force);
}
