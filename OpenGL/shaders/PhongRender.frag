/******************************************************************************/
/*!
\file   PhongRender.frag
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS250
\par    Assignment 7 Phong Render
\par    Summer 2020
\date   7/5/2020
\brief  
    This file contains the implementation of Phong Render in GLSL

  Hours spent on this assignment: 3

  Specific portions that gave you the most trouble:
    
*/
/******************************************************************************/
#version 430 core

uniform vec4 eye_position;
uniform vec3 diffuse_coefficient;
uniform vec3 specular_coefficient;
uniform float specular_exponent;
uniform vec3 ambient_color;

uniform vec4 light_position[8];
uniform vec3 light_color[8];
uniform int light_use[8];

in vec4 world_position;
in vec4 world_normal;

out vec4 frag_color;

void main()
{
  vec3 partial_sum = vec3(0.0, 0.0, 0.0);
  
  vec4 M = normalize(world_normal);
  vec4 view_vector = eye_position - world_position;
  vec4 V = normalize(view_vector);
  
  for(int i = 0; i < 8; ++i)
  {
    // check if light in use
    if(light_use[i] != 0)
    {
      vec4 light_vector = light_position[i] - world_position;
      vec4 L = normalize(light_vector);

      float ML = max(dot(M, L), 0.0);
      
      // skip calculations when ML is 0.0
      if(ML != 0.0)
      {
        vec4 RL = 2.0 * ML* M - L;
  
        float RLV = max(dot(RL, V), 0.0);
        
        partial_sum += (diffuse_coefficient * ML + specular_coefficient * pow(RLV, specular_exponent)) * light_color[i];
      }
    }
  }
  
  frag_color = vec4((diffuse_coefficient * ambient_color + partial_sum), 0.0);
}