/******************************************************************************/
/*!
\file   CubeMesh.h
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS250
\par    Assignment 2 MeshCube
\par    Summer 2020
\date   5/16/2020
\brief
    This file contains the prototype of CubeMesh functions

  Hours spent on this assignment: 2

  Specific portions that gave you the most trouble:

*/
/******************************************************************************/
// CubeMesh.h
// -- standard cube
// cs250 5/19

#ifndef CS250_CUBEMESH_H
#define CS250_CUBEMESH_H

#include "Mesh.h"


namespace cs250 {
/******************************************************************************/
/*!
  \class CubeMesh
  \brief  
    The derived CubeMesh class from Mesh
*/
/******************************************************************************/
  class CubeMesh : public Mesh {
    public:
      // number of vertices
      int vertexCount(void) const override;
      // vertices array
      const glm::vec4* vertexArray(void) const override;
      // dimension of the mesh
      glm::vec4 dimensions(void) const override;
      // center of the mesh
      glm::vec4 center(void) const override;
      // number of faces
      int faceCount(void) const override;
      // faces array
      const Face* faceArray(void) const override;
      // number of edges
      int edgeCount(void) const override;
      // edges array
      const Edge* edgeArray(void) const override;
    private:
      // vertices in object space
      static const glm::vec4 vertices[8];
      // faces array
      static const Face faces[12];
      // edges array
      static const Edge edges[12];
  };

}


#endif

