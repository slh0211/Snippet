/******************************************************************************/
/*!
\file   raytracerenderer.cpp
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
#include "raytracerenderer.h"
#include "rand.h"

# define RussianRoulette 0.8f
# define M_PI           3.14159265358979323846f  /* pi */

RayTraceRenderer::RayTraceRenderer()
{
	_bkImage = ReadHdrImage("Barce_Rooftop_C_3k.hdr", &_bkWidth, &_bkHeight);
	//_bkImage = ReadHdrImage("10-Shiodome_Stairs_3k.hdr", &_bkWidth, &_bkHeight);
	//_bkImage = ReadHdrImage("14-Hamarikyu_Bridge_B_3k.hdr", &_bkWidth, &_bkHeight);
	//_bkImage = ReadHdrImage("Tropical_Beach_3k.hdr", &_bkWidth, &_bkHeight);

	PreProcessCDF();

	_angle = 1.5f;
	//// sanity check I know how to open, read, write and close hdr file
	//rgbe_header_info info;
	//char errbuf[100] = { 0 };
	//info.valid = false;
	//FILE* fp = fopen("out.hdr", "wb");
	//info.valid = false;
	//int r = RGBE_WriteHeader(fp, _bkWidth, _bkHeight, &info, errbuf);
	//if (r != RGBE_RETURN_SUCCESS)
	//    printf("error: %s\n", errbuf);
	//
	//r = RGBE_WritePixels_RLE(fp, _bkImage, _bkWidth, _bkHeight, errbuf);
	//if (r != RGBE_RETURN_SUCCESS)
	//    printf("error: %s\n", errbuf);
	//fclose(fp);
}

RayTraceRenderer::~RayTraceRenderer()
{
	for (unsigned i = 0; i < _pShapes.size(); ++i)
	{
		delete _pShapes[i];
		//_pShapes[i] = NULL;	 
	}

	//for (unsigned i = 0; i < _pLights.size(); ++i)
	//{
	//	delete _pLights[i];
	//}
	for (unsigned i = 0; i < _pMaterials.size(); ++i)
	{
		delete _pMaterials[i]; 
	}

	delete[] _bkImage;
	delete[] pBuffer;
}

void RayTraceRenderer::setScreen(int width, int height)
{
	_width = width;
	_height = height;
}

void RayTraceRenderer::setCamera(const Vector3f& eye, const Quaternionf& orient, const float ry)
{
	_camera.Set(eye, orient, ry, _width, _height);
}

void RayTraceRenderer::addMaterial(Material* material)
{
	_pMaterials.push_back(material);
}

void RayTraceRenderer::addShape(Shape * const pshape)
{
	_pShapes.push_back(pshape);
	if (pshape->isLight())
	{
		_pLights.push_back(pshape);
	}
}

void RayTraceRenderer::addMesh(MeshData* mesh)
{
	for (unsigned int i = 0; i < mesh->triangles.size(); ++i)
	{
		const TriData& tri = mesh->triangles[i];
		
		//const Vector3f v0 = mesh->vertices[tri[0]].pnt;
		//const Vector3f v1 = mesh->vertices[tri[1]].pnt;
		//const Vector3f v2 = mesh->vertices[tri[2]].pnt;
		//
		//const Vector3f n0 = mesh->vertices[tri[0]].nrm;
		//const Vector3f n1 = mesh->vertices[tri[1]].nrm;
		//const Vector3f n2 = mesh->vertices[tri[2]].nrm;
		//
		//Material* mat = mesh->mat;

		Triangle* pTriangle = new Triangle(mesh, tri[0], tri[1], tri[2]);

		addShape(pTriangle);
	}
}

Camera RayTraceRenderer::getCamera() const
{
	return _camera;
}

Color RayTraceRenderer::TracePath(const Ray& ray)
{
	Color C = Color(0.0f, 0.0f, 0.0f); // Accumulated light
	Color W = Color(1.0f, 1.0f, 1.0f); // Accumulated weight
	// Initial ray
	Minimizer minimizer(ray);
	float minDist = BVMinimize(_tree, minimizer);

	Intersection P = minimizer._result;
	//Vector3f N = P._N;
	// No intersection
	if (minDist == FLT_MAX)
	{
		return C;
	}
	// P is a Light
	if (P._pObject->isLight())
	{
		//return EvalRadiance(P);
		return EvalRadianceIBL(P);
	}

	Vector3f omegao = -(ray._D);
	//omegao.normalize();
	while (getARandomFloat() <= RussianRoulette)
	{
		Vector3f N = P._N;
		N.normalize();
		omegao.normalize();
		Vector3f omegai = P._pObject->_pMaterial->SampleBrdf(omegao, N);
		
		// Explicit light connection
		// Randomly choose a light and a point on that light
		
		//Intersection L = SampleLight();
		Intersection L = SampleLightIBL();
		// Probability of L, coverted to angular measure
		//float p0 = PdfLight(L) / GeometryFactor(P, L);
		float p0 = PdfLightIBL(L);
		
		// Direction from P toward L
		Vector3f omegai0 = L._P - P._P;

		//float q = L._pObject->_pMaterial->PdfBrdf(omegao, N, omegai0) * RussianRoulette;
		//float q = P._pObject->_pMaterial->PdfBrdf(omegao, N, omegai0) * RussianRoulette;
		float q = P._pObject->_pMaterial->PdfBrdf(omegao, N, omegai) * RussianRoulette;
		float wmis = p0 * p0 / (p0 * p0 + q * q);

		Ray shadowray = Ray(P._P, omegai0);
		
		omegai0.normalize();
		
		Minimizer mi0(shadowray);
		float md0 = BVMinimize(_tree, mi0);
		
		//Vector3f ref = Vector3f(0.0001f, 0.0001f, 0.0001f);
		
		//if (p0 > 0.0f && (md0 != FLT_MAX) && (mi0._result._P - L._P).isMuchSmallerThan(ref))
		//if (p0 > 0.0f && (md0 != FLT_MAX) && mi0._result._P.isApprox(L._P))
		//if (p0 > 0.0f && (md0 != FLT_MAX) && mi0._result._P == L._P && mi0._result._pObject == L._pObject)
		if (p0 > 0.0f && (md0 != FLT_MAX) && mi0._result._pObject == L._pObject)
		{
			//Color f = P._pObject->_pMaterial->EvalScattering(P._N, omegai0);
		
		

		
			//Color f = P._pObject->_pMaterial->EvalScattering(omegao, N, omegai0);
			Color f = P._pObject->_pMaterial->EvalScattering(omegao, N, omegai0, P._t);
		
			//C += 0.5f * W * f / p0 * EvalRadiance(L);
			//C += W * f / p0 * EvalRadiance(L);
			//C += W * wmis * f / p0 * EvalRadiance(L);
			
			//C += W * f / p0 * EvalRadianceIBL(L);
			C += W * wmis * f / p0 * EvalRadianceIBL(L);
		}
		

		// Extend path
		//Vector3f N = P._N;
		//N.normalize();
		// Choose a sample direction from P
		//Vector3f omegai = P._pObject->_pMaterial->SampleBrdf(N);
		
		omegai.normalize();
		
		// Create a new ray
		Ray newray = Ray(P._P, omegai);
		Minimizer mi(newray);
		// Trace ray from P in direction omegai into the scene
		float md = BVMinimize(_tree, mi);
		// if Q is non-existent: break
		if (md == FLT_MAX)
		{
			break;
		}

		Intersection Q = mi._result;
        
		//Color f = P._pObject->_pMaterial->EvalScattering(N, omegai);
		//Color f = P._pObject->_pMaterial->EvalScattering(omegao, N, omegai);
		//Color f = Q._pObject->_pMaterial->EvalScattering(omegao, N, omegai);
		Color f = P._pObject->_pMaterial->EvalScattering(omegao, N, omegai, P._t);

		//float p = P._pObject->_pMaterial->PdfBrdf(N, omegai) * RussianRoulette;
		float p = P._pObject->_pMaterial->PdfBrdf(omegao, N, omegai) * RussianRoulette;
		//float p = Q._pObject->_pMaterial->PdfBrdf(omegao, N, omegai) * RussianRoulette;

		if (p < 0.000001f)
		{
			break;
		}

		W *= f / p;

		// Implicit light connection
		
		if (Q._pObject->isLight())
		{
			//q = PdfLight(Q) / GeometryFactor(P, Q);
			q = PdfLightIBL(Q);
			wmis = p * p / (p * p + q * q);

			//C += W* wmis * EvalRadiance(Q);
			//C += W * EvalRadiance(Q);
			//C += 0.5f * W * EvalRadiance(Q);
			// may change if an object has both light and brdf properties

			//C += W * EvalRadianceIBL(Q);
			C += W * wmis * EvalRadianceIBL(Q);
			break;
		}

		P = Q;
		omegao = -omegai;
	}


	return C;
}

void RayTraceRenderer::buildBVH()
{
	_tree = KdBVH<float, 3, Shape*>(_pShapes.begin(), _pShapes.end());
}

int RayTraceRenderer::chooseLight() const
{
	if(_pLights.size() == 1)
		return 0;
	
	return getARandomInt(0, _pLights.size() - 1);
}

Intersection RayTraceRenderer::SampleLight() const
{
	int lightIndex = chooseLight();

	Sphere* s = static_cast<Sphere*>(_pLights[lightIndex]);

	return s->SampleSphere(s->_center, s->_r);
}

float RayTraceRenderer::PdfLight(const Intersection& Q) const
{
	Sphere* s = static_cast<Sphere*>(Q._pObject);



	float AreaOfLight = 4.0f * M_PI * s->_r * s->_r;

	int NumberOfLights = _pLights.size();


	return 1.0f / (AreaOfLight * NumberOfLights);
}

float RayTraceRenderer::GeometryFactor(const Intersection& A, const Intersection& B) const
{
	Vector3f D = A._P - B._P;

	//D = D.normalized();

	return abs(A._N.dot(D) *B._N.dot(D) / (D.dot(D) * D.dot(D)));
}

float* RayTraceRenderer::ReadHdrImage(const std::string inName, int* backgroundWidth, int* backgroundHeight)
{
	// Read image header info from file in HDR (a.k.a RADIANCE) format
	rgbe_header_info info;
	char errbuf[100] = { 0 };
	info.valid = false;

	int width;
	int height;

	// open file for reading
	FILE* fp = fopen(inName.c_str(), "rb");

	// Read Header to get width and height
	int r = RGBE_ReadHeader(fp, &width, &height, &info, errbuf);

	if (r != RGBE_RETURN_SUCCESS)
		printf("error: %s\n", errbuf);
	//else
	//   std::cout << width << " " << height << std::endl;

	// allocate chunk of memory to store read in data
	float* data = new float[width * height * 3];

	r = RGBE_ReadPixels_RLE(fp, data, width, height, errbuf);

	if (r != RGBE_RETURN_SUCCESS)
		printf("error: %s\n", errbuf);

	fclose(fp);

	*backgroundWidth = width;
	*backgroundHeight = height;

	return data;
}

Color RayTraceRenderer::EvalRadianceIBL(const Intersection& A)
{
	Vector3f P = A._P.normalized();

	//float angle = M_PI / float(_bkHeight);
	float angle = _angle;

	double u = (angle - atan2(P[1], P[0])) / (M_PI * 2.0);
	u = u - floor(u);
	double v = acos(P[2]) / M_PI;
	int i0 = floor(u * _bkWidth);
	int j0 = floor(v * _bkHeight);
	double uw[2], vw[2];

	uw[1] = u * _bkWidth - i0; uw[0] = 1.0 - uw[1];
	vw[1] = v * _bkHeight - j0; vw[0] = 1.0 - vw[1];
	Vector3f r(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			int k = 3 * (((j0 + j) % _bkHeight) * _bkWidth + ((i0 + i) % _bkWidth));
			for (int c = 0; c < 3; c++) {
				r[c] += uw[i] * vw[j] * _bkImage[k + c];
			}
		}
	}


	return r;
}

void RayTraceRenderer::PreProcessCDF()
{
	// Pre-processing step: Marginal and conditional CDF
	pBuffer = new float[_bkWidth * (_bkHeight + 1)];
	pUDist = &pBuffer[_bkWidth * _bkHeight];
	float* pSinTheta = new float[_bkHeight];
	float angleFrac = M_PI / float(_bkHeight);
	float theta = angleFrac * 0.5f;
	for (unsigned int i = 0; i < _bkHeight; i++, theta += angleFrac)
		pSinTheta[i] = sin(theta);
	for (unsigned int i = 0, m = 0; i < _bkWidth; i++, m += _bkHeight) {
		float* pVDist = &pBuffer[m];
		unsigned int k = i * 3;
		pVDist[0] = 0.2126f * _bkImage[k + 0] + 0.7152f * _bkImage[k + 1] + 0.0722f * _bkImage[k + 2];
		pVDist[0] *= pSinTheta[0];
		for (unsigned int j = 1, k = (_bkWidth + i) * 3; j < _bkHeight; j++, k += _bkWidth * 3) {
			float lum = 0.2126 * _bkImage[k + 0] + 0.7152 * _bkImage[k + 1] + 0.0722 * _bkImage[k + 2];
			pVDist[j] = pVDist[j - 1] + lum * pSinTheta[j];
		}
		if (i == 0)
			pUDist[i] = pVDist[_bkHeight - 1];
		else
			pUDist[i] = pUDist[i - 1] + pVDist[_bkHeight - 1];
	}

}

Intersection RayTraceRenderer::SampleLightIBL() const
{
	Intersection B;
	double u = getARandomFloat();
	double v = getARandomFloat();
	float maxUVal = pUDist[_bkWidth - 1];
	float* pUPos = std::lower_bound(pUDist, pUDist + _bkWidth,
		u * maxUVal);
	int iu = pUPos - pUDist;
	float* pVDist = &pBuffer[_bkHeight * iu];
	float* pVPos = std::lower_bound(pVDist, pVDist + _bkHeight,
		v * pVDist[_bkHeight - 1]);
	int iv = pVPos - pVDist;

	//float angle = M_PI / float(_bkHeight);
	float angle = _angle;

	double phi = angle - 2 * M_PI * iu / _bkWidth;
	double theta = M_PI * iv / _bkHeight;
	B._N = Vector3f(sin(theta) * cos(phi),
		sin(theta) * sin(phi),
		cos(theta));

	//B._P = B._N * radius;
	B._P = B._N * 1000.0f;
	B._pObject = _pLights[0];
	return B;
}
/******************************************************************************/
/*!
  \brief
	Calculate probability density (PDF) given an intersection record for 
	image based lighting using preprocessed array table

  \param B
	Intersection record where ray intersect with skydome

  \return pdf
	The probability 
*/
/******************************************************************************/
float RayTraceRenderer::PdfLightIBL(const Intersection& B) const
{
	Vector3f P = B._P.normalized();
	// adjust angle (rotating the texture of skydome)
	float angle = _angle;

	double fu = (angle - atan2(P[1], P[0])) / (M_PI *2.0);
	fu = fu - floor(fu); // Wrap to be within 0...1
	// calculate u v for skydome texture
	int u = floor(_bkWidth * fu);
	int v = floor(_bkHeight * acos(P[2]) / M_PI);
	
	float angleFrac = M_PI / float(_bkHeight);
	float* pVDist = &pBuffer[_bkHeight * u];

	// calculate pdf of uv using preprocessed table
	float pdfU = (u == 0) ? (pUDist[0]) : (pUDist[u] - pUDist[u - 1]);
	pdfU /= pUDist[_bkWidth - 1];
	pdfU *= _bkWidth / (M_PI * 2.0);
	float pdfV = (v == 0) ? (pVDist[0]) : (pVDist[v] - pVDist[v - 1]);
	pdfV /= pVDist[_bkHeight - 1];
	pdfV *= _bkHeight / M_PI;
	float theta = angleFrac * 0.5 + angleFrac * v;
	// using pdf UV to calculate final pdf (the pdf of the pixel to be hit)
	float pdf = pdfU * pdfV * sin(theta) / (4.0 * M_PI * _radius * _radius);
	//printf("(%f %f %f) %d %d %g\n", P[0], P[1], P[2], u, v, pdf);
	return pdf;
}

