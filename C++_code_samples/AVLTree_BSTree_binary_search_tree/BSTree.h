/******************************************************************************/
/*!
\file   BSTree.h
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS280
\par    Assignment 4
\date   03/16/2017
\brief  
    This file contains the interface of class BSTree<T> and other
    necessary class for the assignment.

  Hours spent on this assignment: 15

  Specific portions that gave you the most trouble:
    Many unexpected behavior and bugs from indexing took some hours to fix.
*/
/******************************************************************************/
//---------------------------------------------------------------------------
#ifndef BSTREE_H
#define BSTREE_H
//---------------------------------------------------------------------------
#ifdef _MSC_VER
#pragma warning( disable : 4290 ) // suppress warning: C++ Exception Specification ignored
#endif

#include <string>    // std::string
#include <stdexcept> // std::exception

#include "ObjectAllocator.h"

class BSTException : public std::exception
{
  public:
    BSTException(int ErrCode, const std::string& Message) :
      error_code_(ErrCode), message_(Message) {
    };

    virtual int code(void) const {
      return error_code_;
    }
    virtual const char *what(void) const throw() {
      return message_.c_str();
    }
    virtual ~BSTException() {}
    enum BST_EXCEPTION{E_DUPLICATE, E_NO_MEMORY};

  private:  
    int error_code_;
    std::string message_;
};
/******************************************************************************/
/*!
  \class BSTree
  \brief  
    The BSTree class implements a binary search tree data structure

    Operations include:

    - Ctor
    - Copy ctor
    - Dtor
    - = opeartor
    - [] operator const
    - insert
    - remove
    - clear
    - find
    - empty
    - size
    - height
    - root
*/
/******************************************************************************/
template <typename T>
class BSTree
{
  public:
    struct BinTreeNode
    {
      BinTreeNode *left;
      BinTreeNode *right;
      T data;
      int balance_factor; // optional
      unsigned count;    // number of nodes in subtree
      
      BinTreeNode(void) : left(0), right(0), data(0), balance_factor(0), count(0) {};
      BinTreeNode(const T& value) : left(0), right(0), data(value), balance_factor(0), count(0) {};
    };
    typedef BinTreeNode* BinTree;

    // Ctor
    BSTree(ObjectAllocator *OA = 0, bool ShareOA = false);    
    // Copy ctor
    BSTree(const BSTree& rhs);
    // Dtor
    virtual ~BSTree();
    // Assignment
    BSTree& operator=(const BSTree& rhs);
    // Indexing
    const BinTreeNode* operator[](int index) const;
    // Insert by value
    virtual void insert(const T& value);
    // Remove by value
    virtual void remove(const T& value);
    // Clear all nodes
    void clear(void);
    // Find a given value
    bool find(const T& value, unsigned &compares) const;
    // Determine if empty
    bool empty(void) const;
    // Total number of the nodes
    unsigned int size(void) const;
    // Height of the tree
    int height(void) const;
    // Return the root to client for printing and other functioning
    BinTree root(void) const;

    // Extra credit
    static bool ImplementedIndexing(void);
    static bool ImplementedBalanceFactor(void);

  protected:
    // Return root_ allows derived class to modify
    BinTree& get_root(void);
    // Construct a node
    BinTree make_node(const T& value);
    // Delete a node
    void free_node(BinTree node);
    // Calculate height recursively
    int tree_height(BinTree tree) const;
    // Find the predecessor of a given node
    void find_predecessor(BinTree tree, BinTree &predecessor) const;
    // Find an item recursively
    bool item_exists(BinTree tree, const T& Data, unsigned& compares) const;
    // Count total nodes recursively
    unsigned int node_count(BinTree tree) const;
    // Compare count of nodes and calculate the index recursively
    const BinTreeNode* select(BinTree node, unsigned index) const;
    // Copy entire tree
    BinTree copy_tree(BinTree rhsroot);
    // Delete entire tree
    void delete_tree(BinTree& tree);
    
  private:
    // private stuff    
    // Insert an item recursively
    void insert_item(BinTree& tree, const T& Data);
    // Delete an item recursively
    void remove_item(BinTree& tree, const T& Data);
    
    // ObjectAllocator
    ObjectAllocator* oa_;
    // Determine if another object uses same oa_
    bool             shareOA_;
    // Determine if oa_ needs to be deleted
    bool             freeOA_;
    // Root of the tree
    BinTree          root_;
};

#include "BSTree.cpp"

#endif
//---------------------------------------------------------------------------
