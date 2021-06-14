/******************************************************************************/
/*!
\file   Node.h
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS350
\par    Assignment 3 Octree and BSPTree
\par    Spring 2021
\date   3/30/2021
\brief
	This file contains the prototype of Node class and related functions

  Hours spent on this assignment:

  Specific portions that gave you the most trouble:

*/
/******************************************************************************/
#pragma once
#include <glm/glm.hpp> // glme
#include "Object.h" // Object
#include <vector>  // std::vector
#include "Geometry.h" // Aabb
/******************************************************************************/
/*!
  \struct Node
  \brief
	The struct is a Node used to build an Octree
*/
/******************************************************************************/
struct Node {
	glm::vec4 _center;
	float _halfWidth;
	Node* _pChild[8];
	//Object* _pObjList;
	std::vector<Object> _objList;
	std::vector<glm::vec4> _vertices;

	// for drawing
	glm::mat4 _modelMatrix;

	void RepackVertices();
	Aabb GetAabb() const;
	bool CheckLeafNode() const;
};