/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: GBufferShader.frag
Purpose: vfrag shader for rendering mesh to GBUffer
Language: C++/glsl
Platform: visual studio
Project: r.shen_CS350_1
Author: Richard Shen, r.shen, 110000516
Creation date: 1/27/2021
End Header --------------------------------------------------------*/
#version 430 core

// GLuint  gPosition, gNormal, gDiffuse, gSpecularShininess, gAmbient;
layout (location = 0) out vec4 gPositionOut;
layout (location = 1) out vec4 gNormalOut;
layout (location = 2) out vec4 gDiffuseOut;
layout (location = 3) out vec4 gSpecularShininessOut;
layout (location = 4) out vec4 gAmbientOut;

//in vec4 world_position;
//in vec4 world_normal;

in vec4 view_position;
in vec4 view_normal;

in vec2 TexCoords;

uniform vec3 ambient_coefficient;
uniform vec3 diffuse_coefficient;
uniform vec3 specular_coefficient;
uniform float specular_exponent;


void main()
{
	gPositionOut = view_position;
	gNormalOut = normalize(view_normal);
	gDiffuseOut = vec4(diffuse_coefficient, 0.0);
	gSpecularShininessOut.rgb = specular_coefficient;
	gSpecularShininessOut.a = specular_exponent;
	gAmbientOut = vec4(ambient_coefficient, 0.0);
}