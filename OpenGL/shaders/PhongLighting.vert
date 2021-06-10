/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: PhongLighting.vert
Purpose: vert shader for phong lighting
Language: C++/glsl
Platform: visual studio
Project: r.shen_CS300_1
Author: Richard Shen, r.shen, 110000516
Creation date: 11/9/2020
End Header --------------------------------------------------------*/
#version 430 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec2 aTexCoord;
const float  PI = 3.1415926535897932384626433832795;

uniform mat4 persp_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
uniform mat4 normal_matrix;

uniform int gpu_calc;
uniform int ventity_type;
uniform int mapping_type;

uniform vec3 dimension;
uniform vec3 center;

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


out vec4 world_normal;
out vec4 world_position;
//out vec2 TexCoord;

vec2 CalcSphericalMap(vec3 vEntity)
{
    float r = sqrt(vEntity.x * vEntity.x + vEntity.y * vEntity.y + vEntity.z * vEntity.z);

    float phi;
    if (r < 0.00001f)
    {
        phi = PI / 2.0f;
    }
    else
    {
        phi = acos(vEntity.z / r);
    }

    float theta;
    if (abs(vEntity.x) < 0.00001) { theta = vEntity.y > 0 ? PI * 0.5 : -PI* 0.5; }
    else
    {
        theta = atan(vEntity.y, vEntity.x);
    }

    if (theta < 0)
        theta += 2.0 * PI;

    float u = theta / (2.0 * PI);
    float v = (PI - phi) / PI;


    return vec2(u, v);
}

vec2 CalcCylindricalMap(vec3 vEntity)
{
    float z_range = (dimension.z / 2.0);
    float z_min = center.z - z_range;
    float z_max = center.z + z_range;

    float theta;
    if (abs(vEntity.x) < 0.00001) { theta = vEntity.y > 0 ? PI * 0.5 : -PI* 0.5; }
    else
    {
        theta = atan(vEntity.y, vEntity.x);
    }

    if (theta < 0)
        theta += 2.0 * PI;
    //if (x_prime < 0.0 )
        //theta += PI;

    float z_out = (vEntity.z - z_min) / (z_max - z_min);


    float u = theta / (2.0 * PI);
    float v = z_out;

    return vec2(u, v);
}

vec2 CalcCubeMap(vec3 vEntity)
{
    vec3 absVec = abs(vEntity);
    vec2 uv = vec2(0.0);

    float maxAxis;
    // +-X
    if (absVec.x >= absVec.y && absVec.x >= absVec.z)
    {
        maxAxis = absVec.x;
        (vEntity.x < 0.0) ? (uv.x = vEntity.z) : (uv.x = -vEntity.z);
        uv.y = vEntity.y;
    }
    // +-y
    if (absVec.y >= absVec.x && absVec.y >= absVec.z)
    {
        maxAxis = absVec.y;
        (vEntity.y < 0.0) ? (uv.y = vEntity.z) : (uv.y = -vEntity.z);
        uv.x = vEntity.x;
    }
    // +-z
    if (absVec.z >= absVec.x && absVec.z >= absVec.y)
    {
        maxAxis = absVec.z;
        (vEntity.z < 0.0) ? (uv.x = -vEntity.x) : (uv.x = vEntity.x);
        uv.y = vEntity.y;
    }
    // and so on … +-Y and +-Z
    // At this point, uv.x and uv.y should be in the range [-1, 1]
    // Convert range from -1 to 1 to 0 to 1
    return (uv / maxAxis + vec2(1.0)) * 0.5;
}

vec2 CalcPlanarMap(vec3 vEntity)
{
    float u = ((vEntity.x) + (dimension.x / 2.0)) / dimension.x;
    float v = ((vEntity.y) + (dimension.y / 2.0)) / dimension.y;

    return vec2(u, v);
}

out vec3 i_final;
//out vec4 frag_color;

void main()
{
  vec4 world_position = model_matrix * position;

  gl_Position = persp_matrix * view_matrix * world_position;

  
  vec4 world_normal = normal_matrix * normal;

  vec2 TexCoord = aTexCoord;

    vec3 vent;
  if(ventity_type == 0)
    vent = position.xyz - center.xyz;
  else
    vent = normalize(normal.xyz);
  if(gpu_calc == 1)
  {
    switch(mapping_type)
    {
        case 0:
            TexCoord = CalcPlanarMap(vent);
            break;
        case 1:
            TexCoord = CalcCubeMap(vent);
            break;
        case 2:
            TexCoord = CalcCylindricalMap(vent);
            break;
        case 3:
            TexCoord = CalcSphericalMap(vent);
            break;
    }
  }
  else
   TexCoord = aTexCoord;

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
            vec4 RL = 2.0 * ML* M - L;
  
            float RLV = max(dot(RL, V), 0.0);
        
            partial_sum += att * (Kd * ML + Ks * pow(RLV, Ns)) * light_color[i];
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
            vec4 RL = 2.0 * ML* M - L;
  
            float RLV = max(dot(RL, V), 0.0);
        
            partial_sum += (Kd * ML + Ks * pow(RLV, Ns)) * light_color[i];
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
                vec4 RL = 2.0 * ML* M - L;
        
                float RLV = max(dot(RL, V), 0.0);
        
                partial_sum += att * (Kd * ML + Ks * pow(RLV, Ns)) * light_color[i];
                partial_sum += (att * ambient_coefficient * light_color[i]);
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
                partial_sum += (att * ambient_coefficient * light_color[i]);
            }
        }
      }
    }
  }
    vec3 i_local = ambient_coefficient * global_ambient_color + partial_sum;
    float S = (z_far - v_length) / (z_far - z_near);
    i_final = S * i_local + (1.0 - S) * fog_color;

    //frag_color = vec4(i_final, 0.0);
}