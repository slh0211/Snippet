#include "minimizer.h"

Minimizer::Minimizer(const Ray& r) : _ray(r)
{
}

float Minimizer::minimumOnObject(Shape* obj)
{
	Intersection currentResult;
	bool hit = obj->intersect(_ray, currentResult); // or whatever
     // Keep track the minimal intersection and object
	if (hit)
	{
		// collect smallest t
		if (currentResult._t < _result._t)
		{
			_result = currentResult;
			return _result._t;
		}
	
	}

	return FLT_MAX;

	//float result = minimumOnVolume(obj->_box);
	//if (result != 0.0f && result != FLT_MAX)
	//{
	//	if (result < _result._t)
	//	{
	//		_result._pObject = obj;
	//		_result._P = _ray.eval(result);
	//		_result._t = result;
	//
	//		return _result._t;
	//	}
	//
	//}
	//
	//return FLT_MAX;
}

float Minimizer::minimumOnVolume(const Bbox& box)
{
	Vector3f L = box.min(); // Box corner
	Vector3f U = box.max(); // Box corner

	Interval iv0 = Interval();

	iv0.intersect(_ray, Vector3f(1.0f, 0.0f, 0.0f), -L.x(), -U.x());
	if (iv0._t0 > iv0._t1)
	{
		return FLT_MAX;
	}
	iv0.intersect(_ray, Vector3f(0.0f, 1.0f, 0.0f), -L.y(), -U.y());
	if (iv0._t0 > iv0._t1)
	{
		return FLT_MAX;
	}
	iv0.intersect(_ray, Vector3f(0.0f, 0.0f, 1.0f), -L.z(), -U.z());

	//Interval iv1 = iv0.intersect(_ray, Vector3f(1.0f, 0.0f, 0.0f), -L.x(), -U.x());
	//Interval iv2 = iv1.intersect(_ray, Vector3f(0.0f, 1.0f, 0.0f), -L.y(), -U.y());
	//Interval iv3 = iv2.intersect(_ray, Vector3f(0.0f, 0.0f, 1.0f), -L.z(), -U.z());
	//
	//iv0 = iv3;

	if (iv0._t0 > iv0._t1)
	{
		return FLT_MAX;
	}
	else
	{
	    if (iv0._t0 >= 0.0f)
		{
			return iv0._t0;
		}
		else if(iv0._t1 >= 0.0f)
		{
			return iv0._t1;
		}
		else
		{
			return 0.0f;
		}
	}
}


