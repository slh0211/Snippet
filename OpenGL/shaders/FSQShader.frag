/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: FSQShader.frag
Purpose: frag shader for rendering FSQ on to screen
Language: C++/glsl
Platform: visual studio
Project: r.shen_CS350_1
Author: Richard Shen, r.shen, 110000516
Creation date: 1/26/2021
End Header --------------------------------------------------------*/
#version 430 core

uniform sampler2D texSampler;

in vec2 TexCoords;
out vec4 FragColor;

void main()
{
	FragColor = texture( texSampler, TexCoords);
	//FragColor = vec4(1.0, 0.0, 1.0, 1.0); 
}