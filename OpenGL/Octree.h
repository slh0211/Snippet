/******************************************************************************/
/*!
\file   Octree.h
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS350
\par    Assignment 3 Octree and BSPTree
\par    Spring 2021
\date   3/30/2021
\brief
	This file contains the prototype of Octree functions

  Hours spent on this assignment:

  Specific portions that gave you the most trouble:

*/
/******************************************************************************/
#pragma once
#include "Node.h" // Node

Node* BuildOctree(glm::vec4 center, float halfWidth, int stopDepth);
void DeleteOctree(Node* leaf);
// input verticeis need to be in world space
Node* BuildAdaptiveOctree(const glm::vec4& center, float halfWidth, const std::vector<Object>& objVector, unsigned int numTriangles);

/******************************************************************************/
/*!
  \struct OctreeChildInfo
  
  \brief
	The struct encapsulates Child Index, Straddle info used to 
	determine whether it's straddle or can be send to lower tree
*/
/******************************************************************************/
struct OctreeChildInfo
{
	int  childIndex;
	bool bStraddle;
	int axis;
	float dist;

	OctreeChildInfo(int i, bool bStra, int axisInfo, float distInfo);
};

OctreeChildInfo getChildIndex(Node* pNode, const Object& newObject);

Plane CreatePlane(const OctreeChildInfo& ocInfo);
