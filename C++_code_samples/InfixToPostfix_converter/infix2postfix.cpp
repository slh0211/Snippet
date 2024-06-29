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
#include "infix2postfix.h" //Evaluate, Infix2postfix
#include <iostream> //cout, endl
#include <stack> //stack
/******************************************************************************/
/*!
  \brief
    Evaluate an input string in postfix and output to an int
    
  \param postfix
    The input string
    
  \return
    The calculated result
*/
/******************************************************************************/
int Evaluate( std::string const& postfix ) 
{
  // Create an empty stack for evaluate
  std::stack<int> stack;
  // Loop through each char from the input string
  for ( std::string::const_iterator it = postfix.begin(); it != postfix.end(); ++it)
  {
    // Current input char
    char token = *it;
    // Push to stack if it is a number
    if (token >= '0' && token <= '9') 
    {
      stack.push(token - '0');
    }
    else
    {
      // Pop 2 numbers from the stack
      int arg2 = stack.top();
      stack.pop();
      int arg1 = stack.top();
      stack.pop();
      // Positions or arg1 and arg2 on the stack swapped for '-' and '/'
      if (token == '+')
      {
        stack.push( arg1 + arg2);
      }
      else if (token == '*')
      {
        stack.push( arg1 * arg2);
      }
      else if(token == '-')
      {
        stack.push( arg1 - arg2);
      } 
      else if(token == '/')
      {
        stack.push( arg1 / arg2);
      }
      else
      { // Return if found an unknown operator
        std::cout << "Unknown operator" << std::endl;
        return 0;
      }
    }
  } 
  // Return the final result
  return stack.top();
}
/******************************************************************************/
/*!
  \brief
    Read an input string in infix and output to a string in postfix
    
  \param infix
    The infix input string
    
  \return
    The postfix output string
*/
/******************************************************************************/
std::string Infix2postfix( std::string const& infix )
{
  //Need a string queue to store final output
  std::string output;
  //Need a stack of char to evaluate () and operator
  std::stack<char> evastack;
  
  //Loop through each char from input
  for ( std::string::const_iterator it = infix.begin(); it != infix.end(); ++it)
  { //Current input
    char token = *it;
    //Operand - send to the output
    if(token >= '0' && token <= '9')
    {
      output.push_back(token);
    }
    else if (token == '(') //Left parenthesis - push onto the stack
    {
      evastack.push(token);
    }
    else if (token == ')') //Right parenthesis - operators are popped off the 
    {                      //stack and sent to the output until a left parenthesis is found (and then discarded)
      //Stop if evastack is empty
      while(!evastack.empty())
      {
        //Operators sent to output
        if(evastack.top() != '(')
        {
          output.push_back(evastack.top());
          evastack.pop();
        }
        else //If found a '('
        {
          //Discard '(' and exit the loop
          evastack.pop();
          break;
        }
      }
    }
    else if (token == '+' || token == '-' || token == '*' || token == '/') //Operators
    {
      //If the stack is empty, push the operator
      if(evastack.empty())
      {
        evastack.push(token);
      }
      else if (evastack.top() == '(') //If the top of the stack is a left parenthesis, push the operator onto the stack.
      {
        evastack.push(token);
      }
      //If the top of the stack is an operator which has the same or lower precendence 
      //than the scanned operator, push the scanned operator.
      else if (token == '*' || token == '/') // '*' and '/' always have higher or same precendence
      {
        evastack.push(token);
      } 
      //'+' and '-' have same precendence with '+' and '-'
      else if (((token == '+') || (token == '-')) && ((evastack.top() == '+') || (evastack.top() == '-')))
      {
        evastack.push(token);
      }
      //If the top of the stack is an operator which has higher precedence, 
      //pop the stack and send to the output. Repeat the algorithm with the new top of stack.
      //'+' and '-' have lower precendence compare to '*' and '/'
      else if (((token == '+') || (token == '-')) && ((evastack.top() == '*') || (evastack.top() == '/')))
      {
        //Loop through the stack until find an operator have same or lower precendece '+' '-'
        while(!evastack.empty() && ((evastack.top() == '*') || (evastack.top() == '/'))) 
        {
          output.push_back(evastack.top());
          evastack.pop();
        }
        evastack.push(token);
      }
    }
  }
  //If the input stream is empty and there are still operators on the stack, 
  //pop all of them and add them to the output.
  while(!evastack.empty())
  {
    output.push_back(evastack.top());
    evastack.pop();
  }

  return output;
}
