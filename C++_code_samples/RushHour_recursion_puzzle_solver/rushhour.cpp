/******************************************************************************/
/*!
\file   rushhour.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS280
\par    Assignment 3
\date   02/26/2017
\brief  
    This file contains the implemantation of class ParkingLotSolver and other
    necessary functions for the assignment.
    
  Hours spent on this assignment: 15

  Specific portions that gave you the most trouble:
    Possible moves took some time to be debugged
*/
/******************************************************************************/
#include <iostream>   // ostream, cout, cerr
#include <fstream>    // ifstream
#include <regex>      // regular expression, smatch
#include <algorithm>  // reverse
#include "rushhour.h"

// Keep this
/******************************************************************************/
/*!
  \brief
    This function prints out Direction
    
  \param os
    The ostream
    
  \param d
    The Direction to be printed
    
  \return
    The ostream
*/
/******************************************************************************/
std::ostream& operator<<( std::ostream& os, Direction const& d ) {
    switch ( d ) {
        case up:    os << "up "; break;
        case left:  os << "left "; break;
        case down:  os << "down "; break;
        case right: os << "right "; break;
        default:    os << "undefined"; break;
    }
    return os;
}
/******************************************************************************/
/*!
  \brief
    This function prints out current positions of ParkingLotSolver
    
  \param os
    The ostream
    
  \param pl
    The ParkingLotSolver to be printed
    
  \return
    The ostream
*/
/******************************************************************************/
std::ostream& operator<<( std::ostream& os, ParkingLotSolver const& pl ) {
//    os << pl.height << std::endl;
//    os << pl.width << std::endl;
    for ( unsigned i=0; i<pl.height; ++i ) {
        for ( unsigned j=0; j<pl.width; ++j ) {
            os << pl.parking_lot[i][j] << " ";
        }
        os << std::endl;
    }
    return os;
}
/******************************************************************************/
/*!
  \brief
    Non-default ctor for ParkingLotSolver class. This function reads a file
    and parses the data.
    
  \param filename
    The file name of the input file that contains the layout of the parking lot
  
  \exception char const*
    Text message indicating the reason for the exception
*/
/******************************************************************************/
ParkingLotSolver::ParkingLotSolver( std::string const&  filename ) : filename(filename)
{
    std::ifstream infile ( filename );
    std::string   data; // all data from input file

    if ( ! infile.is_open()) {
        std::cerr << "Errors in input file: cannot open \"" << filename << "\"" << std::endl;
        throw "Errors in input file: cannot open";
    }

    // read the whole file into a string
    while (! infile.eof() ) {
        std::string line;
        std::getline( infile, line );
        data.append( line );
        data.append( " " );
    }

    infile.close();

    // parse data
    std::smatch match;

    if ( std::regex_match( data, match, std::regex( ".*width\\s+(\\d+).*", std::regex_constants::icase ) ) && match.size() == 2 ) {
        // The first sub_match is the whole string; the next
        // sub_match is the first parenthesized expression.
        width = std::stoi( match[1] );
        data = std::regex_replace( data, std::regex( "width\\s+\\d+", std::regex_constants::icase ), "");
    } else {
        std::cerr << "Errors in input file: cannot find \"width\"" << std::endl;
        throw "Errors in input file: cannot find \"width\"";
    }
    if ( std::regex_match( data, match, std::regex( ".*height\\s+(\\d+).*", std::regex_constants::icase ) ) && match.size() == 2 ) {
        height = std::stoi( match[1] );
        data = std::regex_replace( data, std::regex( "height\\s+\\d+", std::regex_constants::icase ), "");
    } else {
        std::cerr << "Errors in input file: cannot find \"height\"" << std::endl;
        throw "Errors in input file: cannot find \"height\"";
    }
    if ( std::regex_match( data, match, std::regex( ".*car\\s+(\\d+).*", std::regex_constants::icase ) ) && match.size() == 2 ) {
        thecar_ = std::stoi( match[1] );
        data = std::regex_replace( data, std::regex( "car\\s+\\d+", std::regex_constants::icase ), "");
    } else {
        std::cerr << "Errors in input file: cannot find \"car\"" << std::endl;
        throw "Errors in input file: cannot find \"car\"";
    }
    if ( std::regex_match( data, match, std::regex( ".*exit\\s+([a-z]+).*", std::regex_constants::icase ) ) && match.size() == 2 ) {
        std::string dir_str = match[1].str();
        if      ( dir_str == "left" )   { exit_direction = left; }
        else if ( dir_str == "right" )  { exit_direction = right; }
        else if ( dir_str == "up" )     { exit_direction = up; }
        else if ( dir_str == "down" )   { exit_direction = down; }
        else                            { throw "unknown exit direction "; }
        data = std::regex_replace( data, std::regex( "exit\\s+[a-z]+", std::regex_constants::icase ), "");
    } else {
        std::cerr << "Errors in input file: cannot find \"exit\"" << std::endl;
        throw "Errors in input file: cannot find \"exit\"";
    }

    // only cells are left in the data
    std::regex re_cell("(\\d+)"); // each cell is a number or dot
    std::sregex_iterator cell_matches_begin = std::sregex_iterator( data.begin(), data.end(), re_cell );
    std::sregex_iterator cell_matches_end   = std::sregex_iterator();

    // should have exactly height*width numbers and dots
    if ( std::distance( cell_matches_begin , cell_matches_end ) == height*width ) {
        unsigned * parking_lot_data = new unsigned [height*width];
        parking_lot = new unsigned*[height];
        for ( unsigned i=0; i<height; ++i ) {
            parking_lot[i] = parking_lot_data + i*width;
        }
        unsigned pos = 0;
        for (std::sregex_iterator iter = cell_matches_begin; iter != cell_matches_end; ++iter ) {
            parking_lot_data [pos++] = std::stoi( (*iter).str() );
        }
    } else {
        std::cerr << "Errors in input file: number of cells should be " << height << "*" << width << ". Found " << std::distance( cell_matches_begin , cell_matches_end ) << std::endl;
        throw "Errors in input file: number of cells";
    }
    // Create 2 copies of the original positions
    for (unsigned k=0; k<height*width; ++k ) 
    {
      unsigned i = k/width;
      unsigned j = k%width;
      allcars_.push_back(parking_lot[i][j]);
      original_.push_back(parking_lot[i][j]);
    }
    // Sort and remove duplicates and 0 from the allcars_ vector
    std::sort(allcars_.begin(), allcars_.end());
    allcars_.erase(std::unique( allcars_.begin(), allcars_.end() ), allcars_.end());
    // Erase the 0 from allcars_ vector
    allcars_.erase(allcars_.begin());
}
/******************************************************************************/
/*!
  \brief
    Dtor for ParkingLotSolver class
*/
/******************************************************************************/
ParkingLotSolver::~ParkingLotSolver()
{
    delete [] parking_lot[0];
    delete [] parking_lot;
}

// Direction is an enum (see header):
// +------------>j
// |    ^ 0
// |    |
// |1<--+-->3     
// |    |
// |    V 2
// V        update (i,j) given 1 step in direction d
// i        i += (d-1)*((3-d)%2)    j += (d-2)*(d%2)
// 0 = -1,0
// 1 = 0,-1
// 2 = 1,0
// 3 = 0,1
//
//                                move = car,    direction, num positions
/******************************************************************************/
/*!
  \brief
    This function scans the 2D parking lot array and updates new to new 
    position according to the move tuple
    
  \param move
    The tupple contains car ID, the direction of the move, and how many 
    positions the move moves
    
  \exception char const*
    Text message indicating the reason for the exception
*/
/******************************************************************************/
void ParkingLotSolver::makeMove( std::tuple< unsigned, Direction, unsigned > move )
{
    int d = std::get<1>( move ); // convert direction to int
    int di = (d-1)*((3-d)%2);    // see comment before function
    int dj = (d-2)*(d%2);        // see comment before function
    int scan_direction = di+dj; // -1 (up,left) or 1 (down,right)

    unsigned num_positions = std::get<2>( move );
    unsigned car = std::get<0>( move );

    unsigned i_start = ( scan_direction == 1 )?height-1:0;
    unsigned j_start = ( scan_direction == 1 )?width-1 :0;
    for ( unsigned step = 0; step<num_positions; ++step ) { // move car 1 position at a time
        for ( unsigned i=i_start; i<height; i-=scan_direction ) {
            for ( unsigned j=j_start; j<width; j-=scan_direction ) {
                if ( parking_lot[i][j] == car ) {
                    parking_lot[i][j] = 0;
                    // check if legal
                    if ( i+di >= height && j+dj >= width ) {
                        throw( "Car moved outside of parking lot" );
                        return;
                    }
                    if ( parking_lot[i+di][j+dj] > 0 ) {
                        throw( "Car moved on top of another car" );
                        return;
                    }
                    parking_lot[i+di][j+dj] = car;
                }
            }
        }
    }
}
/******************************************************************************/
/*!
  \brief
    This function takes a move and reverses the Direction and calls makeMove
    to undo a move
    
  \param move
    The tupple contains car ID, the direction of the move, and how many 
    positions the move moves
    
  \exception const char*
    Text message indicating the reason for the exception
*/
/******************************************************************************/
void ParkingLotSolver::undoMove( std::tuple< unsigned, Direction, unsigned > move )
{
  Direction d = std::get<1>(move);
  Direction reversed;
  
  switch(d)
  {
        case up:    reversed = down; break;
        case left:  reversed = right; break;
        case down:  reversed = up; break;
        case right: reversed = left; break;
        default:    reversed = undefined; break;
  }
  
  unsigned car = std::get<0>(move);
  unsigned position = std::get<2>(move);
  
  std::tuple< unsigned, Direction, unsigned > undo (car, reversed, position);
  makeMove(undo);
}
/******************************************************************************/
/*!
  \brief
    This function determines if the ParkingLot is solved by checking if the 
    car has reached the boundary of exit_direction
    
  \return
    Returns true if solved else false
*/
/******************************************************************************/
bool ParkingLotSolver::IsSolved() const 
{
    unsigned i_car_pos = height;
    unsigned j_car_pos = width;
    Orientation orientation = horisontal;
    bool done = false;
    for ( unsigned i=0; i<height && !done; ++i ) {
        for ( unsigned j=0; j<width && !done; ++j ) {
            if ( parking_lot[i][j] == thecar_ ) {
                if ( i_car_pos == height ) { // first time seeing car
                    i_car_pos = i;
                    j_car_pos = j;
                } else { //second time - ready to make decision 
                    if ( j_car_pos == j ) { // default is horisontal
                        orientation = vertical;
                    }
                    done = true;
                }
            }
        }
    }

    bool solved = false;
    switch ( exit_direction ) {
        case up:    solved = orientation==vertical   && parking_lot[0][j_car_pos] == thecar_; break;
        case left:  solved = orientation==horisontal && parking_lot[i_car_pos][0] == thecar_; break;
        case down:  solved = orientation==vertical   && parking_lot[height-1][j_car_pos] == thecar_; break;
        case right: solved = orientation==horisontal && parking_lot[i_car_pos][width-1] == thecar_; break;
        default: break;
    }
    return solved;
}
/******************************************************************************/
/*!
  \brief
    This function checks each move (tuple) in the solution vector
    
  \param sol
    The vector that contains all the moves
  
  \return
    Returns 1 if last move complete the solution or encounter an error 
    else 0
*/
/******************************************************************************/
int ParkingLotSolver::Check( std::vector< std::tuple<unsigned, Direction, unsigned> > const& sol ) 
{
    try {
        //Print( "solution.txt" );
        for ( std::tuple<unsigned, Direction, unsigned> const& m : sol ) {
            std::cout << "Car " << std::get<0>( m ) << " to be moved " << std::get<1>( m ) << " " << std::get<2>( m ) << " positions\n";
            makeMove( m );
            //Print( "solution.txt" );
            std::cout << *this;
        }
        bool final_pos = IsSolved( );
        std::cout << "Number of steps = " << sol.size() << std::endl;
        std::cout << "Solved = " << final_pos << std::endl;
        return final_pos;
    } catch ( char const * msg ) {
        std::cout << "ERROR - " << msg << std::endl;
        return 1;
    }
}
/******************************************************************************/
/*!
  \brief
    This function takes a filename for output and prints out current positions
    
  \param filename_out
    The filename of the output file
*/
/******************************************************************************/
void ParkingLotSolver::Print( std::string const& filename_out )
{
    std::ofstream os;
    os.open( filename_out, std::ofstream::out | std::ofstream::app );
    for ( unsigned i=0; i<height; ++i ) {
        for ( unsigned j=0; j<width; ++j ) {
            os << parking_lot[i][j] << " ";
        }
    }
    os << std::endl;
}
/******************************************************************************/
/*!
  \brief
    This function takes a car and finds all possible moves for the car
    
  \param car
    The car to be moved
  
  \return
    The output vector contains all possible moves for a car at current position
*/
/******************************************************************************/
std::vector< std::tuple<unsigned, Direction, unsigned> > ParkingLotSolver::GetPossibleMoves(unsigned car) const
{
  unsigned i_pos_front = height;
  unsigned j_pos_front = width;
  Orientation orientation = horisontal;
  bool done = false;
  // Find the car's position front upper left corner and determine orientation
  for ( unsigned i=0; i<height && !done; ++i ) {
      for ( unsigned j=0; j<width && !done; ++j ) {
          if ( parking_lot[i][j] == car ) {
              if ( i_pos_front == height ) { // first time seeing car
                  i_pos_front = i;
                  j_pos_front = j;
              } else { //second time - ready to make decision 
                  if ( j_pos_front == j ) { // default is horisontal
                      orientation = vertical;
                  }
                  done = true;
              }
          }
      }
  }
  
  unsigned i_pos_tail = height;
  unsigned j_pos_tail = width;
  done = false;
  
  // Find car's tail (the other end)
  switch(orientation)
  {
    case vertical:
      j_pos_tail = j_pos_front;
      for(unsigned i=height - 1; i > i_pos_front; --i)
      {
        if(parking_lot[i][j_pos_tail] == car)
        {
          i_pos_tail = i;
          break;
        }
      }
      break;
    case horisontal:
      i_pos_tail = i_pos_front;
      for(unsigned j=width - 1; j > j_pos_front; --j)
      {
        if(parking_lot[i_pos_tail][j] == car)
        {
          j_pos_tail = j;
          break;
        }
      }
      break;
  }

  std::vector< std::tuple<unsigned, Direction, unsigned> > possible_moves;
  
  switch(orientation)
  {
    case vertical:
      //Check up
      //Do nothing if front is at the most upper index
      if(i_pos_front > 0)
      {
        // ex: car at (2,0), checking (1,0) . (0,0)
        for(unsigned di = 1; di <= i_pos_front; ++di)
        {
          if(parking_lot[i_pos_front - di][j_pos_front] == 0)
          {
            possible_moves.push_back(std::tuple< unsigned, Direction, unsigned >(car, up, di));
          }
          else
            break;
        }
      }
      //Check down
      if(i_pos_tail < height - 1)
      {
        // ex: cat at (4,4) checking (5,4) (6,4) ..... (height - 1, 4)
        for(unsigned di = 1; i_pos_tail + di < height; ++di)
        {
          if(parking_lot[i_pos_tail + di][j_pos_tail] == 0)
          {
            possible_moves.push_back(std::tuple< unsigned, Direction, unsigned >(car, down, di));
          }
          else
            break;
        }
      }
      break;
    case horisontal:
      //Check left
      if(j_pos_front > 0)
      {
        // ex: car at (3,3) checking (3,2) ... (3,0)
        for(unsigned dj = 1; dj <= j_pos_front; ++dj)
        {
          if(parking_lot[i_pos_front][j_pos_front - dj] == 0)
          {
            possible_moves.push_back(std::tuple< unsigned, Direction, unsigned >(car, left, dj));
          }
          else
            break;
        }
      }
      //Check right
      if(j_pos_tail < width - 1)
      {
        // ex: cat at (1,2) checking (1,3) (1,4) .... (1, width - 1)
        for(unsigned dj = 1; j_pos_tail + dj < width; ++dj)
        {
          if(parking_lot[i_pos_tail][j_pos_tail + dj] == 0)
          {
            possible_moves.push_back(std::tuple< unsigned, Direction, unsigned >(car, right, dj));
          }
          else
            break;
        }
      }
      
      break;
  }
  
  return possible_moves;
}
/******************************************************************************/
/*!
  \brief
    This function finds a solution recursively
    
  \param solution
    The final solution if finds one
    
  \param history
    Set storing previously encountered positions to avoid infinite loop
    
  \param depth
    Current depth (step) of the recursive call
    
  \param max_depth
    The max_depth the of the recurive call
  
  \return
    Returns true if find a solution else false
*/
/******************************************************************************/
bool ParkingLotSolver::DFS(std::vector< std::tuple<unsigned, Direction, unsigned> >& solution, 
         std::set< std::vector<unsigned> >& history,
         int depth,
         int const& max_depth)
{
  // Base case
  if(IsSolved())
  {
    return true;
  }
  // Depth limit
  if(depth > max_depth)
  {
    return false;
  }
  
  // Loop through each car in numeric order
  for(std::vector<unsigned>::const_iterator i = allcars_.begin(); i != allcars_.end(); ++i)
  {
    // Get all possible moves for each car
    std::vector< std::tuple< unsigned, Direction, unsigned > > possible_moves = GetPossibleMoves(*i);
    // Loop through each move
    for(std::tuple< unsigned, Direction, unsigned > const& m : possible_moves )
    {
      // Make move
      makeMove(m);
      // Save to history
      // 1) Convert to vector
      std::vector< unsigned > current_position;
      for (unsigned k=0; k<height*width; ++k ) 
      {
        unsigned i = k/width;
        unsigned j = k%width;
        current_position.push_back(parking_lot[i][j]);
      }
      // 2) Check if already in history
      auto it_pos = history.find( current_position );
      // not found, new position
      if ( it_pos == history.end() )
      {
        // insert to history
        auto it_insert = history.insert( current_position ).first;
        // call recursively
        // if recursive call retuns true - solution found, update solution and return
        if ( DFS( solution, history, depth+1, max_depth ) )
        {
          // this terminates search (since we found solution)
          // as we go up the search tree insert current moves from all recursive calls
          solution.push_back(m);
          return true;
        }
        // if here, DFS returned false, no solution found, delete current position/board from history -- or do we?????
        history.erase( it_insert );
      }
      // undo move
      undoMove(m);
    }
  }
  
  return false;
}
/******************************************************************************/
/*!
  \brief
    This function finds a solution within a max_depth
    
  \param max_depth
    The max steps allowed to use
  
  \return
    Returns a vector contains all steps to solve the ParkingLot, if not able
    to find one solution than empty
*/
/******************************************************************************/
std::vector< std::tuple<unsigned, Direction, unsigned> > ParkingLotSolver::Solve()
{
  std::vector< std::tuple<unsigned, Direction, unsigned> > solution;
  std::set< std::vector< unsigned > > history;
  
  int max_depth = 10;
  while(!IsSolved())
  {
	ResetParkingLot();
	DFS(solution, history, 1, max_depth);
	max_depth += 10;
  }
  // Moves are push_backed and have to be reversed
  std::reverse(solution.begin(), solution.end());
  // Reset to original position for debugging purpose
  ResetParkingLot();
  
  return solution;
}
/******************************************************************************/
/*!
  \brief
    This function resets paring_lot to original position
*/
/******************************************************************************/
void ParkingLotSolver::ResetParkingLot(void)
{
  for (unsigned k=0; k<height*width; ++k ) 
  {
    unsigned i = k/width;
    unsigned j = k%width;
    parking_lot[i][j] = original_[k];
  }
}
/******************************************************************************/
/*!
  \brief
    This function finds an optimal solution (min steps)
    
  \return
    Returns a vector contains all steps to solve the ParkingLot, if not able
    to find one solution than empty
*/
/******************************************************************************/
std::vector< std::tuple<unsigned, Direction, unsigned> > ParkingLotSolver::SolveOptimally()
{
  std::vector< std::tuple<unsigned, Direction, unsigned> > solution;
  std::set< std::vector< unsigned > > history;
  
  int max_depth = 1;
  
  while(!IsSolved())
  {
    // Reset parking_lot after each DFS call
    ResetParkingLot();
    DFS(solution, history, 1, max_depth);
    ++max_depth;
  }
  
  std::reverse(solution.begin(), solution.end());
  ResetParkingLot();
  
  return solution;
}
/******************************************************************************/
/*!
  \brief
    This function takes a filename and finds a solution
    
  \param filename
    The filename of the file that contains the level
    
  \return
    Returns a vector contains all steps to solve the ParkingLot, if not able
    to find one solution than empty
*/
/******************************************************************************/
std::vector< std::tuple<unsigned, Direction, unsigned> > SolveRushHour( std::string const& filename )
{
  ParkingLotSolver pl(filename);
  
  return pl.Solve();
}
/******************************************************************************/
/*!
  \brief
    This function takes a filename and finds a optimal solution
    
  \param filename
    The filename of the file that contains the level
    
  \return
    Returns a vector contains all steps to solve the ParkingLot, if not able
    to find one solution than empty
*/
/******************************************************************************/
std::vector< std::tuple<unsigned, Direction, unsigned> > SolveRushHourOptimally( std::string const& filename )
{
  ParkingLotSolver pl(filename);
  
  return pl.SolveOptimally();
}