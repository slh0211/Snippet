#pragma once
#include "geom.h"
#include "ray.h"
#include "intersection.h"
#include "shape.h"

class Minimizer
{
public:
	typedef float Scalar; // KdBVH needs Minimizer::Scalar defined

	Ray _ray;
	Intersection _result;
	// Stuff to track the minimal t and its intersection info
	// Constructor
	Minimizer(const Ray& r);
	// Called by BVMinimize to intersect the ray with a Shape. This
	// should return the intersection t, but should also track
	// the minimum t and it's corresponding intersection info.
	// Return INF to indicate no intersection.
	float minimumOnObject(Shape* obj);

	// Called by BVMinimize to intersect the ray with a Bbox and
	// returns the t value. This should be similar to the already
	// written box (3 slab) intersection. (The difference being: a
	// distance of zero should be returned if the ray starts within the Bbox.)
	// Return INF to indicate no intersection.
	float minimumOnVolume(const Bbox& box);
};
