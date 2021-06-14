/******************************************************************************/
/*!
\file   raytracerenderer.h
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS500
\par    Assignment Raytrace
\par    Winter 2021
\date   2/4/2021
\brief
	This file contains the implementation of RayTraceRenderer class

  Hours spent on this assignment: 

  Specific portions that gave you the most trouble:

*/
/******************************************************************************/
#pragma once
//#include "raytrace.h"
#include <vector>
#include "camera.h"
#include "material.h"
#include "shape.h"
#include "meshdata.h"
#include "minimizer.h"
#include "rgbe.h"


class RayTraceRenderer
{
public:
	RayTraceRenderer();
	~RayTraceRenderer();

	void setScreen(int width, int height);
	void setCamera(const Vector3f& eye, const Quaternionf& orient, const float ry);
	void addMaterial(Material* material);
	void addShape(Shape * const pshape);
	void addMesh(MeshData* mesh);
	Camera getCamera() const;

	Color TracePath(const Ray& ray);

	void buildBVH();


	Camera _camera;
	int	_width;
	int	_height;

	std::vector<Material*> _pMaterials;
	std::vector<Shape*> _pShapes;
	std::vector<Shape*> _pLights;

	KdBVH<float, 3, Shape*> _tree;

	// as5 ibl
	// skydome image width
	int _bkWidth;
	// skydome image height
	int _bkHeight;
	// skydome image rgb float buffer
	float* _bkImage;
	//
	float* pBuffer;
	float* pUDist;
	//
	float _angle;
	//
	float _radius;


	int chooseLight() const;
	Intersection SampleLight() const;
	float PdfLight(const Intersection& Q) const;
	float GeometryFactor(const Intersection& A, const Intersection& B) const;
	// as5 ibl
	float* ReadHdrImage(const std::string inName, int* backgroundWidth, int* backgroundHeight);

	Color EvalRadianceIBL(const Intersection& A);
	void PreProcessCDF();
	Intersection SampleLightIBL() const;
	float PdfLightIBL(const Intersection& B) const;
};