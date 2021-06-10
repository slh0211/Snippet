/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: PhongLighting.frag
Purpose: frag shader for phong lighting
Language: C++/glsl
Platform: visual studio
Project: r.shen_CS300_1
Author: Richard Shen, r.shen, 110000516
Creation date: 11/9/2020
End Header --------------------------------------------------------*/
#version 430 core
//in vec4  frag_color;
in vec3 i_final;
out vec4 FragColor;

void main()
{
    FragColor = vec4(i_final, 0.0);   
}  