/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: WorldShader.vert
Purpose: frag shader for drawing wireframe using vertices in world spce
Language: C++/glsl
Platform: visual studio
Project: r.shen_CS350_3
Author: Richard Shen, r.shen, 110000516
Creation date: 4/2/2021
End Header --------------------------------------------------------*/
#version 430 core

layout (location = 0) in vec4 position;

uniform mat4 persp_matrix;
uniform mat4 view_matrix;

void main()
{
  gl_Position = persp_matrix * view_matrix * position;
}