/******************************************************************************/
/*!
\file   successor.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS280
\par    Lab 3
\date   03/06/2017
\brief  
    This file contains the implemantation of necessary functions for the 
    successor Lab.
    
  Hours spent on this assignment: 2

  Specific portions that gave you the most trouble:
    Took sometime to figure how to go up to parent level without using
    parent pointer
*/
/******************************************************************************/
#include "successor.h"
/******************************************************************************/
/*!
  \brief
    This function finds minimum recursively
    
  \param pRoot
    The node to be checked
    
  \return
    The node that contains the minimum
*/
/******************************************************************************/
Node* findMinimum(Node* pRoot)
{
  if(pRoot->left == NULL)
  {
    return pRoot;
  }
  else
  {
    return findMinimum(pRoot->left);
  }
}
/******************************************************************************/
/*!
  \brief
    This function finds first ancestor recursively
    
  \param pRoot
    The node to be checked
    
  \return
    The node that contains first ancestor
*/
/******************************************************************************/
Node* findFirstAncestor(Node* pRoot, Node* pParent)
{
  // pParent could be NULL so check pParent first
  if(pParent && pParent->left == pRoot)
    return pParent;
  // go up to the upper level if pRoot is right child
  else if(pParent && pParent->right == pRoot)
    return findFirstAncestor(pRoot->parent, pParent->parent);
  else
    return NULL;
}
/******************************************************************************/
/*!
  \brief
    This function finds minimum recursively
    
  \param pRoot
    The node to be checked
    
  \return
    The node that contains the minimum
*/
/******************************************************************************/
Node2* findMinimum(Node2* pRoot)
{
  if(pRoot->left == NULL)
  {
    return pRoot;
  }
  else
  {
    return findMinimum(pRoot->left);
  }
}
/******************************************************************************/
/*!
  \brief
    This function finds successor recursively
    
  \param pRoot
    The node to be checked
    
  \param key
    The key to be used
    
  \return
    The node that contains the successor
*/
/******************************************************************************/
Node* successor(Node* pRoot, INT key)
{
  if ( pRoot->key == key ) { // data comparison
      if (pRoot->right) {
          // implement algorithm part 1)
          // a) if the node has right child, successor is the minimum node in the right subtree
          return findMinimum(pRoot->right);
      } else {
          // implement algorithm part 2)
          // b) otherwise, successor is the first ancestor of the node, which is reached through left link
          // special case - following parent pointers you have reached the root of the tree and all parents
          // were reached through right pointer - no successor, key was maximum key.
          return findFirstAncestor(pRoot, pRoot->parent);
          /*
          Node* walk = pRoot;
          Node* pParent = pRoot->parent;
          while(pParent != NULL && (pParent->right == walk))
          {
            walk = pParent;
            pParent = walk->parent;
          }
          
          walk = pParent;
          
          return walk;
          */
      }
  }

  if (pRoot->key > key) { // data comparison
      return successor( pRoot->left, key);
  }
  else {
      return successor( pRoot->right, key);
  }
}
/******************************************************************************/
/*!
  \brief
    This function finds successor recursively
    
  \param pRoot
    The node to be checked
    
  \param key
    The key to be used
    
  \return
    The node that contains the successor
*/
/******************************************************************************/
Node2* successor2(Node2* pRoot, INT key)
{
  if ( pRoot->key == key ) {
    if (pRoot->right) {
       return findMinimum(pRoot->right);
    } else {
       // Return NULL to let caller to decide what to do
       return NULL;
    } 
  }
 
  if (pRoot->key > key) {
      Node2* pResult = successor2( pRoot->left, key);
      // returning from left
      // If the result is NULL, and returns from left, caller's pRoot is
      // the first ancestor
      if(pResult == NULL)
        return pRoot;
      // else it is the result returning from findMinimum
      else
        return pResult;
  } else {
      Node2* pResult = successor2( pRoot->right, key);
      // returning from right
      // If the result is NULL, and returns from right, caller has to return
      // one more level to let the parent caller to decide
      if(pResult == NULL)
        return NULL;
      // else it is the result returning from findMinimum
      else
        return pResult;
  }
}