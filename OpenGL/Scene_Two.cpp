/******************************************************************************/
/*!
\file   Scene_Two.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS300
\par    Assignment 1
\par    Fall 2020
\date   10/1/2020
\brief
    This file contains the implementation of Scene_Two class

  Hours spent on this assignment:

  Specific portions that gave you the most trouble:

*/
/******************************************************************************/

/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Scene_Two.cpp
Purpose: implementation for a Scene_Two class
Language: C++
Platform: visual studio
Project: r.shen_CS300_1
Author: Richard Shen, r.shen, 110000516
Creation date: 10/1/2020
End Header --------------------------------------------------------*/
#include "Scene_Two.h"
#include "shader.hpp"
#include <glm/vec3.hpp>
#include "ObjLoader.h"
#include <algorithm>
#include "FlatShadedMesh.h"
#include "SphereMesh.h"
#include "Affine.h"
#include "CubeMesh.h"
#include "SkyboxMesh.h"
//#include <glm/detail/type_mat.hpp>

#include "AssimpLoader.h"
#include <fstream>
#include <iostream>
#include <sstream>

#include "Octree.h"
#include "BSPTree.h"
#include "MovingSphere.h"

const float CAM_RATE = glm::radians(360.0f / 7.0f);

const float one_eighth = glm::radians(45.0f);

const glm::vec4 O = cs250::point(0.0f, 0.0f, 0.0f),
EX = cs250::vector(1.0f, 0.0f, 0.0f),
EY = cs250::vector(0.0f, 1.0f, 0.0f),
EZ = cs250::vector(0.0f, 0.0f, 1.0f);


const glm::vec4 EARTH_CENTER = glm::vec4(4.0f, 5.0f, 0.0f, 1.0f),
EARTH_AXIS = EX + 2.0f * EY + EZ,
MOON_AXIS = EX + EY + 3.0f * EZ;

const float MOON_ANGLE = cs250::angle(EY, MOON_AXIS);
const glm::vec4  MOON_VEC = cs250::cross(EY, MOON_AXIS);
const glm::mat4 MOON_TILT = cs250::rotate(MOON_ANGLE, MOON_VEC);

const float EARTH_SIZE = 2.0f,
EARTH_RATE = 360.0f / 5.0f;

const float MOON_SIZE = 1.0f,
MOON_RATE = 360.0f / 2.5f;

const glm::vec3 EARTH_DIFF(1.0f, 0.5f, 0.5f),
WHITE(1.0f, 1.0f, 1.0f),
BLACK(0.0f, 0.0f, 0.0f),
RED(1.0f, 0.0f, 0.0f),
GREEN(0.0f, 1.0f, 0.0f),
BLUE(0.0f, 0.0f, 1.0f),
CYAN(0.0f, 1.0f, 1.0f),
MAGENTA(1.0f, 0.0f, 1.0f),
YELLOW(1.0f, 1.0f, 0.0f),
GRAY(0.5f, 0.5f, 0.5f),
DARKGRAY(0.1f, 0.1f, 0.1f),
DARKRED(0.1f, 0.0f, 0.0f),
DARKGREEN(0.0f, 0.1f, 0.0f),
DARKBLUE(0.0f, 0.0f, 0.1f),
DARKCYAN(0.0f, 0.1f, 0.1f),
DARKMAGENTA(0.1f, 0.0f, 0.1f),
DARKYELLOW(0.1f, 0.1f, 0.0f),
DARKERGRAY(0.05f, 0.05f, 0.05f),
PURPLE(0.5f, 0.0f, 0.5f),
ORANGE(1.0f, 0.6f, 0.0f),
LIMEPULP(0.83f, 0.93f, 0.57f),
LATTE(0.62f, 0.42f, 0.23f),
HALFGREEN(0.0f, 0.5f, 0.0f);

//glm::vec4 CAM_POS = O + EX* 7.0f + 7.0f * EZ + 7.0f * EY;
glm::vec4 CAM_POS = O + EX * 10.0f + 10.0f * EZ + 10.0f * EY;
//glm::vec4 CAM_POS = O + 3.0f * EZ + EY * 1.0f;
//glm::vec4 CAM_POS = O + 2.9f * EZ + EY;
const float FOV = 80.0f,
NEAR = 0.1f,
FAR = 30.0f;

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
void Scene_Two::SetupNanoGUI(GLFWwindow* pWindow)
{
    pWindow = nullptr;
}
/******************************************************************************/
/*!
  \brief
    helper function to draw debug spheres
*/
/******************************************************************************/
void Scene_Two::drawSpheres(void)
{
    for (int i = 0; i < 8; i++)
    {
        // set model
        render.setModel(sphereMMT[i]);

        if (pointLightUse && pointLightCount > i)
        {
            render.setIlluminatorColor(colors[i]);
            render.drawIlluminator(sphereMeshIndex);
        }
        else
        {
            render.setMaterial(colorsAMB[i], colors[i], DARKGRAY, 10.0f);
            render.draw(sphereMeshIndex);
        }
    }
}
/******************************************************************************/
/*!
  \brief
    helper function to read textfile that containing path of obj files

  \param filename
    filepath of the txt file

  \return outvec
    a vector contain all the paths of obj files to be read in
*/
/******************************************************************************/
std::vector<std::string> Scene_Two::readTextFile(const std::string filename)
{
    std::string line;
    std::vector<std::string> outvec;
    std::ifstream myfile(filename.c_str());
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            outvec.push_back(line);
        }
        myfile.close();
    }

    else 
      std::cout << "Unable to open file" << std::endl;


    return outvec;
}
void Scene_Two::drawAvatar(void)
{
    render.setModel(avatarMMT);
    render.setMaterial(BLUE, BLUE, DARKGRAY, 10.0f);

    //render.draw(avatarMeshIndex);
    render.setWireframeColor(GREEN);
    render.drawWireframe(avatarMeshIndex);
    //render.drawWireframeCube();
}
void Scene_Two::drawMovingSphere(void)
{
    render.setModel(movingSphereMMT);
    render.setMaterial(BLUE, BLUE, DARKGRAY, 10.0f);
    render.draw(sphereMeshIndex);
}
void Scene_Two::readTestCommands(const std::string filename)
{
    std::ifstream input(filename.c_str());
    if (input.fail()) {
        std::cerr << "File not found: " << filename << std::endl;
        fflush(stderr);
        exit(-1);
    }

    // For each line in file
    for (std::string line; getline(input, line); ) {
        std::vector<std::string> strings;
        std::vector<float> floats;

        // Parse as parallel lists of strings and floats
        std::stringstream lineStream(line);
        for (std::string s; lineStream >> s; ) { // Parses space-separated strings until EOL
            float f;
            //std::stringstream(s) >> f; // Parses an initial float into f, or zero if illegal
            if (!(std::stringstream(s) >> f)) f = float(nan("")); // An alternate that produced NANs
            floats.push_back(f);
            strings.push_back(s);
        }

        if (strings.size() == 0) continue; // Skip blanks lines
        if (strings[0][0] == '#') continue; // Skip comment lines

        // Pass the line's data to Command(...)
        executeCommand(strings, floats);
        //scene->Command(strings, floats);
    }

    input.close();
}
void Scene_Two::executeCommand(const std::vector<std::string>& strings, const std::vector<float>& f)
{
    if (strings.size() == 0) return;
    std::string c = strings[0];

    if (c == "pointsphere") {
        // syntax: screen width height
        //realtime->setScreen(int(f[1]), int(f[2]));
        //width = int(f[1]);
        //height = int(f[2]);
        //_pRayTracer->setScreen(int(f[1]), int(f[2]));
        std::cout << "Test: Point vs Sphere: ";
        std::cout << "Point (" << f[1] << " " << f[2] << " " << f[3] << ") ";
        std::cout << "Sphere center (" << f[4] << " " << f[5] << " " << f[6] << ") radius " << f[7] <<std::endl;

        Point3D p = Point3D(f[1], f[2], f[3]);
        Sphere  s = Sphere(glm::vec3(f[4], f[5], f[6]), f[7]);

        bool result = PointSphere(p, s);

        printTestResult(result);
    }
    else if (c == "spheresphere") {
        std::cout << "Test: Sphere vs Sphere: ";
        std::cout << "Sphere a center (" << f[1] << " " << f[2] << " " << f[3] << ") radius " << f[4] << " ";
        std::cout << "Sphere b center (" << f[5] << " " << f[6] << " " << f[7] << ") radius " << f[8] << std::endl;

        Sphere  a = Sphere(glm::vec3(f[1], f[2], f[3]), f[4]);
        Sphere  b = Sphere(glm::vec3(f[5], f[6], f[7]), f[8]);

        bool result = SphereSphere(a, b);

        printTestResult(result);
    }
    else if (c == "pointaabb") {
        std::cout << "Test: Point vs Aabb: ";
        std::cout << "Point (" << f[1] << " " << f[2] << " " << f[3] << ") ";
        std::cout << "AAbb min (" << f[4] << " " << f[5] << " " << f[6] << ") max (" << f[7] << " " << f[8] << " " << f[9] << ")"<< std::endl;

        Point3D p = Point3D(f[1], f[2], f[3]);
        Aabb aabb = Aabb(glm::vec3(f[4], f[5], f[6]), glm::vec3(f[7], f[8], f[9]));

        bool result = PointAabb(p, aabb);

        printTestResult(result);
    }
    else if (c == "aabbaabb")
    {
        std::cout << "Test: AAbb vs Aabb: ";
        std::cout << "Aabb a min (" << f[1] << " " << f[2] << " " << f[3] << ") max (" << f[4] << " " << f[5] << " " << f[6] << ")" << " ";
        std::cout << "AAbb b min (" << f[7] << " " << f[8] << " " << f[9] << ") max (" << f[10] << " " << f[11] << " " << f[12] << ")" << std::endl;

        Aabb aabbA = Aabb(glm::vec3(f[1], f[2], f[3]), glm::vec3(f[4], f[5], f[6]));
        Aabb aabbB = Aabb(glm::vec3(f[7], f[8], f[9]), glm::vec3(f[10], f[11], f[12]));

        bool result = AabbAabb(aabbA, aabbB);

        printTestResult(result);
    }
    else if (c == "aabbsphere")
    {
        std::cout << "Test: AAbb vs Sphere: ";
        std::cout << "Aabb min (" << f[1] << " " << f[2] << " " << f[3] << ") max (" << f[4] << " " << f[5] << " " << f[6] << ")" << " ";
        std::cout << "Sphere center (" << f[7] << " " << f[8] << " " << f[9] << ") radius " << f[10] << std::endl;

        Aabb aabb = Aabb(glm::vec3(f[1], f[2], f[3]), glm::vec3(f[4], f[5], f[6]));
        Sphere  s = Sphere(glm::vec3(f[7], f[8], f[9]), f[10]);

        bool result = AabbSphere(aabb, s);

        printTestResult(result);
    }
    else if (c == "sphereaabb")
    {
        std::cout << "Test: Sphere vs Aabb: ";
        std::cout << "Sphere center (" << f[1] << " " << f[2] << " " << f[3] << ") radius " << f[4] << " ";
        std::cout << "AAbb min (" << f[5] << " " << f[6] << " " << f[7] << ") max (" << f[8] << " " << f[9] << " " << f[10] << ")" << std::endl;

        Aabb aabb = Aabb(glm::vec3(f[5], f[6], f[7]), glm::vec3(f[8], f[9], f[10]));
        Sphere  s = Sphere(glm::vec3(f[1], f[2], f[3]), f[4]);

        bool result = SphereAabb(s, aabb);

        printTestResult(result);
    }
    else if (c == "pointtriangle")
    {
        std::cout << "Test: Point vs Triangle: ";
        std::cout << "Point (" << f[1] << " " << f[2] << " " << f[3] << ") ";
        std::cout << "Triangle a (" << f[4] << " " << f[5] << " " << f[6] << 
                            ") b (" << f[7] << " " << f[8] << " " << f[9] << 
                            ") c (" << f[10] << " " << f[11] << " " << f[12] << ")" << std::endl;

        Point3D p = Point3D(f[1], f[2], f[3]);
        glm::vec3 v0 = glm::vec3(f[4], f[5], f[6]);
        glm::vec3 v1 = glm::vec3(f[7], f[8], f[9]);
        glm::vec3 v2 = glm::vec3(f[10], f[11], f[12]);

        Triangle t = Triangle(v0, v1, v2);

        bool result = PointTriangle(p, t);

        printTestResult(result);
    }
    else if (c == "pointplane")
    {
        std::cout << "Test: Point vs Plane: ";
        std::cout << "Point (" << f[1] << " " << f[2] << " " << f[3] << ") ";
        std::cout << "Plane N (" << f[4] << " " << f[5] << " " << f[6] << ") D " << f[7] << std::endl;

        Point3D point = Point3D(f[1], f[2], f[3]);
        Plane plane = Plane(f[4], f[5], f[6], f[7]);

        bool result = PointPlane(point, plane);

        printTestResult(result);
    }
    else if (c == "rayplane")
    {
        std::cout << "Test: Ray vs Plane: ";
        std::cout << "Ray starting point (" << f[1] << " " << f[2] << " " << f[3] << ") direction (" << f[4] << " " << f[5] << " " << f[6] << ")" << " ";
        std::cout << "Plane N (" << f[7] << " " << f[8] << " " << f[9] << ") D " << f[10] << std::endl;

        Ray ray = Ray(glm::vec3(f[1], f[2], f[3]), glm::vec3(f[4], f[5], f[6]));
        Plane plane = Plane(f[7], f[8], f[9], f[10]);

        bool result = RayPlane(ray, plane);

        printTestResult(result);
    }
    else if (c == "rayaabb")
    {
        std::cout << "Test: Ray vs Aabb: ";
        std::cout << "Ray starting point (" << f[1] << " " << f[2] << " " << f[3] << ") direction (" << f[4] << " " << f[5] << " " << f[6] << ")" << " ";
        std::cout << "AAbb min (" << f[7] << " " << f[8] << " " << f[9] << ") max (" << f[10] << " " << f[11] << " " << f[12] << ")" << std::endl;

        Ray ray = Ray(glm::vec3(f[1], f[2], f[3]), glm::vec3(f[4], f[5], f[6]));
        Aabb aabb = Aabb(glm::vec3(f[7], f[8], f[9]), glm::vec3(f[10], f[11], f[12]));

        bool result = RayAabb(ray, aabb);

        printTestResult(result);
    }
    else if (c == "raysphere")
    {
        std::cout << "Test: Ray vs Sphere: ";
        std::cout << "Ray starting point (" << f[1] << " " << f[2] << " " << f[3] << ") direction (" << f[4] << " " << f[5] << " " << f[6] << ")" << " ";
        std::cout << "Sphere center (" << f[7] << " " << f[8] << " " << f[9] << ") radius " << f[10] << std::endl;

        Ray ray = Ray(glm::vec3(f[1], f[2], f[3]), glm::vec3(f[4], f[5], f[6]));
        Sphere  s = Sphere(glm::vec3(f[7], f[8], f[9]), f[10]);

        bool result = RaySphere(ray, s);

        printTestResult(result);
    }
    else if (c == "raytriangle")
    {
        std::cout << "Test: Ray vs Triangle: ";
        std::cout << "Ray starting point (" << f[1] << " " << f[2] << " " << f[3] << ") direction (" << f[4] << " " << f[5] << " " << f[6] << ")" << " ";
        std::cout << "Triangle a (" << f[7] << " " << f[8] << " " << f[9] <<
                            ") b (" << f[10] << " " << f[11] << " " << f[12] <<
                            ") c (" << f[13] << " " << f[14] << " " << f[15] << ")" << std::endl;

        Ray ray = Ray(glm::vec3(f[1], f[2], f[3]), glm::vec3(f[4], f[5], f[6]));
        glm::vec3 v0 = glm::vec3(f[7], f[8], f[9]);
        glm::vec3 v1 = glm::vec3(f[10], f[11], f[12]);
        glm::vec3 v2 = glm::vec3(f[13], f[14], f[15]);

        Triangle t = Triangle(v0, v1, v2);

        bool result = RayTriangle(ray, t);

        printTestResult(result);
    }
    else if (c == "planeaabb")
    {
        std::cout << "Test: Plane vs Aabb: ";
        std::cout << "Plane N (" << f[1] << " " << f[2] << " " << f[3] << ") D " << f[4] << " ";
        std::cout << "AAbb min (" << f[5] << " " << f[6] << " " << f[7] << ") max (" << f[8] << " " << f[9] << " " << f[10] << ")" << std::endl;

        Plane plane = Plane(f[1], f[2], f[3], f[4]);
        Aabb aabb = Aabb(glm::vec3(f[5], f[6], f[7]), glm::vec3(f[8], f[9], f[10]));

        bool result = PlaneAabb(plane, aabb);

        printTestResult(result);
    }
    else if (c == "planesphere")
    {
        std::cout << "Test: Plane vs Sphere: ";
        std::cout << "Plane N (" << f[1] << " " << f[2] << " " << f[3] << ") D " << f[4] << " ";
        std::cout << "Sphere center (" << f[5] << " " << f[6] << " " << f[7] << ") radius " << f[8] << std::endl;

        Plane plane = Plane(f[1], f[2], f[3], f[4]);
        Sphere  s = Sphere(glm::vec3(f[5], f[6], f[7]), f[8]);

        bool result = PlaneSphere(plane, s);

        printTestResult(result);
    }
    else if (c == "planetriangle")
    {
        std::cout << "Test: Plane vs Triangle: ";
        std::cout << "Plane N (" << f[1] << " " << f[2] << " " << f[3] << ") D " << f[4] << " ";
        std::cout << "Triangle a (" << f[5] << " " << f[6] << " " << f[7] <<
                            ") b (" << f[8] << " " << f[9] << " " << f[10] <<
                            ") c (" << f[11] << " " << f[12] << " " << f[13] << ")" << std::endl;

        Plane plane = Plane(f[1], f[2], f[3], f[4]);
        glm::vec3 v0 = glm::vec3(f[5], f[6], f[7]);
        glm::vec3 v1 = glm::vec3(f[8], f[9], f[10]);
        glm::vec3 v2 = glm::vec3(f[11], f[12], f[13]);

        Triangle t = Triangle(v0, v1, v2);

        bool result = PlaneTriangle(plane, t);

        printTestResult(result);
    }
    else {
        fprintf(stderr, "\n*********************************************\n");
        fprintf(stderr, "* Unknown command: %s\n", c.c_str());
        fprintf(stderr, "*********************************************\n\n");
    }
}
void Scene_Two::printTestResult(bool result)
{
    if (result)
        std::cout << "intersected !!" << std::endl;
    else
        std::cout << "not intersected !!" << std::endl;
}
void Scene_Two::drawOctree(Node* leaf)
{
    if (leaf != NULL)
    {
        drawOctree(leaf->_pChild[0]);
        drawOctree(leaf->_pChild[1]);
        drawOctree(leaf->_pChild[2]);
        drawOctree(leaf->_pChild[3]);
        drawOctree(leaf->_pChild[4]);
        drawOctree(leaf->_pChild[5]);
        drawOctree(leaf->_pChild[6]);
        drawOctree(leaf->_pChild[7]);

        render.setModel(leaf->_modelMatrix);
        render.setWireframeColor(GREEN);
        render.drawWireframeCube();
    }
}
void Scene_Two::drawOctree(Node* leaf, int depth)
{
    if (depth < 0)
        return;

    if (leaf != NULL)
    {


        render.setModel(leaf->_modelMatrix);

        glm::vec3 color = WHITE;

        switch (depth)
        {
            case 8:
                //render.setWireframeColor(ORANGE);
                color = ORANGE;
                break;
            case 7:
                //render.setWireframeColor(MAGENTA);
                color = MAGENTA;
                break;
            case 6:
                //render.setWireframeColor(LIMEPULP);
                color = LIMEPULP;
                break;
            case 5:
                //render.setWireframeColor(YELLOW);
                color = YELLOW;
                break;
            case 4:
                //render.setWireframeColor(CYAN);
                color = CYAN;
                break;
            case 3:
                //render.setWireframeColor(BLUE);
                color = BLUE;
                break;
            case 2:
                //render.setWireframeColor(PURPLE);
                color = PURPLE;
                break;
            case 1:
                //render.setWireframeColor(GREEN);
                color = LATTE;
                break;
            case 0:
                color = RED;
                //render.setWireframeColor(RED);
                break;
        }
        render.setWireframeColor(HALFGREEN);
        render.drawWireframeCube();
        //drawObjects(leaf->_objList);
        drawWireframeNode(leaf->_vertices, color);

        drawOctree(leaf->_pChild[0], depth - 1);
        drawOctree(leaf->_pChild[1], depth - 1);
        drawOctree(leaf->_pChild[2], depth - 1);
        drawOctree(leaf->_pChild[3], depth - 1);
        drawOctree(leaf->_pChild[4], depth - 1);
        drawOctree(leaf->_pChild[5], depth - 1);
        drawOctree(leaf->_pChild[6], depth - 1);
        drawOctree(leaf->_pChild[7], depth - 1);
    }
}
void Scene_Two::drawOctreeCollision(Node* leaf)
{
    if (leaf != NULL)
    {


        render.setModel(leaf->_modelMatrix);

        if(leaf != hitRecord)
        render.setWireframeColor(HALFGREEN);
        else
            render.setWireframeColor(RED);
        render.drawWireframeCube();
        //drawObjects(leaf->_objList);
        if(leaf == hitRecord)
            drawWireframeNode(leaf->_vertices, RED);
        else
            drawWireframeNode(leaf->_vertices, YELLOW);

        drawOctreeCollision(leaf->_pChild[0]);
        drawOctreeCollision(leaf->_pChild[1]);
        drawOctreeCollision(leaf->_pChild[2]);
        drawOctreeCollision(leaf->_pChild[3]);
        drawOctreeCollision(leaf->_pChild[4]);
        drawOctreeCollision(leaf->_pChild[5]);
        drawOctreeCollision(leaf->_pChild[6]);
        drawOctreeCollision(leaf->_pChild[7]);
    }
}
/******************************************************************************/
/*!
  \brief
    This function draws hitRecord's cube and vertices
*/
/******************************************************************************/
void Scene_Two::drawHitNode()
{
    render.setModel(hitRecord->_modelMatrix);
    render.drawWireframeCube();
    drawWireframeNode(hitRecord->_vertices, RED);

}
void Scene_Two::drawBSPTree(BSPNode* leaf)
{
    if (leaf != NULL)
    {
        drawBSPTree(leaf->_leftTree);
        drawBSPTree(leaf->_rightTree);
        if (leaf->_leftTree == NULL && leaf->_rightTree == NULL)
        {
            if (leaf->_vertices.size() != 0)
            {
                //render.setWireframeColor(HALFGREEN);
                glm::vec3 color = giveMeAColor(leaf->_vertices.size()/3);
                drawWireframeNode(leaf->_vertices, color);
            }
        }
    }
}
glm::vec3 Scene_Two::giveMeAColor(int number)
{
    int modNumber = number % 9;

    glm::vec3 color = WHITE;

    switch (modNumber)
    {
    case 8:
        //render.setWireframeColor(ORANGE);
        color = ORANGE;
        break;
    case 7:
        //render.setWireframeColor(MAGENTA);
        color = MAGENTA;
        break;
    case 6:
        //render.setWireframeColor(LIMEPULP);
        color = LIMEPULP;
        break;
    case 5:
        //render.setWireframeColor(YELLOW);
        color = YELLOW;
        break;
    case 4:
        //render.setWireframeColor(CYAN);
        color = CYAN;
        break;
    case 3:
        //render.setWireframeColor(BLUE);
        color = BLUE;
        break;
    case 2:
        //render.setWireframeColor(PURPLE);
        color = PURPLE;
        break;
    case 1:
        //render.setWireframeColor(GREEN);
        color = LATTE;
        break;
    case 0:
        color = RED;
        //render.setWireframeColor(RED);
        break;
    }


    return color;
}
// Test function when implementing drawing single triangle
void Scene_Two::drawObjects(const std::vector<Object>& objects)
{
    //render.setWireframeTriangleColor(GREEN);

    for (unsigned int i = 0; i < objects.size(); ++i)
    {
        render.drawWireframeTriangle(objects[i]);
    }
}
/******************************************************************************/
/*!
  \brief
    This function reads vertices in world space and send to render to 
    draw triangles in wireframe mode

  \param vertices
    vertex data in world space

  \param color
    color to be used
*/
/******************************************************************************/
void Scene_Two::drawWireframeNode(const std::vector<glm::vec4>& vertices, const glm::vec3& color)
{
    render.setWireframeColor(color);

    if (vertices.size() >= 3) 
    {
        render.drawWireframeTriangles(vertices);
    }
}
/******************************************************************************/
/*!
  \brief
    This function converts each loaded mesh's vertices in world space to Objects
    data and loads into one big chunk vector
*/
/******************************************************************************/
void Scene_Two::convertMeshesToObject()
{
    // for each mesh
    for (unsigned int i = 0; i < meshIndexes.size(); ++i)
    {
        const NMesh& mesh = meshes[i];
        // for each face
        for (int facesi = 0; facesi < mesh.faceCount(); ++facesi)
        {
            // get vertex index using face
            int i1 = mesh.faceArray()[facesi].index1;
            int i2 = mesh.faceArray()[facesi].index2;
            int i3 = mesh.faceArray()[facesi].index3;

            // use vertex index to get vertex
            glm::vec4 p0 = mesh.vertexArray()[i1];
            glm::vec4 p1 = mesh.vertexArray()[i2];
            glm::vec4 p2 = mesh.vertexArray()[i3];

            // apply model matrix to get vertex in world space
            glm::vec4 wp0 = modelMatrices[i] * p0;
            glm::vec4 wp1 = modelMatrices[i] * p1;
            glm::vec4 wp2 = modelMatrices[i] * p2;

            // use world vertex and construct an object
            objectVector.push_back(Object(wp0, wp1, wp2));

        }
    }


}
/******************************************************************************/
/*!
  \brief
   Check if leaf node collide with MovingSphere. MidPhase to traverse down the
   tree before

  \param s
    a moving sphere
  \param treeNode
    leaf Node
  \return
    true if collide else false
*/
/******************************************************************************/
bool Scene_Two::DetectCollision_MidPhase(const MovingSphere& s, Node* treeNode)
{
    if (treeNode == NULL)
        return false;
    // if treeNode == LEAF, then perform GJK



    if (treeNode->CheckLeafNode())
    {
        if (DetectCollision_GJK(s, treeNode))
        {
            // Render polygons of treeNode & sphere in RED color
            // do what ever (stop the scene, draw or record?)
            hitRecord = treeNode;
            return true;
        }
    }

    Aabb sphereBox = s.GetAabb();
    Aabb nodeAabb = treeNode->GetAabb();
    // standard box-box intersection
    if (!AabbAabb(sphereBox, nodeAabb))
    {
        return false;
    }

    // Now recurse through the child nodes
    for (unsigned int i = 0; i < 8; ++i)
    {
        if (DetectCollision_MidPhase(s, treeNode->_pChild[i]))
        {
            return true;
        }
    }
    // no intersection with the scene
    return false;
}
/******************************************************************************/
/*!
  \brief
   Check if leaf node collide with MovingSphere. BroadPhase to check top level

  \param s
    a moving sphere
  \param treeNode
    leaf Node
  \return
    true if collide else false
*/
/******************************************************************************/
bool Scene_Two::DetectCollision_BroadPhase(const MovingSphere& s, Node* treeNode)
{
    // The broad phase part of collision detection
    Aabb sphereBox = s.GetAabb();
    Aabb nodeAabb = treeNode->GetAabb();

    if (!AabbAabb(sphereBox, nodeAabb))
    {
        return false;
    }

    // boxes collide, move to mid-phase
    // In mid-phase, we check for intersection of S with internal nodes
    for (unsigned int i = 0; i < 8; ++i)
    {
        if (DetectCollision_MidPhase(s, treeNode->_pChild[i]))
        {
            return true;
        }
    }
    return false;
}
/******************************************************************************/
/*!
  \brief
   Potential approaches for GJK with S
    a. (pre-)Calculate the convex hull of treeNode geometry – most precise calculation
    
    b. use BBox of treeNode geometry – approximate solution
    
    c. Level-of-detail based pseudo-geometry for dynamic objects
    We will use approach b. for our assignment

    Check if leaf node collide with MovingSphere

  \param s
    a moving sphere
  \param treeNode
    leaf Node
  \return 
    true if collide else false
*/
/******************************************************************************/
bool Scene_Two::DetectCollision_GJK(const MovingSphere& s, Node* treeNode) const
{
    
    Aabb sphereBox = s.GetAabb();
    Aabb nodeAabb = treeNode->GetAabb();
    
    // Use regular Aabb detection or Minkowski Difference?
    // Minkowski
    Aabb MinkowskiDifference = sphereBox.minkowskiDifference(nodeAabb);
    return MinkowskiDifference.Colliding();

    //// Normal Aabb detection
    //return AabbAabb(sphereBox, nodeAabb);
}
/******************************************************************************/
/*!
  \brief
    Dtor of Scene_Two
*/
/******************************************************************************/
Scene_Two::~Scene_Two()
{
    CleanUp();
}
/******************************************************************************/
/*!
  \brief
    Ctor of Scene_TWo

  \param windowWidth
    windowWidth

  \param windowHeight
    windowHeight
*/
/******************************************************************************/
Scene_Two::Scene_Two(int windowWidth, int windowHeight) : Scene(windowWidth, windowHeight)
{
    initMembers();
    ticks_last = SDL_GetTicks();    
}

//#pragma clang diagnostic push
//#pragma ide diagnostic ignored "bugprone-unused-return-value"
/******************************************************************************/
/*!
  \brief
    init members
*/
/******************************************************************************/
void Scene_Two::initMembers()
{

    time = 0.0;
    //time = glfwGetTime();
    _aspect = static_cast<float>(_windowWidth) / static_cast<float>(_windowHeight);


    sphereMMT = std::vector<glm::mat4>(8, glm::mat4(1.0f));
    colors = std::vector<glm::vec3>(8, WHITE);
    colorsAMB = std::vector<glm::vec3>(8, DARKGRAY);
    spherePositions = std::vector<glm::vec4>(8, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

    globalAmbientColor[0] = 0.2f;
    globalAmbientColor[1] = 0.2f;
    globalAmbientColor[2] = 0.2f;

    fogColor[0] = 0.0f;
    fogColor[1] = 0.0f;
    fogColor[2] = 0.3f;

    bCopyDepthInfo = true;

    pointLightUse = true;
    pointLightCount = 1;
    directionalLightCount = 1;
    spotLightCount = 1;
    pointLightMultiColor = true;

    inner = 4.0f;
    outer = 12.0f;

    camera0 = cs250::Camera(CAM_POS, mainCenter - CAM_POS, EY, FOV, _aspect, NEAR, FAR);
    camera1 = cs250::Camera(mainCenter, -EZ, EY, 90.0f, 1.0f, NEAR, FAR);
    camera2 = cs250::Camera(mainCenter, EX, EY, 90.0f, 1.0f, NEAR, FAR);
    camera3 = cs250::Camera(mainCenter, EZ, EY, 90.0f, 1.0f, NEAR, FAR);
    camera4 = cs250::Camera(mainCenter, -EX, EY, 90.0f, 1.0f, NEAR, FAR);
    camera5 = cs250::Camera(mainCenter, EY, -EZ, 90.0f, 1.0f, NEAR, FAR);
    camera6 = cs250::Camera(mainCenter, -EY, EZ, 90.0f, 1.0f, NEAR, FAR);

    currentCamera = 0;
    currentTransparentMode = 0;
    eta_in = 1.0f;
    eta_out = 1.33f;
    epsilon = 0.05f;

    currentGBuffer = 0;
    currentDebugDrawingMode = 1;

    //avatarMMT = glm::mat4(1.0f);

    avatarMMT = cs250::scale(5.0f);
    avatarPosition = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

    //ms = MovingSphere(O, 2.0f, glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
    //ms = MovingSphere(camera0.eye(), 0.5f, -camera0.back(), 0.0f);
    movingSphereMMT = cs250::translate(camera0.eye()) * cs250::scale(0.5f);
    hitRecord = NULL;
    bCollisionDebugger = true;
    //octreeRoot = BuildOctree(O, 5.0f, 1);
}
//#pragma clang diagnostic pop
/******************************************************************************/
/*!
  \brief
    Clean Up
*/
/******************************************************************************/
void Scene_Two::CleanUp()
{
    // Cleanup VBO
    //glDeleteBuffers(1, &indexbuffer);
    //glDeleteBuffers(1, &vertexbuffer);
    //glDeleteVertexArrays(1, &VertexArrayID);
    //glDeleteProgram(programID);
    DeleteOctree(octreeRoot);
    DeleteBSPTree(bsptreeRoot);
}
/******************************************************************************/
/*!
  \brief
    SetUpBuffers
*/
/******************************************************************************/
void Scene_Two::SetupBuffers()
{
    //glUseProgram(programID);
    //glGenVertexArrays(1, &VertexArrayID);
    //glBindVertexArray(VertexArrayID);
    //
    //glGenBuffers(1, &vertexbuffer);
    //
    //glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    //glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
    //    vertices.data(), GL_STATIC_DRAW);
    //
    //glGenBuffers(1, &indexbuffer);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    //
    //glBindVertexArray(0);

    return;
}
/******************************************************************************/
/*!
  \brief
    Set up ImGUI

  \param pWindow
    main context window
*/
/******************************************************************************/
void Scene_Two::SetupImGUI(GLFWwindow* pWindow)
{
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(pWindow, true);

    ImGui::StyleColorsDark();
}
/******************************************************************************/
/*!
  \brief
    Clean uo ImGUI
*/
/******************************************************************************/
void Scene_Two::CleanupImGUI()
{
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
}
/******************************************************************************/
/*!
  \brief
    read files to load meshes, set up model matrices, set up GBufferFBO
*/
/******************************************************************************/
int Scene_Two::Init()
{
    //programID = LoadMultiShaders("res/shaders/OrbitShader.vert", "res/shaders/OrbitShader.frag",
    //    "res/shaders/OrbitShader.geom");

    //cs250::SkyboxMesh skb = cs250::SkyboxMesh();
    //skyboxMeshIndex = render.loadMesh(skb);

    // load a text file

    //std::string userFileNamesTextString;
    //std::cout << "Enter the input textfile's file name: ";
    //std::getline(std::cin, userFileNamesTextString);
    //std::cout << "Read in " << userFileNamesTextString << std::endl;
    //filenames = readTextFile("..\\" + userFileNamesTextString);

    //filenames = readTextFile("..\\debug.txt");
    filenames = readTextFile("..\\Section21.txt");
    //filenames = readTextFile("..\\Section1.txt");
    
    

    //bool res = loadOBJtoMesh("Common/models/sphere.obj", mesh);
    //loadModel("Common/models/cube2.obj");
    //loadModelToMesh("Common/models/sphere.obj", this);

    for (unsigned i = 0; i < filenames.size(); ++i)
    {
        NMesh mesh;
        std::string path = "..\\";
        path += filenames[i];
        loadModelToMesh(path, this);
        //loadModelToMesh("Common/PowerPlant_Section1/ppsection1/part_g/g0.obj", this);
        //loadModelToMesh("ppsection1/part_g/g0.obj", this);
        //loadModelToMesh("ppsection1/g0.obj", this);
    
        //loadModelToMesh("Common/models/g0.obj", this);
        //loadModelToMesh("Common/models/g1.obj", this);
        //loadModelToMesh("ppsection1/g0.obj", this);
        //loadModelToMesh("Common/models/g0.obj", this);
        //loadModelToMesh("Common/models/part_g/g0.obj", this);
        mesh = meshes[i];
    
        cpuTexCoordsPlan = mesh.createTCP(NMesh::PLAN, 0);
        //cpuTexCoordsCube = mesh.createTCP(NMesh::CUBE, 0);
        //cpuTexCoordsCyli = mesh.createTCP(NMesh::CYLI, 0);
        //cpuTexCoordsSphe = mesh.createTCP(NMesh::SPHE, 0);
    
        //cpuTexCoordsNorPlan = mesh.createTCP(NMesh::PLAN, 1);
        //cpuTexCoordsNorCube = mesh.createTCP(NMesh::CUBE, 1);
        //cpuTexCoordsNorCyli = mesh.createTCP(NMesh::CYLI, 1);
        //cpuTexCoordsNorSphe = mesh.createTCP(NMesh::SPHE, 1);
        mesh.texCoords = cpuTexCoordsPlan;
        //mainMeshIndex = render.loadMesh(mesh);
        meshIndexes.push_back(render.loadMesh(mesh));
    }


    render.backfaceCull(true);

    d = meshes[0].dimensions();
    c = meshes[0].center();

    cs250::NormalMesh* sphere = cs250::createFlatShadedMesh(cs250::SphereMesh(5));
    sphereMeshIndex = render.loadMesh(*sphere);
    delete sphere;
    


    cs250::NormalMesh* cube = cs250::createFlatShadedMesh(cs250::CubeMesh());
    avatarMeshIndex = render.loadMesh(*cube);
    delete cube;
    //NMesh mesh2;
    //res = loadOBJtoMesh("Common/models/quad.obj", mesh2);
    //
    //mesh2.createTexCoordsPlanar();
    //quadMeshIndex = render.loadMesh(mesh2);
    //quad_d = mesh2.dimensions();
    //quad_c = mesh2.center();

    //vertices = { {0.0f, 0.0f, 0.0f}
    //};
    //
    //indices = { 0
    //};
    


    // scale the model to [-1, 1] box
    float maxAxis = std::max(d.x, std::max(d.y, d.z));

    float scale = 10.0f / maxAxis;

    glm::vec4 move_up = d.y / maxAxis * EY;

    mainCenter = O + move_up;



    //mainMMT = cs250::translate(move_up) * cs250::scale(scale) * cs250::translate(O - c);
    //mainMMT = cs250::translate(move_up) * cs250::scale(scale);
    //mainMMT = cs250::scale(scale);
    //mainMMT = glm::mat4(1.0f);
    //bool resfbo = render.SetUpFBO();
    for (unsigned i = 0; i < meshes.size(); i++)
    {
        modelMatrices.push_back(cs250::translate(move_up)  *cs250::scale(scale) * cs250::translate(O - c));
    }

    // set up GBufferFbo
    bool resfbo = render.SetUpGbufferFBO(_windowWidth, _windowHeight);

    //readTestCommands("testscene.scn");
    convertMeshesToObject();

    octreeRoot = BuildAdaptiveOctree(O, 10.0f, objectVector, 500);
    //bsptreeRoot = BuildBSPTree(objectVector, 500, 0);



      SetupBuffers();
    return Scene::Init();
}
/******************************************************************************/
/*!
  \brief
    set up light properties and debug objects positions

  \return 0
    unused debuggin value
*/
/******************************************************************************/
int Scene_Two::preRender()
{
    if (spotLightUse)
    {
        render.setLightInUse(8, 1);
        for (int i = 1; i < spotLightCount; i++)
        {
            render.setLightInUse(i + 8, 1);
        }

        for (int i = spotLightCount; i < spotLightCount + 5; i++)
        {
            render.setLightInUse(i + 8, 0);
        }
    }
    else
    {
        render.setLightInUse(8, 0);
        render.setLightInUse(9, 0);
        render.setLightInUse(10, 0);
        render.setLightInUse(11, 0);
        render.setLightInUse(12, 0);
    }

    if (pointLightUse)
    {
        render.setLightInUse(0, 1);
        for (int i = 1; i < pointLightCount; i++)
        {
            render.setLightInUse(i, 1);
        }

        for (int i = pointLightCount; i < 8; i++)
        {
            render.setLightInUse(i, 0);
        }
    }
    else
    {
        render.setLightInUse(0, 0);
        render.setLightInUse(1, 0);
        render.setLightInUse(2, 0);
        render.setLightInUse(3, 0);
        render.setLightInUse(4, 0);
        render.setLightInUse(5, 0);
        render.setLightInUse(6, 0);
        render.setLightInUse(7, 0);
    }

    if (directionalLightUse)
    {
        render.setLightInUse(13, 1);
        for (int i = 1; i < directionalLightCount; i++)
        {
            render.setLightInUse(i + 13, 1);
        }

        for (int i = directionalLightCount; i < directionalLightCount + 3; i++)
        {
            render.setLightInUse(i + 13, 0);
        }
    }
    else
    {
        render.setLightInUse(13, 0);
        render.setLightInUse(14, 0);
        render.setLightInUse(15, 0);
    }

    if (pointLightMultiColor)
    {
        colors[1] = RED;
        colors[2] = GREEN;
        colors[3] = BLUE;
        colors[4] = GRAY;
        colors[5] = CYAN;
        colors[6] = MAGENTA;
        colors[7] = YELLOW;

        colorsAMB[1] = DARKRED;
        colorsAMB[2] = DARKGREEN;
        colorsAMB[3] = DARKBLUE;
        colorsAMB[4] = DARKERGRAY;
        colorsAMB[5] = DARKCYAN;
        colorsAMB[6] = DARKMAGENTA;
        colorsAMB[7] = DARKYELLOW;
    }
    else
    {
        for (int i = 1; i < 8; ++i)
        {
            colors[i] = WHITE;
            colorsAMB[i] = DARKGRAY;
        }
    }

    glm::vec3 CD1 = WHITE;
    glm::vec3 CD2 = WHITE;

    if (directionalLightMultiColor)
    {
        CD1 = CYAN;
        CD2 = YELLOW;
    }


    glm::vec3 CS1 = WHITE;
    glm::vec3 CS2 = WHITE;
    glm::vec3 CS3 = WHITE;
    glm::vec3 CS4 = WHITE;

    if (spotLightMultiColor)
    {
        CS1 = RED;
        CS2 = GREEN;
        CS3 = BLUE;
        CS4 = MAGENTA;
    }

    render.setSpotLight(9, O + 5.0f * -EX + 2.0f * EY, EX - EY, CS1, inner, outer, fallout);
    render.setSpotLight(10, O + 5.0f * EZ + 2.0f * EY, -EZ - EY, CS2, inner, outer, fallout);
    render.setSpotLight(11, O + 5.0f * -EZ + 2.0f * EY, EZ - EY, CS3, inner, outer, fallout);
    render.setSpotLight(12, O + 5.0f * EX + 2.0f * EY, -(EX + EY), CS4, inner, outer, fallout);
    render.setSpotLight(8, O + 5.0f * EX + 5.0f * EY, -(EX + EY), WHITE, inner, outer, fallout);

    float rotation = CAM_RATE * float(time);

    glm::vec4 P_dir = 5.0f * EY + O + 3.0f * cos(rotation) * EZ - 3.0f * sin(rotation) * EX;
    glm::vec4 P_dir1 = 3.0f * EY + O + 5.0f * cos(rotation + one_eighth * 3.0f) * EZ - 5.0f * sin(rotation + one_eighth * 3.0f) * EX;
    glm::vec4 P_dir2 = 2.0f * EY + O + 2.0f * cos(rotation + one_eighth * 3.0f) * EZ - 2.0f * sin(rotation + one_eighth * 6.0f) * EX;

    render.setDirectionalLight(13, (mainCenter- P_dir), WHITE);
    render.setDirectionalLight(14, (mainCenter- P_dir1), CD1);
    render.setDirectionalLight(15, (mainCenter- P_dir2), CD2);

    //spherePositions[0] = mainCenter + 3.0f * cos(rotation) * EZ
    //    - 3.0f * sin(rotation) * EX;
    //
    //spherePositions[1] = mainCenter + 3.0f * cos(rotation + one_eighth) * EZ
    //    - 3.0f * sin(rotation + one_eighth) * EX;
    //
    //spherePositions[2] = mainCenter + 3.0f * cos(rotation + one_eighth * 2.0f) * EZ
    //    - 3.0f * sin(rotation + one_eighth * 2.0f) * EX;
    //
    //spherePositions[3] = mainCenter + 3.0f * cos(rotation + one_eighth * 3.0f) * EZ
    //    - 3.0f * sin(rotation + one_eighth * 3.0f) * EX;
    //
    //spherePositions[4] = mainCenter + 3.0f * cos(rotation + one_eighth * 4.0f) * EZ
    //    - 3.0f * sin(rotation + one_eighth * 4.0f) * EX;
    //
    //spherePositions[5] = mainCenter + 3.0f * cos(rotation + one_eighth * 5.0f) * EZ
    //    - 3.0f * sin(rotation + one_eighth * 5.0f) * EX;
    //
    //spherePositions[6] = mainCenter + 3.0f * cos(rotation + one_eighth * 6.0f) * EZ
    //    - 3.0f * sin(rotation + one_eighth * 6.0f) * EX;
    //
    //spherePositions[7] = mainCenter + 3.0f * cos(rotation + one_eighth * 7.0f) * EZ
    //    - 3.0f * sin(rotation + one_eighth * 7.0f) * EX;

    spherePositions[0] = mainCenter + 7.0f * cos(rotation) * EZ
        - 7.0f * sin(rotation) * EX;

    spherePositions[1] = mainCenter + 7.0f * cos(rotation + one_eighth) * EZ
        - 7.0f * sin(rotation + one_eighth) * EX;

    spherePositions[2] = mainCenter + 7.0f * cos(rotation + one_eighth * 2.0f) * EZ
        - 7.0f * sin(rotation + one_eighth * 2.0f) * EX;

    spherePositions[3] = mainCenter + 7.0f * cos(rotation + one_eighth * 3.0f) * EZ
        - 7.0f * sin(rotation + one_eighth * 3.0f) * EX;

    spherePositions[4] = mainCenter + 7.0f * cos(rotation + one_eighth * 4.0f) * EZ
        - 7.0f * sin(rotation + one_eighth * 4.0f) * EX;

    spherePositions[5] = mainCenter + 7.0f * cos(rotation + one_eighth * 5.0f) * EZ
        - 7.0f * sin(rotation + one_eighth * 5.0f) * EX;

    spherePositions[6] = mainCenter + 7.0f * cos(rotation + one_eighth * 6.0f) * EZ
        - 7.0f * sin(rotation + one_eighth * 6.0f) * EX;

    spherePositions[7] = mainCenter + 7.0f * cos(rotation + one_eighth * 7.0f) * EZ
        - 7.0f * sin(rotation + one_eighth * 7.0f) * EX;

    for (int i = 0; i < 8; ++i)
    {
        render.setLight(i, spherePositions[i], colors[i]);
    }


    render.setAmbient(glm::vec3(globalAmbientColor[0], globalAmbientColor[1], globalAmbientColor[2]));
    render.setAttenauation(0.01f, 0.01f, 0.01f);
    render.setAtomsphericAttenauation(FAR, NEAR, glm::vec3(fogColor[0], fogColor[1], fogColor[2]));

    glm::mat4 partialM = MOON_TILT
        * cs250::rotate(MOON_RATE * static_cast<float>(time), EY)
        * cs250::scale(0.2f);

    for (int i = 0; i < 8; ++i)
    {
        sphereMMT[i] = cs250::translate(spherePositions[i]) * partialM;
    }

    // reset sphere if out of boundary
    if (ms.OutOfBoundary(camera0))
    {
        ms.ResetSphere();
        //bCopyDepthInfo = true;
    }

    // only update sphere's position and check collision if the sphere is active
    if (ms._bActive == true)
    {
        ms._position = ms._position + ms._direction * ms._speed * deltaTime;

        glm::mat4 rotM0 = cs250::rotate(-MOON_RATE * static_cast<float>(time), camera0.right());
        glm::mat4 rotM = cs250::affine(camera0.right(), camera0.up(), camera0.back(), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        //movingSphereMMT = cs250::translate(ms._position) * cs250::scale(ms._radius);
        movingSphereMMT = cs250::translate(ms._position) * rotM0 * rotM * cs250::scale(ms._radius);
        //movingSphereMMT = cs250::affine(camera0.right(), camera0.up(), -camera0.back(), ms._position) * cs250::scale(ms._radius);

        bCollide = DetectCollision_BroadPhase(ms, octreeRoot);
        // if collide and stop is on paus
        if (bCollide && bCollisionDebugger)
            pause = true;
    }
    return 0;
}
/******************************************************************************/
/*!
  \brief
    maind render loop, calls custom FBO and default FBO to draw,
    set camera, model, material and make draw calls,
    displays ImGUI

  \return 0
    unused debuggin value
*/
/******************************************************************************/
int Scene_Two::Render()
{
    
    render.clear(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));


    render.UseFBO(render.getGBufferFBOHandle(), _windowWidth, _windowHeight);
    render.setCamera(camera0);

    for (unsigned i = 0; i < meshIndexes.size(); i++)
    {
        //render.setModel(mainMMT);
        render.setModel(modelMatrices[i]);
        render.setMaterial(meshes[i].Ka, meshes[i].Kd, meshes[i].Ks, meshes[i].Ns);
        render.drawToGBuffer(meshIndexes[i]);
    }
    //glDrawBuffer(GL_COLOR_ATTACHMENT0);
    //render.setCamera(camera1);
    // draw call skybox
    //render.drawSkybox(skyboxMeshIndex);

    //drawSpheres();

    //glDrawBuffer(GL_COLOR_ATTACHMENT1);
    //render.setCamera(camera2);
    // draw call skybox
    //render.drawSkybox(skyboxMeshIndex);

    //drawSpheres();

    //glDrawBuffer(GL_COLOR_ATTACHMENT2);
    //render.setCamera(camera3);
    // draw call skybox
    //render.drawSkybox(skyboxMeshIndex);

    //drawSpheres();

    //glDrawBuffer(GL_COLOR_ATTACHMENT3);
    //render.setCamera(camera4);
    // draw call skybox
    //render.drawSkybox(skyboxMeshIndex);

    //drawSpheres();

    //glDrawBuffer(GL_COLOR_ATTACHMENT4);
    //render.setCamera(camera5);
    // draw call skybox
    //render.drawSkybox(skyboxMeshIndex);

    //drawSpheres();

    //glDrawBuffer(GL_COLOR_ATTACHMENT5);
    //render.setCamera(camera6);
    // draw call skybox
    //render.drawSkybox(skyboxMeshIndex);

    //drawSpheres();


    render.UseFBO(0, _windowWidth, _windowHeight);

    render.setCamera(camera0);

    //switch (currentCamera)
    //{
    //case 0:
    //    render.setCamera(camera0);
    //    glViewport(0, 0, _windowWidth, _windowHeight);
    //    break;
    //case 1:
    //    render.setCamera(camera1);
    //    if (_windowHeight < _windowWidth)
    //        glViewport(0, 0, _windowHeight, _windowHeight);
    //    else
    //        glViewport(0, 0, _windowWidth, _windowWidth);
    //    break;
    //case 2:
    //    render.setCamera(camera2);
    //    if (_windowHeight < _windowWidth)
    //        glViewport(0, 0, _windowHeight, _windowHeight);
    //    else
    //        glViewport(0, 0, _windowWidth, _windowWidth);
    //    break;
    //case 3:
    //    render.setCamera(camera3);
    //    if (_windowHeight < _windowWidth)
    //        glViewport(0, 0, _windowHeight, _windowHeight);
    //    else
    //        glViewport(0, 0, _windowWidth, _windowWidth);
    //    break;
    //case 4:
    //    render.setCamera(camera4);
    //    if (_windowHeight < _windowWidth)
    //        glViewport(0, 0, _windowHeight, _windowHeight);
    //    else
    //        glViewport(0, 0, _windowWidth, _windowWidth);
    //    break;
    //case 5:
    //    render.setCamera(camera5);
    //    if (_windowHeight < _windowWidth)
    //        glViewport(0, 0, _windowHeight, _windowHeight);
    //    else
    //        glViewport(0, 0, _windowWidth, _windowWidth);
    //    break;
    //case 6:
    //    render.setCamera(camera6);
    //    if (_windowHeight < _windowWidth)
    //        glViewport(0, 0, _windowHeight, _windowHeight);
    //    else
    //        glViewport(0, 0, _windowWidth, _windowWidth);
    //    break;
    //}
    //

    //render.drawSkybox(skyboxMeshIndex);
    // hide fsq if collide
    if (!bCollide)
    {
        render.drawFSQ(currentGBuffer);
    }


    if (bCopyDepthInfo)
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, render.getGBufferFBOHandle());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
        // blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
        // the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the 		
        // depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
        glBlitFramebuffer(0, 0, _windowWidth, _windowHeight, 0, 0, _windowWidth, _windowHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    drawSpheres();
    //drawAvatar();
    //drawOctree(octreeRoot);
    
    //render.setModel(mainMMT);

    //render.setMaterial(glm::vec3(ambient[0], ambient[1], ambient[2]), glm::vec3(0.9f, 0.3f, 0.1f), glm::vec3(0.7f, 0.7f, 0.7f), 50.0f);
    //render.draw(mainMeshIndex);

    //if (render.getShaderType() == cs250::PhongRender::shader_type::TRANSPARENT)
    //{
    //    render.setTransparentIndex(eta_in, eta_out, epsilon);
    //    render.drawFBOTexture(mainMeshIndex, currentTransparentMode, shading_use);
    //}
    //else
    //{
    //    if (gpu_use)
    //        render.drawTexture(mainMeshIndex, 1, ventity_type, mapping_type);
    //    else
    //        render.drawTexture(mainMeshIndex, 0, ventity_type, mapping_type);
    //}
    //drawObjects(objectVector);
    // draw moving sphere normally if it's active and not collided
    if(!bCollide && ms._bActive)
    drawMovingSphere();

    if (drawNormal)
    {
        for (unsigned i = 0; i < meshIndexes.size(); i++)
        {
            render.setModel(modelMatrices[i]);
            render.drawNormal(meshIndexes[i]);
        }
    }
    if (drawFaceNormal)
    {
        for (unsigned i = 0; i < meshIndexes.size(); i++)
        {
            render.setModel(modelMatrices[i]);
            render.drawFaceNormal(meshIndexes[i]);
        }
    }

    if (currentDebugDrawingMode == 1)
    {
        drawOctree(octreeRoot, 8);
    }
    //else if (currentDebugDrawingMode == 2)
    //{
    //    drawBSPTree(bsptreeRoot);
    //}

    // if collide
    if (bCollide)
    {
        // disable depth test to draw on top
        glDisable(GL_DEPTH_TEST);
        // draw the wireframe sphere
        render.setModel(movingSphereMMT);
        render.setWireframeColor(RED);
        render.drawWireframe(sphereMeshIndex);
        // draw the collided node
        drawHitNode();
        glEnable(GL_DEPTH_TEST);
    }

      ImGui_ImplGlfwGL3_NewFrame();
      {
          // Display some text (you can use a format string too)

          ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

          ImGui::Checkbox("Normal", &drawNormal);
          ImGui::SameLine(100);
          ImGui::Checkbox("Face Normal", &drawFaceNormal);
          //ImGui::Checkbox("Octree", &bDisplayOctree);

          
          ImGui::Checkbox("Copy Depth Info", &bCopyDepthInfo);
          ImGui::SameLine(200);
          ImGui::Checkbox("Pause when Collide", &bCollisionDebugger);
          ImGui::SameLine(400);
          ImGui::Checkbox("Pause", &pause);

          //if (ImGui::CollapsingHeader("Octree & BSPTree", 0, true, true))
          if (ImGui::CollapsingHeader("Octree", 0, true, true))
          {
              //const char* items5[] = { "Default", "Octree", "BSPTree"};
              const char* items5[] = { "Default", "Octree"};
              //static int item_current3 = 0;
              ImGui::ListBox("Grid/BSP Drawing", &currentDebugDrawingMode, items5, IM_ARRAYSIZE(items5));
          }
          //if (ImGui::CollapsingHeader("Transparent Mode and Parameter", 0, true, true))
          //{
          //    const char* transparentMode[] = { "Reflection", "Refraction", "Refraction (Chromatic Aberration)","Reflection + Refraction"};
          //    ImGui::ListBox("Mode", &currentTransparentMode, transparentMode, IM_ARRAYSIZE(transparentMode));
          //    ImGui::Checkbox("Phong Shading Usage", &shading_use);
          //    ImGui::SliderFloat("Eta In", &eta_in, 1.0f, 100.0f);
          //    ImGui::SliderFloat("Eta Out", &eta_out, 1.0f, 100.0f);
          //    ImGui::SliderFloat("Epsilon", &epsilon, 0.00f, 2.5f);
          //}
          if (ImGui::CollapsingHeader("Lighting", 0, true, false))
          {
              ImGui::Text("Light Type");
              ImGui::SameLine(100);
              ImGui::Checkbox("Point", &pointLightUse);
              ImGui::SameLine(220);
              ImGui::Checkbox("Directional", &directionalLightUse);
              ImGui::SameLine(340);
              ImGui::Checkbox("Spot", &spotLightUse);

              ImGui::Text("          ");
              ImGui::SameLine(100);
              ImGui::Checkbox("P Multi Color", &pointLightMultiColor);
              ImGui::SameLine(220);
              ImGui::Checkbox("D Multi Color", &directionalLightMultiColor);
              ImGui::SameLine(340);
              ImGui::Checkbox("S Multi Color", &spotLightMultiColor);

              ImGui::SliderInt("Point Light 1 ~ 8", &pointLightCount, 1, 8);
              ImGui::SliderInt("Directional Light  1 ~ 3", &directionalLightCount, 1, 3);
              ImGui::SliderInt("Spot Light  1 ~ 5", &spotLightCount, 1, 5);
          }
          // List box
          //if (ImGui::CollapsingHeader("Shader", 0, true, false))
          //{
          //    const char* items[] = { "Phong Lighting", "Phong Shading", "Blinn Shading", "Transparent + Phong Shading" };
          //    static int item_current = 3;
          //    ImGui::ListBox("Shader Type", &item_current, items, IM_ARRAYSIZE(items));
          //    if (ImGui::Button("Reload Shader"))
          //    {
          //        render.reloadShader(item_current);
          //    }
          //}

          if (ImGui::CollapsingHeader("Lighting Parameter", 0, true, false))
          {
              //ImGui::ColorEdit3("Ambient Coefficient", ambient);

              ImGui::SliderFloat("Inner angle", &inner, 0.0f, 90.0f);
              ImGui::SliderFloat("Outer angle", &outer, 0.0f, 90.0f);
              ImGui::SliderFloat("Fallout value", &fallout, 0.0f, 10.0f);
              ImGui::ColorEdit3("Global Ambient Color", globalAmbientColor);
              ImGui::ColorEdit3("Fog Color", fogColor);
          }

          //if (ImGui::CollapsingHeader("Texture Mapping", 0, true, false))
          //{
          //    ImGui::Checkbox("GPU mapping Calculation (hit reload button after check/uncheck)", &gpu_use);
          //    const char* items1[] = { "Position", "Normal" };
          //    static int item_current1 = 0;
          //    ImGui::ListBox("Texture Entity", &item_current1, items1, IM_ARRAYSIZE(items1));
          //
          //
          //    const char* items2[] = { "Planar", "Cube", "Cylindrical", "Spherical" };
          //    static int item_current2 = 0;
          //    ImGui::ListBox("Texture Mapping", &item_current2, items2, IM_ARRAYSIZE(items2));
          //    if (ImGui::Button("Reload Mapping"))
          //    {
          //        mapping_type = item_current2;
          //        ventity_type = item_current1;
          //        switch (item_current2)
          //        {
          //        case 0:
          //            if (item_current1 == 0)
          //                render.reloadTexCoords(mainMeshIndex, cpuTexCoordsPlan);
          //            else
          //                render.reloadTexCoords(mainMeshIndex, cpuTexCoordsNorPlan);
          //            break;
          //        case 1:
          //            if (item_current1 == 0)
          //                render.reloadTexCoords(mainMeshIndex, cpuTexCoordsCube);
          //            else
          //                render.reloadTexCoords(mainMeshIndex, cpuTexCoordsNorCube);
          //            break;
          //        case 2:
          //            if (item_current1 == 0)
          //                render.reloadTexCoords(mainMeshIndex, cpuTexCoordsCyli);
          //            else
          //                render.reloadTexCoords(mainMeshIndex, cpuTexCoordsNorCyli);
          //            break;
          //        case 3:
          //            if (item_current1 == 0)
          //                render.reloadTexCoords(mainMeshIndex, cpuTexCoordsSphe);
          //            else
          //                render.reloadTexCoords(mainMeshIndex, cpuTexCoordsNorSphe);
          //            break;
          //        }
          //    }
          //}

          if (ImGui::CollapsingHeader("Default Camera Rotation and Movement", 0, true, false))
          {
              if (ImGui::Button("Yaw+"))
              {
                  camera0.yaw(5.0f);
              }
              ImGui::SameLine(70);
              if (ImGui::Button("Yaw-"))
              {
                  camera0.yaw(-5.0f);
              }

              if (ImGui::Button("Pitch+"))
              {
                  camera0.pitch(5.0f);
              }
              ImGui::SameLine(70);
              if (ImGui::Button("Pitch-"))
              {
                  camera0.pitch(-5.0f);
              }

              if (ImGui::Button("Roll+"))
              {
                  camera0.roll(5.0f);
              }
              ImGui::SameLine(70);
              if (ImGui::Button("Roll-"))
              {
                  camera0.roll(-5.0f);
              }

              if (ImGui::Button("Forward"))
              {
                  camera0.forward(5.0f);
              }
              ImGui::SameLine(70);
              if (ImGui::Button("Backward"))
              {
                  camera0.forward(-5.0f);
              }

              if (ImGui::Button("Zoom In"))
              {
                  camera0.zoom(0.9f);
              }
              ImGui::SameLine(70);
              if (ImGui::Button("Zoom Out"))
              {
                  camera0.zoom(1.1f);
              }

              if (ImGui::Button("Reset Default Camera"))
              {
                  camera0 = cs250::Camera(CAM_POS, mainCenter - CAM_POS, EY, FOV, _aspect, NEAR, FAR);
              }
          }

          //if (ImGui::CollapsingHeader("Debugging Cameras", 0, true, false))
          //{
          //    const char* items3[] = { "Default", "Back", "Right", "Front", "Left", "Top", "Bottom" };
          //    //static int item_current3 = 0;
          //    ImGui::ListBox("Camera", &currentCamera, items3, IM_ARRAYSIZE(items3));
          //
          //
          //}

          if (ImGui::CollapsingHeader("GBuffer Debug Draw", 0, true, false))
          {
              const char* items4[] = { "Default", "Position", "Normals", "Diffuse (Albedo)", "Specular", "Ambient" };
              //static int item_current3 = 0;
              ImGui::ListBox("GBuffer Color Attachment", &currentGBuffer, items4, IM_ARRAYSIZE(items4));
          
          
          }

      }

      ImGui::Render();
      ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

    return 0;
}
/******************************************************************************/
/*!
  \brief
    update ticks

  \return 0
    unused debuggin value
*/
/******************************************************************************/
int Scene_Two::postRender()
{
    Uint32 ticks = SDL_GetTicks();
    double dt = 0.001 * (ticks - ticks_last);
    //double dt = glfwGetTime() - time;
    deltaTime = 0.0f;
    ticks_last = ticks;

    if (!pause)
    {
        time += dt;
        deltaTime = dt;
    }

    return 0;
}
/******************************************************************************/
/*!
  \brief
    callback function for glfw when resize window event occurs

  \param pWindow
    context window

  \param width
    new window width

  \param height
    new window height
*/
/******************************************************************************/
void Scene_Two::Resize(GLFWwindow* pWindow, int width, int height)
{
    _windowWidth = width;
    _windowHeight = height;
    _aspect = static_cast<float>(_windowWidth)/static_cast<float>(_windowHeight);
    // only aspect should be changed after resizing
    camera0 = cs250::Camera(camera0.eye(), -camera0.back(), camera0.up(), FOV, _aspect, camera0.near(), camera0.far());
    // need to recreate GBuffer after resizing
    render.resizeGBuffer(_windowWidth, _windowHeight);
    glViewport(0, 0, width, height);
}
/******************************************************************************/
/*!
  \brief
    callback function for glfw when Keyboard event occurs

  \param pWindow
    context pWindow

  \param key
    key entered

  \param scancode
    The system-specific scancode of the key

  \param action
    GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT

  \param mods
    Bit field describing which modifier keys were held down
    
*/
/******************************************************************************/
void Scene_Two::Keyboard(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
    if (action == GLFW_REPEAT) return;
    if (action == GLFW_PRESS) {
        switch (key) {

        case GLFW_KEY_W:
            camera0.forward(1.0f);
            break;

        case GLFW_KEY_A:
            camera0.yaw(1.0f);
            break;

        case GLFW_KEY_S:
            camera0.forward(-1.0f);
            break;

        case GLFW_KEY_D:
            camera0.yaw(-1.0f);
            break;

        case GLFW_KEY_UP:
            camera0.pitch(1.0f);

            break;

        case GLFW_KEY_DOWN:
            camera0.pitch(-1.0f);
            break;

        case GLFW_KEY_I:
            //camera0.pitch(1.0f);
            avatarPosition.x = avatarPosition.x + 0.3f;
            //avatarPosition = cs250::translate(glm::vec4(0.3f, 0.0f, 0.0f, 0.0f)) * avatarPosition;
            avatarMMT = cs250::translate(avatarPosition) * cs250::scale(5.0f);

            break;

        case GLFW_KEY_K:
            //camera0.pitch(-1.0f);
            avatarPosition.x = avatarPosition.x - 0.3f;
            avatarMMT = cs250::translate(avatarPosition) * cs250::scale(5.0f);
            //avatarPositi
            break;

        case GLFW_KEY_J:
            //camera0.pitch(1.0f);
            avatarPosition.z = avatarPosition.z - 0.3f;
            //avatarPosition = cs250::translate(glm::vec4(0.3f, 0.0f, 0.0f, 0.0f)) * avatarPosition;
            avatarMMT = cs250::translate(avatarPosition) * cs250::scale(5.0f);
            break;

        case GLFW_KEY_L:
            //camera0.pitch(1.0f);
            avatarPosition.z = avatarPosition.z + 0.3f;
            //avatarPosition = cs250::translate(glm::vec4(0.3f, 0.0f, 0.0f, 0.0f)) * avatarPosition;
            avatarMMT = cs250::translate(avatarPosition) * cs250::scale(5.0f);
            break;

        case GLFW_KEY_U:
            //camera0.pitch(1.0f);
            avatarPosition.y = avatarPosition.y + 0.3f;
            //avatarPosition = cs250::translate(glm::vec4(0.3f, 0.0f, 0.0f, 0.0f)) * avatarPosition;
            avatarMMT = cs250::translate(avatarPosition) * cs250::scale(5.0f);
            break;

        case GLFW_KEY_O:
            //camera0.pitch(1.0f);
            avatarPosition.y = avatarPosition.y - 0.3f;
            //avatarPosition = cs250::translate(glm::vec4(0.3f, 0.0f, 0.0f, 0.0f)) * avatarPosition;
            avatarMMT = cs250::translate(avatarPosition) * cs250::scale(5.0f);
            break;

        case GLFW_KEY_SPACE:

            // each time space is pressed the sphere is reshoot
            if (!bCollisionDebugger)
            {
                //ms._position = ms._position + ms._direction * 0.3f;
                //movingSphereMMT = cs250::translate(ms._position) * cs250::scale(ms._radius);
                //ms._bActive = true;
                //ms._position = camera0.eye();
                //ms._direction = -camera0.back();
                //ms._radius = 0.5;
                //ms._speed = 10.0f;
                ms.ShootSphere(camera0, 0.5f, 10.0f);
                hitRecord = NULL;
                bCollide = false;
                //bCopyDepthInfo = true;
            }
            else
            {
                // if the stop  when collide is on and is paused
                // reset the sphere
                if (bCollide && pause)
                {
                    ms.ResetSphere();
                    ms._position = camera0.eye();
                    ms._direction = -camera0.back();
                    ms._radius = 0.5;
                    hitRecord = NULL;
                    bCollide = false;
                    //bCopyDepthInfo = true;
                    pause = false;

                }
                else
                {
                    // else shoot normally
                    //ms._bActive = true;
                    //ms._position = camera0.eye();
                    //ms._direction = -camera0.back();
                   // ms._radius = 0.5;
                    //ms._speed = 10.0f;
                    ms.ShootSphere(camera0, 0.5f, 10.0f);
                    hitRecord = NULL;
                    bCollide = false;
                    //bCopyDepthInfo = true;

                }
            }
            break;
        }
    }
}
/******************************************************************************/
/*!
  \brief
    helper function to allow assimp to load Mesh into the scene

  \param mesh
    mesh to be loaded
*/
/******************************************************************************/
void Scene_Two::loadMesh(const NMesh& mesh)
{
    meshes.push_back(mesh);
}
