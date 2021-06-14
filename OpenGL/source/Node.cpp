/******************************************************************************/
/*!
\file   Node.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS350
\par    Assignment 3 Octree and BSPTree
\par    Spring 2021
\date   3/30/2021
\brief
	This file contains the implementation of Node class and related functions

  Hours spent on this assignment:

  Specific portions that gave you the most trouble:

*/
/******************************************************************************/
#include "Node.h"
/******************************************************************************/
/*!
  \brief
	This function repacks vertices in each Object into a giant chunk
	of vertices for rendering
*/
/******************************************************************************/
void Node::RepackVertices()
{
	for (unsigned int i = 0; i < _objList.size(); ++i)
	{
		_vertices.push_back(_objList[i]._points[0]);
		_vertices.push_back(_objList[i]._points[1]);
		_vertices.push_back(_objList[i]._points[2]);
	}
}
/******************************************************************************/
/*!
  \brief
	This function returns Aabb for collision detection

  \return Aabb
    An Aabb generated to use for collision detection
*/
/******************************************************************************/
Aabb Node::GetAabb() const
{
	return Aabb(_center, _halfWidth);
}
/******************************************************************************/
/*!
  \brief
	This function check if this Node has any children to determine
	if it's a leaf node

  \return
    true if leaf node else false
*/
/******************************************************************************/
bool Node::CheckLeafNode() const
{
	//for (unsigned int i = 0; i < 8; ++i)
	//{
	//	if (_pChild[i] != NULL)
	//	{
	//		return false;
	//	}
	//}
	if (_pChild[0] != NULL)
		return false;
	if (_pChild[1] != NULL)
		return false;
	if (_pChild[2] != NULL)
		return false;
	if (_pChild[3] != NULL)
		return false;
	if (_pChild[4] != NULL)
		return false;
	if (_pChild[5] != NULL)
		return false;
	if (_pChild[6] != NULL)
		return false;
	if (_pChild[7] != NULL)
		return false;



	return true;
}
