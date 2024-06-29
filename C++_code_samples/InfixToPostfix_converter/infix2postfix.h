/******************************************************************************/
/*!
\file   infix2postfix.h
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS280
\par    Lab 1
\date   1/23/2017
\brief  
    This file contains the interface of infix2postfix lab.

  Hours spent on this assignment: 2

  Specific portions that gave you the most trouble:
    None
*/
/******************************************************************************/
#include <string> //string

  // Calculate an int using postfix input string
int         Evaluate( std::string const& postfix );
  // Takes an input string in infix and output an string in postfix
std::string Infix2postfix( std::string const& infix );
