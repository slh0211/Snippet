/******************************************************************************/
/*!
\file   FlatShadedMesh.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS250
\par    Assignment 2 FlatShadedMesh
\par    Summer 2020
\date   5/17/2020
\brief  
    This file contains the implementation of FlatShadedMesh functions

  Hours spent on this assignment: 2

  Specific portions that gave you the most trouble:
    
*/
/******************************************************************************/
#include "FlatShadedMesh.h"
#include "Affine.h" // cross
#include "MappingFunction.h"
#include <vector>   // std::vector

#include <glm/gtc/epsilon.hpp>

/******************************************************************************/
/*!
  \brief
    returns the number of vertices in the vertex array of the mesh

  \return 
    size of the vetices array
*/
/******************************************************************************/
int NMesh::vertexCount(void) const
{
  return vertices.size();
}
/******************************************************************************/
/*!
  \brief
    returns a pointer to the vertex array of the mesh. The vertices are given
    in object coordinates

  \return vertices
    vertices of the mesh
*/
/******************************************************************************/
const glm::vec4* NMesh::vertexArray(void) const
{
  return &(vertices[0]);
}
/******************************************************************************/
/*!
  \brief
    returns the dimensions (bounding box) for the NMesh

  \return _dimensions
    vec4 x, y, z holds the dimension
*/
/******************************************************************************/
glm::vec4 NMesh::dimensions(void) const
{
  return _dimensions;
}
/******************************************************************************/
/*!
  \brief
    returns the center (Cx, Cy, Cz) of the axis–aligned bounding box in object
    space that contains the mesh

  \return _center
    center of the NMesh in object space
*/
/******************************************************************************/
glm::vec4 NMesh::center(void) const
{
  return _center;
}
/******************************************************************************/
/*!
  \brief
    returns the number of triangular faces in the mesh

  \return 
    size of faces
*/
/******************************************************************************/
int NMesh::faceCount(void) const
{
  return faces.size();
}
/******************************************************************************/
/*!
  \brief
    returns a pointer to the face array of the mesh.
    Faces should be counterclockwise oriented

  \return faces
    array contains faces of the mesh
*/
/******************************************************************************/
const cs250::Mesh::Face* NMesh::faceArray(void) const
{
  return &(faces[0]);
}
/******************************************************************************/
/*!
  \brief
    edgeCount only used for specific shaped Mesh

  \return 
    0
*/
/******************************************************************************/
int NMesh::edgeCount(void) const
{
  return 0;
    //return edges.size();
}
/******************************************************************************/
/*!
  \brief
    edgeArray only used for specific shaped Mesh

  \return
    NULL
*/
/******************************************************************************/
const cs250::Mesh::Edge* NMesh::edgeArray(void) const
{
  return NULL;
    //return &(edges[0]);
}
/******************************************************************************/
/*!
  \brief
    return normals of correlated verterices

  \return normals
    array contains normals
*/
/******************************************************************************/
const glm::vec4* NMesh::normalArray(void) const
{
  return &(normals[0]);
}
/******************************************************************************/
/*!
  \brief
    return texture coordinates of correlated verterices

  \return texCoords
    array contains texture coordinates
*/
/******************************************************************************/
const glm::vec2* NMesh::texCoordArray(void) const
{
  return &(texCoords[0]);
}

void NMesh::createTexCoordsPlanar(void)
{
    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        //float u = ((vertices[i].x - _center.x) + (_dimensions.x / 2.0f)) / _dimensions.x;
        //float v = ((vertices[i].y - _center.y) + (_dimensions.y / 2.0f)) / _dimensions.y;

        //glm::vec4 normal = glm::normalize(normals[i]);

        //float u = ((normals[i].x) + 1.0f)/2.0f;
        //float v = ((normals[i].y) + 1.0f)/2.0f;
        float x_prime = vertices[i].x - _center.x;
        float y_prime = vertices[i].y - _center.y;
        float z_prime = vertices[i].z - _center.z;

        glm::vec3 normal = glm::normalize(glm::vec3(normals[i]));


        //glm::vec2 uv = CalcPlanarMap(glm::vec3(x_prime, y_prime, z_prime), _dimensions);
        glm::vec2 uv = CalcPlanarMap(glm::vec3(normal.x, normal.y, normal.z), glm::vec3(2.0f, 2.0f, 2.0f));

        texCoords.push_back(uv);
    }
}

void NMesh::createTexCoordsCylindrical(void)
{
    //float z_range = (_dimensions.z / 2.0f);
    //float z_min = _center.z - z_range;
    //float z_max = _center.z + z_range;

    //float  PI = 3.14159f;

    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        float x_prime = vertices[i].x - _center.x;
        float y_prime = vertices[i].y - _center.y;
        float z_prime = vertices[i].z - _center.z;

        glm::vec2 uv = CalcCylindricalMap(glm::vec3(x_prime, y_prime, z_prime), _dimensions, _center);

        //float theta = glm::atan(y_prime ,x_prime);
        //if (x_prime == 0.0f)
        //    theta = 0.0f;
        //
        //if (theta < 0)
        //    theta += 2.0f * PI;
        ////if (x_prime < 0.0f )
        //    //theta += PI;
        //
        //
        //
        //float z_out = (z_prime - z_min) / (z_max - z_min);
        //
        //
        //float u = theta / (2.0f * PI);
        //float v = z_out;
        texCoords.push_back(uv);
    }
}

void NMesh::createTexCoordsSpherical(void)
{
    //float z_range = (_dimensions.z / 2.0f);
    //float z_min = _center.z - z_range;
    //float z_max = _center.z + z_range;

    //float  PI = 3.14159f;

    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        float x_prime = vertices[i].x - _center.x;
        float y_prime = vertices[i].y - _center.y;
        float z_prime = vertices[i].z - _center.z;

        glm::vec2 uv = CalcSphericalMap(glm::vec3(x_prime, y_prime, z_prime));
        //float r = sqrt(x_prime * x_prime + y_prime * y_prime + z_prime * z_prime);

        //float phi = glm::acos(z_prime / r);

        //float theta = glm::atan(y_prime, x_prime);
       //if (x_prime == 0.0f)
       //     theta = 0.0f;
       //
       // if (theta < 0)
       //     theta += 2.0f * PI;
        //if (x_prime < 0.0f )
            //theta += PI;



        //float z_out = (z_prime - z_min) / (z_max - z_min);


        //float u = theta / (2.0f * PI);
        //float v = (PI - phi)/PI;

        texCoords.push_back(uv);
    }
}

void NMesh::createTexCoordsCubeMap(void)
{
    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        float x_prime = vertices[i].x - _center.x;
        float y_prime = vertices[i].y - _center.y;
        float z_prime = vertices[i].z - _center.z;

        glm::vec2 uv = CalcCubeMap(glm::vec3(x_prime, y_prime, z_prime));

        texCoords.push_back(uv);
    }
}

void NMesh::createTexCoordsPlanar(std::vector<glm::vec2>& cpuTexCoordsPlanar)
{
    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        float u = ((vertices[i].x - _center.x) + (_dimensions.x / 2.0f)) / _dimensions.x;
        float v = ((vertices[i].y - _center.y) + (_dimensions.y / 2.0f)) / _dimensions.y;

        //glm::vec4 normal = glm::normalize(normals[i]);

        //float u = ((normals[i].x) + 1.0f)/2.0f;
        //float v = ((normals[i].y) + 1.0f)/2.0f;


        cpuTexCoordsPlanar.push_back(glm::vec2(u, v));
    }
}

std::vector<glm::vec2> NMesh::createTCP(int mapping_type, int entity_type)
{
    std::vector<glm::vec2> out_vec;

    glm::vec3 dim;
    glm::vec3 cen;

    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        float x_prime =0.0f;
        float y_prime = 0.0f;
        float z_prime = 0.0f;

        //float x_nm = normals[i].x;
        //float y_nm = normals[i].y;
        //float z_nm = normals[i].z;
        //
        //glm::vec3 normal = glm::vec3(0.0f, 0.0f, 0.0f);
        ////
        ////struct comparVec cV;
        ////
        //if (compareVector(normal, normals[i]))
        //{
        //    normal = glm::vec3(0.0f, 0.0f, 0.0f);
        //}
        //else
        //{
        //    normal = glm::normalize(glm::vec3(normals[i]));
        //}
        float length = glm::length(normals[i]);
        glm::vec3 normal = glm::vec3(0.0f);
        //glm::vec3 normal = glm::normalize(glm::vec3(normals[i])); 
        if(length > 0.0f)
            normal = glm::normalize(glm::vec3(normals[i]));
        //glm::vec3 normal = glm::vec3(normals[i]);

        switch (entity_type)
        {
        case 0:
            glm::vec3 nmd = NormalizeToBoundingBox(vertices[i], _center, _dimensions);

            x_prime = nmd.x;
            y_prime = nmd.y;
            z_prime = nmd.z;
            //x_prime = vertices[i].x - _center.x;
            //y_prime = vertices[i].y - _center.y;
            //z_prime = vertices[i].z - _center.z;

            dim = glm::vec3(2.0f, 2.0f, 2.0f);//_dimensions;
            cen = glm::vec3(0.0f, 0.0f, 0.0f);//_center;
            //dim = _dimensions;
            //cen = _center;
            break;
        case 1:



            x_prime = normal.x;
            y_prime = normal.y;
            z_prime = normal.z;
            //x_prime = normal.x * 0.5f + 0.5f;
            //y_prime = normal.y * 0.5f + 0.5f;
            //z_prime = normal.z * 0.5f + 0.5f;
            dim = glm::vec3(2.0f, 2.0f, 2.0f);
            cen = glm::vec3(0.0f, 0.0f, 0.0f);
            //dim = glm::vec3(1.0f, 1.0f, 1.0f);
            //cen = glm::vec3(0.5f, 0.5f, 0.5f);
            break;
        }

        //glm::vec3 normal = glm::normalize(glm::vec3(normals[i]));

        glm::vec2 uv;

        switch (mapping_type)
        {
        case PLAN:
            uv = CalcPlanarMap(glm::vec3(x_prime, y_prime, z_prime), dim);
            break;
        case CUBE:
            uv = CalcCubeMap(glm::vec3(x_prime, y_prime, z_prime));
            break;
        case CYLI:
            uv = CalcCylindricalMap(glm::vec3(x_prime, y_prime, z_prime), dim, cen);
            break;
        case SPHE:
            uv = CalcSphericalMap(glm::vec3(x_prime, y_prime, z_prime));
            break;
        }
        //glm::vec2 uv = CalcPlanarMap(glm::vec3(normal.x, normal.y, normal.z), glm::vec3(2.0f, 2.0f, 2.0f));

        out_vec.push_back(uv);
    }

    return out_vec;
}
/******************************************************************************/
/*!
  \brief
    create a NomalMesh using m's vetices

  \param m
    the Mesh to be used

  \return np
    point to a newly created NormalMesh
*/
/******************************************************************************/
cs250::NormalMesh* cs250::createFlatShadedMesh(const Mesh &m)
{
  NMesh *np = new NMesh();
  
  np->_dimensions = m.dimensions();
  np->_center = m.center();
  
  int m_size = m.faceCount();
  // Reserve space to avoid reallocation
  np->vertices.reserve(m_size * 3);
  np->normals.reserve(m_size * 3);
  np->faces.reserve(m_size);
  // adding texCoords (may factor out to other classes)
  np->texCoords.reserve(m_size * 3);
  for(int i = 0; i < m_size; ++i)
  {
    // get index from current face
    const Mesh::Face &F = m.faceArray()[i];
    
    glm::vec4 v1 = m.vertexArray()[F.index1];
    glm::vec4 v2 = m.vertexArray()[F.index2];
    glm::vec4 v3 = m.vertexArray()[F.index3];

    // adding texCoords (may factor out to other classes)
    glm::vec2 tc = glm::vec2(0.0f, 0.0f);
    // pushback onto NMesh's vertices
    np->vertices.push_back(v1);
    np->vertices.push_back(v2);
    np->vertices.push_back(v3);
    // adding texCoords (may factor out to other classes)
    np->texCoords.push_back(tc);
    np->texCoords.push_back(tc);
    np->texCoords.push_back(tc);

    // create a face and pushback onto NMesh's faces
    np->faces.push_back(Mesh::Face(3 * i, 3 * i + 1, 3 * i + 2));
    // recalculate normal for each face(a Mesh object doesn't necessary have normalArray unless it's NoramlMesh)
    glm::vec4 v2_v1 = v2 - v1;
    glm::vec4 v3_v1 = v3 - v1;
    
    glm::vec4 normal = cs250::cross(v2_v1, v3_v1);
    // push 3 normal to normals
    np->normals.push_back(normal);
    np->normals.push_back(normal);
    np->normals.push_back(normal);
  }
  
  //for (int i = 0; i < m.edgeCount(); ++i)
  //{
  //    np->edges.push_back(m.edgeArray()[i]);
  //}



  return np;
}

//bool compareVector(const glm::vec3& lhs, const glm::vec3& rhs)
//{
//
//    float V = glm::dot(lhs, rhs);
//
//    //glm::vec3 V = glm::cross(lhs, rhs);
//    //bool bRetCode = glm::epsilonNotEqual(V, 1.0f, 0.01f);
//    bool bRetCode = glm::epsilonEqual(V, 1.0f, 0.01f);
//    //bool result = false;
//    //if ((V * V) > 0 && lhs != rhs)
//    //if ((V.x == 0.0f && V.y == 0.0f && V.z== 0.0f) && rhs != lhs)
//        //result = true;
//    //return result;
//    return bRetCode;
//}
