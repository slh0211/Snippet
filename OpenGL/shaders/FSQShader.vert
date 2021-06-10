/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: FSQShader.vert
Purpose: vert shader for rendering FSQ on to screen
Language: C++/glsl
Platform: visual studio
Project: r.shen_CS350_1
Author: Richard Shen, r.shen, 110000516
Creation date: 1/26/2021
End Header --------------------------------------------------------*/
#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}