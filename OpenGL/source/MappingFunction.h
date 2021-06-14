/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: MappingFunction.h
Purpose: Header for uv mapping funcions
Language: C++/glsl
Platform: visual studio
Project: r.shen_CS300_2
Author: Richard Shen, r.shen, 110000516
Creation date: 11/13/2020
End Header --------------------------------------------------------*/
#pragma once
#include <vector>
#include <glm/glm.hpp>

glm::vec2 CalcCubeMap(const glm::vec3& vEntity);
glm::vec2 CalcPlanarMap(const glm::vec3& vEntity, const glm::vec3& dimension);
glm::vec2 CalcCylindricalMap(const glm::vec3& vEntity, const glm::vec3& dimension, const glm::vec3& center);
glm::vec2 CalcSphericalMap(const glm::vec3& vEntity);

glm::vec3 NormalizeToBoundingBox(const glm::vec3& vEntity, const glm::vec3& center, const glm::vec3& dimension);