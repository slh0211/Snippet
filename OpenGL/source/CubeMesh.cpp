/******************************************************************************/
/*!
\file   CubeMesh.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS250
\par    Assignment 2 MeshCube
\par    Summer 2020
\date   5/16/2020
\brief  
    This file contains the implementation of CubeMesh functions

  Hours spent on this assignment: 2

  Specific portions that gave you the most trouble:
    
*/
/******************************************************************************/
#include "Affine.h" // point, vector
#include "CubeMesh.h"

const glm::vec4 cs250::CubeMesh::vertices[] = {{1.0f, 1.0f, 1.0f, 1.0f}, 
                                               {1.0f, 1.0f, -1.0f, 1.0f},
                                               {1.0f, -1.0f, 1.0f, 1.0f},
                                               {1.0f, -1.0f, -1.0f, 1.0f},
                                               {-1.0f, 1.0f, 1.0f, 1.0f},
                                               {-1.0f, 1.0f, -1.0f, 1.0f},
                                               {-1.0f, -1.0f, 1.0f, 1.0f},
                                               {-1.0f, -1.0f, -1.0f, 1.0f}};

const cs250::CubeMesh::Face cs250::CubeMesh::faces[] = {{0, 3, 1},
                                                        {0, 2, 3},
                                                        {0, 4, 6},
                                                        {0, 6, 2},
                                                        {1, 4, 0},
                                                        {1, 5, 4},
                                                        {3, 6, 7},
                                                        {3, 2, 6},
                                                        {4, 5, 7},
                                                        {4, 7, 6},
                                                        {1, 7, 5},
                                                        {1, 3, 7}};


const cs250::CubeMesh::Edge cs250::CubeMesh::edges[] = {{0, 1},
                                                        {0, 2},
                                                        {0, 4},
                                                        {1, 3},
                                                        {1, 5},
                                                        {2, 3},
                                                        {2, 6},
                                                        {3, 7},
                                                        {4, 5},
                                                        {4, 6},
                                                        {5, 7},
                                                        {6, 7}};
/******************************************************************************/
/*!
  \brief
    returns the number of vertices in the vertex array of the mesh

  \return 8
    hard coded number for cube
*/
/******************************************************************************/
int cs250::CubeMesh::vertexCount(void) const
{
  return 8;
}
/******************************************************************************/
/*!
  \brief
    returns a pointer to the vertex array of the mesh. The vertices are given
    in object coordinates

  \return vertices
    vertices of the mesh
*/
/******************************************************************************/
const glm::vec4* cs250::CubeMesh::vertexArray(void) const
{
  return vertices;
}
/******************************************************************************/
/*!
  \brief
    returns the vector x, y, z that gives the dimensions of the (tight)
    axis–aligned bounding box in object space that contains the mesh

  \return vector
    vec4 contains the dimension of the mesh
*/
/******************************************************************************/
glm::vec4 cs250::CubeMesh::dimensions(void) const
{
  return vector(2.0f,2.0f,2.0f);
}
/******************************************************************************/
/*!
  \brief
    returns the center (Cx, Cy, Cz) of the axis–aligned bounding box in object
    space that contains the mesh

  \return point
    vec4 contains the center of the mesh
*/
/******************************************************************************/
glm::vec4 cs250::CubeMesh::center(void) const
{
  return point(0.0f,0.0f,0.0f);
}
/******************************************************************************/
/*!
  \brief
    returns the number of triangular faces in the mesh

  \return 12
    hard coded face count for cube mesh
*/
/******************************************************************************/
int cs250::CubeMesh::faceCount(void) const
{
  return 12;
}
/******************************************************************************/
/*!
  \brief
    returns a pointer to the face array of the mesh. 
    Faces should be counterclockwise oriented

  \return faces
    array contains faces of the mesh
*/
/******************************************************************************/
const cs250::Mesh::Face* cs250::CubeMesh::faceArray(void) const
{
  return faces;
}
/******************************************************************************/
/*!
  \brief
    returns the number of edges in the mesh

  \return 12
    hard coded edge count for cube mesh
*/
/******************************************************************************/
int cs250::CubeMesh::edgeCount(void) const
{
  return 12;
}
/******************************************************************************/
/*!
  \brief
    returns a pointer to the edge array of the mesh

  \return edges
    array contains edges of the mesh
*/
/******************************************************************************/
const cs250::Mesh::Edge* cs250::CubeMesh::edgeArray(void) const
{
  return edges;
}