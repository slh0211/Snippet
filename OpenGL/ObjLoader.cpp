/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ObjLoader.cpp
Purpose: Implementaion for a ladoOBJ functions
Language: C++
Platform: visual studio
Project: r.shen_CS300_1
Author: Richard Shen, r.shen, 110000516
Creation date: 10/1/2020
End Header --------------------------------------------------------*/
#include "ObjLoader.h"

#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include "Affine.h"

#include <unordered_set>
#include <glm/gtx/hash.hpp>

bool loadOBJ(
	std::string path,
	std::vector<glm::vec3>& out_vertices, std::vector<unsigned int>& out_indices,
	glm::vec3& min,
	glm::vec3& max

) {
	std::cout << "Loading OBJ file " << path << "..." << std::endl;
	//printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices;
	std::vector<glm::vec3> temp_vertices;
	//std::vector<glm::vec2> temp_uvs;
	//std::vector<glm::vec3> temp_normals;

	std::ifstream inFile;
	inFile.open(path);
	if (inFile.bad() || inFile.eof() || inFile.fail())
	{
		std::cout << "Impossible to open the file ! Are you in the right path ?" << std::endl;
		return false;
	}


	while (!inFile.eof())
	{
		char buffer[256] = "\0";
		inFile.getline(buffer, 256, '\n');
		ParseOBJRecord(buffer, out_vertices, out_indices, min, max);
	}

	return true;
}

bool loadOBJtoMesh(std::string path, NMesh& mesh)
{
	std::cout << "Loading OBJ file " << path << "..." << std::endl;



	std::ifstream inFile;
	inFile.open(path);
	if (inFile.bad() || inFile.eof() || inFile.fail())
	{
		std::cout << "Impossible to open the file ! Are you in the right path ?" << std::endl;
		return false;
	}

	glm::vec3 min = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	glm::vec3 max = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	while (!inFile.eof())
	{
		char buffer[256] = "\0";
		inFile.getline(buffer, 256, '\n');
		ParseOBJRecordToMesh(buffer, mesh, min, max);
	}

	//mesh._center = cs250::point(0.0f, 0.0f, 0.0f);//cs250::point((min.x + max.x)/2, (min.y + max.y)/2, (min.z + max.z)/2);
	mesh._center = cs250::point((min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f, (min.z + max.z) / 2.0f);
	mesh._dimensions = cs250::vector(max.x - min.x, max.y - min.y, max.z - min.z);

	//std::vector<std::unordered_set <glm::vec3, std::hash<glm::vec3>,compareVec> > container_temp_set(mesh.vertexCount());
	std::vector<std::unordered_set <glm::vec3, std::hash<glm::vec3>, compareVec> > container_temp_set(mesh.vertexCount());
	mesh.normals.resize(mesh.vertexCount());

	for (int i = 0; i < mesh.faceCount(); ++i)
	{
		const cs250::Mesh::Face& F = mesh.faceArray()[i];

		glm::vec4 v1 = mesh.vertexArray()[F.index1];
		glm::vec4 v2 = mesh.vertexArray()[F.index2];
		glm::vec4 v3 = mesh.vertexArray()[F.index3];

		glm::vec4 v2_v1 = v2 - v1;
		glm::vec4 v3_v1 = v3 - v1;

		glm::vec4 normal = cs250::cross(v2_v1, v3_v1);

		glm::vec3 normal3 = glm::normalize(normal);

		container_temp_set[F.index1].insert(normal3);
		container_temp_set[F.index2].insert(normal3);
		container_temp_set[F.index3].insert(normal3);

		//mesh.normals[F.index1] += normal;
		//mesh.normals[F.index2] += normal;
		//mesh.normals[F.index3] += normal;
	}

	for (int i = 0; i < mesh.vertexCount(); ++i)
	{
		glm::vec3 normal_sum(0.0f, 0.0f, 0.0f);

		for (auto itr = container_temp_set[i].begin(); itr != container_temp_set[i].end(); ++itr)
		{
			normal_sum += *itr;
		}

		//mesh.normals[i] = normalize(mesh.normals[i]);
		mesh.normals[i] = glm::vec4(glm::normalize(normal_sum), 0.0f);
		//mesh.normals[i] = glm::vec4(normal_sum.x/container_temp_set.size(), normal_sum.y/container_temp_set.size(), normal_sum.z/container_temp_set.size(), 0.0f);
	}

	return true;
}

void ParseOBJRecord(char* buffer, std::vector<glm::vec3>& out_vertices, std::vector<unsigned int>& out_indices, glm::vec3& min, glm::vec3& max)
{
	const char* delims = " \r\n\t";
	GLfloat x, y, z;

	GLfloat temp;
	GLuint firstIndex, secondIndex, thirdIndex;

	char* token = strtok(buffer, delims);

	if (token == nullptr)
		return;

	switch (token[0])
	{
		case 'v':
			// vertex coordinates
			if (token[1] == '\0')
			{
				token = strtok(nullptr, delims);
				temp = static_cast<GLfloat &&>(atof(token));
				if (min.x > temp)
					min.x = temp;
				if (max.x <= temp)
					max.x = temp;
				x = temp;

				token = strtok(nullptr, delims);
				temp = static_cast<GLfloat&&>(atof(token));
				if (min.y > temp)
					min.y = temp;
				if (max.y <= temp)
					max.y = temp;
				y = temp;

				token = strtok(nullptr, delims);
				temp = static_cast<GLfloat&&>(atof(token));
				if (min.z > temp)
					min.z = temp;
				if (max.z <= temp)
					max.z = temp;
				z = temp;

				glm::vec3 vertex = { x, y, z };

				out_vertices.push_back(vertex);
			}
			else if (token[1] == 'n')
			{
				// vertex normal
				// do nothing for now
			}
			break;

		case 'f':
			token = strtok(nullptr, delims);
			if (token == nullptr)
				break;
			firstIndex = static_cast<unsigned int&&>(atoi(token) - 1);

			token = strtok(nullptr, delims);
			if (token == nullptr)
				break;
			secondIndex = static_cast<unsigned int&&>(atoi(token) - 1);

			token = strtok(nullptr, delims);
			if (token == nullptr)
				break;
			thirdIndex = static_cast<unsigned int&&>(atoi(token) - 1);

			out_indices.push_back(firstIndex);
			out_indices.push_back(secondIndex);
			out_indices.push_back(thirdIndex);

			token = strtok(nullptr, delims);
			while (token != nullptr)
			{
				secondIndex = thirdIndex;
				thirdIndex = static_cast<unsigned int&&>(atoi(token) - 1);

				out_indices.push_back(firstIndex);
				out_indices.push_back(secondIndex);
				out_indices.push_back(thirdIndex);

				token = strtok(nullptr, delims);
			}

			break;

		case '#':
		default:
			break;

	}
}

void ParseOBJRecordToMesh(char* buffer, NMesh& mesh, glm::vec3& min, glm::vec3& max)
{
	const char* delims = " \r\n\t";
	GLfloat x, y, z;

	GLfloat temp;
	GLuint firstIndex, secondIndex, thirdIndex;

	char* token = strtok(buffer, delims);

	if (token == nullptr)
		return;

	switch (token[0])
	{
	case 'v':
		// vertex coordinates
		if (token[1] == '\0')
		{
			token = strtok(nullptr, delims);
			temp = static_cast<GLfloat&&>(atof(token));
			if (min.x > temp)
				min.x = temp;
			if (max.x <= temp)
				max.x = temp;
			x = temp;

			token = strtok(nullptr, delims);
			temp = static_cast<GLfloat&&>(atof(token));
			if (min.y > temp)
				min.y = temp;
			if (max.y <= temp)
				max.y = temp;
			y = temp;

			token = strtok(nullptr, delims);
			temp = static_cast<GLfloat&&>(atof(token));
			if (min.z > temp)
				min.z = temp;
			if (max.z <= temp)
				max.z = temp;
			z = temp;

			//glm::vec3 vertex = { x, y, z };

			//out_vertices.push_back(vertex);
			mesh.vertices.push_back(cs250::point(x, y, z));
		}
		else if (token[1] == 'n')
		{
			// vertex normal
			// do nothing for now
		}
		break;

	case 'f':
		token = strtok(nullptr, delims);
		if (token == nullptr)
			break;
		firstIndex = static_cast<unsigned int&&>(atoi(token) - 1);

		token = strtok(nullptr, delims);
		if (token == nullptr)
			break;
		secondIndex = static_cast<unsigned int&&>(atoi(token) - 1);

		token = strtok(nullptr, delims);
		if (token == nullptr)
			break;
		thirdIndex = static_cast<unsigned int&&>(atoi(token) - 1);

		mesh.faces.push_back(cs250::Mesh::Face(firstIndex, secondIndex, thirdIndex));

		//out_indices.push_back(firstIndex);
		//out_indices.push_back(secondIndex);
		//out_indices.push_back(thirdIndex);

		token = strtok(nullptr, delims);
		while (token != nullptr)
		{
			secondIndex = thirdIndex;
			thirdIndex = static_cast<unsigned int&&>(atoi(token) - 1);

			//out_indices.push_back(firstIndex);
			//out_indices.push_back(secondIndex);
			//out_indices.push_back(thirdIndex);
			mesh.faces.push_back(cs250::Mesh::Face(firstIndex, secondIndex, thirdIndex));

			token = strtok(nullptr, delims);
		}

		break;

	case '#':
	default:
		break;

	}
}
