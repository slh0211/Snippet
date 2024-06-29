/******************************************************************************/
/*!
\file   ChHashTable.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS280
\par    Assignment 5
\date   04/02/2017
\brief  
    This file contains the implemantation of class ChHashTable<T> for the 
    assignment.

  Hours spent on this assignment: 8
  
  Operations include:
    - Ctor
    - Dtor
    - insert
    - remove
    - find
    - clear
    - GetStat
    - GetTable
    
  Specific portions that gave you the most trouble:
    
*/
/******************************************************************************/
#include <cmath> // ceil
/******************************************************************************/
/*!
  \brief
    Ctor for ChHashTable<T> class
    
  \param Config
    Config to set up the class
    
  \param allocator
    Shared object allocator if provided by client
    
  \exception HashTableException
    Indicate no memory available to allocate oa or table
*/
/******************************************************************************/
template <typename T>
ChHashTable<T>::ChHashTable(const HTConfig& Config, ObjectAllocator* allocator) : htconfig_(Config), oa_(allocator), freeOA_(false), table_(0), stats_()
{
  // Check if client provides oa
  if(oa_ == 0)
  {
    // Create self owned ObjectAllocator if not provided by the client
    OAConfig oaconfig(true);
    
    try
    {
      oa_ = new ObjectAllocator(sizeof(ChHTNode), oaconfig);
    }
    catch(const std::bad_alloc& e)
    {
      throw(HashTableException(HashTableException::E_NO_MEMORY, e.what()));
    }
    // Set to true to allow deallocation in dtor
    freeOA_ = true;
  }
  else
  {
    // Only set when shared oa is used
    stats_.Allocator_ = oa_;
  }
  
  // Needs to update HTStats
  stats_.TableSize_ = htconfig_.InitialTableSize_;
  stats_.HashFunc_ = htconfig_.HashFunc_;
  
  // Allocate array of ChHTHeadNode
  try
  {
    table_ = new ChHTHeadNode[htconfig_.InitialTableSize_];
  }
  catch(const std::bad_alloc& e)
  {
    throw(HashTableException(HashTableException::E_NO_MEMORY, e.what()));
  }
}
/******************************************************************************/
/*!
  \brief
    Dtor for ChHashTable<T> class
*/
/******************************************************************************/
template <typename T>
ChHashTable<T>::~ChHashTable()
{
  // Clear out all the list under each head nodes
  clear();
  // Delete table
  delete [] table_;
  
  // Delete OA if is self owned
  if(freeOA_)
  {
    delete oa_;
  }
}
/******************************************************************************/
/*!
  \brief
    Insert an item to ChHashTable with a given Key, grow the table if necessary
    
  \param Key
    Unique key to determine where to insert to
    
  \param Data
    Data to be stored inside the node
    
  \exception HashTableException
    Indicate no memory available to grow table or duplicate key
*/
/******************************************************************************/
template <typename T>
void ChHashTable<T>::insert(const char *Key, const T& Data)
{
  // Check to see if needs to grow the table first
  if ((stats_.Count_ + 1) / static_cast<double>(stats_.TableSize_) > htconfig_.MaxLoadFactor_)
  {
    // Calculate closest prime for the new table size
    double factor = std::ceil(stats_.TableSize_ * htconfig_.GrowthFactor_);
    unsigned new_size = GetClosestPrime(static_cast<unsigned>(factor));
    // Allocate new table
    ChHTHeadNode* new_table = 0;
    
    try
    {
      new_table = new ChHTHeadNode[new_size];
    }
    catch(const std::bad_alloc& e)
    {
      throw(HashTableException(HashTableException::E_NO_MEMORY, e.what()));
    }
    // Call helper function to reinsert to the new table
    reinsert(new_table, new_size);
    // Delete old table
    delete [] table_;
    // Assign new table
    table_ = new_table;
    // Update stats
    stats_.TableSize_ = new_size;
    ++stats_.Expansions_;
  }
  
  // Call hash function to get slot index
  unsigned index = htconfig_.HashFunc_(Key, stats_.TableSize_);
  // Go through the list in the slot to verify no dublicate key
  ChHTNode* walk = table_[index].Nodes;
  ++stats_.Probes_;
  
  // Walk the list to find any duplicate
  while(walk)
  {
    ++stats_.Probes_;
    if(strcmp(walk->Key, Key) == 0)
    {
      throw (HashTableException(HashTableException::E_DUPLICATE, "Duplicate Key"));
    }
    walk = walk->Next;
  }
  
  // Call make node
  ChHTNode* pnewnode = make_node(Data);
  // Copy key to the node
  strcpy(pnewnode->Key, Key);
  // Make current head the next
  pnewnode->Next = table_[index].Nodes;
  // Make the new node the head
  table_[index].Nodes = pnewnode;
  ++table_[index].Count;
}
/******************************************************************************/
/*!
  \brief
    Remove an item from ChHashTable with a given Key
    
  \param Key
    Unique key to determine where to insert to
    
  \exception HashTableException
    Indicate given key not found
*/
/******************************************************************************/
template <typename T>
void ChHashTable<T>::remove(const char *Key)
{
  // Call hash function to get slot number
  unsigned index = htconfig_.HashFunc_(Key, stats_.TableSize_);
  // Create a node to walk the list
  ChHTNode* walk = table_[index].Nodes;
  ChHTNode* prev = 0;
  
  ++stats_.Probes_;
  // If the head node has the key
  if(walk && strcmp(walk->Key, Key) == 0)
  {
    // Make the next new head
    table_[index].Nodes = walk->Next;
    // Free the node
    free_node(walk);
    --table_[index].Count;
  }
  else
  { // If head is not empty
    if(walk)
    {
      // Move to the next node
      prev = walk;
      walk = walk->Next;
      // Walk the list to find the node that has the given key
      while(walk)
      {
        ++stats_.Probes_;
        // If found key, delete the item
        if(strcmp(walk->Key, Key) == 0)
        {
          prev->Next = walk->Next;
          free_node(walk);
          --table_[index].Count;
          // Exit out the loop
          return;
        }
      
        prev = walk;
        walk = walk->Next;
      }
      // Key is not found after reaching the end of the list
    }

    throw (HashTableException(HashTableException::E_ITEM_NOT_FOUND, "Item Not Found"));
  }
}
/******************************************************************************/
/*!
  \brief
    Find an item from ChHashTable with a given Key
    
  \param Key
    Unique key to determine where to insert to
    
  \exception HashTableException
    Indicate given key not found
*/
/******************************************************************************/
template <typename T>
const T& ChHashTable<T>::find(const char *Key) const
{
  // Call hash function to get slot number
  unsigned index = htconfig_.HashFunc_(Key, stats_.TableSize_);
  // Create a node to walk the list
  ChHTNode* walk = table_[index].Nodes;
  while(walk)
  {
    ++stats_.Probes_;
    if(strcmp(walk->Key, Key) == 0)
    {
      return walk->Data;
    }
    
    walk = walk->Next;
  }
  
  throw (HashTableException(HashTableException::E_ITEM_NOT_FOUND, "Item Not Found"));
}
/******************************************************************************/
/*!
  \brief
    Clear out all the nodes on the table
*/
/******************************************************************************/
template <typename T>
void ChHashTable<T>::clear()
{
  for(unsigned i = 0; i < stats_.TableSize_; ++i)
  {
    ChHTNode* walk = table_[i].Nodes;
    while(walk)
    {
      ChHTNode* temp = walk->Next;
      // Free nodes
      free_node(walk);
      walk = temp;
    }
    // Reset count and head pointer
    table_[i].Nodes = 0;
    table_[i].Count = 0;
  }
}
/******************************************************************************/
/*!
  \brief
    Return stats to client for debugging
    
  \return
    Stats of the hash table
*/
/******************************************************************************/
template <typename T>
HTStats ChHashTable<T>::GetStats() const
{
  return stats_;
}
/******************************************************************************/
/*!
  \brief
    Return table to client for debugging
    
  \return
    Array that contains pointers to each list
*/
/******************************************************************************/
template <typename T>
const typename ChHashTable<T>::ChHTHeadNode* ChHashTable<T>::GetTable() const
{
  return table_;
}
/******************************************************************************/
/*!
  \brief
    Create a node with give data
  
  \param Data
    The data to be stored in the node
    
  \exception HashTableException
    Indicate no memory available from object allocator
  
  \return
    Pointer to the newly created node
*/
/******************************************************************************/
template <typename T>
typename ChHashTable<T>::ChHTNode* ChHashTable<T>::make_node(const T& Data)
{
  ChHTNode* pnode = 0;
  try
  {
    ChHTNode* buf = reinterpret_cast<ChHTNode*>(oa_->Allocate());
    // Placement new to construct the object using ctor onto preallocated memory
    pnode = new (buf) ChHTNode(Data);
  }
  catch (const OAException &e)
  {
    throw(HashTableException(HashTableException::E_NO_MEMORY, e.what()));
  }
  
  ++stats_.Count_;
  
  return pnode;
}
/******************************************************************************/
/*!
  \brief
    Free a node
  
  \param Node
    The node to be freed
*/
/******************************************************************************/
template <typename T>
void ChHashTable<T>::free_node(ChHTNode* Node)
{
  // Check FreeProc_ to see if the data has to be freed using callback function
  if(htconfig_.FreeProc_ != 0)
  {
    htconfig_.FreeProc_(Node->Data);
  }
  
  Node->~ChHTNode();
  oa_->Free(Node);
  --stats_.Count_;
}
/******************************************************************************/
/*!
  \brief
    Reinsert nodes from old tabl to new table
  
  \param NewTable
    New table to store all lists of nodes
    
  \param NewSize
    New table's size
*/
/******************************************************************************/
template <typename T>
void ChHashTable<T>::reinsert(ChHTHeadNode* NewTable, unsigned NewSize)
{
  // Walk each list on old table
  for(unsigned i = 0; i < stats_.TableSize_; ++i)
  {
    ChHTNode* walk = table_[i].Nodes;
    while(walk)
    {
      ChHTNode* temp = walk->Next;
      // Call hash function to get new index
      unsigned new_index = htconfig_.HashFunc_(walk->Key, NewSize);
      // Jump to the new index on the new table and insert
      walk->Next = NewTable[new_index].Nodes;
      NewTable[new_index].Nodes = walk;
      ++NewTable[new_index].Count;
      walk = temp;
    }
  }
}