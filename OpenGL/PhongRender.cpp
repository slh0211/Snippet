/******************************************************************************/
/*!
\file   PhongRender.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS250
\par    Assignment 7 Phong Render
\par    Summer 2020
\date   6/29/2020
\brief  
    This file contains the implementation of PhongRender class

  Hours spent on this assignment: 3

  Specific portions that gave you the most trouble:
    
*/
/******************************************************************************/
#include <stdexcept> // runtime_error
#include <iostream>  // cout, endl
#include <fstream>   // ifstream
#include <string>    // string
#include "PhongRender.h"
#include "shader.hpp"
#include "stb_image/stb_image.h"
#include "Affine.h"

/******************************************************************************/
/*!
  \brief
    creates a Phong illumination rendering object
*/
/******************************************************************************/
cs250::PhongRender::PhongRender(void)
{

  program = LoadShaders("res/shaders/PhongShading.vert", "res/shaders/PhongShading.frag");
  //program = LoadShaders("res/shaders/TransparentShader.vert", "res/shaders/TransparentShader.frag");
  //stype = shader_type::TRANSPARENT;
  stype = shader_type::PHONGSHADING;

  // turn on depth buffer
  glEnable(GL_DEPTH_TEST);
  
  // Shader to draw normal
  nProgram = LoadMultiShaders("res/shaders/NormalShader.vert", "res/shaders/NormalShader.frag",
      "res/shaders/NormalShader.geom");

  // Shader to draw Face Normal
  fnProgram = LoadMultiShaders("res/shaders/FaceNormalShader.vert", "res/shaders/NormalShader.frag",
      "res/shaders/FaceNormalShader.geom");

  // Shader to draw light sphere
  iProgram = LoadShaders("res/shaders/IlluminatorShader.vert", "res/shaders/IlluminatorShader.frag");

  // Texture for non-tranparent shader
  diffTexID = loadTexture("res/textures/metal_roof_diff_512x512.png");
  specTexID = loadTexture("res/textures/metal_roof_spec_512x512.png");

  // Shader to draw skybox
  skbProgram = LoadShaders("res/shaders/SkyboxShader.vert", "res/shaders/SkyboxShader.frag");

  // Texture for skybox
  std::vector<std::string> faces
  {
                "res/textures/left.jpg",
                "res/textures/right.jpg",
                "res/textures/bottom.jpg",
                "res/textures/top.jpg",
                "res/textures/back.jpg",
                "res/textures/front.jpg"
  };

  skyboxTexID = loadSkyboxTexture(faces);

  //testProgram = LoadShaders("res/shaders/TestShader.vert", "res/shaders/TestShader.frag");

  //fsqProgram = LoadShaders("res/shaders/FSQShader.vert", "res/shaders/FSQShader.frag");
  fsqProgram = LoadShaders("res/shaders/FSQShader.vert", "res/shaders/FSQLightingPassShader.frag");

  SetUpFSQBuffer();

  gBufferProgram = LoadShaders("res/shaders/GBufferShader.vert", "res/shaders/GBufferShader.frag");

  //wProgram = LoadShaders("res/shaders/IlluminatorShader.vert", "res/shaders/IlluminatorShader.frag");
  wProgram = LoadShaders("res/shaders/WireframeShader.vert", "res/shaders/WireframeShader.frag");

  setUpWireframeCube();

  worldProgram = LoadShaders("res/shaders/WorldShader.vert", "res/shaders/WorldShader.frag");
}

/******************************************************************************/
/*!
  \brief
    destroys a Phong illumination rendering object. All
    meshes that have not been explicitly unloaded from the GPU should be unloaded.
*/
/******************************************************************************/
cs250::PhongRender::~PhongRender(void)
{
  // check if there is any unloaded Mesh
  for(unsigned int i = 0; i < mesh_data.size(); ++i)
  {
    if(mesh_data[i].face_count != 0)
    {
      this->unloadMesh(i);
    }
  }

  glDeleteBuffers(1, &FSQVBO);
  glDeleteVertexArrays(1, &FSQVAO);

  glDeleteFramebuffers(1, &FBOHandle);
  glDeleteFramebuffers(1, &GBufferFBOHandle);

  glDeleteBuffers(1, &cubeVertexBufferHandle);
  glDeleteBuffers(1, &cubeEdgeBufferHandle);
  glDeleteVertexArrays(1, &cubeVAOHandle);


  glUseProgram(0);
  glDeleteProgram(program);
  glDeleteProgram(nProgram);
  glDeleteProgram(fnProgram);
  glDeleteProgram(iProgram);
  glDeleteProgram(skbProgram);
  glDeleteProgram(fsqProgram);
  glDeleteProgram(gBufferProgram);
  glDeleteProgram(wProgram);
  glDeleteProgram(worldProgram);
  //glDeleteProgram(testProgram);
}
/******************************************************************************/
/*!
  \brief
    if yes has the value of true, back-face culling is turned on. Otherwise, 
    it is turned off

  \param yes
    bool to determine enable or disable back face culling
*/
/******************************************************************************/
void cs250::PhongRender::backfaceCull(bool yes)
{
  // turn on back-face culling (disabled by default)
  if(yes)
  {
    glEnable(GL_CULL_FACE);
  }
  else
  {
    glDisable(GL_CULL_FACE);
  }
}
/******************************************************************************/
/*!
  \brief
    clears the frame buffer with the speciffed color, and clears the depth buffer
    with the default value of 1.

  \param color
    color to be used fot glClearColor
*/
/******************************************************************************/
void cs250::PhongRender::clear(const glm::vec4 &color)
{
  // clear frame buffer and depth buffer
  glClearColor(color.r,color.g,color.b,color.a);
  glClearDepth(1);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}
/******************************************************************************/
/*!
  \brief
    uploads the data of mesh m to the GPU

  \param m
    NormalMesh to be binded and uploaded to GPU

  \return mi
    The mesh index to be used by client
*/
/******************************************************************************/
int cs250::PhongRender::loadMesh(const NormalMesh &m)
{
  int mi = mesh_data.size();
  
  mesh_data.push_back(MeshData());
  
  // load sphere mesh (solid frame only)
  glGenBuffers(1,&mesh_data[mi].buffer_objects[MeshData::VERT]);
  glBindBuffer(GL_ARRAY_BUFFER,mesh_data[mi].buffer_objects[MeshData::VERT]);
  glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec4)*m.vertexCount(),
               m.vertexArray(),GL_STATIC_DRAW);
               
  glGenBuffers(1,&mesh_data[mi].buffer_objects[MeshData::NORM]);
  glBindBuffer(GL_ARRAY_BUFFER,mesh_data[mi].buffer_objects[MeshData::NORM]);
  glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec4)*m.vertexCount(),
               m.normalArray(),GL_STATIC_DRAW);
               
  // buggy?
  glGenBuffers(1, &mesh_data[mi].buffer_objects[MeshData::TEXT]);
  glBindBuffer(GL_ARRAY_BUFFER, mesh_data[mi].buffer_objects[MeshData::TEXT]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * m.vertexCount(),
      m.texCoordArray(), GL_STATIC_DRAW);

  glGenBuffers(1,&mesh_data[mi].buffer_objects[MeshData::FACE]);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mesh_data[mi].buffer_objects[MeshData::FACE]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(cs250::Mesh::Face)*m.faceCount(),
               m.faceArray(),GL_STATIC_DRAW);
  
  //glGenBuffers(1, &mesh_data[mi].buffer_objects[MeshData::EDGE]);
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_data[mi].buffer_objects[MeshData::EDGE]);
  //glBufferData(GL_ELEMENT_ARRAY_BUFFER,
  //    sizeof(cs250::Mesh::Edge) * m.edgeCount(),
  //    m.edgeArray(), GL_STATIC_DRAW);

  //mesh_data[mi].edge_count = m.edgeCount();


  mesh_data[mi].face_count = m.faceCount();
  mesh_data[mi].dim = m.dimensions();
  mesh_data[mi].cen = m.center();

  // save drawing states in VAO
  GLint aposition = glGetAttribLocation(program,"position"),
        anormal = glGetAttribLocation(program,"normal"),
        atexcoord = glGetAttribLocation(program,"aTexCoord");
  
  // create vertex array object
  glGenVertexArrays(1,&mesh_data[mi].vertex_array_buffer);
  
  // start state recording
  glUseProgram(program);
  
  glBindVertexArray(mesh_data[mi].vertex_array_buffer);
  
  glBindBuffer(GL_ARRAY_BUFFER,mesh_data[mi].buffer_objects[MeshData::VERT]);
  glVertexAttribPointer(aposition,4,GL_FLOAT,false,0,0);
  glEnableVertexAttribArray(aposition);
  
  glBindBuffer(GL_ARRAY_BUFFER,mesh_data[mi].buffer_objects[MeshData::NORM]);
  glVertexAttribPointer(anormal,4,GL_FLOAT,false,0,0);
  glEnableVertexAttribArray(anormal);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mesh_data[mi].buffer_objects[MeshData::FACE]);

  glBindBuffer(GL_ARRAY_BUFFER, mesh_data[mi].buffer_objects[MeshData::TEXT]);
  glVertexAttribPointer(atexcoord, 2, GL_FLOAT, false, 0, 0);
  glEnableVertexAttribArray(atexcoord);
  // stop state recording
  glBindVertexArray(0);

    aposition = glGetAttribLocation(nProgram, "aPos"),
      anormal = glGetAttribLocation(nProgram, "aNormal");
  
  
  glUseProgram(nProgram);
  glBindVertexArray(mesh_data[mi].vertex_array_buffer);
  
  glBindBuffer(GL_ARRAY_BUFFER, mesh_data[mi].buffer_objects[MeshData::VERT]);
  glVertexAttribPointer(aposition, 4, GL_FLOAT, false, 0, 0);
  glEnableVertexAttribArray(aposition);
  
  glBindBuffer(GL_ARRAY_BUFFER, mesh_data[mi].buffer_objects[MeshData::NORM]);
  glVertexAttribPointer(anormal, 4, GL_FLOAT, false, 0, 0);
  glEnableVertexAttribArray(anormal);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_data[mi].buffer_objects[MeshData::FACE]);
  // stop state recording
  glBindVertexArray(0);
  
  aposition = glGetAttribLocation(fnProgram, "aPos");
  
  
  glUseProgram(fnProgram);
  glBindVertexArray(mesh_data[mi].vertex_array_buffer);
  
  glBindBuffer(GL_ARRAY_BUFFER, mesh_data[mi].buffer_objects[MeshData::VERT]);
  glVertexAttribPointer(aposition, 4, GL_FLOAT, false, 0, 0);
  glEnableVertexAttribArray(aposition);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_data[mi].buffer_objects[MeshData::FACE]);
  // stop state recording
  glBindVertexArray(0);
  
  aposition = glGetAttribLocation(iProgram, "position");
  
  glUseProgram(iProgram);
  glBindVertexArray(mesh_data[mi].vertex_array_buffer);
  
  glBindBuffer(GL_ARRAY_BUFFER, mesh_data[mi].buffer_objects[MeshData::VERT]);
  glVertexAttribPointer(aposition, 4, GL_FLOAT, false, 0, 0);
  glEnableVertexAttribArray(aposition);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_data[mi].buffer_objects[MeshData::FACE]);
  // stop state recording
  glBindVertexArray(0);
  
  aposition = glGetAttribLocation(skbProgram, "aPos");
  atexcoord = glGetAttribLocation(skbProgram, "aTexCoord");
  glUseProgram(skbProgram);
  glBindVertexArray(mesh_data[mi].vertex_array_buffer);
  
  glBindBuffer(GL_ARRAY_BUFFER, mesh_data[mi].buffer_objects[MeshData::VERT]);
  glVertexAttribPointer(aposition, 4, GL_FLOAT, false, 0, 0);
  glEnableVertexAttribArray(aposition);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_data[mi].buffer_objects[MeshData::FACE]);
  
  glBindBuffer(GL_ARRAY_BUFFER, mesh_data[mi].buffer_objects[MeshData::TEXT]);
  glVertexAttribPointer(atexcoord, 2, GL_FLOAT, false, 0, 0);
  glEnableVertexAttribArray(atexcoord);
  
  glBindVertexArray(0);
  
  aposition = glGetAttribLocation(gBufferProgram, "aPos"),
      anormal = glGetAttribLocation(gBufferProgram, "aNormal"),
      atexcoord = glGetAttribLocation(gBufferProgram, "aTexCoords");
  
  // start state recording
  glUseProgram(gBufferProgram);
  
  glBindVertexArray(mesh_data[mi].vertex_array_buffer);
  
  glBindBuffer(GL_ARRAY_BUFFER, mesh_data[mi].buffer_objects[MeshData::VERT]);
  glVertexAttribPointer(aposition, 4, GL_FLOAT, false, 0, 0);
  glEnableVertexAttribArray(aposition);
  
  glBindBuffer(GL_ARRAY_BUFFER, mesh_data[mi].buffer_objects[MeshData::NORM]);
  glVertexAttribPointer(anormal, 4, GL_FLOAT, false, 0, 0);
  glEnableVertexAttribArray(anormal);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_data[mi].buffer_objects[MeshData::FACE]);
  
  glBindBuffer(GL_ARRAY_BUFFER, mesh_data[mi].buffer_objects[MeshData::TEXT]);
  glVertexAttribPointer(atexcoord, 2, GL_FLOAT, false, 0, 0);
  glEnableVertexAttribArray(atexcoord);
  // stop state recording
  glBindVertexArray(0);
  
  //aposition = glGetAttribLocation(testProgram, "position"),
  //    anormal = glGetAttribLocation(testProgram, "normal"),
  //    atexcoord = glGetAttribLocation(testProgram, "aTexCoord");
  
  //// start state recording
  //glUseProgram(testProgram);
  //
  //glBindVertexArray(mesh_data[mi].vertex_array_buffer);
  //
  //glBindBuffer(GL_ARRAY_BUFFER, mesh_data[mi].buffer_objects[MeshData::VERT]);
  //glVertexAttribPointer(aposition, 4, GL_FLOAT, false, 0, 0);
  //glEnableVertexAttribArray(aposition);
  //
  //glBindBuffer(GL_ARRAY_BUFFER, mesh_data[mi].buffer_objects[MeshData::NORM]);
  //glVertexAttribPointer(anormal, 4, GL_FLOAT, false, 0, 0);
  //glEnableVertexAttribArray(anormal);
  //
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_data[mi].buffer_objects[MeshData::FACE]);
  //
  //glBindBuffer(GL_ARRAY_BUFFER, mesh_data[mi].buffer_objects[MeshData::TEXT]);
  //glVertexAttribPointer(atexcoord, 2, GL_FLOAT, false, 0, 0);
  //glEnableVertexAttribArray(atexcoord);
  //// stop state recording
  //glBindVertexArray(0);
  aposition = glGetAttribLocation(wProgram, "position");
  
  glUseProgram(wProgram);
  glBindVertexArray(mesh_data[mi].vertex_array_buffer);
  
  glBindBuffer(GL_ARRAY_BUFFER, mesh_data[mi].buffer_objects[MeshData::VERT]);
  glVertexAttribPointer(aposition, 4, GL_FLOAT, false, 0, 0);
  glEnableVertexAttribArray(aposition);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_data[mi].buffer_objects[MeshData::FACE]);
  
  // stop state recording
  glBindVertexArray(0);

  return mi;
}
/******************************************************************************/
/*!
  \brief
    unloads the mesh at index mi (as returned by a call to loadMesh) from 
    the GPU.

  \param mi
    mesh index of the mesh to be unloaded
*/
/******************************************************************************/
void cs250::PhongRender::unloadMesh(int mi)
{
  glDeleteBuffers(4,mesh_data[mi].buffer_objects); 
  glDeleteVertexArrays(1,&mesh_data[mi].vertex_array_buffer);
  // setting face_count to 0 as not used mesh
  mesh_data[mi].face_count = 0;
}
/******************************************************************************/
/*!
  \brief
    sets the current modeling transformation to be used in rendering a mesh.
  
  \param M
    Model Matrix
*/
/******************************************************************************/
void cs250::PhongRender::setModel(const glm::mat4 &M)
{
  glUseProgram(program);
  //glUniformMatrix4fv(umodel_matrix,1,false,&M[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, false, &M[0][0]);

  glm::mat4 M_inverse = affineInverse(M);
  glm::mat3 L = glm::mat3(M_inverse);
  glm::mat4 N = glm::mat4(glm::transpose(L));
  
  //glUniformMatrix4fv(unormal_matrix,1,false,&N[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(program, "normal_matrix"), 1, false, &N[0][0]);

  glUseProgram(nProgram);
  //glUniformMatrix4fv(unmodel_matrix, 1, false, &M[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(nProgram, "model"), 1, false, &M[0][0]);

  glUseProgram(fnProgram);
  //glUniformMatrix4fv(ufnmodel_matrix, 1, false, &M[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(fnProgram, "model"), 1, false, &M[0][0]);

  glUseProgram(iProgram);
  //glUniformMatrix4fv(ufnmodel_matrix, 1, false, &M[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(iProgram, "model_matrix"), 1, false, &M[0][0]);

  glUseProgram(gBufferProgram);
  glUniformMatrix4fv(glGetUniformLocation(gBufferProgram, "model_matrix"), 1, false, &M[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(gBufferProgram, "normal_matrix"), 1, false, &N[0][0]);

  glUseProgram(wProgram);
  //glUniformMatrix4fv(ufnmodel_matrix, 1, false, &M[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(wProgram, "model_matrix"), 1, false, &M[0][0]);

  //glUseProgram(testProgram);
  //glUniformMatrix4fv(glGetUniformLocation(testProgram, "model_matrix"), 1, false, &M[0][0]);
  //glUniformMatrix4fv(glGetUniformLocation(testProgram, "normal_matrix"), 1, false, &N[0][0]);
}
/******************************************************************************/
/*!
  \brief
    sets the current camera that is to be used to render a mesh (this sets
    both the viewing and perspective transformations).

  \param cam
    Camera object
*/
/******************************************************************************/
void cs250::PhongRender::setCamera(const cs250::Camera &cam)
{
  glUseProgram(program);
  
  glm::vec4 eye = cam.eye();
  
  //glUniform4fv(ueye_position,1,&eye[0]);
  glUniform4fv(glGetUniformLocation(program, "eye_position"), 1, &eye[0]);
  
  glm::mat4 V = view(cam);
  //glUniformMatrix4fv(uview_matrix,1,false,&V[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(program, "view_matrix"), 1, false, &V[0][0]);
  
  glm::mat4 P = perspective(cam);
  //glUniformMatrix4fv(upersp_matrix,1,false,&P[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(program, "persp_matrix"), 1, false, &P[0][0]);

  glUseProgram(nProgram);
  //glUniformMatrix4fv(unview_matrix, 1, false, &V[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(nProgram, "view"), 1, false, &V[0][0]);
  //glUniformMatrix4fv(unpersp_matrix, 1, false, &P[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(nProgram, "projection"), 1, false, &P[0][0]);

  glUseProgram(fnProgram);
  //glUniformMatrix4fv(ufnview_matrix, 1, false, &V[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(fnProgram, "view"), 1, false, &V[0][0]);
  //glUniformMatrix4fv(ufnpersp_matrix, 1, false, &P[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(fnProgram, "projection"), 1, false, &P[0][0]);

  glUseProgram(iProgram);
  glUniformMatrix4fv(glGetUniformLocation(iProgram, "view_matrix"), 1, false, &V[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(iProgram, "persp_matrix"), 1, false, &P[0][0]);

  glUseProgram(skbProgram);
  // remove the translation part from view matrix to generate view matrix for skybox
  glm::mat4 skbView = glm::mat4(glm::mat3(V));

  glUniformMatrix4fv(glGetUniformLocation(skbProgram, "view"), 1, false, &skbView[0][0]);
  //glUniformMatrix4fv(ufnpersp_matrix, 1, false, &P[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(skbProgram, "projection"), 1, false, &P[0][0]);

  glUseProgram(gBufferProgram);
  glUniformMatrix4fv(glGetUniformLocation(gBufferProgram, "view_matrix"), 1, false, &V[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(gBufferProgram, "persp_matrix"), 1, false, &P[0][0]);

  glUseProgram(fsqProgram);
  glUniform4fv(glGetUniformLocation(fsqProgram, "eye_position"), 1, &eye[0]);
  glUniformMatrix4fv(glGetUniformLocation(fsqProgram, "view_matrix"), 1, false, &V[0][0]);

  glUseProgram(wProgram);
  glUniformMatrix4fv(glGetUniformLocation(wProgram, "view_matrix"), 1, false, &V[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(wProgram, "persp_matrix"), 1, false, &P[0][0]);

  glUseProgram(worldProgram);
  glUniformMatrix4fv(glGetUniformLocation(worldProgram, "view_matrix"), 1, false, &V[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(worldProgram, "persp_matrix"), 1, false, &P[0][0]);
  //glUseProgram(testProgram);
  ////glUniform4fv(ueye_position,1,&eye[0]);
  //glUniform4fv(glGetUniformLocation(testProgram, "eye_position"), 1, &eye[0]);
  //
  ////glUniformMatrix4fv(uview_matrix,1,false,&V[0][0]);
  //glUniformMatrix4fv(glGetUniformLocation(testProgram, "view_matrix"), 1, false, &V[0][0]);
  //
  ////glUniformMatrix4fv(upersp_matrix,1,false,&P[0][0]);
  //glUniformMatrix4fv(glGetUniformLocation(testProgram, "persp_matrix"), 1, false, &P[0][0]);
}
/******************************************************************************/
/*!
  \brief
    sets the current Phong material prop-erty parameters, where diff coef
    specifies the diffuse reection coeffcient, spec coef specifies the specular
    reflection coeffcient, and spec exp specifies the specular reflection exponent

  \param ambi_coef
    local ambient coeffcient from indiviual light source

  \param diff_coef
    diff coeffcient

  \param spec_coef
    spec coeffcient

  \param spec_exp
    spec reflection exponent
*/
/******************************************************************************/
void cs250::PhongRender::setMaterial(const glm::vec3 &ambi_coef,
                                     const glm::vec3 &diff_coef,
                                     const glm::vec3 &spec_coef,
                                     float spec_exp)
{
  glUseProgram(program);
  

  //glUniform3fv(udiffuse_coefficient,1,&diff_coef[0]);
  glUniform3fv(glGetUniformLocation(program, "diffuse_coefficient"), 1, &diff_coef[0]);
  //glUniform3fv(uspecular_coefficient,1,&spec_coef[0]);
  glUniform3fv(glGetUniformLocation(program, "specular_coefficient"), 1, &spec_coef[0]);
  //glUniform1f(uspecular_exponent,spec_exp);
  glUniform1f(glGetUniformLocation(program, "specular_exponent"), spec_exp);

  glUniform3fv(glGetUniformLocation(program, "ambient_coefficient"), 1, &ambi_coef[0]);

  glUseProgram(gBufferProgram);
  glUniform3fv(glGetUniformLocation(gBufferProgram, "diffuse_coefficient"), 1, &diff_coef[0]);
  glUniform3fv(glGetUniformLocation(gBufferProgram, "specular_coefficient"), 1, &spec_coef[0]);
  glUniform1f(glGetUniformLocation(gBufferProgram, "specular_exponent"), spec_exp);
  glUniform3fv(glGetUniformLocation(gBufferProgram, "ambient_coefficient"), 1, &ambi_coef[0]);
  //glUseProgram(testProgram);
  //
  //
  ////glUniform3fv(udiffuse_coefficient,1,&diff_coef[0]);
  //glUniform3fv(glGetUniformLocation(testProgram, "diffuse_coefficient"), 1, &diff_coef[0]);
  ////glUniform3fv(uspecular_coefficient,1,&spec_coef[0]);
  //glUniform3fv(glGetUniformLocation(testProgram, "specular_coefficient"), 1, &spec_coef[0]);
  ////glUniform1f(uspecular_exponent,spec_exp);
  //glUniform1f(glGetUniformLocation(testProgram, "specular_exponent"), spec_exp);
  //
  //glUniform3fv(glGetUniformLocation(testProgram, "ambient_coefficient"), 1, &ambi_coef[0]);
}
/******************************************************************************/
/*!
  \brief
    sets a point light source to the specified position and color. A maximum of
    8 light sources may be used. The lights are indexed from 0 to 7.

  \param li
    light index

  \param position
    light position

  \param color
    light color
*/
/******************************************************************************/
void cs250::PhongRender::setLight(int li, const glm::vec4 &position,
                                  const glm::vec3 &color)
{
  glUseProgram(program);
  
  //glUniform1i(glGetUniformLocation(program, "light_use") + li, 1);
  glUniform4fv(glGetUniformLocation(program, "light_position") + li,1,&position[0]);
  glUniform3fv(glGetUniformLocation(program, "light_color") + li,1,&color[0]);
  glUniform1i(glGetUniformLocation(program, "light_type") + li, POINT);

  glUseProgram(fsqProgram);

  //glUniform1i(glGetUniformLocation(program, "light_use") + li, 1);
  glUniform4fv(glGetUniformLocation(fsqProgram, "light_position") + li, 1, &position[0]);
  glUniform3fv(glGetUniformLocation(fsqProgram, "light_color") + li, 1, &color[0]);
  glUniform1i(glGetUniformLocation(fsqProgram, "light_type") + li, POINT);
  //glUseProgram(testProgram);
  //
  ////glUniform1i(glGetUniformLocation(program, "light_use") + li, 1);
  //glUniform4fv(glGetUniformLocation(testProgram, "light_position") + li, 1, &position[0]);
  //glUniform3fv(glGetUniformLocation(testProgram, "light_color") + li, 1, &color[0]);
  //glUniform1i(glGetUniformLocation(testProgram, "light_type") + li, POINT);
}
/******************************************************************************/
/*!
  \brief
    sets a Directional light source to the specified position and color. A maximum of
    3 light sources may be used. The lights are indexed from 13 to 15.

  \param li
    light index

  \param direction
    light direction

  \param color
    light color
*/
/******************************************************************************/
void cs250::PhongRender::setDirectionalLight(int li, const glm::vec4& direction, const glm::vec3& color)
{
    glUseProgram(program);

    //glUniform1i(glGetUniformLocation(program, "light_use") + li, 1);
    //glUniform4fv(ulight_position + li, 1, &position[0]);
    //glUniform3fv(ulight_color + li, 1, &color[0]);
    glUniform3fv(glGetUniformLocation(program, "light_color") + li, 1, &color[0]);
    glUniform1i(glGetUniformLocation(program, "light_type") + li, DIRECTIONAL);
    glUniform4fv(glGetUniformLocation(program, "light_direction") + li, 1, &direction[0]);


    glUseProgram(fsqProgram);

    glUniform3fv(glGetUniformLocation(fsqProgram, "light_color") + li, 1, &color[0]);
    glUniform1i(glGetUniformLocation(fsqProgram, "light_type") + li, DIRECTIONAL);
    glUniform4fv(glGetUniformLocation(fsqProgram, "light_direction") + li, 1, &direction[0]);
    //glUseProgram(testProgram);
    //
    ////glUniform1i(glGetUniformLocation(program, "light_use") + li, 1);
    ////glUniform4fv(ulight_position + li, 1, &position[0]);
    ////glUniform3fv(ulight_color + li, 1, &color[0]);
    //glUniform3fv(glGetUniformLocation(testProgram, "light_color") + li, 1, &color[0]);
    //glUniform1i(glGetUniformLocation(testProgram, "light_type") + li, DIRECTIONAL);
    //glUniform4fv(glGetUniformLocation(testProgram, "light_direction") + li, 1, &direction[0]);
}
/******************************************************************************/
/*!
  \brief
    sets a Spot light source to the specified position and color. A maximum of
    5 light sources may be used. The lights are indexed from 8 to 12.

  \param li
    light index
  
  \param position
    light positon

  \param direction
    light direction

  \param color
    light color

  \param theta
    inner angle for spot light

  \param phi
    outer angle for spot light

  \param fallout
    fallout value for spot light
*/
/******************************************************************************/
void cs250::PhongRender::setSpotLight(int li, const glm::vec4& position, const glm::vec4& direction, const glm::vec3& color, float theta, float phi, float fallout)
{
    glUseProgram(program);

    //glUniform1i(glGetUniformLocation(program, "light_use") + li, 1);
    glUniform4fv(glGetUniformLocation(program, "light_position") + li, 1, &position[0]);
    glUniform4fv(glGetUniformLocation(program, "light_direction") + li, 1, &direction[0]);
    glUniform1i(glGetUniformLocation(program, "light_type") + li, SPOT);
    glUniform3fv(glGetUniformLocation(program, "light_color") + li, 1, &color[0]);


    //GLint loc = glGetUniformLocation(program, "light_inner_theta");

    glUniform1f(glGetUniformLocation(program, "light_inner_theta") + li, theta);
    glUniform1f(glGetUniformLocation(program, "light_outer_phi") + li, phi);
    //loc = glGetUniformLocation(program, "light_outer_phi");
    glUniform1f(glGetUniformLocation(program, "light_fallout") + li, fallout);
    //loc = glGetUniformLocation(program, "light_fallout");

    glUseProgram(fsqProgram);

    glUniform4fv(glGetUniformLocation(fsqProgram, "light_position") + li, 1, &position[0]);
    glUniform4fv(glGetUniformLocation(fsqProgram, "light_direction") + li, 1, &direction[0]);
    glUniform1i(glGetUniformLocation(fsqProgram, "light_type") + li, SPOT);
    glUniform3fv(glGetUniformLocation(fsqProgram, "light_color") + li, 1, &color[0]);
    glUniform1f(glGetUniformLocation(fsqProgram, "light_inner_theta") + li, theta);
    glUniform1f(glGetUniformLocation(fsqProgram, "light_outer_phi") + li, phi);
    glUniform1f(glGetUniformLocation(fsqProgram, "light_fallout") + li, fallout);

    //glUseProgram(testProgram);
    //
    ////glUniform1i(glGetUniformLocation(program, "light_use") + li, 1);
    //glUniform4fv(glGetUniformLocation(testProgram, "light_position") + li, 1, &position[0]);
    //glUniform4fv(glGetUniformLocation(testProgram, "light_direction") + li, 1, &direction[0]);
    //glUniform1i(glGetUniformLocation(testProgram, "light_type") + li, SPOT);
    //glUniform3fv(glGetUniformLocation(testProgram, "light_color") + li, 1, &color[0]);
    //
    //
    ////GLint loc = glGetUniformLocation(program, "light_inner_theta");
    //
    //glUniform1f(glGetUniformLocation(testProgram, "light_inner_theta") + li, theta);
    //glUniform1f(glGetUniformLocation(testProgram, "light_outer_phi") + li, phi);
    ////loc = glGetUniformLocation(program, "light_outer_phi");
    //glUniform1f(glGetUniformLocation(testProgram, "light_fallout") + li, fallout);
}
/******************************************************************************/
/*!
  \brief
    sets the light to be turn on or off

  \param li
    light index

  \param use
    0 to turn off, 1 to turn on
*/
/******************************************************************************/
void cs250::PhongRender::setLightInUse(int li, int use)
{
    glUseProgram(program);
    glUniform1i(glGetUniformLocation(program, "light_use") + li, use);

    glUseProgram(fsqProgram);
    glUniform1i(glGetUniformLocation(fsqProgram, "light_use") + li, use);

    //glUseProgram(testProgram);
    //glUniform1i(glGetUniformLocation(testProgram, "light_use") + li, use);
}
/******************************************************************************/
/*!
  \brief
    sets the current ambient light color to be used to render a mesh.

  \param color
    global ambient color
*/
/******************************************************************************/
void cs250::PhongRender::setAmbient(const glm::vec3 &color)
{
  glUseProgram(program);
  
  //glUniform3fv(uglobal_ambient_color,1,&color[0]);
  glUniform3fv(glGetUniformLocation(program, "global_ambient_color"), 1, &color[0]);


  glUseProgram(fsqProgram);

  //glUniform3fv(uglobal_ambient_color,1,&color[0]);
  glUniform3fv(glGetUniformLocation(fsqProgram, "global_ambient_color"), 1, &color[0]);
  //glUseProgram(testProgram);
  //
  ////glUniform3fv(uglobal_ambient_color,1,&color[0]);
  //glUniform3fv(glGetUniformLocation(testProgram, "global_ambient_color"), 1, &color[0]);
}
/******************************************************************************/
/*!
  \brief
    sets local Attenuation parameters.

  \param constant_att
    constant attenuation

  \param linear_att
    linear attenuation

  \param quadratic_att
    quadratic attenuation
*/
/******************************************************************************/
void cs250::PhongRender::setAttenauation(float constant_att, float linear_att, float quadratic_att)
{
    glUseProgram(program);

    GLint att_location = glGetUniformLocation(program, "constant_attenuation");
    glUniform1f(att_location, constant_att);

    att_location = glGetUniformLocation(program, "linear_attenuation");
    glUniform1f(att_location, linear_att);
    //att test
    att_location = glGetUniformLocation(program, "quadratic_attenuation");
    glUniform1f(att_location, quadratic_att);


    glUseProgram(fsqProgram);

    att_location = glGetUniformLocation(fsqProgram, "constant_attenuation");
    glUniform1f(att_location, constant_att);

    att_location = glGetUniformLocation(fsqProgram, "linear_attenuation");
    glUniform1f(att_location, linear_att);
    //att test
    att_location = glGetUniformLocation(fsqProgram, "quadratic_attenuation");
    glUniform1f(att_location, quadratic_att);
    //glUseProgram(testProgram);
    //
    //att_location = glGetUniformLocation(testProgram, "constant_attenuation");
    //glUniform1f(att_location, constant_att);
    //
    //att_location = glGetUniformLocation(testProgram, "linear_attenuation");
    //glUniform1f(att_location, linear_att);
    ////att test
    //att_location = glGetUniformLocation(testProgram, "quadratic_attenuation");
    //glUniform1f(att_location, quadratic_att);
}
/******************************************************************************/
/*!
  \brief
    sets Atomspheric Attenuation parameters.

  \param z_far
    far plane

  \param z_near
    near plane

  \param fog_color
    fog color
*/
/******************************************************************************/
void cs250::PhongRender::setAtomsphericAttenauation(float z_far, float z_near, const glm::vec3& fog_color)
{
    glUseProgram(program);
    glUniform1f(glGetUniformLocation(program, "z_far"), z_far);
    glUniform1f(glGetUniformLocation(program, "z_near"), z_near);
    glUniform3fv(glGetUniformLocation(program, "fog_color"), 1, &fog_color[0]);

    glUseProgram(fsqProgram);
    glUniform1f(glGetUniformLocation(fsqProgram, "z_far"), z_far);
    glUniform1f(glGetUniformLocation(fsqProgram, "z_near"), z_near);
    glUniform3fv(glGetUniformLocation(fsqProgram, "fog_color"), 1, &fog_color[0]);

    //glUseProgram(testProgram);
    //glUniform1f(glGetUniformLocation(testProgram, "z_far"), z_far);
    //glUniform1f(glGetUniformLocation(testProgram, "z_near"), z_near);
    //glUniform3fv(glGetUniformLocation(testProgram, "fog_color"), 1, &fog_color[0]);
}
/******************************************************************************/
/*!
  \brief
    set lighting sphere color

  \param color
    solid color for lighting sphere
*/
/******************************************************************************/
void cs250::PhongRender::setIlluminatorColor(const glm::vec3& color)
{
    glUseProgram(iProgram);
    glUniform3fv(glGetUniformLocation(iProgram, "color"), 1, &color[0]);
}
/******************************************************************************/
/*!
  \brief
    set Wireframe Color when using wireframe shaders

  \param color
    wireframe color
*/
/******************************************************************************/
void cs250::PhongRender::setWireframeColor(const glm::vec3& color)
{
    glUseProgram(wProgram);
    glUniform3fv(glGetUniformLocation(wProgram, "color"), 1, &color[0]);

    glUseProgram(worldProgram);
    glUniform3fv(glGetUniformLocation(worldProgram, "color"), 1, &color[0]);
}
/******************************************************************************/
/*!
  \brief
    set Wireframe Color when using wireframe shaders

  \param color
    wireframe color
*/
/******************************************************************************/
void cs250::PhongRender::setWireframeTriangleColor(const glm::vec3& color)
{
    glUseProgram(worldProgram);
    glUniform3fv(glGetUniformLocation(worldProgram, "color"), 1, &color[0]);
}
/******************************************************************************/
/*!
  \brief
    set transparent parameters to modify refraction texture

  \param eta_in
    refractive index for the material where light entering
  
  \param eta_out
    refractive index for the material where light leaving

  \param epsilon
    epsilon for chromatic aberration
*/
/******************************************************************************/
void cs250::PhongRender::setTransparentIndex(float eta_in, float eta_out, float epsilon)
{
    glUseProgram(program);
    glUniform1f(glGetUniformLocation(program, "eta_in"), eta_in);
    glUniform1f(glGetUniformLocation(program, "eta_out"), eta_out);
    glUniform1f(glGetUniformLocation(program, "epsilon"), epsilon);

    //glUseProgram(testProgram);
    //glUniform1f(glGetUniformLocation(testProgram, "eta_in"), eta_in);
    //glUniform1f(glGetUniformLocation(testProgram, "eta_out"), eta_out);
    //glUniform1f(glGetUniformLocation(testProgram, "epsilon"), epsilon);
}
/******************************************************************************/
/*!
  \brief
    load a texture to GPU

  \param path
    the file path for the textrue

  \return texID
    the handle for the loaded texture
*/
/******************************************************************************/
GLuint cs250::PhongRender::loadTexture(const std::string& path)
{
    GLuint texID;

    int m_Width, m_Height, m_BPP;

    stbi_set_flip_vertically_on_load(1);
    unsigned char* m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

    printf("%d %d %d %s\n", m_BPP, m_Width, m_Height, path.c_str());
    if (!m_LocalBuffer) {
        printf("\nRead error on file %s:\n  %s\n\n", path.c_str(), stbi_failure_reason());
        exit(-1);
    }

    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (m_LocalBuffer)
        stbi_image_free(m_LocalBuffer);

    return texID;
}
/******************************************************************************/
/*!
  \brief
    load 6 texture to GPU for skybox

  \param facePaths
    the file path for the textrue

  \return out_vector
    a vector contains 6 handles for skybox textures
*/
/******************************************************************************/
std::vector<GLuint> cs250::PhongRender::loadSkyboxTexture(const std::vector<std::string>& facePaths)
{
    std::vector<GLuint> out_vector;

    for (unsigned int i = 0; i < 6; ++i)
    {
        out_vector.push_back(loadTexture(facePaths[i]));
    }

    return out_vector;
} 
/******************************************************************************/
/*!
  \brief
    draws a single mesh whose mesh index is mi, as returned by loadMesh.

  \param mi
    mesh index
*/
/******************************************************************************/
void cs250::PhongRender::draw(int mi)
{
  glUseProgram(program);

  glUniform1i(glGetUniformLocation(program, "objectID"), mi);
  // recall state
  glBindVertexArray(mesh_data[mi].vertex_array_buffer);

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glDrawElements(GL_TRIANGLES,3 * mesh_data[mi].face_count,GL_UNSIGNED_INT,0);

  //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBindVertexArray(0);
}
/******************************************************************************/
/*!
  \brief
    draws a single mesh's normals whose mesh index is mi, as returned by loadMesh.

  \param mi
    mesh index
*/
/******************************************************************************/
void cs250::PhongRender::drawNormal(int mi)
{
    glUseProgram(nProgram);
    // recall state
    glBindVertexArray(mesh_data[mi].vertex_array_buffer);
    glDrawElements(GL_TRIANGLES, 3 * mesh_data[mi].face_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
/******************************************************************************/
/*!
  \brief
    draws a single mesh's face normals whose mesh index is mi, as returned by loadMesh.

  \param mi
    mesh index
*/
/******************************************************************************/
void cs250::PhongRender::drawFaceNormal(int mi)
{
    glUseProgram(fnProgram);
    // recall state
    glBindVertexArray(mesh_data[mi].vertex_array_buffer);
    glDrawElements(GL_TRIANGLES, 3 * mesh_data[mi].face_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
/******************************************************************************/
/*!
  \brief
    draws a single mesh's as wireframe

  \param mi
    mesh index
*/
/******************************************************************************/
void cs250::PhongRender::drawWireframe(int mi)
{
    glUseProgram(wProgram);

    // recall state
    glBindVertexArray(mesh_data[mi].vertex_array_buffer);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, 3 * mesh_data[mi].face_count, GL_UNSIGNED_INT, 0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindVertexArray(0);
}
/******************************************************************************/
/*!
  \brief
    draws a wireframe cube (without diagonal line)
*/
/******************************************************************************/
void cs250::PhongRender::drawWireframeCube()
{
    glUseProgram(wProgram);

    glBindVertexArray(cubeVAOHandle);

    glDrawElements(GL_LINES, 2 * 12, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}
/******************************************************************************/
/*!
  \brief
    draws a single wireframe Triangle from world space.
    (Too slow, it's for prototype purpose)

  \param object
    A triangle Object
*/
/******************************************************************************/
void cs250::PhongRender::drawWireframeTriangle(const Object& object) const
{
    GLuint VBO;
    GLuint VAO;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * 3, &object._points[0], GL_STATIC_DRAW);

    GLint aposition = glGetAttribLocation(worldProgram, "position");

    glVertexAttribPointer(aposition, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(aposition);

    glUseProgram(worldProgram);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}
/******************************************************************************/
/*!
  \brief
    Draws a array of wireframe triangles from world space

  \param vertices 
    An array of vertices
*/
/******************************************************************************/
void cs250::PhongRender::drawWireframeTriangles(const std::vector<glm::vec4>& vertices) const
{
    GLuint VBO;
    GLuint VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    GLint aposition = glGetAttribLocation(worldProgram, "position");

    glVertexAttribPointer(aposition, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(aposition);

    glUseProgram(worldProgram);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}
/******************************************************************************/
/*!
  \brief
    draws a skybox mesh's face normals whose mesh index is mi, as returned by 
    loadMesh.

  \param mi
    mesh index
*/
/******************************************************************************/
void cs250::PhongRender::drawSkybox(int mi)
{
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    glUseProgram(skbProgram);

    GLint loc = glGetUniformLocation(skbProgram, "skyboxTex");

    glUniform1i(loc, 0);
    glBindVertexArray(mesh_data[mi].vertex_array_buffer);

    for (size_t i = 0; i < 6; ++i)
    {
        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, skyboxTexID[i]);

        size_t firstIndex = i * 6;
        size_t indexDataSize = sizeof(GLuint);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, reinterpret_cast<const GLvoid*>(firstIndex * indexDataSize));
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindVertexArray(0);

    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
}
/******************************************************************************/
/*!
  \brief
    draws a mesh with texture whose mesh index is mi, as returned by loadMesh.

  \param mi
    mesh index

  \param gpu_use
    determine whether to use precalculated texture coordinates or using gpu to
    calculate

  \param entity_type
    determine using vertex coordinates or normals to generate texture 
    coordinates

  \param mapping_type
    determine which mapping type to use to generate texture coordinates
*/
/******************************************************************************/
void cs250::PhongRender::drawTexture(int mi, int gpu_use, int entity_type, int mapping_type)
{
    glUseProgram(program);
    // recall state
    glUniform1i(glGetUniformLocation(program, "objectID"), mi);
    glUniform1i(glGetUniformLocation(program, "gpu_calc"), gpu_use);
    glUniform1i(glGetUniformLocation(program, "ventity_type"), entity_type);
    glUniform1i(glGetUniformLocation(program, "mapping_type"), mapping_type);

    glUniform3fv(glGetUniformLocation(program, "dimension"), 1, &mesh_data[mi].dim[0]);
    glUniform3fv(glGetUniformLocation(program, "center"), 1, &mesh_data[mi].cen[0]);

    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, diffTexID);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, specTexID);


    glUniform1i(glGetUniformLocation(program, "diffTex"), 0);

    glUniform1i(glGetUniformLocation(program, "specTex"), 1);


    glBindVertexArray(mesh_data[mi].vertex_array_buffer);

    glDrawElements(GL_TRIANGLES, 3 * mesh_data[mi].face_count, GL_UNSIGNED_INT, 0);

    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindVertexArray(0);

}
/******************************************************************************/
/*!
  \brief
    draws a mesh with solid color whose mesh index is mi, as returned by
    loadMesh.

  \param mi
    mesh index
*/
/******************************************************************************/
void cs250::PhongRender::drawIlluminator(int mi)
{
    glUseProgram(iProgram);
    // recall state
    glBindVertexArray(mesh_data[mi].vertex_array_buffer);
    glDrawElements(GL_TRIANGLES, 3 * mesh_data[mi].face_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
/******************************************************************************/
/*!
  \brief
    delete current using shader from gpu and reload a new one

  \param shader
    determine which shader to be reloaded
*/
/******************************************************************************/
void cs250::PhongRender::reloadShader(int shader)
{
    // delete old shader
    glUseProgram(0);
    glDeleteProgram(program);
    // recomplie, link the shader
    switch (shader)
    {
        case 0:
            program = LoadShaders("res/shaders/PhongLighting.vert", "res/shaders/PhongLighting.frag");
            stype = shader_type::PHONGLIGHTING;
            break;
        case 1:
            program = LoadShaders("res/shaders/PhongShading.vert", "res/shaders/PhongShading.frag");
            stype = shader_type::PHONGSHADING;
            break;
        case 2:
            program = LoadShaders("res/shaders/BlinnShading.vert", "res/shaders/BlinnShading.frag");
            stype = shader_type::BLINNSHADING;
            break;
        case 3:
            program = LoadShaders("res/shaders/TransparentShader.vert", "res/shaders/TransparentShader.frag");
            stype = shader_type::TRANSPARENT;
            break;
    }
    // go through eash mesh and rebind everything with the new shader
    for (unsigned int i = 0; i < mesh_data.size(); ++i)
    {
        GLint aposition = glGetAttribLocation(program, "position"),
            anormal = glGetAttribLocation(program, "normal"),
            atexcoord = glGetAttribLocation(program, "aTexCoord");

        // start state recording
        glUseProgram(program);

        glBindVertexArray(mesh_data[i].vertex_array_buffer);

        glBindBuffer(GL_ARRAY_BUFFER, mesh_data[i].buffer_objects[MeshData::VERT]);
        glVertexAttribPointer(aposition, 4, GL_FLOAT, false, 0, 0);
        glEnableVertexAttribArray(aposition);

        glBindBuffer(GL_ARRAY_BUFFER, mesh_data[i].buffer_objects[MeshData::NORM]);
        glVertexAttribPointer(anormal, 4, GL_FLOAT, false, 0, 0);
        glEnableVertexAttribArray(anormal);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_data[i].buffer_objects[MeshData::FACE]);

        glBindBuffer(GL_ARRAY_BUFFER, mesh_data[i].buffer_objects[MeshData::TEXT]);
        glVertexAttribPointer(atexcoord, 2, GL_FLOAT, false, 0, 0);
        glEnableVertexAttribArray(atexcoord);
        // stop state recording
        glBindVertexArray(0);
    }
}
/******************************************************************************/
/*!
  \brief
    delete texture coordinates bind with a mesh and reload it

  \param mi
    mesh index

  \param texCoords
    texture coordinates
*/
/******************************************************************************/
void cs250::PhongRender::reloadTexCoords(int mi, const std::vector<glm::vec2>& texCoords)
{
    // remove tex from GPU
    glDeleteBuffers(1, &mesh_data[mi].buffer_objects[MeshData::TEXT]);
    // reattach to GPU
    glGenBuffers(1, &mesh_data[mi].buffer_objects[MeshData::TEXT]);
    glBindBuffer(GL_ARRAY_BUFFER, mesh_data[mi].buffer_objects[MeshData::TEXT]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * texCoords.size(),
        &(texCoords[0]), GL_STATIC_DRAW);

    glUseProgram(program);
    GLint atexcoord = glGetAttribLocation(program, "aTexCoord");
    glBindVertexArray(mesh_data[mi].vertex_array_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, mesh_data[mi].buffer_objects[MeshData::TEXT]);
    glVertexAttribPointer(atexcoord, 2, GL_FLOAT, false, 0, 0);
    glEnableVertexAttribArray(atexcoord);

    glBindVertexArray(0);
}
/******************************************************************************/
/*!
  \brief
    Setup up custom framebuffer object to draw texture dynamically

  \return
    result from glCheckFramebufferStatus
*/
/******************************************************************************/
bool cs250::PhongRender::SetUpFBO(void)
{
    glGenFramebuffers(1, &FBOHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, FBOHandle);

    // The texture we're going to render to
    glGenTextures(1, &renderedTexture0);
    //glGenTextures(1, &renderedTexture1);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, renderedTexture0);
    //glBindTexture(GL_TEXTURE_2D, renderedTexture1);

    // Give an empty image to OpenGL ( the last "0" means "empty" )
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
        1024, 1024, 0,
        GL_RGB, GL_UNSIGNED_BYTE, 0);

    // Poor filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture0, 0);

    glGenTextures(1, &renderedTexture1);
    glBindTexture(GL_TEXTURE_2D, renderedTexture1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
        1024, 1024, 0,
        GL_RGB, GL_UNSIGNED_BYTE, 0);

    // Poor filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, renderedTexture1, 0);

    glGenTextures(1, &renderedTexture2);
    glBindTexture(GL_TEXTURE_2D, renderedTexture2);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
        1024, 1024, 0,
        GL_RGB, GL_UNSIGNED_BYTE, 0);

    // Poor filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, renderedTexture2, 0);

    glGenTextures(1, &renderedTexture3);
    glBindTexture(GL_TEXTURE_2D, renderedTexture3);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
        1024, 1024, 0,
        GL_RGB, GL_UNSIGNED_BYTE, 0);

    // Poor filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, renderedTexture3, 0);
    
    glGenTextures(1, &renderedTexture4);
    glBindTexture(GL_TEXTURE_2D, renderedTexture4);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
        1024, 1024, 0,
        GL_RGB, GL_UNSIGNED_BYTE, 0);

    // Poor filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, renderedTexture4, 0);
    
    glGenTextures(1, &renderedTexture5);
    glBindTexture(GL_TEXTURE_2D, renderedTexture5);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
        1024, 1024, 0,
        GL_RGB, GL_UNSIGNED_BYTE, 0);

    // Poor filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, renderedTexture5, 0);
    // The depth buffer
    GLuint depthrenderbuffer;
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 1024);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
        GL_RENDERBUFFER, depthrenderbuffer);

// Always check that our framebuffer is ok
    return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

/******************************************************************************/
/*!
  \brief
    Setup up custom framebuffer object for GBuffer and deffered shading

  \param viewportWidth
    viewportWidth to be used

  \param viewportHeight
    viewportHeight to be used

  \return
    result from glCheckFramebufferStatus
*/
/******************************************************************************/
bool cs250::PhongRender::SetUpGbufferFBO(int viewportWidth, int viewportHeight)
{
    glGenFramebuffers(1, &GBufferFBOHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, GBufferFBOHandle);

    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, viewportWidth, viewportHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, viewportWidth, viewportHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

    glGenTextures(1, &gDiffuse);
    glBindTexture(GL_TEXTURE_2D, gDiffuse);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, viewportWidth, viewportHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gDiffuse, 0);

    glGenTextures(1, &gSpecularShininess);
    glBindTexture(GL_TEXTURE_2D, gSpecularShininess);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, viewportWidth, viewportHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gSpecularShininess, 0);

    glGenTextures(1, &gAmbient);
    glBindTexture(GL_TEXTURE_2D, gAmbient);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, viewportWidth, viewportHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, gAmbient, 0);

    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, viewportWidth, viewportHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    GLuint attachments[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
    glDrawBuffers(5, attachments);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Framebuffer not complete!" << std::endl;
        return false;
    }
    else
        return true;
}
/******************************************************************************/
/*!
  \brief
    helper function to set up FSQ
*/
/******************************************************************************/
void cs250::PhongRender::SetUpFSQBuffer(void)
{
    FSQBuffer = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    glUseProgram(fsqProgram);
    glGenVertexArrays(1, &FSQVAO);
    glGenBuffers(1, &FSQVBO);
    glBindVertexArray(FSQVAO);
    glBindBuffer(GL_ARRAY_BUFFER, FSQVBO);
    glBufferData(GL_ARRAY_BUFFER, FSQBuffer.size() * sizeof(GLfloat), FSQBuffer.data(), GL_STATIC_DRAW);

    GLint aposition = glGetAttribLocation(fsqProgram, "aPos"),
          atexcoord = glGetAttribLocation(fsqProgram, "aTexCoords");

    glEnableVertexAttribArray(aposition);
    glVertexAttribPointer(aposition, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(atexcoord);
    glVertexAttribPointer(atexcoord, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}
/******************************************************************************/
/*!
  \brief
    allow client to use custom FBO or revert to default

  \param FBO_Handle
    0 to use default framebuffer, or call get FBO handle to use custom one
    created by PhongRender

  \param viewportWidth
    viewport width

  \param viewportHeight
    viewport height
*/
/******************************************************************************/
void cs250::PhongRender::UseFBO(int FBO_Handle, int viewportWidth, int viewportHeight)
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO_Handle);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "[UseFBO] Cannot switch to FBO - " << FBO_Handle << std::endl;
        std::cout << "[UseFBO] FBO not complete" << std::endl;
        return;
    }

    glViewport(0, 0, viewportWidth, viewportHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

//void cs250::PhongRender::drawFBOTexture(int mi, int gpu_use, int entity_type, int mapping_type, int slot)
//{
//    glUseProgram(testProgram);
//    // recall state
//    glUniform1i(glGetUniformLocation(testProgram, "objectID"), mi);
//    glUniform1i(glGetUniformLocation(testProgram, "gpu_calc"), gpu_use);
//    glUniform1i(glGetUniformLocation(testProgram, "ventity_type"), entity_type);
//    glUniform1i(glGetUniformLocation(testProgram, "mapping_type"), mapping_type);
//
//    glUniform3fv(glGetUniformLocation(testProgram, "dimension"), 1, &mesh_data[mi].dim[0]);
//    glUniform3fv(glGetUniformLocation(testProgram, "center"), 1, &mesh_data[mi].cen[0]);
//
//
//    glActiveTexture(GL_TEXTURE0 + 0);
//    glBindTexture(GL_TEXTURE_2D, renderedTexture);
//
//    glActiveTexture(GL_TEXTURE0 + 1);
//    glBindTexture(GL_TEXTURE_2D, renderedTexture1);
//    
//    glActiveTexture(GL_TEXTURE0 + 2);
//    glBindTexture(GL_TEXTURE_2D, renderedTexture2);
//    
//    glActiveTexture(GL_TEXTURE0 + 3);
//    glBindTexture(GL_TEXTURE_2D, renderedTexture3);
//    
//    glActiveTexture(GL_TEXTURE0 + 4);
//    glBindTexture(GL_TEXTURE_2D, renderedTexture4);
//    
//    glActiveTexture(GL_TEXTURE0 + 5);
//    glBindTexture(GL_TEXTURE_2D, renderedTexture5);
//
//    GLint loc = glGetUniformLocation(testProgram, "backTex");
//    glUniform1i(glGetUniformLocation(testProgram, "backTex"), 0);
//    glUniform1i(glGetUniformLocation(testProgram, "rightTex"), 1);
//    glUniform1i(glGetUniformLocation(testProgram, "frontTex"), 2);
//    glUniform1i(glGetUniformLocation(testProgram, "leftTex"), 3);
//    glUniform1i(glGetUniformLocation(testProgram, "topTex"), 4);
//    glUniform1i(glGetUniformLocation(testProgram, "bottomTex"), 5);
//
//
//    loc = glGetUniformLocation(testProgram, "faceID");
//    glUniform1i(glGetUniformLocation(testProgram, "faceID"), slot);
//
//    glBindVertexArray(mesh_data[mi].vertex_array_buffer);
//
//    glDrawElements(GL_TRIANGLES, 3 * mesh_data[mi].face_count, GL_UNSIGNED_INT, 0);
//
//    glActiveTexture(GL_TEXTURE0 + 0);
//    glBindTexture(GL_TEXTURE_2D, 0);
//    
//    glActiveTexture(GL_TEXTURE0 + 1);
//    glBindTexture(GL_TEXTURE_2D, 0);
//    
//    glActiveTexture(GL_TEXTURE0 + 2);
//    glBindTexture(GL_TEXTURE_2D, 0);
//    
//    glActiveTexture(GL_TEXTURE0 + 3);
//    glBindTexture(GL_TEXTURE_2D, 0);
//    
//    glActiveTexture(GL_TEXTURE0 + 4);
//    glBindTexture(GL_TEXTURE_2D, 0);
//    
//    glActiveTexture(GL_TEXTURE0 + 5);
//    glBindTexture(GL_TEXTURE_2D, 0);
//
//    glBindVertexArray(0);
//
//}
/******************************************************************************/
/*!
  \brief
    draw dynamically rendered texture onto a mesh

  \param mi
    mesh index

  \param transparent_type
    transparent type  to determine on gpu reflection/refraction/mix

  \param shading_use
    whether to apply phong shading on texture
*/
/******************************************************************************/
void cs250::PhongRender::drawFBOTexture(int mi, int transparent_type, bool shading_use)
{
    glUseProgram(program);
    // recall state
    glUniform1i(glGetUniformLocation(program, "objectID"), mi);
    glUniform1i(glGetUniformLocation(program, "transparentID"), transparent_type);

    int su = 0;

    if (shading_use)
        su = 1;

    glUniform1i(glGetUniformLocation(program, "shading_use"), su);

    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, renderedTexture0);

    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, renderedTexture1);

    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, renderedTexture2);

    glActiveTexture(GL_TEXTURE0 + 3);
    glBindTexture(GL_TEXTURE_2D, renderedTexture3);

    glActiveTexture(GL_TEXTURE0 + 4);
    glBindTexture(GL_TEXTURE_2D, renderedTexture4);

    glActiveTexture(GL_TEXTURE0 + 5);
    glBindTexture(GL_TEXTURE_2D, renderedTexture5);

    glUniform1i(glGetUniformLocation(program, "backTex"), 0);
    glUniform1i(glGetUniformLocation(program, "rightTex"), 1);
    glUniform1i(glGetUniformLocation(program, "frontTex"), 2);
    glUniform1i(glGetUniformLocation(program, "leftTex"), 3);
    glUniform1i(glGetUniformLocation(program, "topTex"), 4);
    glUniform1i(glGetUniformLocation(program, "bottomTex"), 5);


    glBindVertexArray(mesh_data[mi].vertex_array_buffer);

    glDrawElements(GL_TRIANGLES, 3 * mesh_data[mi].face_count, GL_UNSIGNED_INT, 0);

    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE0 + 3);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE0 + 4);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE0 + 5);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindVertexArray(0);

}
/******************************************************************************/
/*!
  \brief
    return FBOHandle for client to use custom FBO

  \return FBOHandle
    custom FBOHandle generate by PhongRander
*/
/******************************************************************************/
GLuint cs250::PhongRender::getFBOHandle(void) const
{
    return FBOHandle;
}
/******************************************************************************/
/*!
  \brief
    return GBufferFBOHandle for client to use custom FBO

  \return GBufferFBOHandle
    custom FBOHandle generate by PhongRander
*/
/******************************************************************************/
GLuint cs250::PhongRender::getGBufferFBOHandle(void) const
{
    return GBufferFBOHandle;
}
/******************************************************************************/
/*!
  \brief
    draw FSQ onto screen

  \param mode
    to determine which color attachment to draw for debug
*/
/******************************************************************************/
void cs250::PhongRender::drawFSQ(int mode)
{
    glUseProgram(fsqProgram);

    glActiveTexture(GL_TEXTURE0 + 0);
    //glBindTexture(GL_TEXTURE_2D, diffTexID);
    glBindTexture(GL_TEXTURE_2D, gPosition);

    glActiveTexture(GL_TEXTURE0 + 1);
    //glBindTexture(GL_TEXTURE_2D, diffTexID);
    glBindTexture(GL_TEXTURE_2D, gNormal);


    glActiveTexture(GL_TEXTURE0 + 2);
    //glBindTexture(GL_TEXTURE_2D, diffTexID);
    glBindTexture(GL_TEXTURE_2D, gDiffuse);

    glActiveTexture(GL_TEXTURE0 + 3);
    //glBindTexture(GL_TEXTURE_2D, diffTexID);
    glBindTexture(GL_TEXTURE_2D, gSpecularShininess);

    glActiveTexture(GL_TEXTURE0 + 4);
    //glBindTexture(GL_TEXTURE_2D, diffTexID);
    glBindTexture(GL_TEXTURE_2D, gAmbient);
    //glUniform1i(glGetUniformLocation(fsqProgram, "texSampler"), 0);
    glUniform1i(glGetUniformLocation(fsqProgram, "gPositionSampler"), 0);

    glUniform1i(glGetUniformLocation(fsqProgram, "gNormalSampler"), 1);
    glUniform1i(glGetUniformLocation(fsqProgram, "gDiffuseSampler"), 2);
    glUniform1i(glGetUniformLocation(fsqProgram, "gSpecularShininessSampler"), 3);
    glUniform1i(glGetUniformLocation(fsqProgram, "gAmbientSampler"), 4);

    glUniform1i(glGetUniformLocation(fsqProgram, "uMode"), mode);

    glBindVertexArray(FSQVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
/******************************************************************************/
/*!
  \brief
    draw mesh to GBuffer

  \param mi
    mesh index given to client
*/
/******************************************************************************/
void cs250::PhongRender::drawToGBuffer(int mi)
{
    glUseProgram(gBufferProgram);
    // recall state
    glBindVertexArray(mesh_data[mi].vertex_array_buffer);
    glDrawElements(GL_TRIANGLES, 3 * mesh_data[mi].face_count, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}
/******************************************************************************/
/*!
  \brief
    resize GBuffer FBO

  \param viewportWidth
    viewportWidth

  \param viewportHeight
    viewportHeight
*/
/******************************************************************************/
void cs250::PhongRender::resizeGBuffer(int viewportWidth, int viewportHeight)
{
    glDeleteFramebuffers(1, &GBufferFBOHandle);

    SetUpGbufferFBO(viewportWidth, viewportHeight);
}
/******************************************************************************/
/*!
  \brief
    return current using shader type to client

  \return stype
    current shader type
*/
/******************************************************************************/
enum cs250::PhongRender::shader_type cs250::PhongRender::getShaderType(void) const
{
    return stype;
}

void cs250::PhongRender::setUpWireframeCube()
{
    const glm::vec4 vertices[] = { {1.0f, 1.0f, 1.0f, 1.0f},
                                           {1.0f, 1.0f, -1.0f, 1.0f},
                                           {1.0f, -1.0f, 1.0f, 1.0f},
                                           {1.0f, -1.0f, -1.0f, 1.0f},
                                           {-1.0f, 1.0f, 1.0f, 1.0f},
                                           {-1.0f, 1.0f, -1.0f, 1.0f},
                                           {-1.0f, -1.0f, 1.0f, 1.0f},
                                           {-1.0f, -1.0f, -1.0f, 1.0f} };

    const cs250::Mesh::Edge edges[] = { {0, 1},
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
                                                        {6, 7} };
    glUseProgram(wProgram);


    glGenBuffers(1, &cubeVertexBufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * 8,
        vertices, GL_STATIC_DRAW);


    glGenBuffers(1, &cubeEdgeBufferHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEdgeBufferHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(cs250::Mesh::Edge) * 12,
        edges, GL_STATIC_DRAW);




    
    glGenVertexArrays(1, &cubeVAOHandle);

    glBindVertexArray(cubeVAOHandle);

    GLint aposition = glGetAttribLocation(wProgram, "position");
    //glBindBuffer(GL_ARRAY_BUFFER, mesh_data[mi].buffer_objects[MeshData::VERT]);
    glVertexAttribPointer(aposition, 4, GL_FLOAT, false, 0, 0);
    glEnableVertexAttribArray(aposition);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEdgeBufferHandle);

    glBindVertexArray(0);
}
