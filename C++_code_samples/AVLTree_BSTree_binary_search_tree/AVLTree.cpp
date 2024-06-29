/******************************************************************************/
/*!
\file   AVLTree.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS280
\par    Assignment 4
\date   03/16/2017
\brief  
    This file contains the implemantation of class AVLTree<T> and for the 
    assignment.

  Hours spent on this assignment: 15
  
  Operations include:
    - Ctor
    - Dtor
    - insert
    - remove
    
  Specific portions that gave you the most trouble:
    The different conditions of insert function took some time to fully debuged.
*/
/******************************************************************************/
/******************************************************************************/
/*!
  \brief
    Ctor for AVLTree<T> class
    
  \param OA
    Shared object allocator if provided
    
  \param ShareOA
    Determine if the object allocator can be shared with other AVLTree
*/
/******************************************************************************/
template <typename T>
AVLTree<T>::AVLTree(ObjectAllocator *OA, bool ShareOA) : BSTree<T>(OA, ShareOA)
{
}
/******************************************************************************/
/*!
  \brief
    Dtor for AVLTree<T> class
*/
/******************************************************************************/
template <typename T>
AVLTree<T>::~AVLTree()
{
}
/******************************************************************************/
/*!
  \brief
    This function calls insert_avl_item to insert an item
    
  \param value
    The value to be used to create a node
*/
/******************************************************************************/
template <typename T>
void AVLTree<T>::insert(const T& value)
{
  std::stack<typename BSTree<T>::BinTree *> nodes;
  insert_avl_item(this->get_root(), value, nodes);
}
/******************************************************************************/
/*!
  \brief
    This function calls remove_avl_item to delete an item
    
  \param value
    The value to be removed from the tree
*/
/******************************************************************************/
template <typename T>
void AVLTree<T>::remove(const T& value)
{
  unsigned compares = 0;
  // Call remove_avl_item on an item not on the tree may cause subtree count logic
  // error so do checking before any recursive call
  if(this->find(value, compares))
  {
    std::stack<typename BSTree<T>::BinTree *> nodes;
    remove_avl_item(this->get_root(), value, nodes);
  }
}
/******************************************************************************/
/*!
  \brief
    This function rotate a node to right (promote left child)
    
  \param tree
    The tree to be rotated
*/
/******************************************************************************/
template <typename T>
void AVLTree<T>::rotate_right(typename BSTree<T>::BinTree& tree)
{
  if(tree && tree->left)
  {
    // Create a temp to store current node
    typename BSTree<T>::BinTree temp = tree;
    // Store current node's count
    unsigned tempcount = tree->count;
    unsigned tempchildcount = 0;
    // Store left child's right child's count before rotation
    if(tree->left->right != 0)
      tempchildcount = tree->left->right->count;
    
    // Promote left child
    tree = temp->left;
    // Original root's minus left child's count plus right child's count
    temp->count = temp->count - tree->count + tempchildcount;
    // Store original root's count to the new root's count
    tree->count = tempcount;
    // Attach left child's dangling right child to original's left
    temp->left = tree->right;
    // Attach original root to the new root's right
    tree->right = temp;
  }
}
/******************************************************************************/
/*!
  \brief
    This function rotate a node to left (promote right child)
    
  \param tree
    The tree to be rotated
*/
/******************************************************************************/
template <typename T>
void AVLTree<T>::rotate_left(typename BSTree<T>::BinTree& tree)
{
  if(tree && tree->right)
  {
    // Create a temp to store current node
    typename BSTree<T>::BinTree temp = tree;
    // Store current node's count
    unsigned tempcount = tree->count;
    unsigned tempchildcount = 0;
    // Store right child's left child's count before rotation
    if(tree->right->left != 0)
      tempchildcount = tree->right->left->count;
    
    // Promote right child
    tree = temp->right;
    // Original root's minus right child's count plus left child's count
    temp->count = temp->count - tree->count + tempchildcount;
    // Store original root's count to the new root's count
    tree->count = tempcount;
    // Attach right child's dangling left child to original's right
    temp->right = tree->left;
    // Attach original root to the new root's left
    tree->left = temp;
  }
}
/******************************************************************************/
/*!
  \brief
    This function inserts an item and maintaining balanced recursively
    
  \param tree
    The tree to store the item
    
  \param Data
    The data to create a new node
    
  \param nodes
    The stack contains all visited nodes
*/
/******************************************************************************/
template <typename T>
void AVLTree<T>::insert_avl_item(typename BSTree<T>::BinTree& tree, const T& Data, std::stack<typename BSTree<T>::BinTree *>& nodes)
{
  if(tree == 0)
  {
    tree = this->make_node(Data);
    // Increment the count immediately after insert
    tree->count += 1;
    // balance the tree
    balance_avl_tree(nodes, true);
  }
  else if(Data < tree->data)
  {
    // Push visited node onto stack
    nodes.push(&tree);
    // Increment the count before recursive call (tree may point to 0 after balancing)
    tree->count += 1;
    insert_avl_item(tree->left, Data, nodes);
  }
  else if(Data > tree->data)
  {
    // Push visited node onto stack
    nodes.push(&tree);
    // Increment the count before recursive call (tree may point to 0 after balancing)
    tree->count += 1;
    insert_avl_item(tree->right, Data, nodes);
  }
}
/******************************************************************************/
/*!
  \brief
    This function removes an item and maintaining balanced recursively
    
  \param tree
    The tree contains item to be removed
    
  \param Data
    The data to be removed
    
  \param nodes
    The stack contains all visited nodes
*/
/******************************************************************************/
template <typename T>
void AVLTree<T>::remove_avl_item(typename BSTree<T>::BinTree& tree, const T& Data, std::stack<typename BSTree<T>::BinTree *>& nodes)
{
  if (tree == 0)
    return;
  else if (Data < tree->data)
  {
    // Push visited node onto stack
    nodes.push(&tree);
    // Decrement the count before recursive call (tree may point to 0 after balancing)
    tree->count -= 1;
    remove_avl_item(tree->left, Data, nodes);
  }
  else if (Data > tree->data)
  {
    // Push visited node onto stack
    nodes.push(&tree);
    // Decrement the count before recursive call (tree may point to 0 after balancing)
    tree->count -= 1;
    remove_avl_item(tree->right, Data, nodes);
  }
  else // (Data == tree->data)
  {
    if (tree->left == 0) // Promote the right child
    {
      typename BSTree<T>::BinTree temp = tree;
      tree = tree->right;
      this->free_node(temp);
      balance_avl_tree(nodes, false);
    }
    else if (tree->right == 0) // Promote the left child
    {
      typename BSTree<T>::BinTree temp = tree;
      tree = tree->left;
      this->free_node(temp);
      balance_avl_tree(nodes, false);
    }
    else // Replace data with predecessor's data
    {
      typename BSTree<T>::BinTree pred = 0;
      this->find_predecessor(tree, pred);
      tree->data = pred->data;
      // Decrement the count before recursive call (tree may point to 0 after balancing)
      tree->count -= 1;
      remove_avl_item(tree->left, tree->data, nodes);
    }
  }
}
/******************************************************************************/
/*!
  \brief
    This function balances an AVL tree 
    
  \param nodes
    The stack contains all visited nodes
    
  \param insertmode
    Determine if it is inserting
*/
/******************************************************************************/
template <typename T>
void AVLTree<T>::balance_avl_tree(std::stack<typename BSTree<T>::BinTree *>& nodes, bool insertmode)
{
  while(!nodes.empty())
  {
    typename BSTree<T>::BinTree* node = nodes.top();
    nodes.pop();
    
    // Check the height of the left and right subtree of y to find which is greater.
    int leftHeight = this->tree_height((*node)->left);
    int rightHeight = this->tree_height((*node)->right);
    int difference = leftHeight - rightHeight;

    // Find the difference to determine if balance is needed for subtree under this node
    // Only 3 possible cases so no need abs
    if(difference != -1&& difference != 0 && difference != 1)
    {
      // Case 1: Right is greater
      if(rightHeight > leftHeight)
      {
        int leftChildHeight = this->tree_height((*node)->right->left);
        int rightChildHeight = this->tree_height((*node)->right->right);
        // Unbalanced right-left
        if(leftChildHeight > rightChildHeight)
        {
          rotate_right((*node)->right);
          rotate_left(*node);
        } // Unbalanced right right
        else
        {
          rotate_left(*node);
        }
      }
      // Case 2: Left is greater
      else
      {
        int leftChildHeight = this->tree_height((*node)->left->left);
        int rightChildHeight = this->tree_height((*node)->left->right);
        // Unbalance left-right
        if(rightChildHeight > leftChildHeight)
        {
          rotate_left((*node)->left);
          rotate_right(*node);
        } // Unbalance left-left
        else
        {
          rotate_right(*node);
        }
      }
      
      // Insertion only need to perform one balance so return immediately
      if(insertmode == true)
        return;
    }
  }
}
