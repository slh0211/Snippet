/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: WorldShader.frag
Purpose: frag shader for drawing wireframe using vertices in world spce
Language: C++/glsl
Platform: visual studio
Project: r.shen_CS350_3
Author: Richard Shen, r.shen, 110000516
Creation date: 4/2/2021
End Header --------------------------------------------------------*/
#version 430 core

uniform vec3 color;

out vec4 frag_color;

void main()
{
  frag_color = vec4((color), 0.0);
}