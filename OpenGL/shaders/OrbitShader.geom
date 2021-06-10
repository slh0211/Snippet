/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: OrbitShader.geom
Purpose: Drawing orbit for rotating spheres
Language: C++/glsl
Platform: visual studio
Project: r.shen_CS300_1
Author: Richard Shen, r.shen, 110000516
Creation date: 10/1/2020
End Header --------------------------------------------------------*/

#version 430 core
layout (points) in;
layout (line_strip, max_vertices = 24) out;

uniform mat4 view_matrix;
uniform mat4 persp_matrix;

const float PI = 3.1415926;

void main() {    
    for (int i = 0; i <= 23; i++) {
        // Angle between each side in radians
        float ang = PI * 2.0 / 23.0 * i;

        // Offset from center of point (0.3 to accomodate for aspect ratio)
        vec4 offset = vec4(-sin(ang)*3.0, 0.0, cos(ang)*3.0, 0.0);
        gl_Position = persp_matrix * view_matrix * (gl_in[0].gl_Position + offset);

        EmitVertex();
    }

    EndPrimitive();
}  