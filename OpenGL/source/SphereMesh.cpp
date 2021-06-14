/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: SphereMesh.cpp
Purpose: Implementation for a SphereMesh class
Language: C++
Platform: visual studio
Project: r.shen_CS250_2
Author: Richard Shen, r.shen, 110000516
Creation date: 5/19/2020
End Header --------------------------------------------------------*/
// SphereMesh.cpp
// -- implementation of 3D sphere triangular mesh.
// cs250 5/19


#include <cmath>
#include "SphereMesh.h"
#include "Affine.h"
using namespace std;


const float PI = 4.0f * atan(1.0f);


#define M (2*size)
#define N (size)
#define NORTH (M*(N-1))
#define SOUTH (M*(N-1)+1)
cs250::SphereMesh::SphereMesh(int size)
    : vertices(M*(N-1)+2),
      normals(M*(N-1)+2), texCoords(M* (N - 1) + 2){
  for (int i=1; i < N; ++i) {
    float theta = PI*i/N;
    for (int j=0; j < M; ++j) {
      int index = M*(i-1)+j;
      float phi = 2*PI*j/M;
      normals[index].x = sin(theta)*cos(phi);
      normals[index].y = sin(theta)*sin(phi);
      normals[index].z = cos(theta);
      normals[index].w = 0;
    }
  }
  normals[NORTH] = glm::vec4(0.0f,0.0f,1.0f,0.0f);
  normals[SOUTH] = glm::vec4(0.0f,0.0f,-1.0f,0.0f);

  for (unsigned n=0; n < normals.size(); ++n) {
    vertices[n] = normals[n];
    vertices[n].w = 1;
    texCoords[n] = glm::vec2(0.0f, 0.0f);
  }

  for (int i=2; i < N; ++i) {
    for (int j=0; j < M; ++j) {
      Face face;
      int jp1 = (j+1)%M;
      face.index1 = M*(i-2) + j;
      face.index2 = M*(i-1) + jp1;
      face.index3 = M*(i-2) + jp1;
      faces.push_back(face);
      face.index2 = M*(i-1) + j;
      face.index3 = M*(i-1) + jp1;
      faces.push_back(face);
    }
  }
  for (int j=0; j < M; ++j) {
    Face face;
    int jp1 = (j+1)%M;
    face.index1 = j;
    face.index2 = jp1;
    face.index3 = NORTH;
    faces.push_back(face);
    face.index1 = M*(N-2) + j;
    face.index2 = SOUTH;
    face.index3 = M*(N-2) + jp1;
    faces.push_back(face);
  }

  for (int i=2; i < N; ++i) {
    for (int j=0; j < M; ++j) {
      Edge edge;
      int jp1 = (j+1)%M;
      edge.index1 = M*(i-2) + j;
      edge.index2 = M*(i-2) + jp1;
      edges.push_back(edge);
      edge.index2 = M*(i-1) + jp1;
      edges.push_back(edge);
      edge.index1 = M*(i-2) + jp1;
      edges.push_back(edge);
    }
  }
  for (int j=0; j < M; ++j) {
    Edge edge;
    int jp1 = (j+1)%M;
    edge.index1 = M*(N-2) + j;
    edge.index2 = M*(N-2) + jp1;
    edges.push_back(edge);
    edge.index2 = SOUTH;
    edges.push_back(edge);
    edge.index1 = j;
    edge.index2 = NORTH;
    edges.push_back(edge);
  }

}
#undef SOUTH
#undef NORTH
#undef N
#undef M


int cs250::SphereMesh::vertexCount(void) const {
  return vertices.size();
}


const glm::vec4* cs250::SphereMesh::vertexArray(void) const {
  return &(vertices[0]);
}


const glm::vec4* cs250::SphereMesh::normalArray(void) const {
  return &(normals[0]);
}


int cs250::SphereMesh::faceCount(void) const {
  return faces.size();
}


const cs250::Mesh::Face* cs250::SphereMesh::faceArray(void) const {
  return &(faces[0]);
}


glm::vec4 cs250::SphereMesh::dimensions(void) const {
  return vector(2,2,2);
}


glm::vec4 cs250::SphereMesh::center(void) const {
  return point(0,0,0);
}


int cs250::SphereMesh::edgeCount(void) const {
  return edges.size();
}


const cs250::Mesh::Edge* cs250::SphereMesh::edgeArray(void) const {
  return &edges[0];
}

const glm::vec2* cs250::SphereMesh::texCoordArray(void) const
{
    return &(texCoords[0]);
}
