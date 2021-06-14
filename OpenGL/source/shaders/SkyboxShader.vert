/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: SkyboxShader.vert
Purpose: vert shader for rendering Skybox
Language: C++/glsl
Platform: visual studio
Project: r.shen_CS300_3
Author: Richard Shen, r.shen, 110000516
Creation date : 11/23/2020
End Header --------------------------------------------------------*/
#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    //TexCoords = aPos;
    TexCoords = aTexCoord;
    gl_Position = projection * view * vec4(aPos, 1.0);
}  