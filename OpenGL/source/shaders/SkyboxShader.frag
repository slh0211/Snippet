/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: SkyboxShader.frag
Purpose: frag shader for rendering Skybox
Language: C++/glsl
Platform: visual studio
Project: r.shen_CS300_3
Author: Richard Shen, r.shen, 110000516
Creation date : 11/23/2020
End Header --------------------------------------------------------*/
#version 430 core
//out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D skyboxTex;

layout(location = 0) out vec4 frag_color;

void main()
{    
     frag_color = texture(skyboxTex, TexCoords);
    //FragColor = vec4(1.0, 0.0, 1.0, 1.0); 
}