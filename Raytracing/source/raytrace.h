//#pragma once
//#include "raytracerenderer.h"
///////////////////////////////////////////////////////////////////////
// A framework for a raytracer.
////////////////////////////////////////////////////////////////////////
#include "material.h"
#include "meshdata.h"
class Shape;

const float PI = 3.14159f;

//////////////////////////////////////////////////////////////////////////
//// Material: encapsulates a BRDF and communication with a shader.
//////////////////////////////////////////////////////////////////////////
//class Material
//{
// public:
//    Vector3f Kd, Ks;
//    float alpha;
//    unsigned int texid;
//
//    virtual bool isLight() { return false; }
//
//    Material()  : Kd(Vector3f(1.0, 0.5, 0.0)), Ks(Vector3f(1,1,1)), alpha(1.0), texid(0) {}
//    Material(const Vector3f d, const Vector3f s, const float a) 
//        : Kd(d), Ks(s), alpha(a), texid(0) {}
//    Material(Material& o) { Kd=o.Kd;  Ks=o.Ks;  alpha=o.alpha;  texid=o.texid; }
//
//    void setTexture(const std::string path);
//    //virtual void apply(const unsigned int program);
//};

//////////////////////////////////////////////////////////////////////////
//// Data structures for storing meshes -- mostly used for model files
//// read in via ASSIMP.
////
//// A MeshData holds two lists (stl::vector) one for vertices
//// (VertexData: consisting of point, normal, texture, and tangent
//// vectors), and one for triangles (TriData: consisting of three
//// indices into the vertex array).
//typedef Eigen::Matrix<unsigned int, 3, 1 > TriData;
//    
//class VertexData
//{
// public:
//    Vector3f pnt;
//    Vector3f nrm;
//    Vector2f tex;
//    Vector3f tan;
//    VertexData(const Vector3f& p, const Vector3f& n, const Vector2f& t, const Vector3f& a) 
//        : pnt(p), nrm(n), tex(t), tan(a) 
//    {}
//};
//
//struct MeshData
//{
//    std::vector<VertexData> vertices;
//    std::vector<TriData> triangles;
//    Material *mat;
//};

//////////////////////////////////////////////////////////////////////////
//// Light: encapsulates a light and communiction with a shader.
//////////////////////////////////////////////////////////////////////////
//class Light: public Material
//{
//public:
//
//    Light(const Vector3f e) : Material() { Kd = e; }
//    virtual bool isLight() { return true; }
//    //virtual void apply(const unsigned int program);
//};

////////////////////////////////////////////////////////////////////////////////
// Scene
class Realtime;

class RayTraceRenderer;

class Scene {
public:
    int width, height;
    Realtime* realtime;         // Remove this (realtime stuff)
    Material* currentMat;

    RayTraceRenderer* _pRayTracer;

    Scene();
    ~Scene();
    void Finit();

    // The scene reader-parser will call the Command method with the
    // contents of each line in the scene file.
    void Command(const std::vector<std::string>& strings,
                 const std::vector<float>& f);

    // To read a model file into the scene via ASSIMP, call ReadAssimpFile.  
    void ReadAssimpFile(const std::string& path, const Matrix4f& M);

    // Once ReadAssimpFile parses the information from the model file,
    // it will call:
    void triangleMesh(MeshData* mesh);

    // The main program will call the TraceImage method to generate
    // and return the image.  This is the Ray Tracer!
    void TraceImage(Color* image, const int pass);
};
