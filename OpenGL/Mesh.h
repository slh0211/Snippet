/******************************************************************************/
/*!
\file   Mesh.h
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS250
\par    Assignment 2 MeshCube
\par    Summer 2020
\date   5/16/2020
\brief
    This file contains the prototype of Mesh class

  Hours spent on this assignment: 2

  Specific portions that gave you the most trouble:

*/
/******************************************************************************/
// Mesh.h
// -- 3D triangular mesh interfaces
// cs250 5/19

#ifndef CS250_MESH_H
#define CS250_MESH_H


#include <glm/glm.hpp>


namespace cs250 {
/******************************************************************************/
/*!
  \struct Mesh
  \brief  
    The virtual base struct of Mesh
*/
/******************************************************************************/
  struct Mesh {
/******************************************************************************/
/*!
  \struct Face
  \brief  
    a triangle Face contains 3 index to hold the index to be used to draw
    triangle from a vertices array
*/
/******************************************************************************/
    struct Face {
      int index1, index2, index3;
      Face(int i=-1, int j=-1, int k=-1)
          : index1(i), index2(j), index3(k) {}
    };
/******************************************************************************/
/*!
  \struct Edge
  \brief  
    an Edge contains 2 index to hold the index to be used to draw lines from
    a vertices array
*/
/******************************************************************************/
    struct Edge {
      int index1, index2;
      Edge(int i=-1, int j=-1)
          : index1(i), index2(j) {}
    };
    // destructor
    virtual ~Mesh(void) {}
    // return vertex count
    virtual int vertexCount(void) const = 0;
    // return vertex array
    virtual const glm::vec4* vertexArray(void) const = 0;
    // return dimensions
    virtual glm::vec4 dimensions(void) const = 0;
    // return center
    virtual glm::vec4 center(void) const = 0;
    // return face count
    virtual int faceCount(void) const = 0;
    // return face array
    virtual const Face* faceArray(void) const = 0;
    // return edge count
    virtual int edgeCount(void) const = 0;
    // return edge array
    virtual const Edge* edgeArray(void) const = 0;
  };

/******************************************************************************/
/*!
  \struct NormalMesh
  
  \brief  
    derived from Mesh class, a NormalMesh contains normalArray and texCoordArray
*/
/******************************************************************************/
  struct NormalMesh : Mesh {
    virtual const glm::vec4* normalArray(void) const = 0;
    virtual const glm::vec2* texCoordArray(void) const = 0;
  };

  // create a NormalMesh using given Mesh
  NormalMesh* createFlatShadedMesh(const Mesh &m);

}


#endif

