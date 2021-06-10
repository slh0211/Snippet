/******************************************************************************/
/*!
\file   Geometry.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS350
\par    Assignment 2 Geometry Library
\par    Spring 2021
\date   2/12/2021
\brief
	This file contains the implementation of Geomerty library functions

  Hours spent on this assignment:

  Specific portions that gave you the most trouble:

*/
/******************************************************************************/
#include "Geometry.h"
#include <algorithm>  // min, max
/******************************************************************************/
/*!
  \brief
	default ctor for Point3D
*/
/******************************************************************************/
Point3D::Point3D() : m_coordinates(glm::vec3())
{
}
/******************************************************************************/
/*!
  \brief
	non-default default ctor for Point3D

  \param point
	a vec3
*/
/******************************************************************************/
Point3D::Point3D(const glm::vec3& point) : m_coordinates(point)
{
}
/******************************************************************************/
/*!
  \brief
	non-default default ctor for Point3D

  \param x
	vec3.x
  \param y
    vec3.y
  \param z
    vec3.z
*/
/******************************************************************************/
Point3D::Point3D(const float x, const float y, const float z) : m_coordinates(glm::vec3(x, y, z))
{
}
/******************************************************************************/
/*!
  \brief
	default ctor for Plane
*/
/******************************************************************************/
Plane::Plane() : m_Normal(glm::vec3()), m_d(0.0f)
{
}
/******************************************************************************/
/*!
  \brief
	non-default default ctor for Plane

  \param normal
	normal
  \param distance
    distance from plane to the origin in (normal dot P) = distance form  
*/
/******************************************************************************/
Plane::Plane(const glm::vec3& normal, const float distance)
{
	float l = length(normal);
	glm::vec3 n = normalize(normal);
	// d has to be divided by length of the original normal to match normalized normal
	float d = distance / l;

	//m_Normal = glm::vec4(n.x, n.y, n.z, d);
	// separate ax + by + cz = distance like the orange book
	m_Normal = n;
	m_d = d;

}
/******************************************************************************/
/*!
  \brief
	non-default default ctor for Plane

  \param nx
	normal.x
  \param ny
    normal.y
  \param nz
    normal.z
  \param distance
	distance from plane to the origin in (normal dot P) = distance form
*/
/******************************************************************************/
Plane::Plane(const float nx, const float ny, const float nz, const float distance)
{
	float l = length(glm::vec3(nx, ny, nz));
	glm::vec3 n = normalize(glm::vec3(nx, ny, nz));
	// d has to be divided by length of the original normal to match normalized normal
	float d = distance / l;

	//m_Normal = glm::vec4(n.x, n.y, n.z, d);
	// separate ax + by + cz = distance like the orange book
	m_Normal = n;
	m_d = d;
}
/******************************************************************************/
/*!
  \brief
	default ctor for Triangle
*/
/******************************************************************************/
Triangle::Triangle() : m_v0(glm::vec3()), m_v1(glm::vec3()), m_v2(glm::vec3())
{
}
/******************************************************************************/
/*!
  \brief
	non-default default ctor for Triangle

  \param v0
	vertex v0
  \param v1
	vertex v1
  \param v2
    vertex v2
*/
/******************************************************************************/
Triangle::Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) : m_v0(v0), m_v1(v1), m_v2(v2)
{
}
/******************************************************************************/
/*!
  \brief
	default ctor for Sphere
*/
/******************************************************************************/
Sphere::Sphere(): m_Position(glm::vec3()), m_Radius(0.0f)
{
}
/******************************************************************************/
/*!
  \brief
	non-default default ctor for Sphere

  \param position
	Sphere center
  \param radius
	Sphere radius
*/
/******************************************************************************/
Sphere::Sphere(const glm::vec3& position, const float radius) : m_Position(position), m_Radius(radius)
{
}
/******************************************************************************/
/*!
  \brief
	default ctor for Aabb
*/
/******************************************************************************/
Aabb::Aabb() : m_Min(glm::vec3()), m_Max(glm::vec3()), m_Center(glm::vec3()), m_Radius(0.0f)
{
}
/******************************************************************************/
/*!
  \brief
	non-default default ctor for Aabb
	no eror handling done when invalid min and max inserted

  \param min
	aabb min
  \param max
	aabb max
*/
/******************************************************************************/
Aabb::Aabb(const glm::vec3& min, const glm::vec3& max) : m_Min(min), m_Max(max)
{
	m_Center = (m_Min + m_Max) / 2.0f;
	m_Radius = length(m_Max - m_Center);
}
/******************************************************************************/
/*!
  \brief
	non-default default ctor for Aabb use center and radius to construct
	no eror handling done when invalid min and max inserted

  \param center
	center
  \param radius
	radius
*/
/******************************************************************************/
Aabb::Aabb(const glm::vec4& center, float radius) : m_Center(center), m_Radius(radius)
{
	m_Min = glm::vec3(center.x - radius, center.y - radius, center.z - radius);
	m_Max = glm::vec3(center.x + radius, center.y + radius, center.z + radius);
}
/******************************************************************************/
/*!
  \brief
	given two Aabb, calculate their mikowskiDifference for collision detection

  \param other
	the other Aabb

  \return Aabb
    new Aabb of minkowski Difference
*/
/******************************************************************************/
Aabb Aabb::minkowskiDifference(const Aabb& other) const
{
	glm::vec3 new_Min = m_Min - other.m_Max;
	float new_Radius = m_Radius + other.m_Radius;

	glm::vec3 new_Center = glm::vec3(new_Min.x + new_Radius, new_Min.y + new_Radius, new_Min.z + new_Radius);

	return Aabb(glm::vec4(new_Center, 1.0f), new_Radius);
}
/******************************************************************************/
/*!
  \brief
	Check if this Aabb has origin inside

  \return 
	true if origin is inside, else false
*/
/******************************************************************************/
bool Aabb::Colliding() const
{
	if (m_Min.x <= 0.0f && m_Max.x >= 0.0f && m_Min.y <= 0.0f && m_Max.y >= 0.0f && m_Min.z <= 0 && m_Max.z >= 0)
	{
		return true;
	}

	return false;
}
/******************************************************************************/
/*!
  \brief
	default ctor for Ray
*/
/******************************************************************************/
Ray::Ray() : m_Q(glm::vec3()), m_D(glm::vec3())
{
}
/******************************************************************************/
/*!
  \brief
	non default ctor for Ray
  
  \param start
	starting point of the ray
  \param direction
    direction vector of the ray
*/
/******************************************************************************/
Ray::Ray(const glm::vec3& start, const glm::vec3& direction) : m_Q(start), m_D(normalize(direction))
{
}
/******************************************************************************/
/*!
  \brief
	Check point and sphere's intersection

  \param point
	a Point3D class
  \param sphere
	a Sphere class
  \return 
    true if intersected else false
*/
/******************************************************************************/
bool PointSphere(const Point3D& point, const Sphere& sphere)
{
	// calculate distance between the point and sphere center
	float d = glm::length(point.m_coordinates - sphere.m_Position);
	// compare d * d with r * r
	float r_sq = sphere.m_Radius * sphere.m_Radius;


	return ((d * d) <= r_sq);
}
/******************************************************************************/
/*!
  \brief
	Check sphere and sphere's intersection

  \param a
	a Sphere class
  \param b
	a Sphere class
  \return
	true if intersected else false
*/
/******************************************************************************/
bool SphereSphere(const Sphere& a, const Sphere& b)
{
	return PointSphere(b.m_Position, Sphere(a.m_Position, a.m_Radius + b.m_Radius));
}
/******************************************************************************/
/*!
  \brief
	Check point and aabb's intersection

  \param point
	a Point3D class
  \param aabb
	an Aabb class
  \return
	true if intersected else false
*/
/******************************************************************************/
bool PointAabb(const Point3D& point, const Aabb& aabb)
{
	if (point.m_coordinates.x < aabb.m_Min.x)
		return false;
	if (point.m_coordinates.x > aabb.m_Max.x)
		return false;
	if (point.m_coordinates.y < aabb.m_Min.y)
		return false;
	if (point.m_coordinates.y > aabb.m_Max.y)
		return false;
	if (point.m_coordinates.z < aabb.m_Min.z)
		return false;
	if (point.m_coordinates.z > aabb.m_Max.z)
		return false;

	return true;
}
/******************************************************************************/
/*!
  \brief
	Check aabb and aabb's intersection

  \param a
	an Aabb class
  \param b
	an Aabb class
  \return
	true if intersected else false
*/
/******************************************************************************/
bool AabbAabb(const Aabb& a, const Aabb& b)
{
	// for each axis { X, Y, Z }
	for (unsigned int i = 0; i < 3; ++i)
	{
		// if no overlap for the axis, no overlap overall
		if (a.m_Max[i] < b.m_Min[i] || b.m_Max[i] < a.m_Min[i])
			return false;
	}
	return true;
}
/******************************************************************************/
/*!
  \brief
	Check aabb and sphsere's intersection

  \param aabb
	an Aabb class
  \param sphere
	a Sphere class
  \return
	true if intersected else false
*/
/******************************************************************************/
bool AabbSphere(const Aabb& aabb, const Sphere& sphere)
{
	float sqDist = SqDistPointAabb(sphere.m_Position, aabb);

	return sqDist <= (sphere.m_Radius * sphere.m_Radius);
}
/******************************************************************************/
/*!
  \brief
	Check sphsere and aabb's intersection
  
  \param sphere
	a Sphere class
  \param aabb
	an Aabb class
  \return
	true if intersected else false
*/
/******************************************************************************/
bool SphereAabb(const Sphere& sphere, const Aabb& aabb)
{
	return AabbSphere(aabb, sphere);
}
/******************************************************************************/
/*!
  \brief
	Check Point3D and Triangle's intersection

  \param p
	a Point3D class
  \param triangle
	a triangle class
  \return
	true if intersected else false
*/
/******************************************************************************/
bool PointTriangle(const Point3D& p, const Triangle& triangle)
{
	float u, v, w;
	// Calculate barycetric coords
	Barycentric(Point3D(triangle.m_v0), Point3D(triangle.m_v1), Point3D(triangle.m_v2), p, u, v, w);

	return v >= 0.0f && w >= 0.0f && (v + w) <= 1.0f;
}
/******************************************************************************/
/*!
  \brief
	Check Point3D and Plane's intersection

  \param point
	a Point3D class
  \param plane
	a Plane class
  \return
	true if intersected else false
*/
/******************************************************************************/
bool PointPlane(const Point3D& point, const Plane& plane)
{
    // Plane in the form of |p.n| = 1 and p.n * point = p.d
	float result = dot(plane.m_Normal, point.m_coordinates) - plane.m_d;

	if (result == 0.0f)
		return true;
	else
		return false;
}
/******************************************************************************/
/*!
  \brief
	Check Ray and Plane's intersection

  \param ray
	a Ray class
  \param plane
	a Plane class
  \return
	true if intersected else false
*/
/******************************************************************************/
bool RayPlane(const Ray& ray, const Plane& plane)
{
	glm::vec3 pn = plane.m_Normal;
	float d = plane.m_d;

	const float NdotD = dot(pn, ray.m_D);

	if (NdotD == 0.0f)
		return false;

	const float NdotQ = dot(pn, ray.m_Q);

	// plane in the form of |p.n = 1|, p.n * point = d
	float t0 = (d - NdotQ) / NdotD;

	// only need to check if t is negative, anything above 0 means 
	// the ray will eventually hit 
	if (t0 >= 0.0f)
	{
		return true;
	}

	return false;
}
/******************************************************************************/
/*!
  \brief
	Check Ray and aabb's intersection

  \param ray
	a Ray class
  \param aabb
	an Aabb class
  \return
	true if intersected else false
*/
/******************************************************************************/
bool RayAabb(const Ray& ray, const Aabb& aabb)
{
	glm::vec3 L = aabb.m_Min; // Box corner
	glm::vec3 U = aabb.m_Max; // Box corner

	Interval iv0 = Interval();

	iv0.intersect(ray, glm::vec3(1.0f, 0.0f, 0.0f), -L.x, -U.x);
	if (iv0._t0 > iv0._t1)
	{
		return false;
	}
	iv0.intersect(ray, glm::vec3(0.0f, 1.0f, 0.0f), -L.y, -U.y);
	if (iv0._t0 > iv0._t1)
	{
		return false;
	}
	iv0.intersect(ray, glm::vec3(0.0f, 0.0f, 1.0f), -L.z, -U.z);

	if (iv0._t0 > iv0._t1)
	{
		return false;
	}
	else
	{
		if (iv0._t0 >= 0.0f)
		{
			//intersect._t = iv0._t0;
			//intersect._N = iv0._n0;
			//intersect._P = ray.eval(intersect._t);
			//intersect._pObject = this;
			//intersect._N.normalize();
			return true;
		}
		else if (iv0._t1 >= 0.0f)
		{
			//intersect._t = iv0._t1;
			//intersect._N = iv0._n1;
			//intersect._P = ray.eval(intersect._t);
			//intersect._pObject = this;
			//intersect._N.normalize();
			return true;
		}

		return false;
	}
}
/******************************************************************************/
/*!
  \brief
	Check Ray and Sphere's intersection

  \param ray
	a Ray class
  \param sphere
	a Sphere class
  \return
	true if intersected else false
*/
/******************************************************************************/
bool RaySphere(const Ray& ray, const Sphere& sphere)
{
	glm::vec3 Qbar = ray.m_Q - sphere.m_Position;

	const float QbarDotD = dot(Qbar, ray.m_D);
	const float QbarDotQbar = dot(Qbar, Qbar);

	const float discriminant = QbarDotD * QbarDotD - QbarDotQbar + sphere.m_Radius * sphere.m_Radius;

	// if discriminaant < 0, no intersection
	if (discriminant < 0.0f)
		return false;

	const float sqrtDiscri = sqrt(discriminant);

	const float tminus = -QbarDotD - sqrtDiscri;
	const float tplus = -QbarDotD + sqrtDiscri;
	// if both are negative, no interscetion
	if (tminus < 0.0f && tplus < 0.0f)
		return false;

	//float t = 0.0f;

	// else return smallest positive value
	//if (tminus > 0.0f)
	//{
	//	t = tminus;
	//}
	//else
	//{
	//	t = tplus;
	//}
	//
	//intersect._pObject = this;
	//intersect._t = t;
	//intersect._P = ray.eval(t);
	//intersect._N = intersect._P - _center;
	//intersect._N.normalize();
	return true;
}
/******************************************************************************/
/*!
  \brief
	Check Ray and Triangle's intersection

  \param ray
	a Ray class
  \param triangle
	a Triangle class
  \return
	true if intersected else false
*/
/******************************************************************************/
bool RayTriangle(const Ray& ray, const Triangle& triangle)
{
	const glm::vec3 V0 = triangle.m_v0;
	const glm::vec3 V1 = triangle.m_v1;
	const glm::vec3 V2 = triangle.m_v2;

	//Vector3f N0 = _pMesh->vertices[_i0].nrm;
	//Vector3f N1 = _pMesh->vertices[_i1].nrm;
	//Vector3f N2 = _pMesh->vertices[_i2].nrm;

	const glm::vec3 E1 = V1 - V0;
	const glm::vec3 E2 = V2 - V0;
	const glm::vec3 p = glm::cross(ray.m_D, E2);
	const float d = glm::dot(p, E1);

	if (d == 0.0f)
	{
		return false;
	}

	const glm::vec3 S = ray.m_Q - V0;
	const float u = (glm::dot(p, S)) / d;

	if (u < 0.0f || u > 1.0f)
	{
		return false;
	}

	const glm::vec3 q = glm::cross(S,E1);
	const float v = (glm::dot(ray.m_D, q)) / d;

	if (v < 0.0f || (u + v) > 1.0f)
	{
		return false;
	}

	const float t = (glm::dot(E2, q)) / d;

	if (t < 0.0f)
	{
		return false;
	}

	//intersect._pObject = this;
	//intersect._t = t;
	//intersect._P = ray.eval(t);
	//
	//N0.normalize();
	//N1.normalize();
	//N2.normalize();
	//
	//intersect._N = ((1.0f - u - v) * N0 + u * N1 + v * N2);
	//intersect._N.normalize();

	return true;
}
/******************************************************************************/
/*!
  \brief
	Check Plane and Aabb's intersection

  \param plane
	a Plane class
  \param aabb
	a Aabb class
  \return
	true if intersected else false
*/
/******************************************************************************/
bool PlaneAabb(const Plane& plane, const Aabb& aabb)
{
	// compute aabb center
	glm::vec3 c = (aabb.m_Max + aabb.m_Min) / 2.0f;
	// compute positive extents
	glm::vec3 e = aabb.m_Max - c;

	glm::vec3 n = plane.m_Normal;
	float d = plane.m_d;

	// compute the projection interval radius of b onto L(t) = b.c + t * p.n
	float r = e[0] * abs(n[0]) + e[1] * abs(n[1]) + e[2] * abs(n[2]);
	// compute distance of box center from plane
	float s = glm::dot(n, c) -d;

	return abs(s) <= r;
}
/******************************************************************************/
/*!
  \brief
	Check Plane and Sphere's intersection

  \param plane
	a Plane class
  \param sphere
	a Sphere class
  \return
	true if intersected else false
*/
/******************************************************************************/
bool PlaneSphere(const Plane& plane, const Sphere& sphere)
{
	// For a normalized plane (|p.n| = 1), evaluating the plane equation
	// for a point gives the signed distance of the point to the plane
	float dist = dot(sphere.m_Position, plane.m_Normal) - plane.m_d;
	// If sphere center within +/-radius from plane, plane intersects sphere
	return abs(dist) <= sphere.m_Radius * sphere.m_Radius;
}
/******************************************************************************/
/*!
  \brief
	Check Plane and Triangle's intersection

  \param plane
	a Plane class
  \param triangle
	a Triangle class
  \return
	true if intersected else false
*/
/******************************************************************************/
bool PlaneTriangle(const Plane& plane, const Triangle& triangle)
{
	float v0result = dot(plane.m_Normal, triangle.m_v0) - plane.m_d;
	float v1result = dot(plane.m_Normal, triangle.m_v1) - plane.m_d;
	float v2result = dot(plane.m_Normal, triangle.m_v2) - plane.m_d;

	// check if all points fall onto negative side
	if (v0result < 0.0f && v1result < 0.0f && v2result < 0.0f)
	{
		return false;
	} // or all points fall onto positive side
	else if (v0result > 0.0f && v1result > 0.0f && v2result > 0.0f)
	{
		return false;
	}

	return true;;
}
/******************************************************************************/
/*!
  \brief
	Calculate Squared distance between a point and an aabb

  \param point
	a Point3D class
  \param aabb
	a Aabb class
  \return sqDist
    squared distance between point and aabb
*/
/******************************************************************************/
float SqDistPointAabb(const Point3D& point, const Aabb& aabb)
{
	float sqDist = 0.0f;
	for (unsigned int i = 0; i < 3; ++i)
	{
		// For each axis count any excess distance outside box extents
		float v = point.m_coordinates[i];

		if (v < aabb.m_Min[i])
		{
			sqDist += (aabb.m_Min[i] - v) * (aabb.m_Min[i] - v);
		}

		if (v > aabb.m_Max[i])
		{
			sqDist += (v - aabb.m_Max[i]) * (v - aabb.m_Max[i]);
		}
	}

	return sqDist;
}
/******************************************************************************/
/*!
  \brief
	inline helper function to calculate triangle area in 2D

  \param x0
	v0's x
  \param y0
	v0's y
  \param x1
    v1's x
  \param y1
    v1's y
  \param x2
    v2's x
  \param y2
    v2's y
  \return
	float value of the area
*/
/******************************************************************************/
inline float TriArea2D(const float x0, const float y0, const float x1, const float y1, const float x2, const float y2)
{
	return (x0 - x1) * (y1 - y2) - (x1 - x2) * (y0 - y1);
}
/******************************************************************************/
/*!
  \brief
	Calculate baricentric coords and store into u, v, w

  \param a
	point a
  \param b
	point b
  \param c
    point c
  \param p
    point p
  \param u
    u coord
  \param v
    v coord
  \param w
    w coord
*/
/******************************************************************************/
void Barycentric(const Point3D& a, const Point3D& b, const Point3D& c, const Point3D& p, float& u, float& v, float& w)
{
	// Unnormalized triangle normal
	glm::vec3 m = glm::cross(b.m_coordinates - a.m_coordinates, c.m_coordinates - a.m_coordinates);
	// Nominators and one-over-denominator for u and v ratios
	float nu, nv, ood;
	// Absolute components for determining projection plane
	float x = abs(m.x), y = abs(m.y), z = abs(m.z);

	// Compute areas in plane of largest projection
	if (x >= y && x >= z)
	{
		// x is largest, project to the yz plane

		// Area of PBC in yz plane
		nu = TriArea2D(p.m_coordinates.y, p.m_coordinates.z, b.m_coordinates.y, b.m_coordinates.z, c.m_coordinates.y, c.m_coordinates.z);
		// Area of PCA in yz plane
		nv = TriArea2D(p.m_coordinates.y, p.m_coordinates.z, c.m_coordinates.y, c.m_coordinates.z, a.m_coordinates.y, a.m_coordinates.z);
		// 1/(2*area of ABC in yz plane)
		ood = 1.0f / m.x;
	}
	else if (y >= x && y >= z)
	{
		// y is largest, projec to the xz plane

		// Area of PBC in xz plane
		nu = TriArea2D(p.m_coordinates.x, p.m_coordinates.z, b.m_coordinates.x, b.m_coordinates.z, c.m_coordinates.x, c.m_coordinates.z);
		// Area of PCA in xz plane
		nv = TriArea2D(p.m_coordinates.x, p.m_coordinates.z, c.m_coordinates.x, c.m_coordinates.z, a.m_coordinates.x, a.m_coordinates.z);
		// 1/(2*area of ABC in yz plane)
		ood = 1.0f / -m.y;
	}
	else
	{
		// z is largest, projec to the xy plane

		// Area of PBC in xy plane
		nu = TriArea2D(p.m_coordinates.x, p.m_coordinates.y, b.m_coordinates.x, b.m_coordinates.y, c.m_coordinates.x, c.m_coordinates.y);
		// Area of PCA in xy plane
		nv = TriArea2D(p.m_coordinates.x, p.m_coordinates.y, c.m_coordinates.x, c.m_coordinates.y, a.m_coordinates.x, a.m_coordinates.y);
		// 1/(2*area of ABC in yz plane)
		ood = 1.0f / m.z;
	}

	u = nu * ood;
	v = nv * ood;

	w = 1.0f - u - v;
}
/******************************************************************************/
/*!
  \brief
	This function classifies a point to a plane

  \param p
	A point
  \param plane
	A plane
  \return Point_Classification
	Where the point locates from the plane
*/
/******************************************************************************/
Point_Classification ClassifyPointToPlane(const glm::vec3& p, const Plane& plane)
{
	float dist = dot(plane.m_Normal, p) - plane.m_d;
	float PLANE_THICKNESS_EPSILON = 0.00000001f;

	if (dist > PLANE_THICKNESS_EPSILON)
		return Point_Classification::POINT_IN_FRONT_OF_PLANE;
	if (dist < -PLANE_THICKNESS_EPSILON)
		return Point_Classification::POINT_BEHIND_PLANE;

	return Point_Classification::POINT_ON_PLANE;
}
/******************************************************************************/
/*!
  \brief
	This function calculates an intesection point of and edge with a plane

  \param a
	A point
  \param b
	A point
  \param plane
	A plane
  \return intersect
    An intersection point
*/
/******************************************************************************/
glm::vec3 IntersectEdgeAgainstPlane(const glm::vec3& a, const glm::vec3& b, const Plane& plane)
{
	// Do dir need to be normalized?
	glm::vec3 dir = b - a;

	//dir = glm::normalize(dir);
	float NdotDir = dot(plane.m_Normal, dir);
	//if (NdotDir == 0.0f)
		//return b;


	float t = (plane.m_d - dot(plane.m_Normal,a)) / NdotDir;


	glm::vec3 intersect;

	intersect = a + dir * t;

	return intersect;
}
/******************************************************************************/
/*!
  \brief
	default ctor for Interval
*/
/******************************************************************************/
Interval::Interval() : _t0(0.0f), _t1(FLT_MAX), _n0(glm::vec3()), _n1(glm::vec3())
{
}
/******************************************************************************/
/*!
  \brief
	non default ctor for Interval. This is used to rearrange t0 and t1
	if t0 > t1

  \param t0
	first intersection
  \param t1
    second intersection
  \param n0
    first normal
  \param n1
    second normal
*/
/******************************************************************************/
Interval::Interval(const float t0, const float t1, const glm::vec3& n0, const glm::vec3& n1)
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
/******************************************************************************/
/*!
  \brief
	make the Interval invalid
*/
/******************************************************************************/
void Interval::empty()
{
	_t0 = 0.0f;
	_t1 = -1.0f;
}
/******************************************************************************/
/*!
  \brief
	compare this Interval with another to narrow this interval if found
	a better value using min and max

  \param other
    ther other Interval
*/
/******************************************************************************/
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
/******************************************************************************/
/*!
  \brief
	compare this Interval with a given slab to narrow this Interval if found
	one or two value

  \param ray
    A ray to be intersect with given slab
  \param N
   Normal of the slab
  \param d0
   distance to the first plane
  \param d1
   distance to the second plane
*/
/******************************************************************************/
void Interval::intersect(const Ray& ray, const glm::vec3& N, const float d0, const float d1)
{
	const float NdotD = dot(N, ray.m_D);
	const float NdotQ = dot(N, ray.m_Q);

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
