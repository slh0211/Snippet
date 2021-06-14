/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ObjLoader.h
Purpose: Prototype for a ladoOBJ functions
Language: C++
Platform: visual studio
Project: r.shen_CS300_1
Author: Richard Shen, r.shen, 110000516
Creation date: 10/1/2020
End Header --------------------------------------------------------*/
#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "FlatShadedMesh.h"
#include <glm/gtc/epsilon.hpp>

struct compareVec
{
	bool operator()(const glm::vec3& lhs, const glm::vec3& rhs) const
	{

		float V = glm::dot(lhs, rhs);

		//glm::vec3 V = glm::cross(lhs, rhs);
		//bool bRetCode = glm::epsilonNotEqual(V, 1.0f, 0.01f);
		bool bRetCode = glm::epsilonEqual(V, 1.0f, FLT_EPSILON);
		//bool result = false;
		//if ((V * V) > 0 && lhs != rhs)
		//if ((V.x == 0.0f && V.y == 0.0f && V.z== 0.0f) && rhs != lhs)
			//result = true;
	    //return result;
		return bRetCode;
	}
};

bool loadOBJ(
	std::string path,
	std::vector<glm::vec3>& out_vertices,
	std::vector<unsigned int>& out_indices,
	glm::vec3& min,
	glm::vec3& max
);

bool loadOBJtoMesh(std::string path, NMesh &mesh);

void ParseOBJRecord(char* buffer, std::vector<glm::vec3>& out_vertices, std::vector<unsigned int>& out_indices, glm::vec3& min, glm::vec3& max);

void ParseOBJRecordToMesh(char* buffer, NMesh &mesh, glm::vec3& min, glm::vec3& max);