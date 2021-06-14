/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: TransparentShader.vert
Purpose: vert shader for transparnet texture using environment mapping
Language: C++/glsl
Platform: visual studio
Project: r.shen_CS300_3
Author: Richard Shen, r.shen, 110000516
Creation date : 11/29/2020
End Header --------------------------------------------------------*/
#version 430 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec2 aTexCoord;
const float  PI = 3.1415926535897932384626433832795;

uniform mat4 persp_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
uniform mat4 normal_matrix;

uniform int gpu_calc;
uniform int ventity_type;
uniform int mapping_type;

uniform vec3 dimension;
uniform vec3 center;

out vec4 world_normal;
out vec4 world_position;
out vec2 TexCoord;

void main()
{
  world_position = model_matrix * position;

  gl_Position = persp_matrix * view_matrix * world_position;
  
  world_normal = normal_matrix * normal;

  TexCoord = aTexCoord;
}