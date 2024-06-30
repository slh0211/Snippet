/******************************************************************************/
/*!
\file   perm-jt.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS330
\par    Lab 2
\date   06/06/2017
\brief  
    This file contains the implemantation of Johnson Trotter
    functions for the assignment.
    
  Hours spent on this assignment: 

  Specific portions that gave you the most trouble:
    Using index incorrectly took sometime to debug
*/
/******************************************************************************/
#include "perm-jt.h"
/******************************************************************************/
/*!
  \brief
    Ctor for PermJohnTrotter class
    
  \param size
    Size of total number of elements
*/
/******************************************************************************/
PermJohnsonTrotter::PermJohnsonTrotter(int size) : size_(size), elements_(), dir_(), mobile_()
{
  for(int i = 1; i <= size_; ++i)
  {
    elements_.push_back(i);
    dir_.push_back(-1);
    mobile_.push_back(true);
  }
  // To initialize the first element correctly
  mobile_[0] = false;
}
/******************************************************************************/
/*!
  \brief
    This function generates next permutation or return false if no more
    permutation
    
  \return
    Returns true if generates a permutation or false if no more permutation
    
*/
/******************************************************************************/
bool PermJohnsonTrotter::Next()
{

  int index = 0;
  int target_index = 0;

  bool result = IsMobile();
  if(result)
  {
    index = findLargestMobileIndex();
    target_index = index + dir_[index];

    std::swap(elements_[index], elements_[target_index]);
    std::swap(dir_[index], dir_[target_index]);
    reverseDir(target_index);
  }
  
  return result;
}
/******************************************************************************/
/*!
  \brief
    This function returns the private vector to client for printing
    
  \return
    Private vector contains current permutation
*/
/******************************************************************************/
std::vector<int> const& PermJohnsonTrotter::Get() const
{
  return elements_;
}
/******************************************************************************/
/*!
  \brief
    This function determine if current permutation is able to be modified
    
  \return
    Return true if current permutation allows move, else false
*/
/******************************************************************************/
bool PermJohnsonTrotter::IsMobile()
{
  for(int i = 0; i < size_; ++i)
  {
    if(mobile_[i] == true)
     return true;
  }
  
  return false;
}
/******************************************************************************/
/*!
  \brief
    This function finds the largest mobile element's index on vector
    
  \return
    The index of the largest mobile element
    
  \attention
    This function must always called after IsMobile, otherwise 
    largest_index_so_far's initial value -1 will cause bug (accessing incorrect
    elements on vector). Didn't have enough time to implement better solution.
*/
/******************************************************************************/
int PermJohnsonTrotter::findLargestMobileIndex()
{
  int largest_index_so_far = -1;

  // Find the first mobile element's index on mobile, ugly code but it works
  for(int i = 0; i < size_; ++i)
  {
    if(mobile_[i] == true)
    {
      largest_index_so_far = i;
      break;
    }
  }
  // Continue from the index but search largest element on the actual element vector
  for(int i = largest_index_so_far; i < size_; ++i)
  {
    if(mobile_[i] == true)
    {
      if(elements_[i] > elements_[largest_index_so_far])
        largest_index_so_far = i;
    }
  }
  
  return largest_index_so_far;
}
/******************************************************************************/
/*!
  \brief
    This function takes the index of largest mobile element and reverse the 
    direction of elements larger than the largest mobile element
    
  \param largestIndex
    The index of the largest mobile element after swap
    
  \attention
    The index of largest after swap is the swapped element's index originally
*/
/******************************************************************************/
void PermJohnsonTrotter::reverseDir(int largestIndex)
{
  for(int i = 0; i < size_; ++i)
  {
    if(elements_[i] > elements_[largestIndex])
      dir_[i] *= -1;
  }
  updateMobility();
}
/******************************************************************************/
/*!
  \brief
    This function updates entire mobile vector after reverse direction
    
*/
/******************************************************************************/
void PermJohnsonTrotter::updateMobility()
{
  for(int i = 0; i < size_; ++i)
  {
    // The distination has to be within boundary
    if((i+dir_[i] > -1) && (i+dir_[i] < size_ ) && (elements_[i] > elements_[i + dir_[i]]))
    {
      mobile_[i] = true;
    }
    else
      mobile_[i] = false;
  }
}