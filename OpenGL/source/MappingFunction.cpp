/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: MappingFunction.cpp
Purpose: Implementation for uv mapping funcions
Language: C++/glsl
Platform: visual studio
Project: r.shen_CS300_2
Author: Richard Shen, r.shen, 110000516
Creation date: 11/13/2020
End Header --------------------------------------------------------*/
#pragma once
#include "MappingFunction.h"
#include <algorithm>

const float  PI = 3.1415926535897932384626433832795f;

glm::vec2 CalcCubeMap(const glm::vec3& vEntity)
{
    glm::vec3 absVec = abs(vEntity);
    glm::vec2 uv = glm::vec2(0.0f);

    float maxAxis;
    // +-X
    if (absVec.x >= absVec.y && absVec.x >= absVec.z)
    {
        maxAxis = absVec.x;
        (vEntity.x < 0.0f) ? (uv.x = vEntity.z) : (uv.x = -vEntity.z);
        uv.y = vEntity.y;
    }
    // +-y
    if (absVec.y >= absVec.x && absVec.y >= absVec.z)
    {
        maxAxis = absVec.y;
        (vEntity.y < 0.0f) ? (uv.y = vEntity.z) : (uv.y = -vEntity.z);
        uv.x = vEntity.x;
    }
    // +-z
    if (absVec.z >= absVec.x && absVec.z >= absVec.y)
    {
        maxAxis = absVec.z;
        (vEntity.z < 0.0f) ? (uv.x = -vEntity.x) : (uv.x = vEntity.x);
        uv.y = vEntity.y;
    }
    // and so on … +-Y and +-Z
    // At this point, uv.x and uv.y should be in the range [-1, 1]
    // Convert range from -1 to 1 to 0 to 1
    return (uv / maxAxis + glm::vec2(1.0f)) * 0.5f;
}

glm::vec2 CalcPlanarMap(const glm::vec3& vEntity, const glm::vec3& dimension)
{
    //float u = vEntity.x;// ((vEntity.x) + (dimension.x / 2.0f)) / dimension.x;
    //float v = vEntity.y;// ((vEntity.y) + (dimension.y / 2.0f)) / dimension.y;

    float u = ((vEntity.x) + (dimension.x / 2.0f)) / dimension.x;
    float v = ((vEntity.y) + (dimension.y / 2.0f)) / dimension.y;

    //float u = (vEntity.x  + 1.0f) * 0.5f;
    //float v = (vEntity.y + 1.0f) * 0.5f;

    //float u = vEntity.x;// ((vEntity.x) + (dimension.x / 2.0f)) / dimension.x;
    //float v = vEntity.y;// ((vEntity.y) + (dimension.y / 2.0f)) / dimension.y;

    return glm::vec2(u, v);
}

glm::vec2 CalcCylindricalMap(const glm::vec3& vEntity, const glm::vec3& dimension, const glm::vec3& center)
{
    float z_range = (dimension.z / 2.0f);
    float z_min = center.z - z_range;
    float z_max = center.z + z_range;

    //float z_range = 2.0f;
    //float z_min = -1.0f;
    //float z_max = 1.0f;


    float theta;
    //if (vEntity.x == 0.0f)
        //theta = 0.0f;
    if (abs(vEntity.x) < 0.00001f) { theta = vEntity.y > 0.0f ? PI * 0.5f : -PI* 0.5f; }
    else
    {
        theta = glm::atan(vEntity.y, vEntity.x);
    }

    if (theta < 0)
        theta += 2.0f * PI;
    //if (x_prime < 0.0f )
        //theta += PI;

    float z_out = (vEntity.z - z_min) / (z_max - z_min);
    //float z_out = vEntity.z * 0.5f + 0.5f;
    //float z_out = vEntity.z + 1.0f;
    //float z_out = vEntity.z + 1.0f;
    float u = theta / (2.0f * PI);
    float v = z_out;

    return glm::vec2(u, v);
}

glm::vec2 CalcSphericalMap(const glm::vec3& vEntity)
{
    //float z_range = (dimension.z / 2.0f);
    //float z_min = center.z - z_range;
    //float z_max = center.z + z_range;

    float r = sqrt(vEntity.x * vEntity.x + vEntity.y * vEntity.y + vEntity.z * vEntity.z);

    float phi;
    if (r < 0.00001f)
    {
        phi = PI / 2.0f;
    }
    else
    {
        phi = glm::acos(vEntity.z / r);
    }

    float theta;

    if (abs(vEntity.x) < 0.00001f) { theta = vEntity.y > 0.0f ? PI * 0.5f : -PI * 0.5f; }
    else
    {
        theta = glm::atan(vEntity.y, vEntity.x);
    }

    if (theta < 0)
        theta += 2.0f * PI;

    float u = theta / (2.0f * PI);
    float v = (PI - phi) / PI;


    return glm::vec2(u, v);
}

glm::vec3 NormalizeToBoundingBox(const glm::vec3& vEntity, const glm::vec3& center, const glm::vec3& dimension)
{
    //float x = ((vEntity.x) + (dimension.x * 0.5f)) / dimension.x;
    //float y = ((vEntity.y) + (dimension.y * 0.5f)) / dimension.y;
    //float z = ((vEntity.z) + (dimension.z * 0.5f)) / dimension.z;
    //glm::vec3 absVec = abs(vEntity);

    float maxAxis = std::max(dimension.x, std::max(dimension.y, dimension.z));
    float x = ((vEntity.x) - center.x) / (maxAxis / 2.0f);
    float y = ((vEntity.y) - center.y) / (maxAxis / 2.0f);
    float z = ((vEntity.z) - center.z) / (maxAxis / 2.0f);
    //float maxAxis = std::max(absVec.x, std::max(absVec.y, absVec.z));
    //float x = ((vEntity.x) - center.x);
    //float y = ((vEntity.y) - center.y);
    //float z = ((vEntity.z) - center.z);
    //float x = ((vEntity.x) - center.x) / (dimension.x);
    //float y = ((vEntity.y) - center.y) / (dimension.y);
    //float z = ((vEntity.z) - center.z) / (dimension.z);

    return glm::vec3(x, y, z);
}
