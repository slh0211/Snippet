/******************************************************************************/
/*!
\file   hull-brueteforce.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS330
\par    Assignment 1
\date   06/15/2017
\brief  
    This file contains the implemantation of convex hull brute force 
    functions for the assignment.
    
  Hours spent on this assignment: 5

  Specific portions that gave you the most trouble:
    Termination flags has to be set precisely otherwise it's a debugging
    nightmare.
*/
/******************************************************************************/
#include "hull-bruteforce.h"
#include <algorithm>
#include <iostream> // cout

bool Point::operator==( Point const& arg2 ) const {
    return ( (x==arg2.x) && (y==arg2.y) );
}

std::ostream& operator<< (std::ostream& os, Point const& p) {
	os << "(" << p.x << " , " << p.y << ") ";
	return os;
}

std::istream& operator>> (std::istream& os, Point & p) {
	os >> p.x >> p.y;
	return os;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//return value is (on left, on right)
std::pair<bool,bool> get_location(
		float const& p1x, //check which side of the line (p1x,p1y)-->(p2x,p2y) 
		float const& p1y, //point (qx,qy) is on
		float const& p2x,
		float const& p2y,
		float const& qx,
		float const& qy
		) 
{
	Point dir   = {p2x-p1x,p2y-p1y};
	Point norm  = {dir.y, -dir.x};
	Point point = {qx-p1x,qy-p1y};
	//scalar product is positive if on right side
	float scal_prod = norm.x*point.x + norm.y*point.y;
	return std::make_pair( (scal_prod<0), (scal_prod>0));
}

// 1) hullBruteForce: for each pair points determine whether all other points are one side of the
// line formed by the pair of points. If it does - add the points (or rather 
// indices to the hull). Since hull is represented by a std::set, you do not have 
// to worry about duplicates.
// returns a set of indices of points that form convex hull
/******************************************************************************/
/*!
  \brief
    This function finds all points that form a convex hull using brute force
    O(n^3) nested loop.
    
  \param points
    Reference to a vector of point
    
  \return
    Return a set of index that form a conver hull
*/
/******************************************************************************/
std::set<int> hullBruteForce ( std::vector< Point > const& points ) {
  int num_points = points.size();
  //std::cout << "number of points " << num_points << std::endl;
  if ( num_points < 3 ) throw "bad number of points";

  std::set<int> hull_indices;
  // Create variables outside the nested O(n^3) loop to avoid unnecessary slow down
  bool same_side;
  std::pair<bool, bool> prev_result;
  std::pair<bool, bool> result;
  // for each pair points determine whether all other points are one side of the
  // line formed by the pair of points.
  // Loop through each points to pick a point as p1
  for(unsigned i = 0; i < points.size() - 1; i++)
  {
    // Loop through all other points as p2 to form an edge with p1
    for(unsigned j = i + 1; j < points.size(); j++)
    {
      // Create a flag to terminate the loop for this edge if found a point that is on a different side
      same_side = true;
      // true true can not be a real point so used as the initial value to indicate prev_result has not been initialized yet
      prev_result = std::make_pair(true, true);
      // Loop through all other points to check which sides the point is on
      for(unsigned k = 0; k < points.size() && same_side; k++)
      {
        // Skip if k is p1 or p2
        if(k == i || k == j)
          continue;
        
        // Initialize prev_result for first point compared with this edge
        // Don't know which point is the first one to be compared so initialize in the loop
        if(prev_result.first && prev_result.second)
        {
          prev_result = get_location(points[i].x, points[i].y, points[j].x, points[j].y, points[k].x, points[k].y);
          continue;
        }
        
        result = get_location(points[i].x, points[i].y, points[j].x, points[j].y, points[k].x, points[k].y);
        // Each point has to be on the same side so early temination once found an edge on differnet side
        if(prev_result != result)
          same_side = false;
      }
      
      if(same_side)
      {
        hull_indices.insert(i);
        hull_indices.insert(j);
      }
    }
  }
  return hull_indices;
}

// 2) hullBruteForce2: find the first point that is in the hull (smalles or biggest x or y coordinate),
// then find the next vertex of the hull in counter-clockwise order by considering all lines through the
// previous vertex and requiring that there are no points to the right of it.
/******************************************************************************/
/*!
  \brief
    This function finds all points that form a convex hull using brute force
    with slight optimization by only picking points on the hulls. O(K * n ^2)
    Worst case is O(n^3) where all points are on the hull.
    
  \param points
    Reference to a vector of point
    
  \return
    Return a vector of index that form a convex hull
*/
/******************************************************************************/
std::vector<int> hullBruteForce2 ( std::vector< Point > const& points ) {
  int num_points = points.size();
  if ( num_points < 3 ) throw "bad number of points";

  std::vector<int> hull_indices;

  // (a) add vertex v1 with the smallest x coordinate to the hull
  // Loop through each point to pick a point as v1
  float min_x_so_far = std::numeric_limits<float>::max();
  unsigned start_index = 0;
  
  for(unsigned i = 0; i < points.size(); i++)
  {
    if(points[i].x <= min_x_so_far)
    {
      min_x_so_far = points[i].x;
      // If multiple points has the same smallest value of x, the latest one will be used as the starting point
      start_index = i;
    }
  }
  hull_indices.push_back(start_index);

  // (b) consider all edges v1 - pi and find v2 so that v1 - v2 has all points of P on one side, add
  // v2 to convex hull
  // (c) consider all edges v2 - pi and find v3 so that v2 - v3 has all points of P on one side, add
  // v3 to convex hull
  // (d) .....
  // (e) if just added vertex is equal to v1 stop.
  
  // Loop through all remaining points
  // While v1 has not been used twice
  unsigned prev_index = start_index;
  bool incomplete = true;
  std::pair<bool, bool> right_side = std::make_pair(false, true);
  std::pair<bool, bool> result;
  bool all_points_on_left = true;
  
  while(incomplete)
  {
    // Pick a point other than prev_index as p2 to form an edge
    for(unsigned i = 0; i < points.size(); i++)
    {
      if(i == prev_index)
        continue;
      // reset flags
      all_points_on_left = true;
      // Pick a point other than prev_index or i to check sides
      for(unsigned j = 0; j < points.size() && all_points_on_left ; j++)
      {
        if(j == i || j == prev_index)
          continue;
        
        // check which side the point is at
        result = get_location(points[prev_index].x, points[prev_index].y, points[i].x, points[i].y, points[j].x, points[j].y);
      
        // if the result is on the right side of the edge, set flag
        if(result == right_side)
        {
          all_points_on_left = false;
        }
      }
      
      // If all points on left, we found a vertex
      if(all_points_on_left)
      {
        // Check if the vertex is the first one we add to result
        if(i != start_index)
        {
          hull_indices.push_back(i);
          prev_index = i;
          // Terminate the outmost for loop to start from index 0 again in the next while loop
          break;
        }
        else
        {
          incomplete = false;
          break;
        }
      }
    }
  }
  return hull_indices;
}
