/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: IlluminatorShader.frag
Purpose: frag shader for lighging sphere
Language: C++/glsl
Platform: visual studio
Project: r.shen_CS300_2
Author: Richard Shen, r.shen, 110000516
Creation date: 11/11/2020
End Header --------------------------------------------------------*/
#version 430 core

uniform vec3 color;

out vec4 frag_color;

void main()
{
  frag_color = vec4((color), 0.0);
}