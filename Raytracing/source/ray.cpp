#include "ray.h"

Ray::Ray(const Vector3f& Q, const Vector3f& D) : _Q(Q), _D(D)
{
	_D.normalize();
}

Vector3f Ray::eval(float t) const
{
	return _Q + t * _D;
}
