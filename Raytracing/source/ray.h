#pragma once
#include "geom.h"

class Ray
{
public:
	Ray(const Vector3f& Q, const Vector3f& D);

	Vector3f eval(float t) const;

	Vector3f _Q; // Starting point
	Vector3f _D; // Direction - should be scaled to a unit length vector

};