/******************************************************************************/
/*!
\file   PhongRender.h
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS250
\par    Assignment 7 Phong Render
\par    Summer 2020
\date   6/29/2020
\brief
    This file contains the prototype of PhongRender class

  Hours spent on this assignment: 3

  Specific portions that gave you the most trouble:

*/
/******************************************************************************/
// PhongRender.h
// -- engine for rendering solid meshes using Phong shading
//    with multiple meshes and light sources
// cs250 10/19

#ifndef CS250_PHONGRENDER_H
#define CS250_PHONGRENDER_H


#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <string>
#include "Mesh.h"
#include "Camera.h"
#include "Affine.h"
#include "Object.h"



namespace cs250 {
/******************************************************************************/
/*!
  \class PhongRender
  \brief  
    encapsulated class to manage glsl functionality
*/
/******************************************************************************/
  class PhongRender {
    public:
      enum class shader_type { PHONGLIGHTING = 0, PHONGSHADING = 1, BLINNSHADING = 2, TRANSPARENT = 3 };
      enum { POINT = 0, DIRECTIONAL = 1, SPOT = 2 };

      PhongRender(void);
      ~PhongRender(void);
      static void backfaceCull(bool yes=true);
      static void clear(const glm::vec4 &color);
      int loadMesh(const cs250::NormalMesh &m);



      void unloadMesh(int mi);
      void setModel(const glm::mat4 &M);
      void setCamera(const cs250::Camera &cam);
      void setMaterial(const glm::vec3 &ambi_coef,
                       const glm::vec3 &diff_coef,
                       const glm::vec3 &spec_coef,
                       float spec_exp);
      void setLight(int li, const glm::vec4 &position,
                            const glm::vec3 &color);

      void setDirectionalLight(int li, const glm::vec4 &direction,
                               const glm::vec3 &color);

      void setSpotLight(int li, const glm::vec4& position, const glm::vec4& direction,
          const glm::vec3& color, float theta, float phi, float fallout);

      void setLightInUse(int li, int use);

      void setAmbient(const glm::vec3 &color);

      void setAttenauation(float constant_att, float linear_att, float quadratic_att);

      void setAtomsphericAttenauation(float z_far, float z_near, const glm::vec3& fog_color);

      void setIlluminatorColor(const glm::vec3& color);
      void setWireframeColor(const glm::vec3& color);
      void setWireframeTriangleColor(const glm::vec3& color);

      void setTransparentIndex(float eta_in, float eta_out, float epsilon);

      GLuint loadTexture(const std::string& path);
      std::vector<GLuint> loadSkyboxTexture(const std::vector<std::string>& facePaths);
      

      void draw(int mi);
      void drawNormal(int mi);
      void drawFaceNormal(int mi);
      void drawWireframe(int mi);

      void drawWireframeCube();
      void drawWireframeTriangle(const Object& object) const;
      void drawWireframeTriangles(const std::vector<glm::vec4>& vertices) const;

      void drawSkybox(int mi);

      void drawTexture(int mi, int gpu_use, int entity_type, int mapping_type);

      void drawIlluminator(int mi);

      void reloadShader(int shader);
      void reloadTexCoords(int mi, const std::vector<glm::vec2>& texCoords);


      bool SetUpFBO(void);

      bool SetUpGbufferFBO(int viewportWidth, int viewportHeight);
      void SetUpFSQBuffer(void);

      void UseFBO(int FBO_Handle, int viewportWidth, int viewportHeight);
      //void drawFBOTexture(int mi, int gpu_use, int entity_type, int mapping_type, int slot);
      void drawFBOTexture(int mi, int transparent_type, bool shading_use);

      GLuint getFBOHandle(void) const;
      GLuint getGBufferFBOHandle(void) const;

      void drawFSQ(int mode);
      void drawToGBuffer(int mi);
      void resizeGBuffer(int viewportWidth, int viewportHeight);

      enum shader_type getShaderType(void) const;
    private:
        GLuint program,
            nProgram,
            fnProgram,
            iProgram,
            skbProgram,
            fsqProgram,
            gBufferProgram,
            wProgram,
            worldProgram;

        //GLuint testProgram;
/******************************************************************************/
/*!
  \struct MeshData
  
  \brief
    struct to store mesh related data on cpu
*/
/******************************************************************************/
      struct MeshData {
        enum { VERT=0, NORM=1, FACE=2, TEXT = 3 };
        GLuint vertex_array_buffer,
               buffer_objects[4];
        int face_count;
        //int edge_count;
        glm::vec3 dim;
        glm::vec3 cen;
      };

      std::vector<MeshData> mesh_data;

      GLuint diffTexID;
      GLuint specTexID;

      std::vector<GLuint> skyboxTexID;
      GLuint  renderedTexture0, renderedTexture1, renderedTexture2, renderedTexture3, renderedTexture4, renderedTexture5;

      GLuint  FBOHandle;
      GLuint  GBufferFBOHandle;

      GLuint  gPosition, gNormal, gDiffuse, gSpecularShininess, gAmbient;
      GLuint  rboDepth;

      enum class shader_type stype;

      std::vector<float> FSQBuffer;
      GLuint FSQVAO;
      GLuint FSQVBO;

      GLuint cubeVertexBufferHandle;
      GLuint cubeEdgeBufferHandle;
      GLuint cubeVAOHandle;

      void setUpWireframeCube();
  };

}
#endif

