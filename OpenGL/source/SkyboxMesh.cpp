/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: SkyboxMesh.h
Purpose: Implematation for SkyboxMesh
Language: C++/glsl
Platform: visual studio
Project: r.shen_CS300_3
Author: Richard Shen, r.shen, 110000516
Creation date : 11/24/2020
End Header --------------------------------------------------------*/
/******************************************************************************/
/*!
\file   SkyboxMesh.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS300
\par    Assignment 3 Relection and Refraction
\par    Fall 2020
\date   11/24/2020
\brief
    This file contains the implementation of SkyboxMesh functions

  Hours spent on this assignment: 2

  Specific portions that gave you the most trouble:

*/
/******************************************************************************/
#include "Affine.h" // point, vector
#include "SkyboxMesh.h"

const glm::vec4 cs250::SkyboxMesh::vertices[] = { 
                                                  //left
                                                  {-1.0f, -1.0f,  1.0f, 1.0f},
                                                  {-1.0f, -1.0f, -1.0f, 1.0f},
                                                  {-1.0f,  1.0f, -1.0f, 1.0f},
                                                  {-1.0f,  1.0f,  1.0f, 1.0f},
                                                  //right
                                                  { 1.0f, -1.0f, -1.0f, 1.0f},
                                                  { 1.0f, -1.0f,  1.0f, 1.0f},
                                                  { 1.0f,  1.0f,  1.0f, 1.0f},
                                                  { 1.0f,  1.0f, -1.0f, 1.0f},
                                                  //bottom
                                                  {-1.0f,  -1.0f,  1.0f, 1.0f},
                                                  { 1.0f,  -1.0f,  1.0f, 1.0f},
                                                  { 1.0f,  -1.0f, -1.0f, 1.0f},
                                                  {-1.0f,  -1.0f, -1.0f, 1.0f},
                                                  //top
                                                  {-1.0f,  1.0f, -1.0f, 1.0f},
                                                  { 1.0f,  1.0f, -1.0f, 1.0f},
                                                  { 1.0f,  1.0f,  1.0f, 1.0f},
                                                  {-1.0f,  1.0f,  1.0f, 1.0f},
                                                  //back
                                                  {-1.0f, -1.0f, -1.0f, 1.0f},
                                                  { 1.0f, -1.0f, -1.0f, 1.0f},
                                                  { 1.0f,  1.0f, -1.0f, 1.0f},
                                                  {-1.0f,  1.0f, -1.0f, 1.0f},
                                                  //front
                                                  { 1.0f, -1.0f,  1.0f, 1.0f},
                                                  {-1.0f, -1.0f,  1.0f, 1.0f},
                                                  {-1.0f,  1.0f,  1.0f, 1.0f},
                                                  { 1.0f,  1.0f,  1.0f, 1.0f}


};


const glm::vec4 cs250::SkyboxMesh::normals[] = { 
                                                 //left
                                                 {0.0f, 0.0f, 0.0f, 0.0f},
                                                 {0.0f, 0.0f, 0.0f, 0.0f},
                                                 {0.0f, 0.0f, 0.0f, 0.0f},
                                                 {0.0f, 0.0f, 0.0f, 0.0f},
                                                 //right
                                                 {0.0f, 0.0f, 0.0f, 0.0f},
                                                 {0.0f, 0.0f, 0.0f, 0.0f},
                                                 {0.0f, 0.0f, 0.0f, 0.0f},
                                                 {0.0f, 0.0f, 0.0f, 0.0f},
                                                 //bottom
                                                 {0.0f, 0.0f, 0.0f, 0.0f},
                                                 {0.0f, 0.0f, 0.0f, 0.0f},
                                                 {0.0f, 0.0f, 0.0f, 0.0f},
                                                 {0.0f, 0.0f, 0.0f, 0.0f},
                                                 //top
                                                 {0.0f, 0.0f, 0.0f, 0.0f},
                                                 {0.0f, 0.0f, 0.0f, 0.0f},
                                                 {0.0f, 0.0f, 0.0f, 0.0f},
                                                 {0.0f, 0.0f, 0.0f, 0.0f},
                                                 //back
                                                 {0.0f, 0.0f, 0.0f, 0.0f},
                                                 {0.0f, 0.0f, 0.0f, 0.0f},
                                                 {0.0f, 0.0f, 0.0f, 0.0f},
                                                 {0.0f, 0.0f, 0.0f, 0.0f},
                                                 //front
                                                 {0.0f, 0.0f, 0.0f, 0.0f},
                                                 {0.0f, 0.0f, 0.0f, 0.0f},
                                                 {0.0f, 0.0f, 0.0f, 0.0f},
                                                 {0.0f, 0.0f, 0.0f, 0.0f}
};

const glm::vec2 cs250::SkyboxMesh::texCoords[] = { 
                                                   //left
                                                   {0.0f, 0.0f},
                                                   {1.0f, 0.0f},
                                                   {1.0f, 1.0f},
                                                   {0.0f, 1.0f},
                                                   //right
                                                   {0.0f, 0.0f},
                                                   {1.0f, 0.0f},
                                                   {1.0f, 1.0f},
                                                   {0.0f, 1.0f},
                                                   //bottom
                                                   {0.0f, 0.0f},
                                                   {1.0f, 0.0f},
                                                   {1.0f, 1.0f},
                                                   {0.0f, 1.0f},
                                                   //top
                                                   {0.0f, 0.0f},
                                                   {1.0f, 0.0f},
                                                   {1.0f, 1.0f},
                                                   {0.0f, 1.0f},
                                                   //back
                                                   {0.0f, 0.0f},
                                                   {1.0f, 0.0f},
                                                   {1.0f, 1.0f},
                                                   {0.0f, 1.0f},
                                                   //front
                                                   {0.0f, 0.0f},
                                                   {1.0f, 0.0f},
                                                   {1.0f, 1.0f},
                                                   {0.0f, 1.0f}
};


const cs250::SkyboxMesh::Face cs250::SkyboxMesh::faces[] = { 
    //left
{0, 1, 2},
{0, 2, 3},
//right
{4, 5, 6},
{4, 6, 7},
//bottom
{8, 9, 10},
{8, 10, 11},
//top
{12, 13, 14},
{12, 14, 15},
//back
{16, 17, 18},
{16, 18, 19},


                                                             //front
                                                             {20, 21, 22},
                                                             {20, 22, 23},



};

const cs250::SkyboxMesh::Edge cs250::SkyboxMesh::edges[] = { //back
                                                             {0, 1},
                                                             {1, 2},
                                                             {2, 3},
                                                             {0, 3},
                                                             //right
                                                             {4, 5},
                                                             {5, 6},
                                                             {6, 7},
                                                             //front
                                                             {8, 9},
                                                             {9, 10},
                                                             {10, 11},
                                                             //left
                                                             {12, 13},
                                                             {14, 15}
};
/******************************************************************************/
/*!
  \brief
    returns the number of vertices in the vertex array of the mesh

  \return 24
    hard coded number for Skybox
*/
/******************************************************************************/
int cs250::SkyboxMesh::vertexCount(void) const
{
    return 24;
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
const glm::vec4* cs250::SkyboxMesh::vertexArray(void) const
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
glm::vec4 cs250::SkyboxMesh::dimensions(void) const
{
    return vector(2.0f, 2.0f, 2.0f);
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
glm::vec4 cs250::SkyboxMesh::center(void) const
{
    return point(0.0f, 0.0f, 0.0f);
}
/******************************************************************************/
/*!
  \brief
    returns the number of triangular faces in the mesh

  \return 12
    hard coded face count for Skybox mesh
*/
/******************************************************************************/
int cs250::SkyboxMesh::faceCount(void) const
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
const cs250::Mesh::Face* cs250::SkyboxMesh::faceArray(void) const
{
    return faces;
}
/******************************************************************************/
/*!
  \brief
    returns the number of edges in the mesh

  \return 12
    hard coded edge count for Skybox mesh
*/
/******************************************************************************/
int cs250::SkyboxMesh::edgeCount(void) const
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
const cs250::Mesh::Edge* cs250::SkyboxMesh::edgeArray(void) const
{
    return edges;
}
/******************************************************************************/
/*!
  \brief
    returns a pointer to the normals array of the mesh

  \return normals
    array contains normals of the mesh
*/
/******************************************************************************/
const glm::vec4* cs250::SkyboxMesh::normalArray(void) const
{
    return normals;
}
/******************************************************************************/
/*!
  \brief
    returns a pointer to the texture coordinates array of the mesh

  \return texCoords
    array contains texture coordinates of the mesh
*/
/******************************************************************************/
const glm::vec2* cs250::SkyboxMesh::texCoordArray(void) const
{
    return texCoords;
}


