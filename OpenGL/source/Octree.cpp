#include "Octree.h"
#include "Affine.h" // cs250::translate, cs250::scale
#include <iostream> 

/******************************************************************************/
/*!
  \brief
	This function build an Octree recursively.
	Not used, only for prototype purpose

  \param center
	center of the current node

  \param halfWidth
	radius of the current node

  \param stopDepth
	Terminates the loop whe stopDepth reached below 0

  \return pNode
    Pointer to newly created Node
*/
/******************************************************************************/
Node* BuildOctree(glm::vec4 center, float halfWidth, int stopDepth)
{
	// Termination condition
	if (stopDepth < 0)
		return nullptr;

	// Contruct and fill in 'root' of this subtree
	Node* pNode = new Node;

	pNode->_center = center;
	pNode->_halfWidth = halfWidth;
	//pNode->_pObjList = NULL;

	// for drawing
	glm::mat4 transM = cs250::translate(pNode->_center);
	glm::mat4 scaleM = cs250::scale(pNode->_halfWidth);

	pNode->_modelMatrix = transM * scaleM;


	// Recursively construct the eight children of the subtree
	glm::vec4 offset;
	float step = halfWidth * 0.5f;

	for (int i = 0; i < 8; ++i)
	{
		offset.x = ((i & 1) ? step : -step);
		offset.y = ((i & 2) ? step : -step);
		offset.z = ((i & 4) ? step : -step);

		pNode->_pChild[i] = BuildOctree(center + offset, step, stopDepth - 1);
	}

	return pNode;
}
/******************************************************************************/
/*!
  \brief
	Delete Octree recursively

  \param leaf
	current node
*/
/******************************************************************************/
void DeleteOctree(Node* leaf)
{
	if (leaf != 0)
	{
		DeleteOctree(leaf->_pChild[0]);
		DeleteOctree(leaf->_pChild[1]);
		DeleteOctree(leaf->_pChild[2]);
		DeleteOctree(leaf->_pChild[3]);
		DeleteOctree(leaf->_pChild[4]);
		DeleteOctree(leaf->_pChild[5]);
		DeleteOctree(leaf->_pChild[6]);
		DeleteOctree(leaf->_pChild[7]);

		delete leaf;
	}
}
/******************************************************************************/
/*!
  \brief
	This function build an AdpativeOctree recursively.

  \param center
	center of the current node

  \param halfWidth
	halfWidth of the current node

  \param objVector
	A giant chunk vector contains all vertices in world space

  \param numTriangles
    Terminates the loop when the a current node's triangles
	is less equal to numTriangles

  \return pNode
	Pointer to newly created Node
*/
/******************************************************************************/
Node* BuildAdaptiveOctree(const glm::vec4& center, float halfWidth, const std::vector<Object>& objVector, unsigned int numTriangles)
{
	// Base Case 1
	// if no triangle, return null
	if (objVector.size() == 0)
		return NULL;
	// Base Case 2
	// if less than numTriangles
	if (objVector.size() <= numTriangles)
	{
		// create a node
		Node* pNode = new Node;
		// fill with the info
		pNode->_center = center;
		pNode->_halfWidth = halfWidth;
		pNode->_objList = objVector;

		// for drawing
		glm::mat4 transM = cs250::translate(pNode->_center);
		glm::mat4 scaleM = cs250::scale(pNode->_halfWidth);

		pNode->_modelMatrix = transM * scaleM;

		pNode->_pChild[0] = NULL;
		pNode->_pChild[1] = NULL;
		pNode->_pChild[2] = NULL;
		pNode->_pChild[3] = NULL;
		pNode->_pChild[4] = NULL;
		pNode->_pChild[5] = NULL;
		pNode->_pChild[6] = NULL;
		pNode->_pChild[7] = NULL;
		// Repack Vertices into one chunk for faster drawing
		pNode->RepackVertices();
		//pNode->_objList.clear();
		return pNode;
	}


	// else
	// construct and fill the root
	// go through the given objlist
	// divide into 8
	Node* pNode = new Node;

	pNode->_center = center;
	pNode->_halfWidth = halfWidth;
	//pNode->_pObjList = NULL;

	// for drawing
	glm::mat4 transM = cs250::translate(pNode->_center);
	glm::mat4 scaleM = cs250::scale(pNode->_halfWidth);

	pNode->_modelMatrix = transM * scaleM;

	glm::vec4 offset;
	float step = halfWidth * 0.5f;
	
	std::vector<std::vector<Object>> childObjectVectors(8);
	
	for (unsigned int objectIndex = 0; objectIndex < objVector.size(); ++objectIndex)
	{
		// go through the given objlist
		OctreeChildInfo ocInfo = getChildIndex(pNode, objVector[objectIndex]);
		// divide into 8
		if (!ocInfo.bStraddle)
		{
			// not straddle, goes to one of the eight children
			childObjectVectors[ocInfo.childIndex].push_back(objVector[objectIndex]);
		}
		else
		{
			// straddle, goes to parent or divide?
			pNode->_objList.push_back(objVector[objectIndex]);

			//// too many bugs! broken !
			//std::vector<glm::vec3> frontPoly;
			//std::vector<glm::vec3> backPoly;
			//
			//
			//Plane plane = CreatePlane(ocInfo);
			//
			//Object o = objVector[objectIndex];
			//
			//
			//SplitObject(objVector[objectIndex], plane, frontPoly, backPoly);
			//
			//
			//if (frontPoly.size() == 3)
			//{
			//	// create a new object
			//	Object ob = Object(frontPoly[0], frontPoly[1], frontPoly[2]);
			//	// 
			//	OctreeChildInfo oInfo = getChildIndex(pNode, ob);
			//	if (!oInfo.bStraddle)
			//	{
			//
			//		childObjectVectors[oInfo.childIndex].push_back(ob);
			//	}
			//	else
			//	{
			//		std::cout << "wa" << std::endl;
			//	}
			//}
			
			
			
		}
	}

	for (int i = 0; i < 8; ++i)
	{
		offset.x = ((i & 1) ? step : -step);
		offset.y = ((i & 2) ? step : -step);
		offset.z = ((i & 4) ? step : -step);

		pNode->_pChild[i] = BuildAdaptiveOctree(center + offset, step, childObjectVectors[i], numTriangles);
	}
	pNode->RepackVertices();
	//pNode->_objList.clear();
	return pNode;
}
/******************************************************************************/
/*!
  \brief
	This function determine whether an object is straddle and it's index of
	eight children

  \param pNode
	pointer to a Node
  \param newObject
	a (triangle) Object
  \return OctreeChildInfo
	info stuct containing index and straddle info
*/
/******************************************************************************/
OctreeChildInfo getChildIndex(Node* pNode, const Object& newObject)
{
	bool bStraddle = false;
	int index = 0, axinfo = 0;
	float dInfo = 0.0f;

	for (int axis = 0; axis < 3; ++axis)
	{
		float delta = newObject._center[axis] - pNode->_center[axis];
		if (abs(delta) <= newObject._radius)
		{
			bStraddle = true;
			// record axis info for splitting
			axinfo = axis;
			dInfo = pNode->_center[axis];
			break;
		}

		if (delta > 0.0f)
		{
			index |= (1 << axis); //ZYX
		}
	}



	return OctreeChildInfo(index, bStraddle, axinfo, dInfo);
}
/******************************************************************************/
/*!
  \brief
	This function creates an Plane to divide an triangle using given
	ocInfo

  \param ocInfo
	OctreeChildInfo from current node and an Object
  \return Plane
    x, y, or z plane
*/
/******************************************************************************/
Plane CreatePlane(const OctreeChildInfo& ocInfo)
{
	glm::vec3 normal;

	switch (ocInfo.axis)
	{
	case 0:
		normal = glm::vec3(1.0f, 0.0f, 0.0f);
		break;
	case 1:
		normal = glm::vec3(0.0f, 1.0f, 0.0f);
		break;
	case 2:
		normal = glm::vec3(0.0f, 0.0f, 1.0f);
		break;
	}

	return Plane(normal, ocInfo.dist);
}
/******************************************************************************/
/*!
  \brief
	Helper ctor

  \param i
	ChildIndex
  \param bStra
    Straddle info
  \param axisInfo
    Which axis caused straddle
  \param distInfo
    Distance from the axis to origin
*/
/******************************************************************************/
OctreeChildInfo::OctreeChildInfo(int i, bool bStra, int axisInfo, float distInfo) : childIndex(i), bStraddle(bStra), axis(axisInfo), dist(distInfo)
{
}
