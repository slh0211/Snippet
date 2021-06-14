/******************************************************************************/
/*!
\file   Camera.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS250
\par    Assignment 3 Camera
\par    Summer 2020
\date   5/24/2020
\brief  
    This file contains the implementation of Camera functions

  Hours spent on this assignment: 2

  Specific portions that gave you the most trouble:
    
*/
/******************************************************************************/
#include "Camera.h"
#include "Affine.h"
// (default constructor) creates a canonical camera object: the center of 
// projection is the origin, and the right, up, and back vectors are parallel to the x, y, and
// z–axes, respectively. The viewport should have a field of view of 90, aspect ratio of
// 1, near clipping plane distance of 0.1, and a far clipping plane distance of 10 in world
// coordinates.
/******************************************************************************/
/*!
  \brief
    default ctor
*/
/******************************************************************************/
cs250::Camera::Camera(void) : eye_point(point(0.0f, 0.0f, 0.0f)), 
                              right_vector(vector(1.0f, 0.0f, 0.0f)), 
                              up_vector(vector(0.0f, 1.0f, 0.0f)),
                              back_vector(vector(0.0f, 0.0f, 1.0f)),
                              vp_width(2.0f),
                              vp_height(2.0f),
                              vp_distance(1.0f),
                              near_distance(0.1f),
                              far_distance(10.0f)
{
// D = 1, W = 2 * D * tan(45) = 2, H = W / 1 = 2 /1 = 2
}
// (non–default constructor) creates a camera with center of projection E, 
// looking in the direction specified by the vector look, and oriented by the 
// relative up vector rel. Note that rel is not necessarily parallel to the
// actual up vector ~v of the camera that is created. Rather, the plane spanned 
// by rel and look is orthogonal the the right vector ~u of the created camera. 
// The vectors look and rel are not necessarily unit vectors; however, it is 
// assumed that they are not parallel. The view frustum of the created camera 
// has a horizontal field of view angle fov given in degrees, the specified 
// aspect ratio (ratio of the width of the viewport to its height), near 
// clipping plane distance, and far clipping plane distance. All values are 
// given in world coordinates.
/******************************************************************************/
/*!
  \brief
    non-default default ctor

  \param E
    eye position

  \param look
    look at vector

  \param rel
    relateive up vector

  \param fov
    field of view

  \param aspect
    aspect ratio

  \param N
    near

  \param F
    far
*/
/******************************************************************************/
cs250::Camera::Camera(const glm::vec4 &E, const glm::vec4 &look, const glm::vec4 &rel,
                      float fov, float aspect, float N, float F) 
                      : eye_point(E), near_distance(N), far_distance(F)
{
  back_vector = -glm::normalize(look);
  right_vector = glm::normalize(cross(look, rel));
  up_vector = cross(back_vector, right_vector);
  
  float tan_half_r = glm::tan(glm::radians(fov/2.0f));

  vp_distance = (N + F) / 2.0f;
  vp_width = 2.0f * vp_distance * tan_half_r;
  vp_height = vp_width / aspect;
}

/******************************************************************************/
/*!
  \brief
    returns the vector W, H, D, where W is the width of the viewport, H is
    the height of the viewport, and D is the distance from the center of
    projection to the plane of projection

  \return 
    vec4 viewport vector
*/
/******************************************************************************/
glm::vec4 cs250::Camera::viewport(void) const
{
  return vector(vp_width, vp_height, vp_distance);
}
/******************************************************************************/
/*!
  \brief
    changes the dimensions of the viewport by specified amount. That is,
    the width and height of the viewport are multiplied by factor. The instance of the
    camera class is returned.

  \param factor
     width and height of the viewport are multiplied by factor
  \return
    The instance of the camera class
*/
/******************************************************************************/
cs250::Camera& cs250::Camera::zoom(float factor)
{
  vp_width  = factor * vp_width;
  vp_height = factor * vp_height;
  
  return *this;
}
/******************************************************************************/
/*!
  \brief
     moves the camera dist units (in world coordinates) in the direction
    -n (where n is the back vector of the camera). The instance of the camera class is
     returned.

  \param distance
     moves the camera dist units
  \return
    The instance of the camera class
*/
/******************************************************************************/
cs250::Camera& cs250::Camera::forward(float distance)
{
  eye_point = eye_point + distance * -back_vector;
  
  return *this;
}
/******************************************************************************/
/*!
  \brief
    rotates the camera by the specified angle (in degrees) about an axis parallel
    to the camera up vector v and passing through the center of projection. 
    The instance of the camera class is returned.

  \param angle
     rotates the camera by the specified angle (in degrees)
  \return
    The instance of the camera class
*/
/******************************************************************************/
cs250::Camera& cs250::Camera::yaw(float angle)
{
  glm::mat4 R = rotate(angle, up_vector);
  
  right_vector = R * right_vector;
  back_vector  = R * back_vector;
  
  return *this;
}

/******************************************************************************/
/*!
  \brief
    rotates the camera by the specified angle (in degrees) about an axis parallel
    to the camera right vector ~u and passing through the center of projection. 
    The instance of the camera class is returned.

  \param angle
     rotates the camera by the specified angle (in degrees)
  \return
    The instance of the camera class
*/
/******************************************************************************/
cs250::Camera& cs250::Camera::pitch(float angle)
{
  glm::mat4 R = rotate(angle, right_vector);
  
  up_vector = R * up_vector ;
  back_vector = R *back_vector ;
  
  return *this;
}
/******************************************************************************/
/*!
  \brief
    rotates the camera by the specified angle (in degrees) about an axis parallel
    to the camera back vector ~n and passing through the center of projection. 
    The instance of the camera class is returned.

  \param angle
     rotates the camera by the specified angle (in degrees)
  \return
    The instance of the camera class
*/
/******************************************************************************/
cs250::Camera& cs250::Camera::roll(float angle)
{
  glm::mat4 R = rotate(angle, back_vector);
  
  right_vector = R * right_vector;
  up_vector = R * up_vector;
  
  return *this;
}