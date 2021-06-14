#pragma once

#ifndef INTERSECTION_H
#define INTERSECTION_H
//#include "shape.h"
#include "geom.h"

class Shape;

class Intersection
{
public:
	Intersection() { _t = FLT_MAX; }


	float _t; // Parameter value on ray of the point of intersection
	Shape* _pObject; // A pointer to the Shape intersected
	Vector3f _P; // Point of intersection (in world coordinates)
	Vector3f _N; // Normal of surface at intersection point (in world coordinates)
	//Vector2f _TexCoords; 2D texture coordinates
};

#endif