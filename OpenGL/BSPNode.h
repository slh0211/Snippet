/******************************************************************************/
/*!
\file   BSPNode.h
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS350
\par    Assignment 3 Octree and BSPTree
\par    Spring 2021
\date   4/02/2021
\brief
	This file contains the prototype of BSPNode class and related functions

  Hours spent on this assignment:

  Specific portions that gave you the most trouble:

*/
/******************************************************************************/
#pragma once
#include <glm/glm.hpp> // glm
#include "Object.h" // Object class
#include <vector>   // std::vector
#include "Geometry.h" // Plane class
/******************************************************************************/
/*!
  \class BSPNode
  \brief
	This class defines BSPNode for building BSPTree
*/
/******************************************************************************/
struct BSPNode
{
	BSPNode* _leftTree;
	BSPNode* _rightTree;

	Plane _splitPlane;

	std::vector<Object> _objList;
	std::vector<glm::vec4> _vertices;

	BSPNode(const std::vector<Object>& objList);
	BSPNode(BSPNode* frontTree, BSPNode* backTree, const Plane& splitPlane);
	void RepackVertices();
};