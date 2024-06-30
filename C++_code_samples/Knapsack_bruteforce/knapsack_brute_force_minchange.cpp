/******************************************************************************/
/*!
\file   knapsack_brute_force_minchange.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS330
\par    Lab 4
\date   06/17/2017
\brief  
    This file contains the implemantation of GreyCode and knapsack
    functions for the assignment.
    
  Hours spent on this assignment: 5

  Specific portions that gave you the most trouble:
    Took sometime to understand Gray Code because not very familiar with
    bitwise manipulation.
*/
/******************************************************************************/
#include "knapsack_brute_force_minchange.h"
// #include <iostream>
#include <strings.h> //ffs
/******************************************************************************/
/*!
  \brief
    Ctor for GreyCode object
    
  \param s
    How many bits will be used to store data.
*/
/******************************************************************************/
GreyCode::GreyCode(int s) : size_(s), data_(0), last_bit_mask_( 1 << (size_ - 1))
{
}
////////////////////////////////////////////////////////////////////////////////
/******************************************************************************/
/*!
  \brief
    This function generate Next permutation for GreyCode object
    
  \return
    Return a pair of a pair to indicate if it is the last permutation, the
    bit is set or unset for this permutation, and which bit was modified
*/
/******************************************************************************/
std::pair< bool, std::pair< bool, int > > GreyCode::Next() {
    // Increment to generate next permutation in int format
    int new_data = data_ + 1;
    
    // Call getGrayCode to generate grey code for both old and new permutation
    int old_gray_code = getGreyCode(data_);
    int new_gray_code = getGreyCode(new_data);
    // Use ^ to locate the bit was changed
    int which_bit = new_gray_code ^ old_gray_code;
    // Use ffs (find first set) to get the position of the bit from least significant bit
    // the least significant bit starting from 1 
    int  pos = ffs(which_bit) - 1;// which position is modified 
    bool add = (new_gray_code & which_bit);
    // is true if new value is 1 (add item), false otherwise
    // Check if the most signifigant bit is set or if the size reaches
    bool last = (new_gray_code == last_bit_mask_);
    // is this the last permutation 
    
    data_ = new_data;
    return std::make_pair( !last, std::make_pair( add, pos ) );
}
/******************************************************************************/
/*!
  \brief
    This function takes an int and generates its gray code format
    
  \param d
    The input data
    
  \return
    The data in Gray code format in int
*/
/******************************************************************************/
int GreyCode::getGreyCode(int d)
{
  int half = d >> 1;
  int gray_code = d ^ half;
  
  return gray_code;
}

////////////////////////////////////////////////////////////////////////////////
/******************************************************************************/
/*!
  \brief
    This function uses Gray Code to calculate and find best permuation 
    for knapsack problem
    
  \param items
    A vector contains all items
    
  \param W
    Max weight is allowed
    
  \return
    Return a vector of bool to indicate which item is in the best permutation
*/
/******************************************************************************/
std::vector<bool> knapsack_brute_force( std::vector<Item> const& items, Weight const& W )
{
  std::vector<bool> bag( items.size(), false );
  GreyCode gc( items.size() );
  bool go = true;
  Weight max_weight = W;
  // Default weight set by ctor
  Weight curr_weight;
  int curr_value = 0;
  int best_value = 0;
  // If found a better result, update this one
  std::vector< bool > result;
  
  // While it's not the last permutation
  while ( go ) 
  {
    // Generate the next permutation
    std::pair< bool, std::pair< bool, int > > r = gc.Next();
    go = r.first;
        
    bool add = r.second.first;
    int  pos = r.second.second;
    
    // Add or remove item according to the gray code permutation
    if ( add ) 
    {
      bag[ pos ] = true;
      curr_weight += items[pos].GetWeight();
      curr_value  += items[pos].GetValue();
    } 
    else 
    {
      bag[ pos ] = false;
      curr_weight -= items[pos].GetWeight();
      curr_value  -= items[pos].GetValue();
    }
    
    // If the current weight is legit
    if(curr_weight <= max_weight)
    {
      // Check if the value is better than older value
      if(curr_value > best_value)
      {
        // Update best value so far and copy the permutation to the result
        best_value = curr_value;
        result = bag;
      }
    }
  }
    
  return result;
}
