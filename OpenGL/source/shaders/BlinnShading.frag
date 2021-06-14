/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: BlinnShading.frag
Purpose: fragment shader for blinn shading
Language: C++/glsl
Platform: visual studio
Project: r.shen_CS300_2
Author: Richard Shen, r.shen, 110000516
Creation date: 11/11/2020
End Header --------------------------------------------------------*/
#version 430 core

const int   nullId  = 0;
const int   mainId	= 1;
const int   sphereId = 2;
const int   floorId = 3;   

uniform vec4 eye_position;
uniform vec3 diffuse_coefficient;
uniform vec3 specular_coefficient;
uniform float specular_exponent;
uniform vec3 global_ambient_color;

uniform vec3 ambient_coefficient;


uniform vec4 light_position[16];
uniform vec4 light_direction[16];
uniform vec3 light_color[16];
uniform int light_use[16];
uniform int light_type[16];
uniform float light_inner_theta[16];
uniform float light_outer_phi[16];
uniform float light_fallout[16];



uniform float constant_attenuation;
uniform float linear_attenuation;
uniform float quadratic_attenuation;

uniform float z_far;
uniform float z_near;
uniform vec3  fog_color;

uniform int objectID;

uniform sampler2D diffTex;
uniform sampler2D specTex;



in vec4 world_position;
in vec4 world_normal;
in vec2 TexCoord;

out vec4 frag_color;

void main()
{
  vec3 partial_sum = vec3(0.0, 0.0, 0.0);

  vec3 Kd = diffuse_coefficient;
  vec3 Ks = specular_coefficient;
  float Ns = specular_exponent;

  if(objectID == mainId)
  {
    vec2 uv = TexCoord;
    vec4 diffColor = texture(diffTex, uv);
    vec4 specColor = texture(specTex, uv);

    Kd = diffColor.xyz;
    Ks = specColor.xyz;
    Ns = Ks.r * Ks.r;
    //frag_color = diffColor;
    //frag_color = specColor;
    //return;
  }
  
  vec4 M = normalize(world_normal);
  vec4 view_vector = eye_position - world_position;
  vec4 V = normalize(view_vector);
  
  float v_length = distance(eye_position, world_position);

  for(int i = 0; i < 16; ++i)
  {
    // check if light in use
    if(light_use[i] != 0)
    {
      
      
      if(light_type[i] == 0)
      {
        vec4 light_vector = light_position[i] - world_position;
      
        vec4 L = normalize(light_vector);
      
        float distance = distance(light_position[i], world_position);

        float att = min(1.0/(constant_attenuation + linear_attenuation * distance + quadratic_attenuation * distance * distance), 1.0);
        

        float ML = max(dot(M, L), 0.0);
      
        // skip calculations when ML is 0.0
        if(ML != 0.0)
        {
            //vec4 RL = 2.0 * ML* M - L;
            vec4 H = normalize(L + V);
  
            //float RLV = max(dot(RL, V), 0.0);
            float HN = max(dot(H, M), 0.0);
        
            //partial_sum += att * (Kd * ML + Ks * pow(RLV, Ns)) * light_color[i];
            partial_sum += att * (Kd * ML + Ks * pow(HN, Ns)) * light_color[i];
            partial_sum += (att * ambient_coefficient * light_color[i]);
        }
      }
      else if(light_type[i] == 1)
      {
        //negate light direction(light direction light > object)
        vec4 light_vector = -light_direction[i];
      
        vec4 L = normalize(light_vector);
        // No distance
        //float distance = distance(light_position[i], world_position);
        // No att
        //float att = min(1.0/(constant_attenuation + linear_attenuation * distance + quadratic_attenuation * distance * distance), 1.0);


        float ML = max(dot(M, L), 0.0);
      
        // skip calculations when ML is 0.0
        if(ML != 0.0)
        {
            //vec4 RL = 2.0 * ML* M - L;
            vec4 H = normalize(L + V);
  
            //float RLV = max(dot(RL, V), 0.0);
            float HN = max(dot(H, M), 0.0);
        
            //partial_sum += (Kd * ML + Ks * pow(RLV, Ns)) * light_color[i];
            partial_sum += (Kd * ML + Ks * pow(HN, Ns)) * light_color[i];
            partial_sum += (ambient_coefficient * light_color[i]);
        }
      }
      else if(light_type[i] == 2)
      {
        vec4 light_vector = light_position[i] - world_position;

        vec4 D = normalize(light_vector);
        vec4 L = normalize(-light_direction[i]);

        float distance = distance(light_position[i], world_position);

        float att = min(1.0/(constant_attenuation + linear_attenuation * distance + quadratic_attenuation * distance * distance), 1.0);

        float cos_alpha = dot(L, D);
        float cos_phi = cos(radians(light_outer_phi[i]));
        float cos_theta = cos(radians(light_inner_theta[i]));

        if(cos_alpha < cos_phi)
        {
            // outsitde the cone
        }
        else if(cos_alpha > cos_theta)
        {
           // inside the inner cone
            float ML = max(dot(M, L), 0.0);
        
            // skip calculations when ML is 0.0
            if(ML != 0.0)
            {
                //vec4 RL = 2.0 * ML* M - L;
                vec4 H = normalize(L + V);
        
                //float RLV = max(dot(RL, V), 0.0);
                float HN = max(dot(H, M), 0.0);
        
                //partial_sum += att * (Kd * ML + Ks * pow(RLV, Ns)) * light_color[i];
                partial_sum += att * (Kd * ML + Ks * pow(HN, Ns)) * light_color[i];
                partial_sum += (att * ambient_coefficient * light_color[i]);
            }
        }
        else
        {
            // between inner and outer cone

            float ML = max(dot(M, L), 0.0);
            if(ML != 0.0)
            {
                //vec4 RL = 2.0 * ML* M - L;
                vec4 H = normalize(L + V);
  
                //float RLV = max(dot(RL, V), 0.0);
                float HN = max(dot(H, M), 0.0);

                float SpotlightEffect = pow(((cos_alpha - cos_phi)/(cos_theta - cos_phi)), light_fallout[i]);
                //float SpotlightEffect = 1.0;
        
                //partial_sum += att * SpotlightEffect * (Kd * ML + Ks * pow(RLV, Ns)) * light_color[i];
                partial_sum += att * SpotlightEffect * (Kd * ML + Ks * pow(HN, Ns)) * light_color[i];
                partial_sum += (att * ambient_coefficient * light_color[i]);
            }
        }
      }
    }
  }
    vec3 i_local = ambient_coefficient * global_ambient_color + partial_sum;
    float S = (z_far - v_length) / (z_far - z_near);
    vec3 i_final = S * i_local + (1.0 - S) * fog_color;

    frag_color = vec4(i_final, 0.0);
}