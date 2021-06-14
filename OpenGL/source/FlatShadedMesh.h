/******************************************************************************/
/*!
\file   FlatShadedMesh.h
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS250
\par    Assignment 2 MeshCube
\par    Summer 2020
\date   5/16/2020
\brief
	This file contains the prototype of NMesh class

  Hours spent on this assignment: 2

  Specific portions that gave you the most trouble:

*/
/******************************************************************************/
#pragma once
#include "Mesh.h"
#include <vector>
/******************************************************************************/
/*!
  \struct NMesh
  \brief
	NMesh struct to contain datas requied for NormalMesh
*/
/******************************************************************************/
struct NMesh : cs250::NormalMesh {
	int vertexCount(void) const override;
	const glm::vec4* vertexArray(void) const override;
	glm::vec4 dimensions(void) const override;
	glm::vec4 center(void) const override;
	int faceCount(void) const override;
	const Face* faceArray(void) const override;
	int edgeCount(void) const override;
	const Edge* edgeArray(void) const override;
	const glm::vec4* normalArray(void) const override;

	// newly added
	const glm::vec2* texCoordArray(void) const override;

	void createTexCoordsPlanar(void);
	void createTexCoordsCylindrical(void);
	void createTexCoordsSpherical(void);
	void createTexCoordsCubeMap(void);

	void createTexCoordsPlanar(std::vector<glm::vec2>& cpuTexCoordsPlanar);
	std::vector<glm::vec2> createTCP(int mapping_type, int entity_type);
	//void update

	std::vector<glm::vec4> vertices;
	std::vector<glm::vec4> normals;
	// newly added
	//std::vector<Mesh::Edge> edges;

	std::vector<glm::vec2> texCoords;
	std::vector<Mesh::Face> faces;
	glm::vec4 _dimensions = glm::vec4(0.0f), _center = glm::vec4(0.0f);

	glm::vec3 Ka = glm::vec3(0.0f);
	glm::vec3 Kd = glm::vec3(0.0f);;
	glm::vec3 Ks = glm::vec3(0.0f);;
	float Ns = 0.0f;


	enum { PLAN = 0, CUBE = 1,  CYLI= 2, SPHE = 3 };
};

//bool compareVector(const glm::vec3& lhs, const glm::vec3& rhs);