/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: SphereMesh.h
Purpose: Prototype for a SphereMesh class
Language: C++
Platform: visual studio
Project: r.shen_CS250_2
Author: Richard Shen, r.shen, 110000516
Creation date: 5/19/2020
End Header --------------------------------------------------------*/
// SphereMesh.h
// -- 3D trianglur mesh for spheres
// cs250 5/19

#ifndef CS250_SPHEREMESH_H
#define CS250_SPHEREMESH_H


#include <vector>
#include "Mesh.h"


namespace cs250 {

  class SphereMesh : public NormalMesh {
    public:
      SphereMesh(int mesh_size);
      int vertexCount(void) const override;
      const glm::vec4* vertexArray(void) const override;
      glm::vec4 dimensions(void) const override;
      glm::vec4 center(void) const override;
      int faceCount(void) const override;
      const Face* faceArray(void) const override;
      int edgeCount(void) const override;
      const Edge* edgeArray(void) const override;
      const glm::vec4* normalArray(void) const override;

      // newly added
      const glm::vec2* texCoordArray(void) const override;
    private:
        std::vector<glm::vec4> vertices,
                                normals;
        // newly added
        std::vector<glm::vec2>  texCoords;
      std::vector<Face> faces;
      std::vector<Edge> edges;
  };

}


#endif

