/******************************************************************************/
/*!
\file   CameraTransform.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS250
\par    Assignment 5 CameraTransform
\par    Summer 2020
\date   6/13/2020
\brief  
    This file contains the implementation of CameraTransform functions

  Hours spent on this assignment: 2

  Specific portions that gave you the most trouble:
    
*/
/******************************************************************************/
#include "Camera.h"
#include "Affine.h" //affine, affineInverse


/******************************************************************************/
/*!
  \brief
    returns the modeling transformation that maps a canonical camera to the
    speciffed world space camera.

  \param cam
     canonical camera
  \return
    returns the modeling transformation that maps a canonical camera to the
    speciffed world space camera
*/
/******************************************************************************/
glm::mat4 cs250::model(const Camera& cam)
{
  return affine(cam.right(), cam.up(), cam.back(), cam.eye());
}
/******************************************************************************/
/*!
  \brief
    This transformation is used to view the world from a camera's point of view

  \param cam
     canonical camera
  \return
    returns the viewing (world to camera) transformation for the speciffed cam-
    era
*/
/******************************************************************************/
glm::mat4 cs250::view(const Camera& cam)
{
  return affineInverse(model(cam));
}
/******************************************************************************/
/*!
  \brief
    returns the perspective (camera to ndc) transformation that maps
    the camera space view frustum to the standard cube. OpenGL conventions are 
    to be used: the near plane of the frustum should map to the plane z = -1, 
    and the far plane should map to the plane z = 1.

  \param cam
     canonical camera
  \return
    returns the perspective (camera to ndc) transformation that maps
    the camera space view frustum to the standard cube
*/
/******************************************************************************/
glm::mat4 cs250::perspective(const Camera& cam)
{
  glm::mat4 m = glm::mat4(0.0f);
  
  glm::vec4 viewport = cam.viewport();
  
  float vp_w = viewport.x;
  float vp_h = viewport.y;
  float vp_d = viewport.z;
  
  m[0][0] = 2.0f * vp_d / vp_w;
  m[1][1] = 2.0f * vp_d / vp_h;
  m[2][2] = (cam.near() + cam.far()) / (cam.near()-cam.far());
  m[3][2] = (2.0f * cam.near() * cam.far()) / (cam.near()-cam.far());
  m[2][3] = -1.0f;
  
  return m;
}