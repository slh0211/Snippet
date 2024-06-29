/******************************************************************************/
/*!
\file   disjoint_sets.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS280
\par    Assignment 6
\date   04/19/2017
\brief  
    This file contains the implemantation of class DisjointSets and other
    necessary classes for the assignment.

  Hours spent on this assignment: 5
  
  Specific portions that gave you the most trouble:
    
*/
/******************************************************************************/
#include "disjoint_sets.h"

//class Node implementation
/******************************************************************************/
/*!
  \brief
    Ctor for Node class
    
  \param value
    The representatives value
*/
/******************************************************************************/
Node::Node( size_t const& value ) : value(value), next() {}
/******************************************************************************/
/*!
  \brief
    Return next to client
    
  \return
    Return next to client
*/
/******************************************************************************/
Node* Node::Next() const { return next; }
/******************************************************************************/
/*!
  \brief
    Allow clent to set next
    
  \param new_next
    The new next node
*/
/******************************************************************************/
void  Node::SetNext( Node* new_next ) { next = new_next; }
/******************************************************************************/
/*!
  \brief
    Return value to client
    
  \return
    Return value to client
*/
/******************************************************************************/
size_t Node::Value() const { return value; }
/******************************************************************************/
/*!
  \brief
    Overload operator<< for printing Node
    
  \param os
    The ostream
    
  \param node
    The node to be printed
  
  \return
    Return an reference to ostream
*/
/******************************************************************************/
std::ostream& operator<< (std::ostream& os, Node const& node) 
{
  os << "(" << node.value << " ) -> ";
  return os;
}

//class Head implementation
/******************************************************************************/
/*!
  \brief
    Ctor for Head class
*/
/******************************************************************************/
Head::Head( ) : counter(), first(), last() {} 
/******************************************************************************/
/*!
  \brief
    Dtor for Head class
*/
/******************************************************************************/
Head::~Head() 
{
  Node* walk = first;
  while(walk)
  {
    Node* temp = walk->Next();
    delete walk;
    walk = temp;
  }
}
/******************************************************************************/
/*!
  \brief
    Return the size of the internal Node list
    
  \return
    The counter of the list
*/
/******************************************************************************/
size_t Head::Size() const      { return counter; }
/******************************************************************************/
/*!
  \brief
    Reset the Head to default value (after joining to another Head)
*/
/******************************************************************************/
void   Head::Reset()           { counter = 0; last = first = NULL; }
/******************************************************************************/
/*!
  \brief
    Return the first Node of the internal Node list
    
  \return
    Return the first Node of the internal Node list
*/
/******************************************************************************/
Node*  Head::GetFirst() const  { return first; }
/******************************************************************************/
/*!
  \brief
    Return the last Node of the internal Node list
    
  \return
    Return the last Node of the internal Node list
*/
/******************************************************************************/
Node*  Head::GetLast()  const  { return last; }
/******************************************************************************/
/*!
  \brief
    Initialize the Head with a given value
    
  \param value
    The value to create first Node on the list
*/
/******************************************************************************/
void   Head::Init( size_t value ) 
{
  first = last = new Node( value );
  counter = 1;
}
/******************************************************************************/
/*!
  \brief
    This function connect another Head to this Head
    
  \param pHead2
    Pointer to the head that to be connected after this Head's last node
*/
/******************************************************************************/
void   Head::Join( Head* pHead2 ) 
{
  counter = counter + pHead2->Size();
  last->SetNext(pHead2->GetFirst());
  last = pHead2->GetLast();
  pHead2->Reset();
}
/******************************************************************************/
/*!
  \brief
    Overload operator<< for printing Head
    
  \param os
    The ostream
    
  \param head
    The Head to be printed
  
  \return
    Return an reference to ostream
*/
/******************************************************************************/
std::ostream& operator<< (std::ostream& os, Head const& head) 
{
  os << "[" << head.counter << " ] -> ";
  return os;
}

//class DisjointSets implementation
/******************************************************************************/
/*!
  \brief
    Ctor for DisjointSets class
    
  \param capacity
    The capacity to store vertices
*/
/******************************************************************************/
DisjointSets::DisjointSets( size_t const& capacity ) : size(0), capacity(capacity), representatives(new size_t[capacity]), heads (new Head[capacity])
{
}
/******************************************************************************/
/*!
  \brief
    Dtor for DisjointSets class
*/
/******************************************************************************/
DisjointSets::~DisjointSets() 
{
  delete [] heads;
  delete [] representatives;
}
/******************************************************************************/
/*!
  \brief
    This function initialize the array of Heads and the table in numeric order
*/
/******************************************************************************/
void DisjointSets::Make( ) 
{
  if ( size == capacity ) throw "DisjointSets::Make(...) out of space";
  
  heads[size].Init( size );
  representatives[size] = size;
  ++size;
}
/******************************************************************************/
/*!
  \brief
    This function joins two lists under two Heads
    
  \param id1
    The id of an edge's vertex 1
    
  \param id2
    The id of an edge's vertex 2
*/
/******************************************************************************/
void DisjointSets::Join( size_t const& id1, size_t const& id2 ) 
{
  size_t realid1 = representatives[id1];
  size_t realid2 = representatives[id2];
  // Prevent joining two lists of same ids
  if(realid1 != realid2)
  {
    // If id1's list less than/equal to id2's list
    if(heads[realid1].Size() <= heads[realid2].Size())
    {
      Node* walk = heads[realid1].GetFirst();
      // connect id1's head to id2's tail
      heads[realid2].Join(&heads[realid1]);
      // walk the shorter list to update table
      while(walk)
      {
        representatives[walk->Value()] = heads[realid2].GetFirst()->Value();
      
        walk = walk->Next();
      }
    }
    // else
    else
    {
      Node* walk = heads[realid2].GetFirst();
      // connect id2's head to id1's tail
      heads[realid1].Join(&heads[realid2]);
      // walk the shorter list to update table
      while(walk)
      {
        representatives[walk->Value()] = heads[realid1].GetFirst()->Value();
      
        walk = walk->Next();
      }
    }
  }
}
/******************************************************************************/
/*!
  \brief
    This function returns the representative id on the table with a given id
    
  \param id
    The id of the vertex
    
  \return
    The representative id on the table
*/
/******************************************************************************/
size_t DisjointSets::GetRepresentative( size_t const& id ) const 
{
  return representatives[id];
}
/******************************************************************************/
/*!
  \brief
    This function returns the representative id on the table with a given id
    
  \param id
    The id of the vertex
    
  \return
    The representative id on the table
*/
/******************************************************************************/
size_t DisjointSets::operator[]( size_t const& id ) const 
{
  return representatives[id];
}
/******************************************************************************/
/*!
  \brief
    Overload operator<< for printing DisjointSets
    
  \param os
    The ostream
    
  \param ds
    The DisjointSets to be printed
  
  \return
    Return an reference to ostream
*/
/******************************************************************************/
std::ostream& operator<< (std::ostream& os, DisjointSets const& ds) 
{
  for (size_t i=0; i<ds.size; ++i ) 
  {
    os << i << ":  ";
    Head *p_head = &ds.heads[i];
    os << *p_head;
    Node* p_node = p_head->GetFirst();
    while ( p_node ) 
    {
      os << *p_node;
      p_node = p_node->Next();
    }
    os << "NULL (representative " << ds.representatives[i] << ")\n";
  }
  
  return os;
}
