/******************************************************************************/
/*!
\file   Object.cpp
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
#include "Object.h"
#include "Affine.h" // cs250::point
#include <algorithm> // std::max
#include <vector> // std::vector
/******************************************************************************/
/*!
  \brief
	This function calculates the center and radius of a sphere or AABB
	to encapsulate a given triangle

  \param center
	reference to store calculated center

  \param radius
	reference to store calculated radius

  \param p0
	a point
  \param p1
    a point
  \param p2
    a point
*/
/******************************************************************************/
void CalcCenterRadius(glm::vec4& center, float& radius, glm::vec4 p0, glm::vec4 p1, glm::vec4 p2)
{
	float x_max = std::max(p0.x, std::max(p1.x, p2.x));
	float y_max = std::max(p0.y, std::max(p1.y, p2.y));
	float z_max = std::max(p0.z, std::max(p1.z, p2.z));

	float x_min = std::min(p0.x, std::min(p1.x, p2.x));
	float y_min = std::min(p0.y, std::min(p1.y, p2.y));
	float z_min = std::min(p0.z, std::min(p1.z, p2.z));

	glm::vec4 box_max = cs250::point(x_max, y_max, z_max);
	glm::vec4 box_min = cs250::point(x_min, y_min, z_min);

	center = cs250::point((box_max.x + box_min.x) / 2.0f, (box_max.y + box_min.y) / 2.0f, (box_max.z + box_min.z) / 2.0f);
	radius = glm::length(box_max - center);
}
/******************************************************************************/
/*!
  \brief
	This function splits an Object into two sets of vertices determined
	by a given Plane

  \param original
	The original Object

  \param plane
	Plane to be used to determine which side a point is at

  \param frontPoly
	Reference to output set of vertices in front of the plane
  \param backPoly
	Reference to output set of vertices behind the plane
*/
/******************************************************************************/
void SplitObject(const Object& original, const Plane& plane, std::vector<glm::vec3>& frontPoly, std::vector<glm::vec3>& backPoly)
{
	int numFront = 0, numBack = 0; 

	glm::vec3 a = original._points[2];

	Point_Classification aSide = ClassifyPointToPlane(a, plane);

	for (int n = 0; n < 3; ++n)
	{
		glm::vec3 b = original._points[n];
		Point_Classification bSide = ClassifyPointToPlane(b, plane);

		if (bSide == Point_Classification::POINT_IN_FRONT_OF_PLANE)
		{
			// Straddle
			if (aSide == Point_Classification::POINT_BEHIND_PLANE)
			{
				glm::vec3 intersect = IntersectEdgeAgainstPlane(b, a, plane);
				// may assert
				frontPoly.push_back(intersect);
				backPoly.push_back(intersect);
			}
			frontPoly.push_back(b);
		}
		else if (bSide == Point_Classification::POINT_BEHIND_PLANE)
		{
			// Straddle
			if (aSide == Point_Classification::POINT_IN_FRONT_OF_PLANE)
			{
				glm::vec3 intersect = IntersectEdgeAgainstPlane(b, a, plane);
				// may assert
				frontPoly.push_back(intersect);
				backPoly.push_back(intersect);
			}
			else if (aSide == Point_Classification::POINT_ON_PLANE)
			{
				backPoly.push_back(a);
			}

			backPoly.push_back(b);
		}
		else
		{
			// b is on the plane
			frontPoly.push_back(b);

			// In one case, also ouput b to back side
			if (aSide == Point_Classification::POINT_BEHIND_PLANE)
			{
				backPoly.push_back(b);
			}
		}

		a = b;
		aSide = bSide;
	}
}
/******************************************************************************/
/*!
  \brief
	This function splits a set of 4 vertices into two triangles

  \param poly
	The original set

  \param object0
	Reference to store first triangle
  \param object1
	Refernece to store second triangle
*/
/******************************************************************************/
void SplitPolytoObject(const std::vector<glm::vec3>& poly, Object& object0, Object& object1)
{
	glm::vec4 p0 = glm::vec4(poly[0], 1.0f);
	glm::vec4 p1 = glm::vec4(poly[1], 1.0f);
	glm::vec4 p2 = glm::vec4(poly[2], 1.0f);
	glm::vec4 p3 = glm::vec4(poly[3], 1.0f);

	// using triangle fan
	object0 = Object(p0, p1, p2);
	object1 = Object(p0, p2, p3);
}
/******************************************************************************/
/*!
  \brief
	This function calculate a plane from an Object

  \param object
	The original object

  \return p
	A plane to represent the triangle
*/
/******************************************************************************/
Plane GetPlaneFromObject(const Object& object)
{
	glm::vec3 U = object._points[1] - object._points[0];

	glm::vec3 V = object._points[2] - object._points[0];

	glm::vec3 N;
	N.x = U.y * V.z - U.z * V.y;
	N.y = U.z * V.x - U.x * V.z;
	N.z = U.x * V.y - U.y * V.x;

	Plane p;
	N = glm::normalize(N);
	p.m_Normal = N;
	p.m_d = dot(glm::vec3(object._points[0]), N);

	return p;
}
/******************************************************************************/
/*!
  \brief
	This function pick a best splitting plane using a set of Objects.
	Borrored from the orange book.

  \param objects
	A list of Object

  \return p
	Best splitting plane
*/
/******************************************************************************/
Plane PickSplittingPlane(const std::vector<Object>& objects)
{
	
	// Blend factor for optimizing for balance or splits (should be tweaked)
	const float K = 0.8f;
	// Variables for tracking best splitting plane seen so far
	Plane bestPlane;
	float bestScore = FLT_MAX;

	// Try the plane of each polygon as a dividing plane
	for (unsigned int i = 0; i < objects.size(); ++i)
	{
		int numInFront = 0, numBehind = 0, numStraddling = 0;
		Plane plane = GetPlaneFromObject(objects[i]);
		// Test against all other polygons
		for (unsigned int j = 0; j < objects.size(); ++j)
		{
			// Ignore testing against self
			if (i == j) continue;
			// Keep standing count of the various poly-plane relationships
			switch (ClassifyObjectToPlane(objects[j], plane))
			{
			case Object_Classification::OBJECT_COPLANAR_WITH_PLANE:
				/* Coplanar polygons treated as being in front of plane */
			case Object_Classification::OBJECT_IN_FRONT_OF_PLANE:
				numInFront++;
				break;
			case Object_Classification::OBJECT_BEHIND_PLANE:
				numBehind++;
				break;
			case Object_Classification::OBJECT_STRADDLING_PLANE:
				numStraddling++;
				break;
			}
		}

		// Compute score as a weighted combination (based on K, with K in range
		// 0..1) between balance and splits (lower score is better)
		float score = K * numStraddling + (1.0f - K) * abs(numInFront - numBehind);

		if (score < bestScore)
		{
			bestScore = score;
			bestPlane = plane;
		}
	}

	return bestPlane;
}

/******************************************************************************/
/*!
  \brief
	This function classify wherer the Object is to a Plane.
	Borrowed from the Orange book

  \param object
	An object to be determined

  \param plane
	A plane to be used to determine where the Object is

  \return Object_Classification
    Where the Objecti is from the Plane
*/
/******************************************************************************/
Object_Classification ClassifyObjectToPlane(const Object& object, const Plane& plane)
{
	// Loop over all polygon vertices and count how many vertices
    // lie in front of and how many lie behind of the thickened plane
	int numInFront = 0;
	int numBehind = 0;
	int numVerts = 3;

	for (int i = 0; i < numVerts; i++)
	{
		glm::vec3 p = object._points[i];
		switch (ClassifyPointToPlane(p, plane)) 
		{
		case Point_Classification::POINT_IN_FRONT_OF_PLANE:
			numInFront++;
			break;
		case Point_Classification::POINT_BEHIND_PLANE:
			numBehind++;
			break;
		}
	}
	// If vertices on both sides of the plane, the polygon is straddling
	if (numBehind != 0 && numInFront != 0)
		return Object_Classification::OBJECT_STRADDLING_PLANE;
	// If one or more vertices in front of the plane and no vertices behind
	// the plane, the polygon lies in front of the plane
	if (numInFront != 0)
		return Object_Classification::OBJECT_IN_FRONT_OF_PLANE;
	// Ditto, the polygon lies behind the plane if no vertices in front of
	// the plane, and one or more vertices behind the plane
	if (numBehind != 0)
		return Object_Classification::OBJECT_BEHIND_PLANE;
	// All vertices lie on the plane so the polygon is coplanar with the plane
	return Object_Classification::OBJECT_COPLANAR_WITH_PLANE;
}
/******************************************************************************/
/*!
  \brief
	Helper ctor to construct an Object

  \param p0
	A point
  \param p1
	A point
  \param p2
	A point
*/
/******************************************************************************/
Object::Object(const glm::vec4& p0, const glm::vec4& p1, const glm::vec4& p2)
{
	_points.push_back(p0);
	_points.push_back(p1);
	_points.push_back(p2);

	CalcCenterRadius(_center, _radius, _points[0], _points[1], _points[2]);
}
/******************************************************************************/
/*!
  \brief
	Helper ctor to construct an Object using vec3s

  \param p0
	A point
  \param p1
	A point
  \param p2
	A point
*/
/******************************************************************************/
Object::Object(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2)
{
	_points.push_back(glm::vec4(p0, 1.0f));
	_points.push_back(glm::vec4(p1, 1.0f));
	_points.push_back(glm::vec4(p2, 1.0f));

	CalcCenterRadius(_center, _radius, _points[0], _points[1], _points[2]);
}
/******************************************************************************/
/*!
  \brief
	Default ctor
*/
/******************************************************************************/
Object::Object() : _center(), _radius(), _points()
{
}

Aabb Object::GetAabb() const
{
	return Aabb(_center, _radius);
}
