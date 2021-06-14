#pragma once
#include <iostream>
#include "geom.h"

Quaternionf angleAxis(const float& angle, const Vector3f& axis)
{ return Quaternionf(AngleAxisf(angle, axis)); }

Matrix4f toMat4(const Quaternionf& q)
{ return Transform<float,3,Affine>(q).matrix(); }

Matrix4f rotate(const float& angle, const Vector3f& axis)
{  return Transform<float,3,Affine>(
                         Quaternionf(AngleAxisf(angle, axis))).matrix(); }
        
Matrix4f scale(const Vector3f& v)
{  return Transform<float,3,Affine>(
                         DiagonalMatrix<float,3>(v)).matrix(); }

Matrix4f translate(const Vector3f& v)
{  return Transform<float,3,Affine>(Translation<float,3>(v)).matrix(); }

// The standard glFrustum perspective projection matrix.
Matrix4f frustum(float const& left,    float const& right,
                 float const& bottom,  float const& top,
                 float const& nearVal, float const& farVal)
{
    Matrix4f R = Matrix4f::Zero();
    R(0,0) = (2.0*nearVal)         / (right-left);
    R(1,1) = (2.0*nearVal)         / (top-bottom);
    R(0,2) = (right+left)          / (right-left);
    R(1,2) = (top+bottom)          / (top-bottom);
    R(2,2) = -(farVal+nearVal)     / (farVal-nearVal);
    R(3,2) = -1.0;
    R(2,3) = -(2.0*farVal*nearVal) / (farVal-nearVal);

    return R;
}

template <> void Print(const std::string& s, const float& f)
{ std::cout << s << ": " << f << std::endl; }

template <> void Print(const std::string& s, const double& f)
{ std::cout << s << ": " << f << std::endl; }

template <> void Print(const std::string& s, const Quaternionf& q)
{
    IOFormat Fmt(3, DontAlignCols, ", ", "  |  ", "", "", ": [", "]\n");
    std::cout << s << ": " << q.w() << q.vec().format(Fmt); 
}

