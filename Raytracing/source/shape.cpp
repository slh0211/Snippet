#pragma once
#include "shape.h"
#include "rand.h"

# define EPSILON 0.0001f
# define M_PI           3.14159265358979323846f  /* pi */

Sphere::Sphere(const Vector3f& center, const float radius, Material* const mat) : _center(center), _r(radius)
{
	_pMaterial = mat;

	const Vector3f& bmin = _center - Vector3f(_r, _r, _r);
	const Vector3f& bmax = _center + Vector3f(_r, _r, _r);
	
	Bbox box0 = Bbox(bmin, bmax);
	//box0.extend(bmax);

	_box = box0;
}

bool Sphere::intersect(const Ray& ray, Intersection& intersect)
{
	const Vector3f Qbar = ray._Q - _center;

	const float QbarDotD = Qbar.dot(ray._D);
	const float QbarDotQbar = Qbar.dot(Qbar);

	const float discriminant = QbarDotD * QbarDotD - QbarDotQbar + _r * _r;

	// if discriminaant < 0, no intersection
	if (discriminant < 0.0f)
		return false;

	const float sqrtDiscri = sqrt(discriminant);

	const float tminus = -QbarDotD - sqrtDiscri;
	const float tplus = -QbarDotD + sqrtDiscri;
	// if both are negative, no interscetion
	if (tminus < 0.0f && tplus < 0.0f)
		return false;

	float t = 0.0f;

	// else return smallest positive value
	if (tminus > EPSILON)
	//if (tminus > 0.0f)
	{
		t = tminus;
	}
	else if(tplus > EPSILON)
	{
		t = tplus;
	}

	intersect._pObject = this;
	intersect._t = t;
	intersect._P = ray.eval(t);
	intersect._N = intersect._P - _center;
	intersect._N.normalize();
	return true;
}

float Sphere::distance(const Vector3f& P) const
{
	return (P - _center).norm() -_r;
}

Intersection Sphere::SampleSphere(const Vector3f C, float R)
{
	float xi1 = getARandomFloat();
	float xi2 = getARandomFloat();

	float z = 2.0f * xi1 - 1.0f;

	float r = sqrt(1.0f - z * z);

	float a = 2.0f * M_PI * xi2;

	Vector3f N = Vector3f(r * cos(a), r * sin(a), z);

	
	N.normalize();
	Vector3f P = C + R * N;

	Intersection aPointOnThis;

	aPointOnThis._N = N;
	aPointOnThis._P = P;
	aPointOnThis._pObject = this;


	return aPointOnThis;
}

bool Shape::isLight() const
{
	return _pMaterial->isLight();
}

Bbox Shape::bbox() const
{
	return _box;
}

Box::Box(const Vector3f& cornerVec, const Vector3f& diagonalVec, Material * const mat) : _cornerVec(cornerVec), _diagonalVec(diagonalVec)
{
	_pMaterial = mat;

	const Vector3f& bmin = _cornerVec;
	const Vector3f& bmax = _cornerVec + _diagonalVec;

	Bbox box0 = Bbox(bmin);
	//Bbox box0 = Bbox(bmin, bmax);
	box0.extend(bmax);

	_box = box0;
}

bool Box::intersect(const Ray& ray, Intersection& intersect)
{
	Interval iv0 = Interval();

	iv0.intersect(ray, Vector3f(1.0f, 0.0f, 0.0f), -_cornerVec.x(), -_cornerVec.x() - _diagonalVec.x());
	if (iv0._t0 > iv0._t1)
	{
		return false;
	}
	iv0.intersect(ray, Vector3f(0.0f, 1.0f, 0.0f), -_cornerVec.y(), -_cornerVec.y() - _diagonalVec.y());
	if (iv0._t0 > iv0._t1)
	{
		return false;
	}
	iv0.intersect(ray, Vector3f(0.0f, 0.0f, 1.0f), -_cornerVec.z(), -_cornerVec.z() - _diagonalVec.z());

	//Interval iv1 = iv0.intersect(ray, Vector3f(1.0f, 0.0f, 0.0f), -_cornerVec.x(), -_cornerVec.x() - _diagonalVec.x());
	//Interval iv2 = iv1.intersect(ray, Vector3f(0.0f, 1.0f, 0.0f), -_cornerVec.y(), -_cornerVec.y() - _diagonalVec.y());
	//Interval iv3 = iv2.intersect(ray, Vector3f(0.0f, 0.0f, 1.0f), -_cornerVec.z(), -_cornerVec.z() - _diagonalVec.z());

	//iv0 = iv3;
	if (iv0._t0 > iv0._t1)
	{
		return false;
	}
	else
	{
		if (iv0._t0 > EPSILON)
		//if (iv0._t0 >= 0.0f)
		{
			intersect._t = iv0._t0;
			intersect._N = iv0._n0;
			intersect._P = ray.eval(intersect._t);
			intersect._pObject = this;
			intersect._N.normalize();
			return true;
		}
		else if (iv0._t1 > EPSILON)
		//else if(iv0._t1 >= 0.0f)
		{
			intersect._t = iv0._t1;
			intersect._N = iv0._n1;
			intersect._P = ray.eval(intersect._t);
			intersect._pObject = this;
			intersect._N.normalize();
			return true;
		}

		return false;
	}


	//return false;
}

float Box::distance(const Vector3f& P) const
{
	const Vector3f boxMin = _cornerVec;
	const Vector3f boxMax = boxMin + _diagonalVec;

	// too ugly to use six std::max in one line
	std::vector<float> values;
	// Px - Xmax
	values.push_back(P.x() - boxMax.x());
	// Xmin - Px
	values.push_back(boxMin.x() - P.x());
	
	// Py - Ymax
	values.push_back(P.y() - boxMax.y());
	// Ymin - Py
	values.push_back(boxMin.y() - P.y());
	
	// Pz - Zmax
	values.push_back(P.z() - boxMax.z());
	// Zmin - Pz
	values.push_back(boxMin.z() - P.z());

	// set max to the 1st value
	float output = values[0];
	// go through the list and compare with other 5 values
	for (unsigned int i = 1; i < values.size(); ++i)
	{
		output = std::max(output, values[i]);
	}

	return output;
}

Cylinder::Cylinder(const Vector3f& base, const Vector3f& axis, const float radius, Material* const mat)	: _base(base), _axis(axis), _r(radius)
{
	_pMaterial = mat;
	
	const Vector3f& basemin = _base - Vector3f(_r, _r, _r);
    const Vector3f& basemax = _base + Vector3f(_r, _r, _r);
	const Vector3f& topmin = _base + _axis - Vector3f(_r, _r, _r);
	const Vector3f& topmax = _base + _axis + Vector3f(_r, _r, _r);

	Bbox box0 = Bbox(basemin, basemax);
	Bbox box1 = Bbox(topmin, topmax);
	//Bbox box0 = Bbox(basemin);
	//box0.extend(basemax);
	//box0.extend(topmin);
	//box0.extend(topmax);



	box0.extend(box1);
	
	_box = box0;
}

bool Cylinder::intersect(const Ray& ray, Intersection& intersect)
{
	Interval iv0 = Interval();

	//Interval iv1 = Interval();

	const float d1 = -(_axis.norm());
	
	const Quaternionf q = Quaternionf::FromTwoVectors(_axis, Vector3f::UnitZ());
	Vector3f Qbar = q._transformVector(ray._Q - _base);
	Vector3f Dbar = q._transformVector(ray._D);

	const Ray raybar(Qbar, Dbar);
	// need to use transformed ray instead of the original one
	iv0.intersect(raybar, Vector3f(0.0f, 0.0f, 1.0f), 0.0f, d1);

	//Interval iv1a = iv1.intersect(raybar, Vector3f(0.0f, 0.0f, 1.0f), 0.0f, d1);



	const float a = (Dbar.x() * Dbar.x() + Dbar.y() * Dbar.y());
	const float b = 2.0f * (Dbar.x() * Qbar.x() + Dbar.y() * Qbar.y());
	const float c = (Qbar.x() * Qbar.x() + Qbar.y() * Qbar.y() - (_r * _r));

	const float discri = (b * b - 4.0f * a * c);

	if (discri < 0.0f)
	{
		return false;
	}

	const float sqrtd = sqrt(discri);

	const float tminus = (-b - sqrtd) / (2.0f * a);
	const float tplus = (-b + sqrtd) / (2.0f * a);

	Vector3f Mminus = Qbar + tminus * Dbar;
	const Vector3f Nminusbar = Vector3f(Mminus.x(), Mminus.y(), 0.0f);
	const Vector3f Nminus = q.conjugate()._transformVector(Nminusbar);


	Vector3f Mplus = Qbar + tplus * Dbar;
	const Vector3f Nplusbar = Vector3f(Mplus.x(), Mplus.y(), 0.0f);
	const Vector3f Nplus = q.conjugate()._transformVector(Nplusbar);

	const Interval iv2 = Interval(tminus, tplus, Nminus, Nplus);

	//iv0.intersect(iv1);
	//iv1.intersect(iv2);
	iv0.intersect(iv2);
	//iv0.intersect(iv1a);

	if (iv0._t0 > iv0._t1)
	{
		return false;
	}


	
	    if (iv0._t0 > EPSILON)
		//if (iv0._t0 >= 0.0f)
		{
			intersect._t = iv0._t0;
			intersect._N = iv0._n0;
			intersect._P = ray.eval(intersect._t);
			intersect._pObject = this;
			intersect._N.normalize();


			return true;
		}
		else if (iv0._t1 > EPSILON)
		//else if (iv0._t1 >= 0.0f)
		{
			intersect._t = iv0._t1;
			intersect._N = iv0._n1;
			intersect._P = ray.eval(intersect._t);
			intersect._pObject = this;
			intersect._N.normalize();


			return true;
		}
		else
		{
			return false;
		}



}

float Cylinder::distance(const Vector3f& P) const
{
	Vector2f Pxy = Vector2f(P.x(), P.y());

	return Pxy.norm() - _r;
}

//Triangle::Triangle(const Vector3f& v0, const Vector3f& v1, const Vector3f& v2, const Vector3f& n0, const Vector3f& n1, const Vector3f& n2, Material * const mat)	: _v0(v0), _v1(v1), _v2(v2), _n0(n0), _n1(n1), _n2(n2)
//{
//	_pMaterial = mat;
//}

Triangle::Triangle(MeshData* meshdata, unsigned int index0, unsigned int index1, unsigned int index2) : _pMesh(meshdata), _i0(index0), _i1(index1), _i2(index2)
{
	_pMaterial = _pMesh->mat;

	const Vector3f& V0 = _pMesh->vertices[_i0].pnt;
	const Vector3f& V1 = _pMesh->vertices[_i1].pnt;
	const Vector3f& V2 = _pMesh->vertices[_i2].pnt;

	Bbox box0 = Bbox(V0);
	box0.extend(V1);
	box0.extend(V2);

	//float minx = std::min(V0.x(), std::min(V1.x(), V2.x()));
	//float miny = std::min(V0.y(), std::min(V1.y(), V2.y()));
	//float minz = std::min(V0.z(), std::min(V1.z(), V2.z()));
	//
	//float maxx = std::max(V0.x(), std::max(V1.x(), V2.x()));
	//float maxy = std::max(V0.y(), std::max(V1.y(), V2.y()));
	//float maxz = std::max(V0.z(), std::max(V1.z(), V2.z()));
	//
	//Bbox box0 = Bbox(Vector3f(minx, miny, minz), Vector3f(maxx, maxy, maxz));

	_box = box0;
}

bool Triangle::intersect(const Ray& ray, Intersection& intersect)
{
	const Vector3f& V0 = _pMesh->vertices[_i0].pnt;
	const Vector3f& V1 = _pMesh->vertices[_i1].pnt;
	const Vector3f& V2 = _pMesh->vertices[_i2].pnt;

	Vector3f N0 = _pMesh->vertices[_i0].nrm;
	Vector3f N1 = _pMesh->vertices[_i1].nrm;
	Vector3f N2 = _pMesh->vertices[_i2].nrm;




	const Vector3f& E1 = V1 - V0;
	const Vector3f& E2 = V2 - V0;
	const Vector3f& p = ray._D.cross(E2);
	const float d = p.dot(E1);

	if (d == 0.0f)
	{
		return false;
	}

	const Vector3f& S = ray._Q - V0;
	const float u = (p.dot(S) )/ d;

	if (u < 0.0f || u > 1.0f)
	{
		return false;
	}

	const Vector3f& q = S.cross(E1);
	const float v = (ray._D.dot(q)) / d;

	if (v < 0.0f || (u + v) > 1.0f)
	{
		return false;
	}

	const float t = (E2.dot(q)) / d;

	if (t < 0.0f)
	{
		return false;
	}

	intersect._pObject = this;
	intersect._t = t;
	intersect._P = ray.eval(t);

	N0.normalize();
	N1.normalize();
	N2.normalize();

	intersect._N = ((1.0f - u - v) * N0 + u * N1 + v * N2);
	intersect._N.normalize();




	return true;
}

float Triangle::distance(const Vector3f& P) const
{
	// figure it out later!
	return 0.0f;
}

Interval::Interval() : _t0(0.0f), _t1(FLT_MAX)
{
}

Interval::Interval(const float t0, const float t1, const Vector3f& n0, const Vector3f& n1)
{
	if (t0 > t1)
	{
		_t0 = t1;
		_t1 = t0;
		_n0 = n1;
		_n1 = n0;
	}
	else
	{
		_t0 = t0;
		_t1 = t1;
		_n0 = n0;
		_n1 = n1;
	}

}

void Interval::empty()
{
	_t0 = 0.0f;
	_t1 = -1.0f;
}

void Interval::intersect(const Interval& other)
{
	const float t0out = std::max(_t0, other._t0);

	if (t0out == other._t0)
	{
		_t0 = t0out;
		_n0 = other._n0;
	}

	const float t1out = std::min(_t1, other._t1);

	if (t1out == other._t1)
	{
		_t1 = t1out;
		_n1 = other._n1;
	}
}

//Interval Interval::intersect(const Ray& ray, const Vector3f& N, const float d0, const float d1)
void Interval::intersect(const Ray& ray, const Vector3f& N, const float d0, const float d1)
{
	const float NdotD = N.dot(ray._D);
	const float NdotQ = N.dot(ray._Q);

	if (NdotD != 0.0f)
	{
		const float t0 = -(d0 + NdotQ) / NdotD;
		const float t1 = -(d1 + NdotQ) / NdotD;

		const Interval iv = Interval(t0, t1, -N, N);

		this->intersect(iv);
		//iv.intersect(*this);
		//return iv;
		//return Interval(_t0, _t1, -N, N);
	}
	else
	{
		const float s0 = NdotQ + d0;
		const float s1 = NdotQ + d1;

		if ((s0 > 0.0f && s1 < 0.0f) || (s0 < 0.0f && s1 > 0.0f))
		{
			const Interval iv = Interval();

			*this = iv;
			//return iv;
			//return Interval();
		}
		else
		{
			this->empty();
			//Interval iv = Interval();
			//iv.empty();
			//return iv;
			//return *this;
		}
	}
}

Bbox bounding_box(const Shape* obj)
{
	return obj->bbox();
}

bool RayMarchShape::intersect(const Ray& ray, Intersection& intersect)
{
	const float epsilon = 0.001f;
	float t = epsilon;
	unsigned steps = 0;
	float DistanceLimit = 10000.0f;

	while (true)
	{
		Vector3f P = ray.eval(t);

		float dt = this->distance(P);

		t = t + abs(dt);
		
		if (abs(dt) < 0.000001f)
			break;

		if (steps > 2500)
			return false;

		if (t >= DistanceLimit)
			return false;

		steps++;
	}

	if (t <= epsilon)
		return false;

	// Calculate Normal (via centeral difference)
	intersect._P = ray.eval(t);
	float h = epsilon;
	Vector3f P = intersect._P;

	float nx = this->distance(Vector3f(P.x() + h, P.y(), P.z())) - this->distance(Vector3f(P.x() - h, P.y(), P.z()));
	float ny = this->distance(Vector3f(P.x(), P.y() + h, P.z())) - this->distance(Vector3f(P.x(), P.y() - h, P.z()));
	float nz = this->distance(Vector3f(P.x(), P.y(), P.z() + h)) - this->distance(Vector3f(P.x(), P.y(), P.z() - h));

	intersect._N = Vector3f(nx, ny, nz).normalized();
	intersect._t = t;
	intersect._pObject = this;

	return true;
}

Intersect::Intersect(Shape* A, Shape* B, Material* const mat): _A(A), _B(B)
{
	_pMaterial = mat;

	Bbox box0 = Bbox(_A->bbox());
	box0.extend(_B->bbox());

	_box = box0;
}

Intersect::~Intersect()
{
	delete _A;
	delete _B;
}

float Intersect::distance(const Vector3f& P) const
{
	return std::max(_A->distance(P), _B->distance(P));
}

Union::Union(Shape* A, Shape* B, Material* const mat) : _A(A), _B(B)
{
	_pMaterial = mat;

	Bbox box0 = Bbox(_A->bbox());
	box0.extend(_B->bbox());

	_box = box0;
}

Union::~Union()
{
	delete _A;
	delete _B;
}

float Union::distance(const Vector3f& P) const
{
	return std::min(_A->distance(P), _B->distance(P));
}

Difference::Difference(Shape* A, Shape* B, Material* const mat) : _A(A), _B(B)
{
	_pMaterial = mat;

	Bbox box0 = Bbox(_A->bbox());
	box0.extend(_B->bbox());

	_box = box0;
}

Difference::~Difference()
{
	delete _A;
	delete _B;
}

float Difference::distance(const Vector3f& P) const
{
	return std::max(_A->distance(P), -_B->distance(P));
}

Rotate::Rotate(Shape* A, Quaternionf q) : _A(A), _q(q)
{
	_pMaterial = _A->_pMaterial;
	_box = _A->bbox();
}

Rotate::~Rotate()
{
	delete _A;
}

float Rotate::distance(const Vector3f& P) const
{
	return _A->distance(_q.conjugate()._transformVector(P));
}

Translate::Translate(Shape* A, const Vector3f& t) : _A(A), _translation(t)
{
	_pMaterial = _A->_pMaterial;

	Vector3f min = _A->bbox().min() + _translation;
	Vector3f max = _A->bbox().max() + _translation;

	Bbox box0(min);



	box0.extend(max);
	_box = box0;
}

Translate::~Translate()
{
	delete _A;
}

float Translate::distance(const Vector3f& P) const
{

	//return _A->distance(P - _translation)/(1.0f / _translation.z());
	return _A->distance(P - _translation);
}

Skydome::Skydome(const Vector3f& center, const float radius, Material* const mat) : _center(center), _r(radius)
{
	_pMaterial = mat;

	const Vector3f& bmin = _center - Vector3f(_r, _r, _r);
	const Vector3f& bmax = _center + Vector3f(_r, _r, _r);

	Bbox box0 = Bbox(bmin, bmax);
	//box0.extend(bmax);

	_box = box0;
}

bool Skydome::intersect(const Ray& ray, Intersection& intersect)
{
	// No real intersection calculation. Just extend ray's direction to sphere radius
	// Set normal to -ray's direction
	intersect._pObject = this;
	intersect._t = _r;
	intersect._P = ray.eval(_r);
	intersect._N = -ray._D;
	intersect._N.normalize();

	return true;
}

// dummy function not used
float Skydome::distance(const Vector3f& P) const
{
	return 0.0f;
}
