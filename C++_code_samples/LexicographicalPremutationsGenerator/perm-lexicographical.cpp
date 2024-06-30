/******************************************************************************/
/*!
\file   perm-lexicographical.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS330
\par    Lab 3
\date   06/08/2017
\brief  
    This file contains the implemantation of lexicographical permutation 
    functions for the assignment.
    
  Hours spent on this assignment: 

  Specific portions that gave you the most trouble:
    Using index incorrectly took sometime to debug
*/
/******************************************************************************/
#include "perm-lexicographical.h"
#include <algorithm> //std::swap
/******************************************************************************/
/*!
  \brief
    This function finds the next permutation of lexicographical order
    
  \param p
    Reference to a vector of int
    
  \return
    Return true if find a permutation else false
*/
/******************************************************************************/
bool nextPermLexicographical (std::vector<int> & p) {
  // Find longest non-increasing suffix
  // Starting from the lowest digit
  int i = p.size() - 1;
  // While not reaching the head of the vector
  // Compare two elements
  while (i > 0 && p[i - 1] >= p[i])
    i--;
  // Now i is the head index of the suffix
  if (i <= 0)
    return false;
  
  // Let p[i - 1] be the pivot
  // Find rightmost element that exceeds the pivot
  int j = p.size() - 1;
  // Gurantee there will be an element larger than p[i-1]
  while (p[j] <= p [i-1])
    j--;
  // Now the value p[j] will become the new pivot
  std::swap(p[i-1], p[j]);
  // Reverse the suffix
  j = p.size() - 1;
  while(i < j)
  {
    std::swap(p[i],p[j]);
    i++;
    j--;
  }
  
  return true;
}
