/******************************************************************************/
/*!
\file   Object.h
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS350
\par    Assignment 3 Octree and BSPTree
\par    Spring 2021
\date   3/30/2021
\brief
	This file contains the prototype of Object class and related functions

  Hours spent on this assignment:

  Specific portions that gave you the most trouble:

*/
/******************************************************************************/
#pragma once
#include <glm/glm.hpp> // glm
#include "Geometry.h" // Plane
#include <vector> // std::vector
/******************************************************************************/
/*!
  \struct Object
  \brief
	The struct encapsulate a triangle and it's center and radius
	for AABB or sphere
*/
/******************************************************************************/
struct Object {
	glm::vec4 _center;
	float _radius;
	//Object* _pNextObject;

	std::vector<glm::vec4> _points;

	Object(const glm::vec4& p0, const glm::vec4& p1, const glm::vec4& p2);
	Object(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2);
	Object();

	Aabb GetAabb() const;
};

void CalcCenterRadius(glm::vec4& center, float& radius, glm::vec4 p0, glm::vec4 p1, glm::vec4 p2);

void SplitObject(const Object& original, const Plane& plane, std::vector<glm::vec3>& frontPoly, std::vector<glm::vec3>& backPoly);

// helper function to output 4 vertices to 2 object
void SplitPolytoObject(const std::vector<glm::vec3>& poly, Object& object0, Object& object1);

Plane GetPlaneFromObject(const Object& object);

Plane PickSplittingPlane(const std::vector<Object>& objects);

enum class Object_Classification
{
	OBJECT_STRADDLING_PLANE = 0,
	OBJECT_IN_FRONT_OF_PLANE = 1,
	OBJECT_BEHIND_PLANE = 2,
	OBJECT_COPLANAR_WITH_PLANE = 3
};

Object_Classification ClassifyObjectToPlane(const Object& object, const Plane& plane);