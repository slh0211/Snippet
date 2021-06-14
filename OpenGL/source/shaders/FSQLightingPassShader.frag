/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: FSQLightingPassShader.frag
Purpose: frag shader for rendering FSQ on to screen
Language: C++/glsl
Platform: visual studio
Project: r.shen_CS350_1
Author: Richard Shen, r.shen, 110000516
Creation date: 1/26/2021
End Header --------------------------------------------------------*/
#version 430 core

uniform sampler2D gPositionSampler;
uniform sampler2D gNormalSampler;
uniform sampler2D gDiffuseSampler;
uniform sampler2D gSpecularShininessSampler;
uniform sampler2D gAmbientSampler;

uniform int uMode;

in vec2 TexCoords;
out vec4 FragColor;

uniform vec4 eye_position;

uniform vec4 light_position[16];
uniform vec4 light_direction[16];
uniform vec3 light_color[16];
uniform int light_use[16];
uniform int light_type[16];
uniform float light_inner_theta[16];
uniform float light_outer_phi[16];
uniform float light_fallout[16];

uniform vec3 global_ambient_color;

uniform float constant_attenuation;
uniform float linear_attenuation;
uniform float quadratic_attenuation;

uniform float z_far;
uniform float z_near;
uniform vec3  fog_color;

uniform mat4 view_matrix;

void main()
{
  vec3 partial_sum = vec3(0.0, 0.0, 0.0);

  vec3 Ka = texture(gAmbientSampler, TexCoords).rgb;
  vec3 Kd = texture(gDiffuseSampler, TexCoords).rgb;
  vec3 Ks = texture(gSpecularShininessSampler, TexCoords).rgb;
  float Ns = texture(gSpecularShininessSampler, TexCoords).a;

  //vec4 world_normal = texture(gNormalSampler, TexCoords);
  //vec4 world_position = texture(gPositionSampler, TexCoords);

  vec4 view_normal = texture(gNormalSampler, TexCoords);
  vec4 view_position = texture(gPositionSampler, TexCoords);

  //vec4 M = normalize(world_normal);
  vec4 M = normalize(view_normal);

  vec4 view_eye_position = view_matrix * eye_position;

  vec4 view_vector = (view_eye_position) - view_position;
  //vec4 view_vector = (eye_position) - world_position;
  vec4 V = normalize(view_vector);
  
  float v_length = distance((view_eye_position), view_position);
  //float v_length = distance((eye_position), world_position);

  for(int i = 0; i < 16; ++i)
  {
    // check if light in use
    if(light_use[i] != 0)
    {
      
      
      if(light_type[i] == 0)
      {
        vec4 light_vector = (view_matrix * light_position[i]) - view_position;
        //vec4 light_vector = (light_position[i]) - world_position;
      
        vec4 L = normalize(light_vector);
      
        float distance = distance(view_matrix * light_position[i], view_position);
        //float distance = distance(light_position[i], world_position);

        float att = min(1.0/(constant_attenuation + linear_attenuation * distance + quadratic_attenuation * distance * distance), 1.0);


        float ML = max(dot(M, L), 0.0);
      
        // skip calculations when ML is 0.0
        if(ML != 0.0)
        {
            vec4 RL = 2.0 * ML* M - L;
  
            float RLV = max(dot(RL, V), 0.0);
        
            partial_sum += att * (Kd * ML + Ks * pow(RLV, Ns)) * light_color[i];
            partial_sum += (att * Ka * light_color[i]);
        }
      }
      else if(light_type[i] == 1)
      {
        //negate light direction(light direction light > object)
        vec4 light_vector = -(view_matrix * light_direction[i]);
        //vec4 light_vector = -(light_direction[i]);
      
        vec4 L = normalize(light_vector);
        // No distance
        //float distance = distance((view_matrix *light_position[i]), world_position);
        // No att
        //float att = min(1.0/(constant_attenuation + linear_attenuation * distance + quadratic_attenuation * distance * distance), 1.0);


        float ML = max(dot(M, L), 0.0);
      
        // skip calculations when ML is 0.0
        if(ML != 0.0)
        {
            vec4 RL = 2.0 * ML* M - L;
  
            float RLV = max(dot(RL, V), 0.0);
        
            partial_sum += (Kd * ML + Ks * pow(RLV, Ns)) * light_color[i];
            partial_sum += (Ka * light_color[i]);
        }
      }
      else if(light_type[i] == 2)
      {
        vec4 light_vector = (view_matrix *light_position[i]) - view_position;
        //vec4 light_vector = (light_position[i]) - world_position;

        vec4 D = normalize(light_vector);
        vec4 L = normalize(-(view_matrix *light_direction[i]));
        //vec4 L = normalize(-(light_direction[i]));

        float distance = distance((view_matrix *light_position[i]), view_position);
        //float distance = distance((light_position[i]), world_position);

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
                vec4 RL = 2.0 * ML* M - L;
        
                float RLV = max(dot(RL, V), 0.0);
        
                partial_sum += att * (Kd * ML + Ks * pow(RLV, Ns)) * light_color[i];
                partial_sum += (att * Ka * light_color[i]);
            }
        }
        else
        {
            // between inner and outer cone

            float ML = max(dot(M, L), 0.0);
            if(ML != 0.0)
            {
                vec4 RL = 2.0 * ML* M - L;
  
                float RLV = max(dot(RL, V), 0.0);

                float SpotlightEffect = pow(((cos_alpha - cos_phi)/(cos_theta - cos_phi)), light_fallout[i]);
                //float SpotlightEffect = 1.0;
        
                partial_sum += att * SpotlightEffect * (Kd * ML + Ks * pow(RLV, Ns)) * light_color[i];
                partial_sum += (att * Ka * light_color[i]);
            }
        }
      }
    }
  }
    vec3 i_local = Ka * global_ambient_color + partial_sum;
    float S = (z_far - v_length) / (z_far - z_near);
    vec3 i_final = S * i_local + (1.0 - S) * fog_color;



	switch(uMode)
	{
		case 0:
			FragColor = vec4(i_final, 0.0);
			break;

		case 1:
			FragColor = view_position;
			break;

		case 2:
			FragColor = view_normal;
			break;

		case 3:
			FragColor = vec4(Kd, 0.0);
			break;

		case 4:
			FragColor = vec4(Ks, 0.0);
			break;

		case 5:
			FragColor = vec4(Ka, 0.0);
			break;
			
	}

	//FragColor = texture(gPositionSampler, TexCoords);
	//FragColor = texture(gNormalSampler, TexCoords);
	//FragColor = texture(gDiffuseSampler, TexCoords);
	//FragColor = texture(gSpecularShininessSampler, TexCoords);
	//FragColor = texture(gAmbientSampler, TexCoords);
}