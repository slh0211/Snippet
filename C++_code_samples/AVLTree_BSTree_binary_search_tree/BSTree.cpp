/******************************************************************************/
/*!
\file   BSTree.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS280
\par    Assignment 4
\date   03/16/2017
\brief  
    This file contains the implemantation of class BSTree<T> and for the 
    assignment.

  Hours spent on this assignment: 15
  
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
    
  Specific portions that gave you the most trouble:
    The different conditions of insert function took some time to fully debuged.
*/
/******************************************************************************/
/******************************************************************************/
/*!
  \brief
    Ctor for BSTree<T> class
    
  \param OA
    Shared object allocator if provided
    
  \param ShareOA
    Determine if the object allocator can be shared with other BSTree
*/
/******************************************************************************/
template <typename T>
BSTree<T>::BSTree(ObjectAllocator *OA, bool ShareOA) : oa_(OA), shareOA_(ShareOA), freeOA_(false), root_(0)
{
  // Check if ObjectAllocator is provided by the client
  if(oa_ == 0)
  {
    // Create self owned ObjectAllocator if not provided by the client
    OAConfig config(true);
    oa_ = new ObjectAllocator(sizeof(BinTreeNode), config);
    // Set to true to allow deallocation in dtor
    freeOA_ = true;
  }
}
/******************************************************************************/
/*!
  \brief
    Copy ctor for BSTree<T> class
    
  \param rhs
    The original BSTree<T> object
*/
/******************************************************************************/
template <typename T>
BSTree<T>::BSTree(const BSTree& rhs) : oa_(0), shareOA_(false), freeOA_(false), root_(0)
{
  // If rhs not sharing allocator, need to create a self owned oa
  if(rhs.shareOA_ == false)
  {
    OAConfig config(true);
    oa_ = new ObjectAllocator(sizeof(BinTreeNode), config);
    freeOA_ = true;
  }
  else
  {
    oa_ = rhs.oa_;
    shareOA_ = true;
  }
  // Call helper function to deep copy tree
  root_ = copy_tree(rhs.root_);
}
/******************************************************************************/
/*!
  \brief
    Dtor for BSTree<T> class
*/
/******************************************************************************/
template <typename T>
BSTree<T>::~BSTree()
{
  // Clear out the entire tree
  clear();
  // Delete OA if is self owned
  if(freeOA_ == true)
  {
    delete oa_;
  }
}
/******************************************************************************/
/*!
  \brief
    Assignment operator for BSTree<T> class
    
  \param rhs
    The original BSTree<T> object
    
  \return
    The reference to current object
*/
/******************************************************************************/
template <typename T>
BSTree<T>& BSTree<T>::operator=(const BSTree& rhs)
{
  // First check if self assign
  if(&rhs != this)
  {
    // Clear the tree before modifying oa_
    clear();
    
    if(rhs.shareOA_ == true)
    {
      // If rhs provides oa but current object using self owned oa, delete current oa
      if(freeOA_ == true)
      {
        delete oa_;
      }
      
      oa_ = rhs.oa_;
      shareOA_ = true;
      freeOA_ = false;
    }
    else
    {
      // If rhs does not provide oa but cureent object using shared oa, allocates a self owned oa
      if(freeOA_ == false)
      {
        OAConfig config(true);
        oa_ = new ObjectAllocator(sizeof(BinTreeNode), config);
        shareOA_ = false;
        freeOA_ = true;
      }
    }
    
    root_ = copy_tree(rhs.root_);
  }
  
  return *this;
}
/******************************************************************************/
/*!
  \brief
    This function calls select to return a node at given index
    
  \param index
    The index of the node in the tree
    
  \return
    The node at given index
*/
/******************************************************************************/
template <typename T>
const typename BSTree<T>::BinTreeNode* BSTree<T>::operator[](int index) const
{
  return select(root_, index);
}
/******************************************************************************/
/*!
  \brief
    This function calls insert_item to insert an item
    
  \param value
    The value to be used to create a node
*/
/******************************************************************************/
template <typename T>
void BSTree<T>::insert(const T& value)
{
  // Need an recursive helper function
  insert_item(root_, value);
}
/******************************************************************************/
/*!
  \brief
    This function calls remove_item to delete an item
    
  \param value
    The value to be removed from the tree
*/
/******************************************************************************/
template <typename T>
void BSTree<T>::remove(const T& value)
{
  unsigned compares = 0;
  // Call remove_item on an item not on the tree may cause subtree count logic
  // error so do checking before any recursive call
  if(find(value, compares))
  {
    remove_item(root_, value);
  }
}
/******************************************************************************/
/*!
  \brief
    This function calls delete_tree to delete all nodes and set root_ to 0
*/
/******************************************************************************/
template <typename T>
void BSTree<T>::clear(void)
{
  // Need an recursive helper function
  delete_tree(root_);
  root_ = 0;
}
/******************************************************************************/
/*!
  \brief
    This function calls item_exists to find an item
    
  \param value
    The value to be found
    
  \param compares
    Unsigned to store the number of comparison
    
  \return
    Return true if found else false
*/
/******************************************************************************/
template <typename T>
bool BSTree<T>::find(const T& value, unsigned &compares) const
{
  return item_exists(root_, value, compares);
}
/******************************************************************************/
/*!
  \brief
    This function check if the tree is empty
    
  \return
    Return true if empty else false
*/
/******************************************************************************/
template <typename T>
bool BSTree<T>::empty(void) const
{
  if(root_)
    return false;
  else
    return true;
}
/******************************************************************************/
/*!
  \brief
    This function calls node_count to calculate how many nodes are in the tree
    
  \return
    The total number of nodes
*/
/******************************************************************************/
template <typename T>
unsigned int BSTree<T>::size(void) const
{
  return node_count(root_);
}
/******************************************************************************/
/*!
  \brief
    This function calls height to calculate the tree's height
    
  \return
    The height of the tree
*/
/******************************************************************************/
template <typename T>
int BSTree<T>::height(void) const
{
  return tree_height(root_);
}
/******************************************************************************/
/*!
  \brief
    This function returns root_ to client for printing and debugging
    
  \return
    The root_ of current object
*/
/******************************************************************************/
template <typename T>
typename BSTree<T>::BinTree BSTree<T>::root(void) const
{
  return root_;
}
/******************************************************************************/
/*!
  \brief
    This function inserts an item to the tree with a given value
    
  \param tree
    The tree (path) to be inserted the item
    
  \param Data
    The data to be store at the newly created node
*/
/******************************************************************************/
template <typename T>
void BSTree<T>::insert_item(BinTree& tree, const T& Data)
{
  if (tree == 0)
  {
    tree = make_node(Data);
    // Increment subtree count immediately after returning from make_node
    tree->count += 1;
  }
  else if(Data < tree->data)
  {
    // Increment subtree count before recursive call
    tree->count += 1;
    insert_item(tree->left, Data);
  }
  else if(Data > tree->data)
  {
    // Increment subtree count before recursive call
    tree->count += 1;
    insert_item(tree->right, Data);
  }
  // Not handling duplicates in this assignment
}
/******************************************************************************/
/*!
  \brief
    This function removes an item of a given value
    
  \param tree
    The tree (path) to the item to be removed
    
  \param Data
    The data to be removed
*/
/******************************************************************************/
template <typename T>
void BSTree<T>::remove_item(BinTree& tree, const T& Data)
{
  // This handles item not found
  if (tree == 0)
    return;
  else if (Data < tree->data)
  {
    // Decrement subtree count before recursive call
    // May cause bug if item is not on the tree, so do checking in remove
    // before recursive call
    tree->count -= 1;
    remove_item(tree->left, Data);
  }
  else if (Data > tree->data)
  {
    // Decrement subtree count before recursive call
    // May cause bug if item is not on the tree, so do checking in remove
    // before recursive call
    tree->count -= 1;
    remove_item(tree->right, Data);

  }
  else // (Data == tree->data)
  {
    if (tree->left == 0) // Replace the node  with the child if only one child
    {
      BinTree temp = tree;
      tree = tree->right;
      free_node(temp);
    }
    else if (tree->right == 0) // Replace the node with the child if only one child
    {
      BinTree temp = tree;
      tree = tree->left;
      free_node(temp);
    }
    else // Replace the data of the predecessor than delete the predecessor
    {
      BinTree pred = 0;
      find_predecessor(tree, pred);
      tree->data = pred->data;
      // Decremnet count before remove_item on subtree call
      tree->count -= 1;
      remove_item(tree->left, tree->data);
    }
  }
}
/******************************************************************************/
/*!
  \brief
    This function returns root_ reference allowing modification (for derived class)
    
  \return
    The root_
*/
/******************************************************************************/
template <typename T>
typename BSTree<T>::BinTree& BSTree<T>::get_root(void)
{
  return root_;
}
/******************************************************************************/
/*!
  \brief
    This function calls object allcator and create a node with a given value
    
  \param value
    The value to be stored in the new node
    
  \exception BSTException
    Indicates no memory available from object allocator
    
  \return
    A pointer points at newly created node
*/
/******************************************************************************/
template <typename T>
typename BSTree<T>::BinTree BSTree<T>::make_node(const T& value)
{
  BinTree bnode = 0;
  try
  {
    BinTree buf = reinterpret_cast<BinTree>(oa_->Allocate());
    // Placement new to construct the object using ctor onto preallocated memory
    bnode = new (buf) BinTreeNode(value);
  }
  catch (const OAException &e)
  {
    throw(BSTException(BSTException::E_NO_MEMORY, e.what()));
  }
  
  return bnode;
}
/******************************************************************************/
/*!
  \brief
    This function frees a given node to object allocator
    
  \param node
    The node to be freed
*/
/******************************************************************************/
template <typename T>
void BSTree<T>::free_node(BinTree node)
{
  // Call destructor explicity (for templated class)
  node->~BinTreeNode();
  oa_->Free(node);
}
/******************************************************************************/
/*!
  \brief
    This function finds an item recursively
    
  \param tree
    The tree (path) to the stored item
    
  \param Data
    The data of the item to be found
    
  \param compares
    The number of comparisons
  
  \return
    Return true if found else false
*/
/******************************************************************************/
template <typename T>
bool BSTree<T>::item_exists(BinTree tree, const T& Data, unsigned& compares) const
{
  compares +=1;
  if(tree == 0)
    return false;
  else if(Data == tree->data)
    return true;
  else if (Data < tree->data)
    return item_exists(tree->left, Data, compares);
  else
    return item_exists(tree->right, Data, compares);
}
/******************************************************************************/
/*!
  \brief
    This function calculates total number of nodes in the tree recursively
    
  \param tree
    The tree
*/
/******************************************************************************/
template <typename T>
unsigned int BSTree<T>::node_count(BinTree tree) const
{
  if(tree == 0)
    return 0;
  else
    return 1 + node_count(tree->left) + node_count(tree->right);
}
/******************************************************************************/
/*!
  \brief
    This function calculates the height of the tree recursively
    
  \param tree
    The tree
    
  \return
    The height of the tree
*/
/******************************************************************************/
template <typename T>
int BSTree<T>::tree_height(BinTree tree) const
{
  // Definition of empty node
  if(tree == 0)
    return -1;
  
  int lh = tree_height(tree->left);
  int rh = tree_height(tree->right);
  
  if(lh > rh)
    return lh + 1;
  else
    return rh + 1;
}
/******************************************************************************/
/*!
  \brief
    This function finds the predecessor (on the subtree)
    
  \param tree
    The tree
    
  \param predecessor
    A pointer to points at the right most node on the left subtree
*/
/******************************************************************************/
template <typename T>
void BSTree<T>::find_predecessor(BinTree tree, BinTree &predecessor) const
{
  predecessor = tree->left;
  
  while (predecessor->right != 0)
  {
    predecessor = predecessor->right;
  }
}
/******************************************************************************/
/*!
  \brief
    This function finds a node using given index recursively
    
  \param node
    The tree
    
  \param index
    The index of the item to be returned
    
  \return
    A pointer points at the node of the given index
*/
/******************************************************************************/
template <typename T>
const typename BSTree<T>::BinTreeNode* BSTree<T>::select(BinTree node, unsigned index) const
{
  // If node is NULL, return NULL. (Base case)
  if(node == 0)
    return 0;
  // If the left subtree has more than i nodes (L > i), then it's in the left subtree.
    // Check left == 0 before accessing to prevent accessing to 0
  else if(node->left && node->left->count > index)
  {
    // Recursively call the method with the left node and the index.
    return select(node->left, index);
  }
  // If the left subtree has less than i nodes (L < i), then it's in the right subtree.
    // Check left == 0 and index != 0 to prevent accessing to 0
    // (left == 0 and index == 0 is for next condition)
  else if((node->left == 0 && index != 0)|| node->left->count < index)
    // Recursively call the method with the right node and i - L - 1.
  {
    unsigned leftcount = 0;
    
    if(node->left != 0)
    {
      leftcount = node->left->count;
    }
    
    return select(node->right, index - leftcount -1);
  }
  // Else the left subtree has exactly i nodes so it's in the current node.
  else
  {
    return node;
  }
}
/******************************************************************************/
/*!
  \brief
    This function deep copies entire tree of the rhs recursively
    
  \param rhsroot
    A pointer points at node on the rhs tree
    
  \return
    A pointer points at the newly created node
*/
/******************************************************************************/
template <typename T>
typename BSTree<T>::BinTree BSTree<T>::copy_tree(BinTree rhsroot)
{
  if(rhsroot == 0)
    return 0;
  
  BinTree newnode = make_node(rhsroot->data);
  // copy everything
  newnode->balance_factor = rhsroot->balance_factor;
  newnode->count = rhsroot->count;
  
  newnode->left = copy_tree(rhsroot->left);
  newnode->right = copy_tree(rhsroot->right);
  
  return newnode;
}
/******************************************************************************/
/*!
  \brief
    This function deletes entire tree recursively using post order
    
  \param tree
    The tree to be deleted
*/
/******************************************************************************/
template <typename T>
void BSTree<T>::delete_tree(BinTree& tree)
{
  if(tree)
  {
    delete_tree(tree->left);
    delete_tree(tree->right);
    free_node(tree);
  }
}
/******************************************************************************/
/*!
  \brief
    This function determine if Indexing is implemented
    
  \return
    Return true if implemented else false
*/
/******************************************************************************/
template <typename T>
bool BSTree<T>::ImplementedIndexing(void)
{
  return true;
}
/******************************************************************************/
/*!
  \brief
    This function determine if BalanceFactor is implemented
    
  \return
    Return true if implemented else false
*/
/******************************************************************************/
template <typename T>
bool BSTree<T>::ImplementedBalanceFactor(void)
{
  return false;
}