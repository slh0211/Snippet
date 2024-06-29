/******************************************************************************/
/*!
\file   kruskal.h
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS280
\par    Assignment 6
\date   04/19/2017
\brief  
    This file contains the krusal functions for the assignment.

  Hours spent on this assignment: 5

  Specific portions that gave you the most trouble:
    
*/
/******************************************************************************/
#ifndef KRUSKAL_H
#define KRUSKAL_H
#include "disjoint_sets.h"
#include "graph.h"
#include <list>
//efficient version, using disjoint subsets
/******************************************************************************/
/*!
  \brief
    This function uses disjoint subsets to generate minimal spanning tree (MST)
    
  \param g
    The graph to be used to generate MST
    
  \return
    Return a vector containing edges of MST
*/
/******************************************************************************/
template <typename Vertex, typename Edge>
std::vector<Edge> kruskal_disjoint_subsets( Graph<Vertex,Edge> const& g ) 
{
  std::vector<Edge> mst;
  // Sort edge list
  std::list<Edge> edges(g.GetEdges());
  edges.sort();
  // Create a disjoint set
  DisjointSets ds(g.Size());
  for (unsigned i=0; i<g.Size(); ++i ) ds.Make( );

  // Loop through edge list from smaller weight edge
  for(typename std::list<Edge>::const_iterator it = edges.begin(); it != edges.end(); ++it)
  {
    // If two vertex's rp is not the same
    size_t rp1 = ds.GetRepresentative((*it).ID1());
    size_t rp2 = ds.GetRepresentative((*it).ID2());
    if(rp1 != rp2)
    {
      // Add to mst
      mst.push_back(*it);
      ds.Join(rp1, rp2);
    }
    // MST edges = vertices -1
    if(mst.size() == g.Size() -1)
    {
      break;
    }
  }
  return mst;
}
//inefficient version, using array of vector of "colors"
/******************************************************************************/
/*!
  \brief
    This function uses color array to generate minimal spanning tree (MST)
    
  \param g
    The graph to be used to generate MST
    
  \return
    Return a vector containing edges of MST
*/
/******************************************************************************/
template <typename Vertex, typename Edge>
std::vector<Edge> kruskal_vector( Graph<Vertex,Edge> const& g ) 
{
  std::vector<Edge> mst;
  // Sort edge list
  std::list<Edge> edges(g.GetEdges());
  edges.sort();
  // Create an array or vector size of vertex and color each differently
  size_t* color_array = new size_t[g.Size()];
  for(unsigned i = 0; i < g.Size(); ++i)
  {
    color_array[i] = i;
  }
  // Loop through edge list from smaller weight edge
  for(typename std::list<Edge>::const_iterator it = edges.begin(); it != edges.end(); ++it)
  {
    // If two vertex's color is not the same
    if(color_array[(*it).ID1()] != color_array[(*it).ID2()])
    {
      // Add to mst
      mst.push_back(*it);
      size_t old_color = color_array[(*it).ID2()];
      size_t new_color = color_array[(*it).ID1()];
      // Update color on the array
      for(unsigned i = 0; i < g.Size(); ++i)
      {
        // If a color is old color
        if(color_array[i] == old_color)
        {
          // Update old color to new color
          color_array[i] = new_color;
        }
      }
    }
    // MST edges = vertices -1
    if(mst.size() == g.Size() -1)
    {
      break;
    }
  }
  
  delete [] color_array;
  
  return mst;
}

#endif
