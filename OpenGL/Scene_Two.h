/******************************************************************************/
/*!
\file   Scene_Two.h
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS300
\par    Assignment 1
\par    Fall 2020
\date   10/1/2020
\brief
    This file contains the prototype of Scene_Two class

  Hours spent on this assignment: 

  Specific portions that gave you the most trouble:

*/
/******************************************************************************/
/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Scene_Two.h
Purpose: Prototype for a Scene_Two class
Language: C++
Platform: visual studio
Project: r.shen_CS300_1
Author: Richard Shen, r.shen, 110000516
Creation date: 10/1/2020
End Header --------------------------------------------------------*/
#pragma once
#include "../Common/Scene.h"
#include "PhongRender.h"
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

//#include "ShaderProgram.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "FlatShadedMesh.h"
//#include <string>

#include "Geometry.h"
#include "Node.h"
#include "BSPNode.h"
#include "MovingSphere.h"

class Scene_Two : public Scene
{

public:
    Scene_Two() = default;
    Scene_Two(int windowWidth, int windowHeight);
    virtual ~Scene_Two() override;


public:
    void SetupImGUI(GLFWwindow* pWindow) override;
    void CleanupImGUI() override;
    int Init() override;
    void CleanUp() override;

    int preRender() override;
    int Render() override;
    int postRender() override;
    void Resize(GLFWwindow* pWindow, int width, int height) override;

    virtual void Keyboard(GLFWwindow* pWindow, int key, int scancode, int action, int mods) override;

    void loadMesh(const NMesh& mesh);


private:

    // member functions
    void initMembers();

    // This is the non-software engineered portion of the code
    // Go ahead and modularize the VAO and VBO setup into
    // BufferManagers and ShaderManagers
    void SetupBuffers();


    void SetupNanoGUI(GLFWwindow* pWwindow) override;

    
    //void CleanupImGUI(GLFWwindow* pWwindow);
    glm::vec4 d;
    glm::vec4 c;

    glm::vec4 quad_d;
    glm::vec4 quad_c;

    // data members

    //std::vector<GLfloat>    geometryBuffer;
    cs250::PhongRender render;
    //Shader orbit_shader;

    cs250::Camera camera0;
    cs250::Camera camera1;
    cs250::Camera camera2;
    cs250::Camera camera3;
    cs250::Camera camera4;
    cs250::Camera camera5;
    cs250::Camera camera6;
    Uint32 ticks_last;
    double time;
    float _aspect;

    //GLuint  vertexbuffer;
    //GLuint  indexbuffer;
    //GLuint  VertexArrayID;
    //GLuint  programID;

    //std::vector<glm::vec3> vertices;
    //std::vector<unsigned int> indices;

    bool drawNormal;
    bool drawFaceNormal;
    bool pause;
    float ambient[3];
    float inner;
    float outer;
    float fallout;
    bool pointLightUse;
    bool directionalLightUse;
    bool spotLightUse;
    int pointLightCount;
    int directionalLightCount;
    int spotLightCount;

    bool pointLightMultiColor;
    bool directionalLightMultiColor;
    bool spotLightMultiColor;

    float globalAmbientColor[3];
    float fogColor[3];

    bool gpu_use;
    int ventity_type;
    int mapping_type;

    std::vector<glm::vec2> cpuTexCoordsPlan;
    std::vector<glm::vec2> cpuTexCoordsCube;
    std::vector<glm::vec2> cpuTexCoordsCyli;
    std::vector<glm::vec2> cpuTexCoordsSphe;

    std::vector<glm::vec2> cpuTexCoordsNorPlan;
    std::vector<glm::vec2> cpuTexCoordsNorCube;
    std::vector<glm::vec2> cpuTexCoordsNorCyli;
    std::vector<glm::vec2> cpuTexCoordsNorSphe;

    int skyboxMeshIndex;
    int mainMeshIndex;
    int sphereMeshIndex;
    int quadMeshIndex;

    int avatarMeshIndex;


    int currentCamera;
    int currentTransparentMode;
    bool shading_use;
    float eta_in;
    float eta_out;
    float epsilon;

    int currentGBuffer;
    int currentDebugDrawingMode;

    std::vector<glm::mat4> sphereMMT;
    std::vector<glm::vec3> colors;
    std::vector<glm::vec3> colorsAMB;
    std::vector<glm::vec4> spherePositions;
    glm::mat4 mainMMT;

    glm::vec4 mainCenter;

    glm::mat4 avatarMMT;
    glm::vec4 avatarPosition;

    glm::mat4 movingSphereMMT;


    std::vector<NMesh> meshes;

    MovingSphere ms;
    float deltaTime;


    void drawSpheres(void);
    std::vector<std::string> filenames;
    std::vector<std::string> readTextFile(const std::string filename);

    std::vector<int> meshIndexes;
    std::vector<glm::mat4> modelMatrices;

    std::vector<NMesh> testMeshes;
    std::vector<int> testMeshIndices;
    std::vector<glm::mat4> testMMT;

    Node* octreeRoot;
    BSPNode* bsptreeRoot;

    std::vector<Object> objectVector;


    bool bCopyDepthInfo;
    bool bDisplayOctree;
    bool bCollide;
    bool bCollisionDebugger;

    Node* hitRecord;

    void drawAvatar(void);
    void drawMovingSphere(void);

    void readTestCommands(const std::string filename);
    void executeCommand(const std::vector<std::string>& strings, const std::vector<float>& f);
    void printTestResult(bool result);
    void drawOctree(Node* leaf);
    void drawOctree(Node* leaf, int depth);

    void drawOctreeCollision(Node* leaf);
    void drawHitNode();

    void drawBSPTree(BSPNode* leaf);
    glm::vec3 giveMeAColor(int number);

    void drawObjects(const std::vector<Object>& objects);
    void drawWireframeNode(const std::vector<glm::vec4>& vertices, const glm::vec3& color);

    void convertMeshesToObject();

    bool DetectCollision_GJK(const MovingSphere& s, Node* treeNode) const;

    bool DetectCollision_MidPhase(const MovingSphere& s, Node* treeNode);

    bool DetectCollision_BroadPhase(const MovingSphere& s, Node* treeNode);
};
