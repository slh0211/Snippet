/******************************************************************************/
/*!
\file   FrustumMesh.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS250
\par    Assignment 3 FrustumMesh
\par    Summer 2020
\date   5/24/2020
\brief  
    This file contains the implementation of FrustumMesh functions

  Hours spent on this assignment: 2

  Specific portions that gave you the most trouble:
    
*/
/******************************************************************************/
#include "FrustumMesh.h"
#include "Affine.h" // point, vector, cross

//   5 -- 1
//  /|   /|
// 4 -- 0 |
// | |  | |
// | 7 -| 3
// |/   |/
// 6 -- 2
//
//
// 8

const cs250::FrustumMesh::Face cs250::FrustumMesh::faces[] = {{0, 3, 1},
                                                              {0, 2, 3},
                                                              {0, 4, 6},
                                                              {0, 6, 2},
                                                              {1, 4, 0},
                                                              {1, 5, 4},
                                                              {3, 6, 7},
                                                              {3, 2, 6},
                                                              {4, 5, 7},
                                                              {4, 7, 6},
                                                              {1, 7, 5},
                                                              {1, 3, 7}};


const cs250::FrustumMesh::Edge cs250::FrustumMesh::edges[] = {{0, 1},
                                                              {0, 2},
                                                              {0, 4},
                                                              {1, 3},
                                                              {1, 5},
                                                              {2, 3},
                                                              {2, 6},
                                                              {3, 7},
                                                              {4, 5},
                                                              {4, 6},
                                                              {5, 7},
                                                              {6, 7},
                                                              {0, 8},
                                                              {2, 8},
                                                              {4, 8},
                                                              {6, 8}};
/******************************************************************************/
/*!
  \brief
    ctor of FrusumMesh class

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
cs250::FrustumMesh::FrustumMesh(float fov, float aspect, float N, float F)
{
  float tan_half_r = glm::tan(glm::radians(fov/2.0f));
  
  float D = (N + F) / 2.0f;
  float W = 2.0f * D * tan_half_r;
  float H = W / aspect;
  
  float Wn = W / D * N;
  float Hn = H / D * N;
  
  float Wf = W / D * F;
  float Hf = H / D * F;
  
  float Wn_half = Wn / 2.0f;
  float Hn_half = Hn / 2.0f;
  
  float Wf_half = Wf / 2.0f;
  float Hf_half = Hf / 2.0f;
  
  glm::vec4 p0 = point(0.0f, 0.0f, 0.0f);
  glm::vec4 p1 = point(Wn_half, Hn_half, -N);
  glm::vec4 p2 = point(Wn_half, -Hn_half, -N);
  glm::vec4 p3 = point(-Wn_half, -Hn_half, -N);
  glm::vec4 p4 = point(-Wn_half, Hn_half, -N);
  
  glm::vec4 p5 = point(Wf_half, Hf_half, -F);
  glm::vec4 p6 = point(Wf_half, -Hf_half, -F);
  glm::vec4 p7 = point(-Wf_half, -Hf_half, -F);
  glm::vec4 p8 = point(-Wf_half, Hf_half, -F);

//   p8----p5
//  /|    / |
// p4----p1 |
// | |   |  |
// | p7 -|  p6
// |/    | /
// p3----p2
//
// p0

  vertices[8] = p0;
  vertices[0] = p1;
  vertices[2] = p2;
  vertices[6] = p3;
  vertices[4] = p4;
  vertices[1] = p5;
  vertices[3] = p6;
  vertices[7] = p7;
  vertices[5] = p8;
  
  aabb_center = point(0.0f, 0.0f, -F/2.0f);
  aabb_dimensions = vector(Wf, Hf, F);
}