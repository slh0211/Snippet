/******************************************************************************/
/*!
\file   knapsack_brute_force_minchange.h
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS330
\par    Lab 4
\date   06/17/2017
\brief  
    This file contains the interface of GreyCode
    for the assignment.
    
  Hours spent on this assignment: 5

  Specific portions that gave you the most trouble:
    Took sometime to understand Gray Code because not very familiar with
    bitwise manipulation.
*/
/******************************************************************************/
#ifndef KNAPSACK_BRUTE_FORCE_MINCHANGE
#define KNAPSACK_BRUTE_FORCE_MINCHANGE
#include <utility>   // std::pair
#include <vector>
#include <bitset>
#include "definitions.h"
/******************************************************************************/
/*!
  \class Greycode
  \brief  
    The GreyCode class implements a object that generate grey code permutation

    Operations include:

    - Ctor
    - Next
*/
/******************************************************************************/
class GreyCode {
    private:
        // How many bits used in GreyCode
        int size_;
        // The int format index
        int data_;
        // To store the last permutation
        int last_bit_mask_;
        // Returns the gray code format of the int index in int
        int getGreyCode(int d);
    public:
        // Ctor
        GreyCode( int s );
        // first subset is empty, this function tells what's next 2^n-1 subsets
        // ret value: is last, is add, positin
        std::pair< bool, std::pair< bool, int > > Next();

};

// brute-force knapsack
std::vector<bool> knapsack_brute_force( std::vector<Item> const& items, Weight const& W );
#endif
