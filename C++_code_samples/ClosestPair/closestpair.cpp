/******************************************************************************/
/*!
\file   closestpair.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS330
\par    Assignment Closest Pair
\date   07/08/2017
\brief  
    This file contains the implemantation of Closest Pair
    functions for the assignment.
    
  Hours spent on this assignment: 8

  Specific portions that gave you the most trouble:
    Took sometime to understand the logic of combining the strip
*/
/******************************************************************************/
#include "closestpair.h"
#include <algorithm>
#include <limits>
#include <cmath>    // sqrt, abs
#include <iostream> // cout
#include <utility>

/******************************************************************************/
/*!
  \brief
    This function prints a const Point
    
  \param os
    ostream
    
  \param p
    Point to be printed
    
  \return
    ostream
*/
/******************************************************************************/
std::ostream& operator<< (std::ostream& os, Point const& p) {
	os << "(" << p.x << " , " << p.y << ") ";
	return os;
}
/******************************************************************************/
/*!
  \brief
    This function prints a Point
    
  \param os
    ostream
    
  \param p
    Point to be printed
    
  \return
    ostream
*/
/******************************************************************************/
std::istream& operator>> (std::istream& os, Point & p) {
	os >> p.x >> p.y;
	return os;
}
/******************************************************************************/
/*!
  \brief
    This function compares two points by comparing X coordinates
    
  \param p0
    first point
    
  \param p1
    second point
    
  \return
    Whether p0 is less than p1
*/
/******************************************************************************/
bool compareX(Point p0, Point p1)
{
  // If x is the same, compare y
  if(p0.x == p1.x)
    return p0.y < p1.y;
  else
    return p0.x < p1.x;
}
/******************************************************************************/
/*!
  \brief
    This function compares two points by comparing Y coordinates
    
  \param p0
    first point
    
  \param p1
    second point
    
  \return
    Whether p0 is less than p1
*/
/******************************************************************************/
bool compareY(Point p0, Point p1)
{
  // If y is the same, compare x
  if(p0.y == p1.y)
    return p0.x < p1.x;
  else
    return p0.y < p1.y;
}
/******************************************************************************/
/*!
  \brief
    This function calculates distance between two poins
    
  \param p0
    first point
    
  \param p1
    second point
    
  \return
    The distance between two points
*/
/******************************************************************************/
float dist(Point p0, Point p1)
{
  return sqrt( (p0.x - p1.x) * (p0.x - p1.x) + (p0.y - p1.y) * (p0.y - p1.y) );
}
/******************************************************************************/
/*!
  \brief
    This function partitions points to two vectors by comparing with a mid 
    point
    
  \param left
    left vector
    
  \param right
    right vector
    
  \param original
    original vector
    
  \param mid
    The half distance of (x or y) from first point to last point
    
  \param vertical
    Determine if all points are on a vertical line
    
*/
/******************************************************************************/
void partitionPoints(std::vector<Point>& left, std::vector<Point>& right, std::vector<Point> const& original, float mid, bool vertical)
{
  if(!vertical)
  {
    for(unsigned i = 0; i < original.size(); ++i)
    {
      if(original[i].x < mid)
        left.push_back(original[i]);
      else
        right.push_back(original[i]);
    }
  }
  else
  {
    for(unsigned i = 0; i < original.size(); ++i)
    {
      if(original[i].y < mid)
        left.push_back(original[i]);
      else
        right.push_back(original[i]);
    }
  }
}
/******************************************************************************/
/*!
  \brief
    This function builds a narrowed strip from original points by comparing 
    distance to mid line with the minimum distance found from two halves
    
  \param strip
    The result strip
    
  \param original
    original vector
    
  \param mid
    The half distance of (x or y) from first point to last point
    
  \param d
    The minimum distance found from two halves
    
  \param vertical
    Determine if all points are on a vertical line
    
*/
/******************************************************************************/
void buildStrip(std::vector<Point>& strip, std::vector<Point> const& original, float mid, float d, bool vertical)
{
  if(!vertical)
  {
    for(unsigned i = 0; i < original.size(); ++i)
    {
      if(abs(original[i].x - mid) < d)
        strip.push_back(original[i]);
    }
  }
  else
  {
    for(unsigned i = 0; i < original.size(); ++i)
    {
      if(abs(original[i].y - mid) < d)
        strip.push_back(original[i]);
    }
  }
}
/******************************************************************************/
/*!
  \brief
    This function founds smallest distance from two points within the narrowed
    strip
    
  \param strip
    The result strip
    
  \param d
    The minimum distance found from two halves
    
  \param vertical
    Determine if all points are on a vertical line
    
  \param
    The smallest distance between two points from the narrowed strip
*/
/******************************************************************************/
float stripClosest(std::vector<Point>& strip, float d, bool vertical)
{
  float min = d;
  
  if(!vertical)
  {
    std::sort(strip.begin(), strip.end(), compareY);
    
    for(unsigned i = 0; i < strip.size(); ++i)
    {
      for(unsigned j = i +1 ; j < strip.size() && (strip[j].y - strip[i].y) < min; ++j)
      {
        float temp = dist(strip[i], strip[j]);
        
        if(temp < min)
          min = temp;
      }
    }
  }
  else
  {
    std::sort(strip.begin(), strip.end(), compareX);
    
    for(unsigned i = 0; i < strip.size(); ++i)
    {
      for(unsigned j = i + 1; j < strip.size() && (strip[j].x - strip[i].x) < min; ++j)
      {
        float temp = dist(strip[i], strip[j]);
        
        if(temp < min)
          min = temp;
      }
    }
  }
  
  return min;
}
////////////////////////////////////////////////////////////////////////////////
float closestPair_aux (std::vector<Point>const& points);

////////////////////////////////////////////////////////////////////////////////
/******************************************************************************/
/*!
  \brief
    This function founds distance of closest pair
    
  \param points
    The original points
    
  \return
    The distance between two closest pair
*/
/******************************************************************************/
float closestPair ( std::vector< Point > const& points ) {
	int size = points.size();

	if (size==0) throw "zero size subset";
	if (size==1) return std::numeric_limits<float>::max();

  // Copy vector of points and sort by x
  std::vector<Point> points_copy(points);
  std::sort(points_copy.begin(), points_copy.end(), compareX);
  
	return closestPair_aux(points_copy);
}

////////////////////////////////////////////////////////////////////////////////
/******************************************************************************/
/*!
  \brief
    This function founds distance of closest pair recursively
    
  \param points
    The original points
    
  \return
    The distance between two closest pair
*/
/******************************************************************************/
float closestPair_aux (std::vector<Point>const& points) {
	//int size = indices.size();
  int size = points.size();

	if (size==0) throw "zero size subset";
	if (size==1) return std::numeric_limits<float>::max();
  if (size==2) return dist(points[0], points[1]);
  
  float line = points.back().x - points.front().x;
  bool  vertical = false;
  
  // Check special cases
  if(line == 0.0f)
  {
    line = points.back().y - points.front().y;
    
    // All points are on same coordinate
    if(line == 0.0f)
      return 0.0f;
    //else partion points using Y coordinates
    else
      vertical = true;
  }
  
  float mid = 0.0f;
  
  if(!vertical)
  {
    mid = points.front().x + (line / 2.0f);
  }
  else
  {
    mid = points.front().y + (line / 2.0f);
  }
  
  // Partition points to two halves by comparing with mid
  std::vector<Point> left;
  std::vector<Point> right;
  
  partitionPoints(left, right, points, mid, vertical);
  // Make recursive call to found closestPair from two halves
  float result_l = closestPair_aux(left);
  float result_r = closestPair_aux(right);
  // Store the smaller result
  float result = std::min(result_l, result_r);
  
  std::vector<Point> strip;
  // Use the smaller result to build a narrowed strip from original
  buildStrip(strip, points, mid, result, vertical);
  // Find the closest pair from the narrowed strip
  float result_strip = stripClosest(strip, result, vertical);
  // Compare the final result
  float min_dist = std::min(result_strip, result);

	return min_dist;
}

