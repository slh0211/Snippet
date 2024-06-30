/******************************************************************************/
/*!
\file   catalan.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS330
\par    Lab 1
\date   06/24/2017
\brief  
    This file contains the implemantation of Catalan number
    functions for the assignment.
    
  Hours spent on this assignment: 

  Specific portions that gave you the most trouble:
    Figuring out how to terminate each loop takes a lot of time to debug.
*/
/******************************************************************************/
#include "catalan.h"
int catalan_aux( int size, int b);
/******************************************************************************/
/*!
  \brief
    This function take size of nodes and calculate Catalan number of a binary
    tree.
    
  \param size
    Size of total number of nodes
    
  \return
    How many permutations the size of nodes can generate as binary tree
*/
/******************************************************************************/
int catalan2( int size ) 
{
    if ( size < 2 ) return 1;
    
    int n = 0;
    for ( int s=0; s<size; ++s ) {
        //    left           right    (-1 is for root)
        //    subtree        subtree
        n += ( catalan2(s) * catalan2( size-1 -s ) );
    }
    return n;
}
/******************************************************************************/
/*!
  \brief
    This function take size of nodes and calculate Catalan number of a 3 nodes
    tree.
    
  \param size
    Size of total number of nodes
    
  \return
    How many permutations the size of nodes can generate as 3 nodes tree
*/
/******************************************************************************/
int catalan3( int size ) 
{
    if ( size < 2 ) return 1;
    
    int n = 0;
    for ( int s=0; s<size; ++s ) {
      for (int t=0; t< size - s; ++t){
        //    left           right    (-1 is for root)
        //    subtree        subtree
        n += ( catalan3(s) *catalan3(t)* catalan3( size-1 -s  - t) );
      }
    }
    return n;
}
/******************************************************************************/
/*!
  \brief
    This function take size of nodes and calculate Catalan number of a 4 nodes
    tree.
    
  \param size
    Size of total number of nodes
    
  \return
    How many permutations the size of nodes can generate as 4 nodes tree
*/
/******************************************************************************/
int catalan4( int size ) 
{
    if ( size < 2 ) return 1;
    
    int n = 0;
    for ( int s=0; s<size; ++s ) {
      for (int t=0; t< size - s; ++t){
        for (int u=0; u< size - s - t; ++u) {
        //    left           right    (-1 is for root)
        //    subtree        subtree
        n += ( catalan4(s) *catalan4(t)*catalan4(u)* catalan4( size-1 -s  - t -u) );
        }
      }
    }
    return n;
}
/******************************************************************************/
/*!
  \brief
    This function multiplies partial result recursively 
    and update the final result in the last loop
    
  \param partial
    The partial result up until this loop
    
  \param depth
    Depth indicates which group of nodes current is.
              root
    left   middle        right
     1      2        3(base = max)
     
  \param max
    Max indicates the last loop. (Last group of nodes)
    
  \param size
    Size indicates how many nodes to use in this loop. Decrease by the nodes
    used in the previous loop.
    
  \param final
    A reference create in the loop of the first group of nodes. (Under
    catalan_aux). Each time a multiplication is compled in the last loop
    of catalan_multi, final incremented.
    
  \return
    How many permutations the size of nodes can generate as 3 nodes tree
*/
/******************************************************************************/
void catalan_multi(int partial, int depth, int max, int size, int& final)
{
  // Base case
  // If it's at the last group of nodes
  if(depth == max)
  {
    // Get the catalan of the last group
    // Multiply everything on the partial and returns
    final += partial * catalan_aux(size - 1, max);

    return;
  }
  
  // If it's not the last group
  // call magic catalan function to get catalan number
  // Use temp to store previous result instead using stl push_back and pop (Too slow)
  int temp = partial;
  // Push result to partial and jump to the next group
  for(int s = 0; s < size; ++s)
  {
    partial *= catalan_aux(s, max);
  
    catalan_multi(partial, depth + 1, max, size -s, final);

    partial = temp;
  }
}
/******************************************************************************/
/*!
  \brief
    This function take size of nodes and how many branch to
    calculate Catalan number.
    
  \param size
    Size of total number of nodes
    
  \param b
    How many immediate children the tree has
    
  \return
    How many permutations the size of nodes can generate
*/
/******************************************************************************/
// catalan _aux is the magic function that responsible to return a number
int catalan_aux(int size, int b)
{
  // Base case
  if(size < 2) return 1;

  int partial = 1;
  int d = 1;
  int final = 0;
  int temp = partial;
  
  for(int s = 0; s < size; ++s)
  { 
    // Calculate catalan number of current group and updates partial
    partial *= catalan_aux(s, b);
    // Jump to next group
    catalan_multi(partial, d + 1, b, size - s, final);
    // After all is done for the next group
    // Restore partial (Instead of using stl push_back and pop, too slow)
    partial = temp;
    // Increase the number of nodes to be used under this group
  }

  return final;
}

int catalan(int size, int b)
{
  return catalan_aux(size, b);
}