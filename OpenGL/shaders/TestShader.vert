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

out vec4 world_normal;
out vec4 world_position;
out vec2 TexCoord;

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

    //float phi = acos(vEntity.z / r);

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

void main()
{
  world_position = model_matrix * position;

  gl_Position = persp_matrix * view_matrix * world_position;

  
  world_normal = normal_matrix * normal;

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
}