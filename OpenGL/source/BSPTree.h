/******************************************************************************/
/*!
\file   BSPTree.h
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS350
\par    Assignment 3 Octree and BSPTree
\par    Spring 2021
\date   4/02/2021
\brief
	This file contains the prototype of BSPTree functions

  Hours spent on this assignment:

  Specific portions that gave you the most trouble:

*/
/******************************************************************************/
#pragma once
#include "BSPNode.h" // BSPNode

BSPNode* BuildBSPTree(const std::vector<Object>& objects, int numTriangles, int depth);
void DeleteBSPTree(BSPNode* leaf);