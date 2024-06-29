/******************************************************************************/
/*!
\file   rushhour.h
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS280
\par    Assignment 3
\date   02/26/2017
\brief  
    This file contains the interface of class ParkingLotSolver and other
    necessary functions for the assignment.

  Hours spent on this assignment: 15

  Specific portions that gave you the most trouble:
    Possible moves took some time to be debugged.
*/
/******************************************************************************/
#ifndef RUSHHOUR_H
#define RUSHHOUR_H

#include <vector>
#include <tuple>
#include <set>

// Keep this
enum Direction   { up, left, down, right, undefined };

// Keep this
enum Orientation { horisontal, vertical};

// Keep this
std::ostream& operator<<( std::ostream& os, Direction const& d );

// Keep this
////////////////////////////////////////////////////////////////////////////////
// global functions
std::vector< std::tuple<unsigned, Direction, unsigned> > 
SolveRushHour( std::string const& filename );

std::vector< std::tuple<unsigned, Direction, unsigned> > 
SolveRushHourOptimally( std::string const& filename );

////////////////////////////////////////////////////////////////////////////////
// your stuff
////////////////////////////////////////////////////////////////////////////////
/******************************************************************************/
/*!
  \class ParkingLotSolver
  \brief  
    The ParkingLotSolver class implements a solver for Rush Hour puzzle

      Operations include:
      - ParkingLotSolver
      - ~ParkingLotSolver
      - Check
      - Print
      - Solve
      - SolveOptimally
      - operator<<
      - makeMove
      - undoMove
      - IsSolved
      - GetPossibleMoves
      - DFS
      - ResetParkingLot
*/
/******************************************************************************/
class ParkingLotSolver 
{
  public:
      // Ctor
      ParkingLotSolver( std::string const&  filename );
      // Dtor
      ~ParkingLotSolver();
      // Check to see if each move is valid and the last move solves
      int Check( std::vector< std::tuple<unsigned, Direction, unsigned> > const& sol );
      // Print the position
      void Print( std::string const& filename_out );
      // Find a solution (with a predefined max depth)
      std::vector< std::tuple<unsigned, Direction, unsigned> > Solve();
      // Find a solution with minimal depth
      std::vector< std::tuple<unsigned, Direction, unsigned> > SolveOptimally();
      // Allow ostream to print the positions
      friend std::ostream& operator<<( std::ostream& os, ParkingLotSolver const& pl );

  private:
      // Scan and update the 2D array
      void makeMove( std::tuple< unsigned, Direction, unsigned > move );
      // Undo a move
      void undoMove( std::tuple< unsigned, Direction, unsigned > move );
      // Check if the target car reaches the correct boundary
      bool IsSolved() const;
      // Get all possible moves of a car
      std::vector< std::tuple<unsigned, Direction, unsigned> > GetPossibleMoves(unsigned car) const;
      // Find a solution recursively
      bool DFS(std::vector< std::tuple<unsigned, Direction, unsigned> >& solution, 
               std::set< std::vector<unsigned> >& history,
               int depth,
               int const& max_depth);
      // Reset parking_lot to original position
      void ResetParkingLot(void);
  
      unsigned height             = 0;            // size of parking lot
      unsigned width              = 0;            // size of parking lot
      unsigned ** parking_lot     = nullptr;      // parking lot 
      Direction exit_direction    = undefined;    // exit direction
      unsigned thecar_                = 0;            // car to be navigated
      std::string filename        = std::string();// filename for data
      std::vector<unsigned> allcars_;             // all cars number
      std::vector<unsigned> original_;            // original parking_lot position
      
};

#endif
