/******************************************************************************/
/*!
\file   MovingSphere.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS350
\par    Assignment 4 GJK algorithm
\par    Spring 2021
\date   4/10/2021
\brief
	This file contains the implpementation of MovingSphere class

  Hours spent on this assignment:

  Specific portions that gave you the most trouble:

*/
/******************************************************************************/
#include "MovingSphere.h"
/******************************************************************************/
/*!
  \brief
	Default ctor for MovingSphere
*/
/******************************************************************************/
MovingSphere::MovingSphere() : _position(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)), _radius(1.0f), _direction(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)), _speed(0.0f), _bActive(false)
{
}
/******************************************************************************/
/*!
  \brief
	Ctor for MovingSphere

  \param pos
	Position of the sphere
  \param r
	Radius of the sphere
  \param dir
	Direction of the sphere
  \param speed
	Speed of the sphere
*/
/******************************************************************************/
MovingSphere::MovingSphere(const glm::vec4& pos, float r, const glm::vec4& dir, float speed) : _position(pos), _radius(r), _direction(dir), _speed(speed), _bActive(false)
{
}
/******************************************************************************/
/*!
  \brief
	Get Aabb of the MovingSphere

  \return Aabb
    Construct an Aabb using position and radius for collision detection
*/
/******************************************************************************/
Aabb MovingSphere::GetAabb() const
{
	return Aabb(_position, _radius);
}
/******************************************************************************/
/*!
  \brief
	Check to see if the moving sphere is out of bound

  \param camera
	using camera's far to determine

  \return 
	true if sphere is beyond far else false
*/
/******************************************************************************/
bool MovingSphere::OutOfBoundary(const cs250::Camera& camera) const
{
	if (abs(_position.x - camera.eye().x) > camera.far() ||
		abs(_position.y - camera.eye().y) > camera.far() ||
		abs(_position.z - camera.eye().z) > camera.far())
		return true;

	return false;
}
/******************************************************************************/
/*!
  \brief
	Reset sphere to not active and speed to 0
*/
/******************************************************************************/
void MovingSphere::ResetSphere()
{
	_bActive = false;
	_speed = 0.0f;
}
/******************************************************************************/
/*!
  \brief
	Shoot the Sphere with given camera, radius, and speed

  \param c
	camera
  \param r
	radius
  \param s
    speed
*/
/******************************************************************************/
void MovingSphere::ShootSphere(const cs250::Camera& c, float r, float s)
{
	_position = c.eye();
	_radius = r;
	_direction = -c.back();
	_speed = s;
	_bActive = true;
}
