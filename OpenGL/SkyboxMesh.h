/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: SkyboxMesh.h
Purpose: Prototype for SkyboxMesh
Language: C++/glsl
Platform: visual studio
Project: r.shen_CS300_3
Author: Richard Shen, r.shen, 110000516
Creation date : 11/24/2020
End Header --------------------------------------------------------*/
/******************************************************************************/
/*!
\file   SkyboxMesh.h
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS300
\par    Assignment 3 Relection and Refraction
\par    Fall 2020
\date   11/24/2020
\brief
    This file contains the prototype of SkyboxMesh functions

  Hours spent on this assignment: 2

  Specific portions that gave you the most trouble:

*/
/******************************************************************************/
#include <vector>
#include "Mesh.h"


namespace cs250 {
/******************************************************************************/
/*!
  \class SkyboxMesh
  \brief  
    The derived SkyboxMesh class from Mesh
*/
/******************************************************************************/
    class SkyboxMesh : public NormalMesh {
    public:
        int vertexCount(void) const override;
        const glm::vec4* vertexArray(void) const override;
        glm::vec4 dimensions(void) const override;
        glm::vec4 center(void) const override;
        int faceCount(void) const override;
        const Face* faceArray(void) const override;
        int edgeCount(void) const override;
        const Edge* edgeArray(void) const override;
        const glm::vec4* normalArray(void) const override;
        const glm::vec2* texCoordArray(void) const override;
    private:

        static const glm::vec4 vertices[24];
        static const glm::vec4 normals[24];
        static const glm::vec2 texCoords[24];
        static const Face faces[12];
        static const Edge edges[12];
    };

}