#pragma once
#include "camera.h"

Camera::Camera() : _ry(0.0f)
{
}

Camera::Camera(const Vector3f& eye, const Quaternionf& orient, const float ry) : _eye(eye), _orient(orient), _ry(ry)
{
}

void Camera::Set(const Vector3f& eye, const Quaternionf& orient, const float ry, const int width, const int height)
{
	_eye = eye;
	_orient = orient;
	_ry = ry;

	_rx = _ry * width / height;

	_X = _rx * _orient._transformVector(Vector3f::UnitX());
	_Y = _ry * _orient._transformVector(Vector3f::UnitY());
	_Z = -1.0f * _orient._transformVector(Vector3f::UnitZ());
}

