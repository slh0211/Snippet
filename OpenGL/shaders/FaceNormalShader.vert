/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: FaceNormalShader.vert
Purpose: Drawing Face Normal for a mesh
Language: C++/glsl
Platform: visual studio
Project: r.shen_CS300_1
Author: Richard Shen, r.shen, 110000516
Creation date: 10/1/2020
End Header --------------------------------------------------------*/
#version 430 core
layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = view * model * vec4(aPos, 1.0); 
}