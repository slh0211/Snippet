/******************************************************************************/
/*!
\file   Geometry.h
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS350
\par    Assignment 2 Geometry Library
\par    Spring 2021
\date   2/12/2021
\brief
    This file contains the prototype of  Geomerty library functions

  Hours spent on this assignment:

  Specific portions that gave you the most trouble:

*/
/******************************************************************************/
#pragma once
#include <glm/glm.hpp> //glm::vec3, glm::vec4

/******************************************************************************/
/*!
  \class Point3D
  
  \brief
    represents a 3D point for intersection detection
*/
/******************************************************************************/
class Point3D
{
public:
    Point3D();
    Point3D(const glm::vec3& point);
    Point3D(const float x, const float y, const float z);

    glm::vec3 m_coordinates;
private:
};
/******************************************************************************/
/*!
  \class Plane
  
  \brief
    represents a Plane for intesection detection
*/
/******************************************************************************/
class Plane
{
public:
    Plane();
    Plane(const glm::vec3& normal, const float distance);
    Plane(const float nx, const float ny, const float nz, const float distance);

    //glm::vec4 m_Normal;
    glm::vec3 m_Normal;
    float m_d;
private:
};
/******************************************************************************/
/*!
  \class Triangle

  \brief
    represents a Triangle for intesection detection
*/
/******************************************************************************/
class Triangle
{
public:
    Triangle();
    Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);

    glm::vec3 m_v0;
    glm::vec3 m_v1;
    glm::vec3 m_v2;
private:
};
/******************************************************************************/
/*!
  \class Sphere

  \brief
    represents a Sphere for intesection detection
*/
/******************************************************************************/
class Sphere
{
public:
    Sphere();
    Sphere(const glm::vec3& position, const float radius);

    glm::vec3 m_Position;
    float m_Radius;

private:
};
/******************************************************************************/
/*!
  \class Aabb

  \brief
    represents a Aabb for intesection detection
*/
/******************************************************************************/
class Aabb
{
public:
    Aabb();
    Aabb(const glm::vec3& min, const glm::vec3& max);
    Aabb(const glm::vec4& center, float radius);

    Aabb minkowskiDifference(const Aabb& other) const;

    bool Colliding() const;

    glm::vec3 m_Min;
    glm::vec3 m_Max;
    glm::vec3 m_Center;
    float m_Radius;
private:
};
/******************************************************************************/
/*!
  \class Ray

  \brief
    represents a Ray for intesection detection
    reused from cs500
*/
/******************************************************************************/
class Ray
{
public:
    Ray();
    Ray(const glm::vec3& start, const glm::vec3& direction);

    //Vector3f eval(float t) const;

    glm::vec3 m_Q; // Starting point
    glm::vec3 m_D; // Direction - should be scaled to a unit length vector

private:
};

bool PointSphere(const Point3D& point, const Sphere& sphere);
bool SphereSphere(const Sphere& a, const Sphere& b);
bool PointAabb(const Point3D& point, const Aabb& aabb);
bool AabbAabb(const Aabb& a, const Aabb& b);
bool AabbSphere(const Aabb& aabb, const Sphere& sphere);
bool SphereAabb(const Sphere& sphere, const Aabb& aabb);
bool PointTriangle(const Point3D& p, const Triangle& triangle);
bool PointPlane(const Point3D& point, const Plane& plane);
bool RayPlane(const Ray& ray, const Plane& plane);
bool RayAabb(const Ray& ray, const Aabb& aabb);
bool RaySphere(const Ray& ray, const Sphere& sphere);
bool RayTriangle(const Ray& ray, const Triangle& triangle);
bool PlaneAabb(const Plane& plane, const Aabb& aabb);
bool PlaneSphere(const Plane& plane, const Sphere& sphere);
bool PlaneTriangle(const Plane& plane, const Triangle& triangle);


// helpers
float SqDistPointAabb(const Point3D& point, const Aabb& aabb);
inline float TriArea2D(const float x0, const float y0, const float x1, const float y1, const float x2, const float y2);
void Barycentric(const Point3D& a, const Point3D& b, const Point3D& c, const Point3D& p, float& u, float& v, float& w);
/******************************************************************************/
/*!
  \class Interval

  \brief
    Helper class to compare and rearrange interval when doint intersection
    detection for ray and aabb
*/
/******************************************************************************/
class Interval
{
public:
    Interval();

    Interval(const float t0, const float t1, const glm::vec3& n0, const glm::vec3& n1);
    void empty();

    void intersect(const Interval& other);

    void intersect(const Ray& ray, const glm::vec3& N, const float d0, const float d1);
    //Interval intersect(const Ray& ray, const Vector3f& N, const float d0, const float d1);


//private:
    float _t0;
    float _t1;
    glm::vec3 _n0;
    glm::vec3 _n1;
};

enum class Point_Classification
{
    POINT_ON_PLANE = 0,
    POINT_IN_FRONT_OF_PLANE = 1,
    POINT_BEHIND_PLANE = 2
};

Point_Classification ClassifyPointToPlane(const glm::vec3& p, const Plane& plane);
glm::vec3 IntersectEdgeAgainstPlane(const glm::vec3& a, const glm::vec3& b, const Plane& plane);