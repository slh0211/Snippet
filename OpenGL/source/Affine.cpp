/******************************************************************************/
/*!
\file   Affine.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS250
\par    Assignment 1 Affine
\par    Summer 2020
\date   5/10/2020
\brief  
    This file contains the implementation of Affine functions

  Hours spent on this assignment: 2

  Specific portions that gave you the most trouble:
    
*/
/******************************************************************************/
#include "Affine.h"
/******************************************************************************/
/*!
  \brief
    returns the 3D point with components (x, y, z)

  \param x
    x value of vec3

  \param y
    y value of vec3
  
  \param z
    z value of vec3

  \return vec4
    return w component with 1.0f
*/
/******************************************************************************/
glm::vec4 cs250::point(float x, float y, float z)
{
  return glm::vec4(x, y, z, 1.0f);
}
/******************************************************************************/
/*!
  \brief
    returns the 3D vector with components <x, y, z>

  \param x
    x value of vec3

  \param y
    y value of vec3

  \param z
    z value of vec3

  \return vec4
    return w component with 0.0f
*/
/******************************************************************************/
glm::vec4 cs250::vector(float x, float y, float z)
{
  return glm::vec4(x, y, z, 0.0f);
}
/******************************************************************************/
/*!
  \brief
    returns true if v represents a 3D point, and returns false otherwise

  \param v
    vec4 to be determined whether it's point or vector

  \return true or false
    true if v is a point, false if v is a vector
*/
/******************************************************************************/
bool cs250::isPoint(const glm::vec4 &v)
{
    return near(v.w, 1.0f);
}
/******************************************************************************/
/*!
  \brief
    returns true if v represents a 3D vector, and returns false otherwise

  \param v
    vec4 to be determined whether it's point or vector

  \return true or false
    true if v is a point, false if v is a vector
*/
/******************************************************************************/
bool cs250::isVector(const glm::vec4 &v)
{
    return near(v.w, 0.0f);
}
/******************************************************************************/
/*!
  \brief
    returns the angle, measured in degrees

  \param u
    vec4 u

  \param v
    vec4 v

  \return angle
    angle between u and v in degrees
*/
/******************************************************************************/
float cs250::angle(const glm::vec4 &u, const glm::vec4 &v)
{
  glm::vec3 u_vec3 = glm::vec3(u);
  glm::vec3 v_vec3 = glm::vec3(v);
  
  float udotv = glm::dot(u_vec3,v_vec3);
  
  float s = (udotv)/(glm::length(u_vec3) * glm::length(v_vec3));
  
  float radian = glm::acos(s);
  
  return glm::degrees(radian);
}
/******************************************************************************/
/*!
  \brief
    returns the cross product

  \param u
    vec4 u

  \param v
    vec4 v

  \return cross product
    the cross product of u and v with w component assign to 0.0f
*/
/******************************************************************************/
glm::vec4 cs250::cross(const glm::vec4 &u, const glm::vec4 &v)
{
  glm::vec3 u_vec3 = glm::vec3(u);
  glm::vec3 v_vec3 = glm::vec3(v);
  
  glm::vec3 uxv = glm::cross(u_vec3, v_vec3);
  
  return glm::vec4(uxv.x, uxv.y, uxv.z, 0.0f);
}
/******************************************************************************/
/*!
  \brief
    returns the 3D affinetransformation with columns (in standard matrix
    representation) given by Lx, Ly, Lz, and C

  \param Lx
    vec4 Lx

  \param Ly
    vec4 Ly

  \param Lz
    vec4 Lz

  \param C
    vec4 C

  \return m
    the affine matrix
*/
/******************************************************************************/
glm::mat4 cs250::affine(const glm::vec4 &Lx, const glm::vec4 &Ly,
                        const glm::vec4 &Lz, const glm::vec4 &C)
{
  glm::mat4 m;
  
  m[0] = Lx;
  m[1] = Ly;
  m[2] = Lz;
  m[3] = C;
  
  return m;
}
/******************************************************************************/
/*!
  \brief
    returns true if A represents a 3D affine transformation

  \param M
    mat4 to be detemined

  \return true or false
    whether M is an affine matrix
*/
/******************************************************************************/
bool cs250::isAffine(const glm::mat4 &M)
{
  return near(0.0f, M[0][3])&&near(0.0f, M[1][3])&&near(0.0f, M[2][3])&&near(1.0f, M[3][3]);
}
/******************************************************************************/
/*!
  \brief
    returns the 3D affine transformation Rtv for counterclockwise rotation by
    the angle deg about the vector v. The angle t is assumed to be given in degrees

  \param deg
    deg to be used

  \param v
    v to be rotate about

  \return mat4
   the affine rotation matrix
*/
/******************************************************************************/
glm::mat4 cs250::rotate(float deg, const glm::vec4 &v)
{
  float radian = glm::radians(deg);
  
  float cosr = glm::cos(radian);
  glm::mat3 m0 = cosr * glm::mat3(1.0f);
  
  float vxvx = v.x * v.x;
  float vxvy = v.x * v.y;
  float vxvz = v.x * v.z;
  float vyvy = v.y * v.y;
  float vyvz = v.y * v.z;
  float vzvz = v.z * v.z;
  
  glm::mat3 vvt = {{vxvx, vxvy, vxvz},{vxvy, vyvy, vyvz},{vxvz, vyvz, vzvz}};
  
  float s1 = (1.0f - cosr) / (vxvx + vyvy + vzvz);
  
  glm::mat3 m1 = s1 * vvt;
  
  float s2 = glm::sin(radian) / glm::length(glm::vec3(v));
  
  glm::mat3 lamda_v = {{0.0f, v.z, -v.y},{-v.z, 0.0f, v.x},{v.y, -v.x, 0.0f}};
  
  glm::mat3 m2 = s2 * lamda_v;
  
  return glm::mat4(m0 + m1 + m2);
}
/******************************************************************************/
/*!
  \brief
    returns the 3D affne transformation T~v for translation by the 3D vector v

  \param v
    tranlate along v

  \return mat4
   the affine translation matrix
*/
/******************************************************************************/
glm::mat4 cs250::translate(const glm::vec4 &v)
{
  glm::mat4 m = glm::mat4(1.0f);
  
  m[3][0] = v.x;
  m[3][1] = v.y;
  m[3][2] = v.z;
  
  return m;
}
/******************************************************************************/
/*!
  \brief
    returns the 3D affine transformation Hr for uniform scaling by r with respect to the origin

  \param r
    scaler

  \return mat4
   the affine scalling matrix
*/
/******************************************************************************/
glm::mat4 cs250::scale(float r)
{
  glm::mat4 m = glm::mat4(1.0f);
  
  m[0][0] = r;
  m[1][1] = r;
  m[2][2] = r;
  
  return m;
}
/******************************************************************************/
/*!
  \brief
    returns the 3D affine transformation Hhrx;ry;rzi for nonuniform scaling
    by factors rx, ry, rz with respect to the origin

  \param rx
    scaler

  \param ry
    scaler

  \param rz
    scaler

  \return mat4
   the affine scalling matrix
*/
/******************************************************************************/
glm::mat4 cs250::scale(float rx, float ry, float rz)
{
  glm::mat4 m = glm::mat4(1.0f);
  
  m[0][0] = rx;
  m[1][1] = ry;
  m[2][2] = rz;
  
  return m;
}
// 
/******************************************************************************/
/*!
  \brief
    returns the inverse of the 3D affne transformation

  \param A
    matrix to be inversed

  \return mat4
   the inversed matrix
*/
/******************************************************************************/
glm::mat4 cs250::affineInverse(const glm::mat4 &A)
{
  glm::mat3 L = glm::mat3(A);
  
  glm::vec4 v_inverse = glm::vec4(-A[3][0], -A[3][1], -A[3][2], 0.0f);
  
  glm::mat4 L_inverse = glm::mat4(glm::inverse(L));
  
  return L_inverse * translate(v_inverse);
}