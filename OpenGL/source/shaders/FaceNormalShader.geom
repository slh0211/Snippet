/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: FaceNormalShader.geom
Purpose: Drawing Face Normal for a mesh
Language: C++/glsl
Platform: visual studio
Project: r.shen_CS300_1
Author: Richard Shen, r.shen, 110000516
Creation date: 10/1/2020
End Header --------------------------------------------------------*/
#version 430 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

const float MAGNITUDE = 0.4;

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[1].gl_Position) - vec3(gl_in[0].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[0].gl_Position);
   return normalize(cross(a, b));
}  

uniform mat4 projection;



void main()
{
    vec3 normal = GetNormal();
    vec4 mid = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position)/3.0;


    gl_Position = projection * mid;
    EmitVertex();
    gl_Position = projection * (mid + vec4(normal, 0.0)* MAGNITUDE);
    EmitVertex();
    EndPrimitive();
} 