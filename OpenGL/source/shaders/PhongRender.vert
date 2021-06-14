/******************************************************************************/
/*!
\file   PhongRender.vert
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS250
\par    Assignment 7 Phong Render
\par    Summer 2020
\date   7/5/2020
\brief  
    This file contains the implementation of Phong Render in GLSL

  Hours spent on this assignment: 3

  Specific portions that gave you the most trouble:
    
*/
/******************************************************************************/
#version 430 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;

uniform mat4 persp_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
uniform mat4 normal_matrix;

out vec4 world_normal;
out vec4 world_position;

void main()
{
  world_position = model_matrix * position;

  gl_Position = persp_matrix * view_matrix * world_position;

  
  world_normal = normal_matrix * normal;
}