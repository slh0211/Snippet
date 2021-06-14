/******************************************************************************/
/*!
\file   BSPNode.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS350
\par    Assignment 3 Octree and BSPTree
\par    Spring 2021
\date   4/02/2021
\brief
	This file contains the implementation of BSPNode class and related functions

  Hours spent on this assignment:

  Specific portions that gave you the most trouble:

*/
/******************************************************************************/
#pragma once
#include "BSPNode.h"
/******************************************************************************/
/*!
  \brief
	ctor for BSPNode class

  \param objList
	a list of Object
*/
/******************************************************************************/
BSPNode::BSPNode(const std::vector<Object>& objList) : _leftTree(NULL), _rightTree(NULL), _splitPlane(), _objList(objList)
{
}
/******************************************************************************/
/*!
  \brief
	ctor for BSPNode class

  \param frontTree
	precreated leftTree of the new node

  \param backTree
    precretaed rightTree of the new node

  \param splitPlane
	the dividing plane of the new node
*/
/******************************************************************************/
BSPNode::BSPNode(BSPNode* frontTree, BSPNode* backTree, const Plane& splitPlane) : _leftTree(frontTree), _rightTree(backTree), _splitPlane(splitPlane)
{
}
/******************************************************************************/
/*!
  \brief
	This function repacks vertices in each Object into a giant chunk
	of vertices for rendering
*/
/******************************************************************************/
void BSPNode::RepackVertices()
{
	for (unsigned int i = 0; i < _objList.size(); ++i)
	{
		_vertices.push_back(_objList[i]._points[0]);
		_vertices.push_back(_objList[i]._points[1]);
		_vertices.push_back(_objList[i]._points[2]);
	}
}
