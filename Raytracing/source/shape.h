#pragma once
#include "material.h"
#include "ray.h"
#include "intersection.h"
#include "meshdata.h"

class Shape
{
public:
	virtual bool intersect(const Ray& ray, Intersection& intersect) = 0;
	// added project 5
	virtual float distance(const Vector3f& P) const = 0;
	virtual ~Shape() {};

	bool isLight() const;
	Bbox bbox() const;



//protected:
	Material* _pMaterial;
	Bbox _box;
};

class Sphere : public Shape
{
public:
	Sphere(const Vector3f& center, const float radius, Material * const mat);

	bool intersect(const Ray& ray, Intersection& intersect) override;
	float distance(const Vector3f& P) const override;
	Vector3f _center;
	float _r;

	Intersection SampleSphere(const Vector3f C, float R);
};

class Box : public Shape
{
public:
	Box(const Vector3f& cornerVec, const Vector3f& diagonalVec, Material * const mat);

	bool intersect(const Ray& ray, Intersection& intersect) override;
	float distance(const Vector3f& P) const override;
private:
	Vector3f _cornerVec;
	Vector3f _diagonalVec;
};

class Cylinder : public Shape
{
public:
	Cylinder(const Vector3f& base, const Vector3f& axis, const float radius, Material* const mat);

	bool intersect(const Ray& ray, Intersection& intersect) override;
	float distance(const Vector3f& P) const override;
private:
	Vector3f _base;
	Vector3f _axis;
	float	_r;
};

class Triangle : public Shape
{
public:
	Triangle(MeshData* meshdata, unsigned int index0, unsigned int index1, unsigned int index2);
	//Triangle(const Vector3f& v0, const Vector3f& v1, const Vector3f& v2, const Vector3f& n0, const Vector3f& n1, const Vector3f& n2, Material * const mat);

	bool intersect(const Ray& ray, Intersection& intersect) override;
	float distance(const Vector3f& P) const override;
private:
	MeshData* _pMesh;
	unsigned int _i0;
	unsigned int _i1;
	unsigned int _i2;

	//Vector3f _v0;
	//Vector3f _v1;
	//Vector3f _v2;
	//Vector3f _n0;
	//Vector3f _n1;
	//Vector3f _n2;
};

class Interval
{
public:
	Interval();

	Interval(const float t0, const float t1, const Vector3f& n0, const Vector3f& n1);
	void empty();

	void intersect(const Interval& other);

	void intersect(const Ray& ray, const Vector3f& N, const float d0, const float d1);
	//Interval intersect(const Ray& ray, const Vector3f& N, const float d0, const float d1);


//private:
	float _t0;
	float _t1;
	Vector3f _n0;
	Vector3f _n1;
};

Bbox bounding_box(const Shape* obj);

class RayMarchShape : public Shape //Pure virtual
{
public:
	virtual bool intersect(const Ray& ray, Intersection& intersect);

	virtual float distance(const Vector3f& P) const = 0;
};

class Union : public RayMarchShape
{
public:
	Union(Shape* A, Shape* B, Material* const mat);
	~Union() override;

	float distance(const Vector3f& P) const override;

	Shape* _A;
	Shape* _B;
};


class Intersect : public RayMarchShape
{
public:
	Intersect(Shape* A, Shape* B, Material* const mat);
	~Intersect() override;

	float distance(const Vector3f& P) const override;

	Shape* _A;
	Shape* _B;
};

class Difference : public RayMarchShape
{
public:
	Difference(Shape* A, Shape* B, Material* const mat);
	~Difference() override;

	float distance(const Vector3f& P) const override;

	Shape* _A;
	Shape* _B;
};

class Rotate : public RayMarchShape
{
public:
	Rotate(Shape* A, Quaternionf q);
	~Rotate() override;

	float distance(const Vector3f& P) const override;

	Shape* _A;
	Quaternionf _q;
};

class Translate : public RayMarchShape
{
public:
	Translate(Shape* A, const Vector3f& t);
	~Translate() override;

	float distance(const Vector3f& P) const override;

	Shape* _A;
	Vector3f _translation;
};

class Skydome : public Shape
{
public:
	Skydome(const Vector3f& center, const float radius, Material* const mat);

	bool intersect(const Ray& ray, Intersection& intersect) override;
	float distance(const Vector3f& P) const override;

	Vector3f _center;
	float _r;
};