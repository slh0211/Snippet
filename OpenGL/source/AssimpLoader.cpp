/******************************************************************************/
/*!
\file   AssimpLoader.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS350
\par    Assignment 1 Deferred Shading
\par    Summer 2020
\date   1/18/2020
\brief
    This file contains the implemntation of  AssimpLoader functions

  Hours spent on this assignment:

  Specific portions that gave you the most trouble:

*/
/******************************************************************************/
#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

#include "AssimpLoader.h"



void loadModel(const std::string& path)
{
    std::cout << path << std::endl;

	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

    //std::cout << scene->mNumMeshes << std::endl;
}
/******************************************************************************/
/*!
  \brief
    Loads mesh using assimp to Scene_Two class
    
  \param path
    file path

  \param myscene
    Scene class where everything stored
*/
/******************************************************************************/
void loadModelToMesh(const std::string& path, Scene_Two* myscene)
{
    std::cout << path.c_str() << std::endl;

    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

    processNode(scene->mRootNode, scene, myscene);
}
/******************************************************************************/
/*!
  \brief
    helper class to process aiNode

  \param node
    aiNode contains multiple meshes

  \param scene
    aiScene contains multiple nodes

  \param myscene
    Scene class where everything stored
*/
/******************************************************************************/
void processNode(aiNode* node, const aiScene* scene, Scene_Two* myscene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        //meshes.push_back(processMesh(mesh, scene));
        //std::cout << mesh->mNumVertices << std::endl;
        
        NMesh mymesh = processMesh(mesh, scene);

        myscene->loadMesh(mymesh);
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, myscene);
    }
}
/******************************************************************************/
/*!
  \brief
    helper class to process aiMesh

  \param mesh
    aiMesh used by assimp to store mesh datas

  \param scene
    aiScene contains multiple nodes
*/
/******************************************************************************/
NMesh processMesh(aiMesh* mesh, const aiScene* scene)
{
    NMesh mymesh;

    glm::vec3 min = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    glm::vec3 max = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    //GLfloat x, y, z;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;

        if (min.x > vector.x)
            min.x = vector.x;
        if (max.x <= vector.x)
            max.x = vector.x;

        if (min.y > vector.y)
            min.y = vector.y;
        if (max.y <= vector.y)
            max.y = vector.y;

        if (min.z > vector.z)
            min.z = vector.z;
        if (max.z <= vector.z)
            max.z = vector.z;

        mymesh.vertices.push_back(glm::vec4(vector, 1.0f));

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;

        mymesh.normals.push_back(glm::vec4(vector, 0.0f));
    }

    mymesh._center = cs250::point((min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f, (min.z + max.z) / 2.0f);
    mymesh._dimensions = cs250::vector(max.x - min.x, max.y - min.y, max.z - min.z);

    for (unsigned int t = 0; t < mesh->mNumFaces; ++t) {
        aiFace* aiface = &mesh->mFaces[t];
        //printf("%d: %d %d\n", t, aiface->mNumIndices, aimesh->mNumFaces);
        for (unsigned int i = 2; i < aiface->mNumIndices; i++) {
            mymesh.faces.push_back(cs250::Mesh::Face(aiface->mIndices[0],
                aiface->mIndices[1],
                aiface->mIndices[2]));
        }
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    aiColor3D color;

    material->Get(AI_MATKEY_COLOR_AMBIENT, color);

    mymesh.Ka = glm::vec3(color.r, color.g, color.b);

    material->Get(AI_MATKEY_COLOR_DIFFUSE, color);

    mymesh.Kd = glm::vec3(color.r, color.g, color.b);

    material->Get(AI_MATKEY_COLOR_SPECULAR, color);

    mymesh.Ks = glm::vec3(color.r, color.g, color.b);

    float Ns;

    material->Get(AI_MATKEY_SHININESS, Ns);

    mymesh.Ns = Ns;

    return mymesh;
}
