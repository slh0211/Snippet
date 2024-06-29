/******************************************************************************/
/*!
\file   AVLTree.h
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS280
\par    Assignment 4
\date   03/16/2017
\brief  
    This file contains the interface of class AVLTree<T> and other
    necessary class for the assignment.

  Hours spent on this assignment: 15

  Specific portions that gave you the most trouble:
    Many unexpected behavior and bugs from indexing took some hours to fix.
*/
/******************************************************************************/
//---------------------------------------------------------------------------
#ifndef AVLTREE_H
#define AVLTREE_H
//---------------------------------------------------------------------------
#include <stack> // std::stack
#include "BSTree.h"
/******************************************************************************/
/*!
  \class AVLTree
  \brief  
    The AVLTree class implements a AVL tree data structure

    Operations include:

    - Ctor
    - Dtor
    - insert
    - remove
*/
/******************************************************************************/
template <typename T>
class AVLTree : public BSTree<T>
{
  public:
    // Ctor
    AVLTree(ObjectAllocator *OA = 0, bool ShareOA = false);
    // Dtor
    virtual ~AVLTree();
    // Insert an item
    virtual void insert(const T& value);
    // Remove an item
    virtual void remove(const T& value);
    
  private:
    // private stuff
    // Rotate a node to right (promote left child)
    void rotate_right(typename BSTree<T>::BinTree& tree);
    // Rotate a node to left (promote right child)
    void rotate_left(typename BSTree<T>::BinTree& tree);
    // Insert an item and maintain balanced
    void insert_avl_item(typename BSTree<T>::BinTree& tree, const T& Data, std::stack<typename BSTree<T>::BinTree *>& nodes);
    // Remove an item and maintain balanced
    void remove_avl_item(typename BSTree<T>::BinTree& tree, const T& Data, std::stack<typename BSTree<T>::BinTree *>& nodes);
    // Balance the tree from bottom to top
    void balance_avl_tree(std::stack<typename BSTree<T>::BinTree *>& nodes, bool insertmode);
};

#include "AVLTree.cpp"

#endif
//---------------------------------------------------------------------------
