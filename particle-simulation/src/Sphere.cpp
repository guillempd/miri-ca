#include "Sphere.h"

using Ogre::Vector3;

Sphere::Sphere(Ogre::SceneNode* sceneNode, const Vector3& center, float radius)
	: m_SceneNode(sceneNode)
	, m_Center(center)
	, m_Radius(radius)
{
	m_SceneNode->scale(radius, radius, radius);
}

float Sphere::SignedDistance(const Vector3& point) const
{
	return (point - m_Center).length() - m_Radius;
}

Vector3 Sphere::ContactPoint(const Vector3& origin, const Vector3& destiny) const
{
	Vector3 direction = destiny - origin;
	Vector3 centeredOrigin = origin - m_Center;

	float a = direction.squaredLength();
	float b = 2 * centeredOrigin.dotProduct(direction);
	float c = centeredOrigin.squaredLength() - m_Radius * m_Radius;

	float d = Ogre::Math::Sqrt(b*b - 4*a*c);
	float lambda_0 = (-b + d) / (2*a);
	float lambda_1 = (-b - d) / (2*a);

	float lambda;
	if (lambda_0 < 0.0f) lambda = lambda_1;
	else if (lambda_1 < 0.0f) lambda = lambda_0;
	else lambda = (lambda_0 < lambda_1 ? lambda_0 : lambda_1);

	return origin + lambda * direction;
}