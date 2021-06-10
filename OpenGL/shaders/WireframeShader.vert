/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: IlluminatorShader.vert
Purpose: vert shader for lighging sphere
Language: C++/glsl
Platform: visual studio
Project: r.shen_CS300_2
Author: Richard Shen, r.shen, 110000516
Creation date: 11/11/2020
End Header --------------------------------------------------------*/
#version 430 core

layout (location = 0) in vec4 position;
//layout (location = 1) in vec4 normal;

uniform mat4 persp_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
//uniform mat4 normal_matrix;

//out vec4 world_normal;
//out vec4 world_position;

void main()
{
  //world_position = model_matrix * position;

  gl_Position = persp_matrix * view_matrix * model_matrix * position;

  
  //world_normal = normal_matrix * normal;
}