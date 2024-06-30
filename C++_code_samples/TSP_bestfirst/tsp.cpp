/******************************************************************************/
/*!
\file   tsp.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS330
\par    Assignment 2
\date   06/23/2017
\brief  
    This file contains the implemantation of backtracking branch bound best
    first functions for the assignment.
    
  Hours spent on this assignment: 20

  Specific portions that gave you the most trouble:
    Made mistake by eliminating more rows in the matrix than needed and 
    took forever to figure out.
*/
/******************************************************************************/
#include <iostream>   // cout
#include <fstream>    // ifstream
#include <limits>     // numeric_limits
#include <algorithm>  // find
#include <map>        // multimap
#include "tsp.h"
/******************************************************************************/
/*!
  \brief
    This function read a file and inits a vector matrix for cost
    
  \param filename
    Name of file
    
  \param map
    A vector of vector<int> to store cost
    
  \param TotalCity
    An int to store how many city to be traveled
*/
/******************************************************************************/
void init( char const* filename, MAP& map, int& TotalCity )
{
    map.clear();
    std::ifstream in( filename, std::ifstream::in );
    if( !in.is_open() ) {
        std::cout << "problem reading " << filename << std::endl;
        return;
    }
    in >> TotalCity;
    for( int i = 0; i < TotalCity; ++i ) {
        std::vector<int> row;
        for( int j = 0; j < TotalCity; ++j ) {
            row.push_back( std::numeric_limits<int>::max() );
        }
        map.push_back( row );
    }
    for( int i = 0; i < TotalCity; ++i ) {
        for( int j = i + 1; j < TotalCity; ++j ) {
            if( !in.good() ) {
                std::cout << "problem reading " << filename << std::endl;
                return;
            }
            in >> map[i][j];
            map[j][i] = map[i][j];
        }
    }
}
/******************************************************************************/
/*!
  \brief
    This function estimates lower bound for future cost
    
  \param m
    The matrix contains cost to each city
    
  \param N
    How many city to be traveled
    
  \param solution_so_far
    A vector contains city already traveled
    
  \param cost_solution_so_far
    A int stores how much is the cost so far
    
  \return
    Estimated future cost under the branch
*/
/******************************************************************************/
int
lower_bound( 
        MAP const& m, 
        int    const& N, 
        std::vector<int> &solution_so_far,
        int    const& cost_solution_so_far )
{

  int lower_bound = cost_solution_so_far;
  int temp_cost = 0;
  // Create a copy of solution_so_far
  std::vector<int> past_cities = solution_so_far;
  // Remove current city from past_cities
  past_cities.pop_back();

  // Loop through each unused city toward last city
  for ( int i = 1; i<N; ++i ) 
  {
    // Skip the row (city) already past, the current city on solution so far should kept to calculate the cost to next city
    if (std::find(past_cities.begin(), past_cities.end(), i) != past_cities.end()) continue;

    int min_in_row = std::numeric_limits<int>::max();
    // For each unvisited row
    for ( int j=0; j<N; ++j ) 
    {
      //check if job is not assigned (column is taken)
      // The first city 0 has to be checked again to return to
      if(j == 0 || std::find( solution_so_far.begin(), solution_so_far.end(), j) == solution_so_far.end())
      {
        if ( min_in_row > m[i][j] )
        {
          min_in_row = m[i][j];
        }
      }
    }
    
    temp_cost += min_in_row;
  }

    return lower_bound + temp_cost;
}
/******************************************************************************/
/*!
  \brief
    This recursive function generates best solution for TSP problem
    
  \param m
    The matrix contains cost to each city
    
  \param N
    How many city to be traveled
    
  \param index
    To determine how deep the recursive function currently is
    
  \param solution_so_far
    A vector contains city already traveled
    
  \param cost_solution_so_far
    A int stores how much is the cost so far
    
  \param best_solution_so_far
    A vector contains best solution so far
    
  \param cost_best_solution_so_far
    A int stores the best solution's cost
    
  \param last_index
    A index to indicates which city was traveled before this call
    
*/
/******************************************************************************/
void
backtracking_branch_bound_best_first_aux( 
        MAP const& m, 
        int    const& N, 
        int    const& index, 
        std::vector<int> &solution_so_far, 
        int& cost_solution_so_far, 
        std::vector<int> &best_solution_so_far, 
        int& cost_best_solution_so_far,
        int last_index
        ) 
{
    if(cost_solution_so_far > cost_best_solution_so_far)
      return;
    
    //termination check (if on last level)
    if ( index == N ) {
      // Increment last cost to return to city 0 to the final cost to compare with best solution
      int last_cost = m[last_index][0];
      int final_cost = cost_solution_so_far + last_cost;
        //std::cout << "solution " << final_cost << " ( best " << cost_best_solution_so_far << " )" << std::endl;
        if ( final_cost < cost_best_solution_so_far ) {
            cost_best_solution_so_far = final_cost;
            best_solution_so_far = solution_so_far;
        }
    }
    
    //hack - use map to order, key - bound, value - job.
    //insert pairs, then traverse the map in-order
    std::multimap<int,int> ordered_jobs;

    //generate nodes and order them by bound
    int temp;
    for ( int j=1; j<N; ++j ) {
        //skip if job j is already assigned
        if ( std::find( solution_so_far.begin(), solution_so_far.end(), j) != solution_so_far.end() ) continue;
        //if(j == last_index) continue;
        solution_so_far.push_back( j );
        temp = cost_solution_so_far;
        cost_solution_so_far += m[last_index][j];

        int lb = lower_bound( m, N, solution_so_far, cost_solution_so_far );
        ordered_jobs.insert ( std::pair<int,int>(lb,j) );
        solution_so_far.pop_back( );
        //cost_solution_so_far -= m[last_index][j];
        cost_solution_so_far = temp;
    }

    //traverse nodes in the above order - BEST FIRST
    std::multimap<int,int>::const_iterator b = ordered_jobs.begin(),
        e = ordered_jobs.end();
    for ( ; b != e; ++b ) {
        int j = b->second; // get job index;
        solution_so_far.push_back( j );
        temp = cost_solution_so_far;
        cost_solution_so_far += m[last_index][j];

        int lb = b->first;
        //branch cancelation check
        //std::cout << "lower_bound = " << lower_bound( m, N, solution_so_far, cost_solution_so_far ) << std::endl;
        if ( lb < cost_best_solution_so_far ) {
            backtracking_branch_bound_best_first_aux( m, N, index+1, 
                    solution_so_far, cost_solution_so_far, 
                    best_solution_so_far, cost_best_solution_so_far, j);
        }

        solution_so_far.pop_back( );
        //cost_solution_so_far -= m[last_index][j];
        cost_solution_so_far = temp;
    }
}
/******************************************************************************/
/*!
  \brief
    This function calls the recursive function to 
    generate best solution for TSP problem
    
  \param m
    The matrix contains cost to each city
    
  \param N
    How many city to be traveled
    
  \param
    A vector contains best solution
    
*/
/******************************************************************************/
std::vector<int>
backtracking_branch_bound_best_first(MAP const& m, int const& N ) {
    std::vector<int> best_solution_so_far;
    std::vector<int> solution_so_far;
    int cost_solution_so_far = 0;
    int cost_best_solution_so_far = std::numeric_limits<int>::max();
    
    // Push back city 0 since always starts at 0
    solution_so_far.push_back(0);
    backtracking_branch_bound_best_first_aux( m, N, 1, solution_so_far, cost_solution_so_far, best_solution_so_far, cost_best_solution_so_far, 0);
    // Push back city 0 since always ends at 0
    best_solution_so_far.push_back(0);
    
    return best_solution_so_far;
}
/******************************************************************************/
/*!
  \brief
    This function reads a file and solve TSP problem
    
  \param filename
    The filename to be read
    
  \return
    A vector contains best solution
    
*/
/******************************************************************************/
std::vector<int> SolveTSP(char const* filename)
{
  MAP map;
  int num_cities;
  // load & check
  init(filename, map, num_cities );
  
  std::vector<int> result;

  result = backtracking_branch_bound_best_first(map, num_cities);

  return result;
}