/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: OrbitShader.vert
Purpose: Drawing orbit for rotating spheres
Language: C++/glsl
Platform: visual studio
Project: r.shen_CS300_1
Author: Richard Shen, r.shen, 110000516
Creation date: 10/1/2020
End Header --------------------------------------------------------*/

#version 430 core
layout (location = 0) in vec3 aPos;
uniform mat4 model_matrix;

void main()
{
    gl_Position = model_matrix * vec4(aPos.x, aPos.y, aPos.z, 1.0); 
}