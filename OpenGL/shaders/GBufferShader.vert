/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: GBufferShader.vert
Purpose: vert shader for rendering mesh to GBUffer
Language: C++/glsl
Platform: visual studio
Project: r.shen_CS350_1
Author: Richard Shen, r.shen, 110000516
Creation date: 1/27/2021
End Header --------------------------------------------------------*/
#version 430 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 persp_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
uniform mat4 normal_matrix;

//out vec4 world_position;
//out vec4 world_normal;
out vec2 TexCoords;


out vec4 view_position;
out vec4 view_normal;
void main()
{
  //world_position = model_matrix * aPos;
  view_position = view_matrix * model_matrix * aPos;
  //gl_Position = persp_matrix * view_matrix * world_position;
  gl_Position = persp_matrix * view_position;
  //world_normal = normal_matrix * aNormal;
  view_normal = mat4(transpose( inverse( mat3( view_matrix * model_matrix ) ) ))* aNormal;

  TexCoords = aTexCoords;
}