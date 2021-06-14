//////////////////////////////////////////////////////////////////////
// Provides the framework for a raytracer.
////////////////////////////////////////////////////////////////////////
//#pragma once
#include <vector>


#ifdef _WIN32
    // Includes for Windows
    #include <windows.h>
    #include <cstdlib>
    #include <limits>
    #include <crtdbg.h>
#else
    // Includes for Linux
#endif

#include "geom.h"
#include "raytrace.h"
#include "realtime.h"

#include "raytracerenderer.h"
#include "minimizer.h"
//#include "shape.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//// A good quality *thread-safe* Mersenne Twister random number generator.
//#include <random>
//std::random_device device;
//std::mt19937_64 RNGen(device());
//std::uniform_real_distribution<> myrandom(0.0, 1.0);
//// Call myrandom(RNGen) to get a uniformly distributed random number in [0,1].

#include <chrono>
#include "rand.h"
#include <Eigen/Eigenvalues> 


Scene::Scene() 
{ 
    realtime = new Realtime(); 
    _pRayTracer = new RayTraceRenderer();
}

Scene::~Scene()
{
    delete realtime;
    delete _pRayTracer;
}

void Scene::Finit()
{
}

void Scene::triangleMesh(MeshData* mesh) 
{ 
    realtime->triangleMesh(mesh); 
    _pRayTracer->addMesh(mesh);
}

Quaternionf Orientation(int i, 
                        const std::vector<std::string>& strings,
                        const std::vector<float>& f)
{
    Quaternionf q(1,0,0,0); // Unit quaternion
    while (i<strings.size()) {
        std::string c = strings[i++];
        if (c == "x")  
            q *= angleAxis(f[i++]*Radians, Vector3f::UnitX());
        else if (c == "y")  
            q *= angleAxis(f[i++]*Radians, Vector3f::UnitY());
        else if (c == "z")  
            q *= angleAxis(f[i++]*Radians, Vector3f::UnitZ());
        else if (c == "q")  {
            q *= Quaternionf(f[i+0], f[i+1], f[i+2], f[i+3]);
            i+=4; }
        else if (c == "a")  {
            q *= angleAxis(f[i+0]*Radians, Vector3f(f[i+1], f[i+2], f[i+3]).normalized());
            i+=4; } }
    return q;
}

////////////////////////////////////////////////////////////////////////
// Material: encapsulates surface properties
void Material::setTexture(const std::string path)
{
    int width, height, n;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image = stbi_load(path.c_str(), &width, &height, &n, 0);

    // Realtime code below:  This sends the texture in *image to the graphics card.
    // The raytracer will not use this code (nor any features of OpenGL nor the graphics card).
    glGenTextures(1, &texid);
    glBindTexture(GL_TEXTURE_2D, texid);
    glTexImage2D(GL_TEXTURE_2D, 0, n, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 100);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int)GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (int)GL_LINEAR_MIPMAP_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(image);
}



void Scene::Command(const std::vector<std::string>& strings,
                    const std::vector<float>& f)
{
    if (strings.size() == 0) return;
    std::string c = strings[0];
    
    if (c == "screen") {
        // syntax: screen width height
        realtime->setScreen(int(f[1]),int(f[2]));
        width = int(f[1]);
        height = int(f[2]); 
        _pRayTracer->setScreen(int(f[1]), int(f[2]));
    }

    else if (c == "camera") {
        // syntax: camera x y z   ry   <orientation spec>
        // Eye position (x,y,z),  view orientation (qw qx qy qz),  frustum height ratio ry
        realtime->setCamera(Vector3f(f[1],f[2],f[3]), Orientation(5,strings,f), f[4]); 
        _pRayTracer->setCamera(Vector3f(f[1], f[2], f[3]), Orientation(5, strings, f), f[4]);
    }

    else if (c == "ambient") {
        // syntax: ambient r g b
        // Sets the ambient color.  Note: This parameter is temporary.
        // It will be ignored once your raytracer becomes capable of
        // accurately *calculating* the true ambient light.
        realtime->setAmbient(Vector3f(f[1], f[2], f[3])); }

    else if (c == "brdf") {
        // syntax: brdf  r g b   r g b  alpha
        // later:  brdf  r g b   r g b  alpha  r g b ior
        // First rgb is Diffuse reflection, second is specular reflection.
        // third is beer's law transmission followed by index of refraction.
        // Creates a Material instance to be picked up by successive shapes
        if (f.size() == 12)
        {
            currentMat = new Material(Vector3f(f[1], f[2], f[3]), Vector3f(f[4], f[5], f[6]), f[7], Vector3f(f[8], f[9], f[10]), f[11]);
        }
        else
        {
            currentMat = new Material(Vector3f(f[1], f[2], f[3]), Vector3f(f[4], f[5], f[6]), f[7]);
        }
            _pRayTracer->addMaterial(currentMat);
        }
    else if (c == "light") {
        // syntax: light  r g b   
        // The rgb is the emission of the light
        // Creates a Material instance to be picked up by successive shapes
            currentMat = new Light(Vector3f(f[1], f[2], f[3]));
            _pRayTracer->addMaterial(currentMat);
        }
   
    else if (c == "sphere") {
        // syntax: sphere x y z   r
        // Creates a Shape instance for a sphere defined by a center and radius
            realtime->sphere(Vector3f(f[1], f[2], f[3]), f[4], currentMat);
            Sphere* pSphere = new Sphere(Vector3f(f[1], f[2], f[3]), f[4], currentMat);
            _pRayTracer->addShape(pSphere);
        }

    else if (c == "box") {
        // syntax: box bx by bz   dx dy dz
        // Creates a Shape instance for a box defined by a corner point and diagonal vector
            realtime->box(Vector3f(f[1], f[2], f[3]), Vector3f(f[4], f[5], f[6]), currentMat);
            Box* pBox = new Box(Vector3f(f[1], f[2], f[3]), Vector3f(f[4], f[5], f[6]), currentMat);
            _pRayTracer->addShape(pBox);
        }

    else if (c == "cylinder") {
        // syntax: cylinder bx by bz   ax ay az  r
        // Creates a Shape instance for a cylinder defined by a base point, axis vector, and radius
            realtime->cylinder(Vector3f(f[1], f[2], f[3]), Vector3f(f[4], f[5], f[6]), f[7], currentMat); 
            Cylinder* pCylinder = new Cylinder(Vector3f(f[1], f[2], f[3]), Vector3f(f[4], f[5], f[6]), f[7], currentMat);
            _pRayTracer->addShape(pCylinder);
        }


    else if (c == "mesh") {
        // syntax: mesh   filename   tx ty tz   s   <orientation>
        // Creates many Shape instances (one per triangle) by reading
        // model(s) from filename. All triangles are rotated by a
        // quaternion (qw qx qy qz), uniformly scaled by s, and
        // translated by (tx ty tz) .
        Matrix4f modelTr = translate(Vector3f(f[2],f[3],f[4]))
                          *scale(Vector3f(f[5],f[5],f[5]))
                          *toMat4(Orientation(6,strings,f));
        ReadAssimpFile(strings[1], modelTr);  }

    else if (c == "union") {
        Shape* obj2 = _pRayTracer->_pShapes.back();
        _pRayTracer->_pShapes.pop_back();

        Shape* obj1 = _pRayTracer->_pShapes.back();
        _pRayTracer->_pShapes.pop_back();

        Union* pUnion = new Union(obj1, obj2, currentMat);
        _pRayTracer->addShape(pUnion);
    }

    else if (c == "intersect") {
        Shape* obj2 = _pRayTracer->_pShapes.back();
        _pRayTracer->_pShapes.pop_back();

        Shape* obj1 = _pRayTracer->_pShapes.back();
        _pRayTracer->_pShapes.pop_back();

        Intersect* pIntersect = new Intersect(obj1, obj2, currentMat);
        _pRayTracer->addShape(pIntersect);
    }

    else if (c == "difference") {
        Shape* obj2 = _pRayTracer->_pShapes.back();
        _pRayTracer->_pShapes.pop_back();

        Shape* obj1 = _pRayTracer->_pShapes.back();
        _pRayTracer->_pShapes.pop_back();

        Difference* pDifference = new Difference(obj1, obj2, currentMat);
        _pRayTracer->addShape(pDifference);
    }

    else if (c == "rotatey") {

        Shape* obj1 = _pRayTracer->_pShapes.back();
        _pRayTracer->_pShapes.pop_back();

        Quaternionf q = Quaternionf::FromTwoVectors(Vector3f::UnitZ(), Vector3f::UnitY());

        Rotate* pRotate = new Rotate(obj1, q);
        _pRayTracer->addShape(pRotate);
    }

    else if (c == "rotatex") {

    Shape* obj1 = _pRayTracer->_pShapes.back();
    _pRayTracer->_pShapes.pop_back();

    Quaternionf q = Quaternionf::FromTwoVectors(Vector3f::UnitZ(), Vector3f::UnitX());

    Rotate* pRotate = new Rotate(obj1, q);
    _pRayTracer->addShape(pRotate);
    }

    else if (c == "translate") {

    Shape* obj1 = _pRayTracer->_pShapes.back();
    _pRayTracer->_pShapes.pop_back();

    Vector3f t = Vector3f(f[1], f[2], f[3]);

    //Matrix4f transM = translate(t);

    //EigenSolver<Matrix4f> es(transM, false);

    //std::cout << es.eigenvalues() << std::endl;

    Translate* pTranslate = new Translate(obj1, t);
    _pRayTracer->addShape(pTranslate);
    }
    else if (c == "skydome") {
        realtime->sphere(Vector3f(f[1], f[2], f[3]), f[4], currentMat);
        Skydome* pSkydome = new Skydome(Vector3f(f[1], f[2], f[3]), f[4], currentMat);
        _pRayTracer->addShape(pSkydome);
        _pRayTracer->_radius = f[4];
    }
    
    else {
        fprintf(stderr, "\n*********************************************\n");
        fprintf(stderr, "* Unknown command: %s\n", c.c_str());
        fprintf(stderr, "*********************************************\n\n");
    }
}

void Scene::TraceImage(Color* image, const int pass)
{
    realtime->run();                          // Remove this (realtime stuff)


    //Vector3f eye = realtime->eye;
    //Quaternionf q = realtime->orient;
    //float ry = realtime->ry;

    //int w = realtime->width;
    //int h = realtime->height;



    //Camera cam = _pRayTracer->getCamera();
    Camera cam = _pRayTracer->getCamera();
    //cam.Set(eye, q, ry, w, h);
    //cam._eye = eye;
    //cam._X = cam._rx * cam._orient._transformVector(Vector3f::UnitX());
    //cam._Y = cam._ry * cam._orient._transformVector(Vector3f::UnitY());
    //cam._Z = -1.0f * cam._orient._transformVector(Vector3f::UnitZ());

    
    //Sphere* s = static_cast<Sphere* >(_pRayTracer->_pLights[0]);

    //KdBVH<float, 3, Shape*> Tree(_pRayTracer->_pShapes.begin(), _pRayTracer->_pShapes.end());
    _pRayTracer->buildBVH();
    std::chrono::steady_clock sc;   // create an object of `steady_clock` class
    auto start = sc.now();     // start timer

    int p = 64;

    for (unsigned int i = 1; i <= p; ++i)
    {
#pragma omp parallel for schedule(dynamic, 1) // Magic: Multi-thread y loop
        for (int y = 0; y < height; y++) {

            fprintf(stderr, "Rendering %4d\r", y);
            for (int x = 0; x < width; x++) {
                //Color color;
                //if ((x-width/2)*(x-width/2)+(y-height/2)*(y-height/2) < 100*100)
                //    color = Color(myrandom(RNGen), myrandom(RNGen), myrandom(RNGen));
                //else if (abs(x-width/2)<4 || abs(y-height/2)<4)
                //    color = Color(0.0, 0.0, 0.0);
                //else 
                //    color = Color(1.0, 1.0, 1.0);

                //float dx = 2.0f * (x + 0.5f) / width - 1.0f;
                //float dy = 2.0f * (y + 0.5f) / height - 1.0f;

                float dx = 2.0f * (x + getARandomFloat()) / width - 1.0f;
                float dy = 2.0f * (y + getARandomFloat()) / height - 1.0f;

                //float dx = 2.0f * (x + myrandom(RNGen)) / width - 1.0f;
                //float dy = 2.0f * (y + myrandom(RNGen)) / height - 1.0f;


                Ray ray = Ray(cam._eye, Vector3f(dx * cam._X + dy * cam._Y + cam._Z));

                //Minimizer minimizer(ray);
                //float minDist = BVMinimize(_pRayTracer->_tree, minimizer);

                //color = _pRayTracer->TracePath(ray);
                //color = _pRayTracer->TracePath(ray);
                Color result = _pRayTracer->TracePath(ray);
                image[y * width + x] += CheckColorValue(result);

                //image[y * width + x] += _pRayTracer->TracePath(ray);
               // image[y * width + x] = image[y * width + x] / i;
                // for each shape


                //Intersection result;
                //
                //
                //result._t = FLT_MAX;
                //for (unsigned int i = 0; i < _pRayTracer->_pShapes.size(); ++i)
                //{
                //    // test if it intescet with ray
                //    Intersection intsec;
                //
                //    //intsec._t = INF;
                //    // if it hits a shape
                //    bool hit = _pRayTracer->_pShapes[i]->intersect(ray, intsec);
                //    if(hit)
                //    {
                //        // collect smallest t
                //        if (intsec._t < result._t)
                //        {
                //            result = intsec;
                //        }
                //    }
                //}

                //// calculate color
                ////if (result._t != FLT_MAX)
                //if (minDist != FLT_MAX)
                //{
                //    //lighting
                //    //Vector3f L = (s->_center - result._P);
                //    //L.normalize();
                //    //Vector3f out = result._N.dot(L) * result._pObject->_pMaterial->Kd;
                //    //color = Color(out);
                //
                //    //kd
                //    //color = Color(result._pObject->_pMaterial->Kd);
                //    //t
                //    //float out = (result._t - 5.0f) / 4.0f;
                //    //color = Color(out);
                //    //normal
                //    //color = Color(result._N);
                //    //color = color.abs();
                //    
                //        //color += _pRayTracer->_pLights[i]->_pMaterial.
                //
                //    //lighting
                //    Vector3f L = (s->_center - minimizer._result._P);
                //    L.normalize();
                //    Vector3f out = minimizer._result._N.dot(L) * minimizer._result._pObject->_pMaterial->Kd;
                //    color = Color(out);
                //    
                //    //kd
                //    //color = Color(minimizer._result._pObject->_pMaterial->Kd);
                //    
                //    //t
                //    //float outt = (minimizer._result._t - 5.0f) / 4.0f;
                //    //color = outt;
                //
                //    ////normal
                //    //color = Color(minimizer._result._N);
                //    //color = color.abs();
                //}
                //else
                //    color = Color(1.0, 1.0, 1.0);


                //image[y * width + x] = color/i;
                //image[y * width + x] = color;
            }
        }

    }

    for (unsigned i = 0; i < width * height; ++i)
        image[i] = image[i] / p;

    auto end = sc.now();       // end timer (starting & ending is done by measuring the time at the moment the process started & ended respectively)
    auto time_span = static_cast<std::chrono::duration<double>>(end - start);   // measure time span between start & end
    std::cout << "Operation took: " << time_span.count() << " seconds !!!";
    fprintf(stderr, "\n");
}
