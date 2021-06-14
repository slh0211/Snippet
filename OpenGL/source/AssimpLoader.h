/******************************************************************************/
/*!
\file   AssimpLoader.h
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS350
\par    Assignment 1 Deferred Shading
\par    Summer 2020
\date   1/18/2020
\brief
    This file contains the prototype of  AssimpLoader functions

  Hours spent on this assignment: 

  Specific portions that gave you the most trouble:

*/
/******************************************************************************/
#pragma once
#include <string>
#include "FlatShadedMesh.h"
#include <assimp/scene.h>
#include "Scene_Two.h"

void loadModel(const std::string& path);
void loadModelToMesh(const std::string& path, Scene_Two* myscene);

void processNode(aiNode* node, const aiScene* scene, Scene_Two* myscene);

NMesh processMesh(aiMesh* mesh, const aiScene* scene);