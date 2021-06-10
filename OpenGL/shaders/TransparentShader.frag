/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: TransparentShader.frag
Purpose: frag shader for transparnet texture using environment mapping
Language: C++/glsl
Platform: visual studio
Project: r.shen_CS300_3
Author: Richard Shen, r.shen, 110000516
Creation date : 11/29/2020
End Header --------------------------------------------------------*/
#version 430 core

const int   nullId  = 0;
const int   mainId	= 1;
const int   sphereId = 2;
const int   floorId = 3;  

const int   backID  = 0;
const int   rightID	= 1;
const int   frontID = 2;
const int   leftID = 3;  
const int   topID  =  4;
const int   bottomID = 5;

const int   reflection_only = 0;
const int   refraction_only = 1;
const int   refraction_ca   = 2;
const int   reflec_and_refrac = 3;


uniform int objectID;
uniform int transparentID;

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

uniform sampler2D backTex;
uniform sampler2D rightTex;
uniform sampler2D frontTex;
uniform sampler2D leftTex;
uniform sampler2D topTex;
uniform sampler2D bottomTex;

uniform int shading_use;
uniform float eta_in;
uniform float eta_out;
uniform float epsilon;

in vec4 world_position;
in vec4 world_normal;
in vec2 TexCoord;

out vec4 frag_color;

vec3 CalcRefractionVector(vec3 V, vec3 N, float K)
{
    float N_dot_V = dot(V, N);
    float N_dot_V_sq = N_dot_V * N_dot_V;

    float T1 = K * N_dot_V;
    float T2 = pow(1.0 - (K * K * (1.0 - N_dot_V_sq)), 0.5);
    vec3 T = (T1 - T2) * N - (K * V);

    return T;
}


vec2 ConvertToUV(vec3 vEntity, inout int index)
{
    vec3 absVec = abs(vEntity);

     float absX = absVec.x;
     float absY = absVec.y;
     float absZ = absVec.z;
     //vec2 uv = vec2(0.0);

    bool isXPositive = vEntity.x > 0 ? true : false;
    bool isYPositive = vEntity.y > 0 ? true : false;
    bool isZPositive = vEntity.z > 0 ? true : false;

    float maxAxis, uc, vc;
  
  // POSITIVE X
  if (isXPositive && absX >= absY && absX >= absZ) {
    // u (0 to 1) goes from +z to -z
    // v (0 to 1) goes from -y to +y
    maxAxis = absX;
    uc = -vEntity.z;
    vc = vEntity.y;
    index = 0;
  }
  // NEGATIVE X
  if (!isXPositive && absX >= absY && absX >= absZ) {
    // u (0 to 1) goes from -z to +z
    // v (0 to 1) goes from -y to +y
    maxAxis = absX;
    uc = vEntity.z;
    vc = vEntity.y;
    index = 1;
  }
  // POSITIVE Y
  if (isYPositive && absY >= absX && absY >= absZ) {
    // u (0 to 1) goes from -x to +x
    // v (0 to 1) goes from +z to -z
    maxAxis = absY;
    uc = vEntity.x;
    vc = -vEntity.z;
    index = 2;
  }
  // NEGATIVE Y
  if (!isYPositive && absY >= absX && absY >= absZ) {
    // u (0 to 1) goes from -x to +x
    // v (0 to 1) goes from -z to +z
    maxAxis = absY;
    uc = vEntity.x;
    vc = vEntity.z;
    index = 3;
  }
  // POSITIVE Z
  if (isZPositive && absZ >= absX && absZ >= absY) {
    // u (0 to 1) goes from -x to +x
    // v (0 to 1) goes from -y to +y
    maxAxis = absZ;
    uc = vEntity.x;
    vc = vEntity.y;
    index = 4;
  }
  // NEGATIVE Z
  if (!isZPositive && absZ >= absX && absZ >= absY) {
    // u (0 to 1) goes from +x to -x
    // v (0 to 1) goes from -y to +y
    maxAxis = absZ;
    uc = -vEntity.x;
    vc = vEntity.y;
    index = 5;
  }

  // Convert range from -1 to 1 to 0 to 1
  float u = 0.5 * (uc / maxAxis + 1.0);
  float v = 0.5 * (vc / maxAxis + 1.0);

  return vec2(1 - u, v);
}

vec4 CalcReflectionColor(vec4 V, vec4 N)
{
    float NV = dot(N, V);

    vec4 R = normalize(2.0 * N *  NV - V);

    int index = -1;

    vec2 uv = ConvertToUV(vec3(R.xyz), index);

    vec4 out_c = vec4(1.0, 1.0, 1.0, 1.0);

    switch(index)
    {
        case 0:
            out_c = texture(rightTex, uv);
            break;
        
        case 1:
            out_c = texture(leftTex, uv);
            break;

        case 2:
            out_c = texture(topTex, uv);
            break;

        case 3:
            out_c = texture(bottomTex, uv);
            break;

        case 4:
            out_c = texture(frontTex, uv);
            break;

        case 5:
            out_c = texture(backTex, uv);
            break;
    }

    return out_c;
}

vec4 CalcRefractionColor(vec3 T)
{
    vec3 normalize_T = normalize(T);

    int index = -1;

    vec2 uv = ConvertToUV(normalize_T.xyz, index);
    vec4 out_c = vec4(0.0, 0.0, 0.0, 0.0);
    
    switch(index)
    {
        case 0:
            out_c = texture(rightTex, uv);
            break;
        
        case 1:
            out_c = texture(leftTex, uv);
            break;
    
        case 2:
            out_c = texture(topTex, uv);
            break;
    
        case 3:
            out_c = texture(bottomTex, uv);
            break;
    
        case 4:
            out_c = texture(frontTex, uv);
            break;
    
        case 5:
            out_c = texture(backTex, uv);
            break;
    }

    return out_c;
}

void main()
{
  vec3 partial_sum = vec3(0.0, 0.0, 0.0);

  vec3 Kd = diffuse_coefficient;
  vec3 Ks = specular_coefficient;
  float Ns = specular_exponent;

  vec4 N = normalize(world_normal);
  vec4 view_vector = eye_position - world_position;
  vec4 V = normalize(view_vector);

  if(objectID == mainId)
  {
  float NV = dot(N, V);

    vec4 Reflection_color = CalcReflectionColor(V, N);
    
    vec4 T = vec4(CalcRefractionVector(V.xyz, N.xyz, eta_in/eta_out), 1.0);

    vec4 Refraction_only_color = CalcRefractionColor(T.rgb);

    vec3 T_R = CalcRefractionVector(V.xyz, N.xyz, (eta_in - epsilon)/eta_out);
    vec3 T_G = CalcRefractionVector(V.xyz, N.xyz, (eta_in)/eta_out);
    vec3 T_B = CalcRefractionVector(V.xyz, N.xyz, (eta_in + epsilon)/eta_out);

    float C_R = CalcRefractionColor(T_R).r;
    float C_G = CalcRefractionColor(T_G).g;
    float C_B = CalcRefractionColor(T_B).b;

    vec4 Refraction_color = vec4(C_R, C_G, C_B, 1.0);

    float R0 = (eta_out - eta_in)/(eta_out + eta_in);
    float R1 = R0 * R0;

    float F = R1 + ( (1.0 - R1) * pow(1 - NV, 5.0) );

    vec4 transparent_texture = vec4(0.0, 0.0, 0.0, 0.0);

    switch(transparentID)
    {
        case reflection_only:
            transparent_texture = Reflection_color;
            break;

        case refraction_only:
            transparent_texture = Refraction_only_color;
            break;

        case refraction_ca:
            transparent_texture = Refraction_color;
            break;

        case reflec_and_refrac:
            transparent_texture = mix(Reflection_color, Refraction_color, F);
            break;
    }

    if(shading_use == 0)
    {
        frag_color = transparent_texture;
        return;
    }


    Kd = transparent_texture.rgb;
    Ks = transparent_texture.rgb;
    Ns = 99999.0;
    }

    vec4 M = N;




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
    vec3 i_final = S * i_local + (1.0 - S) * fog_color;

    frag_color = vec4(i_final, 0.0);
}