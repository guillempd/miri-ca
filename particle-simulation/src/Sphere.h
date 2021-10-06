#pragma once

#include <Ogre.h>

struct Sphere
{
	using Vector3 = Ogre::Vector3;
	Vector3 center;
	float radius;

	float DistanceToSurface(const Vector3 &point) const
	{
		return (point - center).length() - radius; // TODO: use squaredLength() and rename function
	}

	Vector3 ContactPoint(const Vector3& origin, const Vector3& destiny) const
	{
		Vector3 direction = destiny - origin;
		Vector3 centeredOrigin = origin - center;

		float a = direction.squaredLength();
		float b = 2 * centeredOrigin.dotProduct(direction);
		float c = centeredOrigin.squaredLength() - radius*radius;

		float d = Ogre::Math::Sqrt(b*b - 4*a*c);
		float lambda_0 = (-b + d)/(2*a);
		float lambda_1 = (-b - d)/(2*a);
		
		float lambda;
		if (lambda_0 < 0.0f) lambda = lambda_1;
		else if (lambda_1 < 0.0f) lambda = lambda_0;
		else lambda = (lambda_0 < lambda_1 ? lambda_0 : lambda_1);

		return origin + lambda * direction;

	}
};
