/******************************************************************************/
/*!
\file   MovingSphere.h
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS350
\par    Assignment 4 GJK algorithm
\par    Spring 2021
\date   4/10/2021
\brief
	This file contains the prototype of MovingSphere class

  Hours spent on this assignment:

  Specific portions that gave you the most trouble:

*/
/******************************************************************************/
#pragma once

#include <glm/glm.hpp> // glm
#include "geometry.h" //Aabb
#include "Camera.h"	// Camera
/******************************************************************************/
/*!
  \class MovingSphere
  \brief
	This class defines an MovingSphere used in a scene
*/
/******************************************************************************/
class MovingSphere
{
public:
	MovingSphere();
	MovingSphere(const glm::vec4& pos, float r, const glm::vec4& dir, float speed);

	Aabb GetAabb() const;
	bool OutOfBoundary(const cs250::Camera&) const;
	void ResetSphere();
	void ShootSphere(const cs250::Camera& c, float r, float s);

	glm::vec4 _position;
	float _radius;
	// moving direction
	glm::vec4 _direction;
	// moving speed
	float _speed;
	// determine whether the sphere is active (for rendering)
	bool _bActive;
private:

	// do I want to store translation, rotation, scale matrix here?
};