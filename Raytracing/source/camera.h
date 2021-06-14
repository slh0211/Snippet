#pragma once
#include "geom.h"

class Camera
{
public:
    Camera();
    Camera(const Vector3f& eye, const Quaternionf& orient, const float ry);

    void Set(const Vector3f& eye, const Quaternionf& orient, const float ry, const int width, const int height);

    Vector3f _eye;      // Position of eye for viewing scene
    Quaternionf _orient;   // Represents rotation of -Z to view direction
    float _ry;
    float _rx;
    Vector3f _X;
    Vector3f _Y;
    Vector3f _Z;
};