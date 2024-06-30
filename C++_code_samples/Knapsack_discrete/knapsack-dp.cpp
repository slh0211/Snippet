/******************************************************************************/
/*!
\file   knapsack-do.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS330
\par    Assignment Closest Pair
\date   07/20/2017
\brief  
    This file contains the implemantation of Knapsack dynamic programming
    functions for the assignment.
    
  Hours spent on this assignment: 4

  Specific portions that gave you the most trouble:
    Index offset made some confusion when debugging
*/
/******************************************************************************/
#include "knapsack-dp.h"
#include <iostream> // cout
#include <numeric>  // max


////////////////////////////////////////////////////////////
Item::Item( int const& weight, int const& value ) 
	: weight(weight), value(value) 
{
}

////////////////////////////////////////////////////////////
Item::Item( Item const& original ) 
	: weight(original.weight), value(original.value)
{
}

////////////////////////////////////////////////////////////
std::ostream& operator<< (std::ostream& os, Item const& item) {
	os << "(" << item.weight << " , " << item.value << ") ";
	return os;
}

////////////////////////////////////////////////////////////
std::istream& operator>> (std::istream& os, Item & item) {
	os >> item.weight >> item.value;
	return os;
}

////////////////////////////////////////////////////////////
typedef std::vector< std::vector<int> > Table; //2-dimensional table

////////////////////////////////////////////////////////////
//the returned value is a vector of indices
/******************************************************************************/
/*!
  \brief
    This function solves Knapsack problem using dynamic programming
    
  \param items
    A vector contains input items
    
  \param W
    The maximum allowed weight
    
  \return
    An int vector contains the indices of items from the input vector
*/
/******************************************************************************/
std::vector<int> knapsackDP( std::vector<Item> const& items, int const& W ) {
  int num_items = items.size();

  // Creat a 2D table of size (W + 1) * (num_items + 1), init to 0 in ctor
  Table table ( W + 1, std::vector<int>(num_items + 1, 0));

  // Use the weight as row and items index as column to match debugging output
  for(int w = 1; w<=W; ++w)
  {
    // For each slot in the table
    for(int n = 1; n<=num_items; ++n)
    {
      // Compare w with the item's weight of the coresponding slot (remember to offset index by decreasing 1 in items
      if(w >= items[n - 1].weight)
      { 
        // If w allows the item to be included, 
        // update the table with the max of previous value without the current item
        // or previous found best value without current item's weight combined with current item's value
        table[w][n] = std::max(table[w][n-1], table[w-items[n-1].weight][n-1] + items[n-1].value);
      }
      else // or just update with previous value when w is not allowed to include this item
        table[w][n] = table[w][n-1];
    }
  }
  
  //print final table - for debugging?
    //do not delete this code
    if ( num_items + W < 50 ) { //print only if table is not too big
        std::cout << "   ";
        for ( int n=0; n<=num_items; ++n) { std::cout << n << "     "; }
        std::cout << "  items\n        ";
        for ( int n=0; n<num_items; ++n) { std::cout << items[n].weight << "," << items[n].value<<"   "; }
        std::cout << "\n   ";
        for ( int n=0; n<=num_items; ++n) { std::cout << "------"; }
        std::cout << std::endl;

        for ( int w=0; w<=W; ++w) {
            std::cout << w << "| ";
            for ( int n=0; n<=num_items; ++n) {
                std::cout << table[w][n] << "     ";
            }
            std::cout << std::endl;
        }
    }
    //end do not delete this code 


  //figure out which items are in the bag based on the table
  std::vector<int> bag;

  int weight = W;
  // Decrease from the max allowed weight and backtracking
  for(int n = num_items; n > 0; --n)
  {
    // If a slot's value is better than its predecessor
    if(table[weight][n] > table[weight][n-1])
    {
      // Decrease the current weight
      weight -= items[n - 1].weight;
      // Push the item's index (remember to offset by decreasing 1)
      bag.push_back(n - 1);
    }
  }
  
  return bag;
}

////////////////////////////////////////////////////////////
/******************************************************************************/
/*!
  \brief
    This function calculate the total value of items in the bag
    
  \param items
    A vector contains input items
    
  \param bag
    A bag contains indices of items
    
  \return
    Value of the items accumulated
*/
/******************************************************************************/
int valueBag( std::vector<Item> const& items, std::vector<int> const& bag ) {
  std::vector<int>::const_iterator it   = bag.begin(),
                              it_e = bag.end();

  int accum = 0;
  //std::cout << "Bag ";
  for ( ; it != it_e; ++it) { 
    accum += items[ *it ].value; 
    //std::cout << *it << " ";
  }
  //std::cout << std::endl;
  return accum;
}

////////////////////////////////////////////////////////////
//prototype
//notice that auxiliary function returns value of the vector of items
//the actual vector is determined later from the table (similar to DP solution)
int knapsackRecMemAux( std::vector<Item> const&, int const&, int, Table& );

////////////////////////////////////////////////////////////
//function to kick start
/******************************************************************************/
/*!
  \brief
    This function solves Knapsack problem using recursive memorization
    
  \param items
    A vector contains input items
    
  \param W
    The maximum allowed weight
    
  \return
    An int vector contains the indices of items from the input vector
*/
/******************************************************************************/
std::vector<int> knapsackRecMem( std::vector<Item> const& items, int const& W ) {
  int num_items = items.size();

    // Crate a table size of (W + 1) * (num_items + 1) and init to -1
    Table table ( W + 1, std::vector<int>(num_items + 1, -1));
    int index = num_items;
    int max_w = W;
    
    // init first row and column to 0
    for(int n = 0; n<=num_items; ++n)
    {
      table[0][n] = 0;
    }
    
    for(int w = 0; w<=W; ++w)
    {
      table[w][0] = 0;
    }
    
    knapsackRecMemAux(items, max_w, index, table);
  //print table - debugging?
    //do not delete this code
    if ( num_items + W < 50 ) { //print only if table is not too big
        std::cout << "   ";
        for ( int n=0; n<=num_items; ++n) { std::cout << n << "     "; }
        std::cout << "  items\n        ";
        for ( int n=0; n<num_items; ++n) { 
            std::cout << items[n].weight << "," << items[n].value<<"   "; 
        }
        std::cout << "\n   ";
        for ( int n=0; n<=num_items; ++n) { std::cout << "------"; }
        std::cout << std::endl;

        for ( int w=0; w<=W; ++w) {
            std::cout << w << "| ";
            for ( int n=0; n<=num_items; ++n) {
                std::cout << table[w][n] << "     ";
            }
            std::cout << std::endl;
        }
    }
    //end do not delete this code 

  //figure out which items are in the bag based on the table
  std::vector<int> bag;
  
  int weight = W;
  // Decrease from the max allowed weight and backtracking
  for(int n = num_items; n > 0; --n)
  {
    // If a slot's value is better than its predecessor
    if(table[weight][n] > table[weight][n-1])
    {
      // Decrease the current weight
      weight -= items[n - 1].weight;
      // Push the item's index (remember to offset by decreasing 1)
      bag.push_back(n - 1);
    }
  }
  return bag;
}

////////////////////////////////////////////////////////////
//the real recursive function
/******************************************************************************/
/*!
  \brief
    This function solves Knapsack problem using recursive memorization
    
  \param items
    A vector contains input items
    
  \param W
    The maximum allowed weight
    
  \param index
    Index indicates current item (at which level of recursion)
    
  \param table
    The table to store values
    
  \return
    An int vector contains the indices of items from the input vector
*/
/******************************************************************************/
int
knapsackRecMemAux( std::vector<Item> const& items, int const& W, int index, Table & table ) {
  // If the value is not negative 1, safely to return precaculated value
  if(table[W][index] != -1)
    return table[W][index];
  
  // Else we have to make recursive calls to find the value that required to calcualte the slot we are at
  // If the item's weight corresponding to the slot is larger than allowed weight
  if(items[index - 1].weight > W)
  {
    // Make a recursive call to find the value of its predecessor slot
    table[W][index] = knapsackRecMemAux(items, W, index - 1, table);
  }
  else
  { // Else we need both it's predecessor and the slot without current item's weight
    table[W][index] = std::max(knapsackRecMemAux(items, W, index - 1, table), knapsackRecMemAux(items, W-items[index-1].weight, index - 1, table) + items[index-1].value);
  }
  return table[W][index];
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
