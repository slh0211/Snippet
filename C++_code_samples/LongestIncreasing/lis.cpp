/******************************************************************************/
/*!
\file   lis.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS330
\par    Lab lis
\date   07/07/2017
\brief  
    This file contains the implemantation of longest_increasing_subsequence
    function for the assignment.
    
  Hours spent on this assignment: 6

  Specific portions that gave you the most trouble:
    Took sometime to write correct algorithm for main logic
*/
/******************************************************************************/
#include "lis.h"
#include <algorithm> // reverse
/******************************************************************************/
/*!
  \brief
    This function finds the longest increasing subsequence
    
  \param sequence
    A vector containing int sequence
    
  \return
    A vector containing indicies used in the longest subsequence
*/
/******************************************************************************/
std::vector<unsigned> 
longest_increasing_subsequence( std::vector<int> const& sequence ) {
    //allocated/reserve table, init all to 0 - including init case
    std::vector<int> table(sequence.size(), 1);
    
    // for each index in the table
    for(unsigned i = 1; i < table.size(); ++i)
    {
      // check each index up until i
      for(unsigned j = 0 ; j < i; ++j)
      {
        // if the sequence[j] is less than sequence[i]
        // but the table has not been updated
        if(sequence[j] < sequence[i] && table[i] < table[j] + 1)
          table[i] = table[j] +1;
      }
    }
    
    // find the max sequence on the final table
    int max = 0;
    for(unsigned i = 0; i < table.size(); i++)
    {
      if(max < table[i])
        max = table[i];
    }
    
    //reconstruct subsequence
    //std::vector<unsigned> answer( length,0 ); //vector of indices corresponding to the LIS
    std::vector<unsigned> answer;
    
    int tmp = max;
    // loop the table backward
    for(unsigned i = table.size(); i-- > 0;)
    {
      // it is guranteed that if multiple indicies have same value,
      // the last one (closer to the end) will be used as the indicies in the longest sequence
      if(table[i] == tmp)
      {
        answer.push_back(i);
        --tmp;
      }
    }
    // reverse the indicies vector
    std::reverse(answer.begin(), answer.end());
    
    return answer;
}
