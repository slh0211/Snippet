/******************************************************************************/
/*!
\file   BSPTree.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS350
\par    Assignment 3 Octree and BSPTree
\par    Spring 2021
\date   4/02/2021
\brief
	This file contains the implementation of BSPTree functions

  Hours spent on this assignment:

  Specific portions that gave you the most trouble:

*/
/******************************************************************************/
#pragma once
#include "BSPTree.h"
/******************************************************************************/
/*!
  \brief
	This function BuildBSPTree recursively

  \param objects
	geometry in triangles

  \param numTriangles
	terminate the loop when current nodes' triangles less equal to numTriangles

  \param depth
    current depth
  
  \return pBSPNode
    newly create node
*/
/******************************************************************************/
BSPNode* BuildBSPTree(const std::vector<Object>& objects, int numTriangles, int depth)
{
	// Return NULL tree if there are no polygons
	if(objects.empty())
		return nullptr;

	// Get number of polygons in the input vector
	int numObjects = objects.size();

	int MAX_DEPTH = 8;

	// If criterion for a leaf is matched, create a leaf node from remaining polygons
	if (depth == MAX_DEPTH || numObjects <= numTriangles)
	{
		BSPNode* pBSPNode = new BSPNode(objects);
		// repack objects into one big chunk list for drawing
		pBSPNode->RepackVertices();
		//pBSPNode->_objList.clear();

		return pBSPNode;
	}

	// Select best possible partitioning plane based on the input geometry
	Plane splitPlane = PickSplittingPlane(objects);

	std::vector<Object> frontList;
	std::vector<Object> backList;

	// Test each polygon against the dividing plane, adding them
	// to the front list, back lit, or both, as appropriate
	for (int i = 0; i < numObjects; ++i)
	{
		Object poly = objects[i];
		std::vector<glm::vec3> frontPart;
		std::vector<glm::vec3> backPart;

		switch (ClassifyObjectToPlane(poly, splitPlane))
		{
		case Object_Classification::OBJECT_COPLANAR_WITH_PLANE:
			// What's done in this case depends on what type of tree is being
			// built. For a node-storing tree, the polygon is stored inside
			// the node at this level (along with all other polygons coplanar
			// with the plane). Here, for a leaf-storing tree, coplanar polygons
			// are sent to either side of the plane. In this case, to the front
			// side, by falling through to the next case
		case Object_Classification::OBJECT_IN_FRONT_OF_PLANE:
			frontList.push_back(poly);
			break;
		case Object_Classification::OBJECT_BEHIND_PLANE:
			backList.push_back(poly);
			break;
		case Object_Classification::OBJECT_STRADDLING_PLANE:
			// Split polygon to plane and send a part to each side of the plane
			SplitObject(poly, splitPlane, frontPart, backPart);

			// Split frontPart and backPart's output
			if (frontPart.size() == 4)
			{
				Object o0;
				Object o1;
				SplitPolytoObject(frontPart, o0, o1);
				frontList.push_back(o0);
				frontList.push_back(o1);
			}
			else if (frontPart.size() == 3)
			{
				Object o0 = Object(frontPart[0], frontPart[1], frontPart[2]);
				frontList.push_back(o0);
			}
			// ignore if a poly is less than 3
			
			if (backPart.size() == 4)
			{
				Object o0;
				Object o1;
				SplitPolytoObject(backPart, o0, o1);
				backList.push_back(o0);
				backList.push_back(o1);
			}
			else if (backPart.size() == 3)
			{
				Object o0 = Object(backPart[0], backPart[1], backPart[2]);
				backList.push_back(o0);
			}
			// ignore if a poly is less than 3
			break;
		}
	}
	// Recursively build child subtrees and return new tree root combining them
	BSPNode* frontTree = BuildBSPTree(frontList, numTriangles, depth + 1);
	BSPNode* backTree = BuildBSPTree(backList, numTriangles, depth +1);
	
	BSPNode* pBSPNode = new BSPNode(frontTree, backTree, splitPlane);

	return pBSPNode;

}
/******************************************************************************/
/*!
  \brief
	Delete BSPTree recursively

  \param leaf
	current node
*/
/******************************************************************************/
void DeleteBSPTree(BSPNode* leaf)
{
	if (leaf != NULL)
	{
		DeleteBSPTree(leaf->_leftTree);
		DeleteBSPTree(leaf->_rightTree);

		delete leaf;
	}
}
