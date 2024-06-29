/******************************************************************************/
/*!
\file   BList.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS280
\par    Assignment 2
\date   01/27/2017
\brief  
    This file contains the implemantation of class BList<T, Size> and other
    necessary class for the assignment.

  Hours spent on this assignment: 16
  
  Operations include:
  - Default ctor
  - Copy ctor
  - Dtor
  - push_back
  - push_front
  - insert
  - remove
  - remove_by_value
  - find
  - [] operator
  - [] operator for const
  - size
  - clear
  - nodesize
  - GetHead
  - GetStats

  Specific portions that gave you the most trouble:
    The different conditions of insert function took some time to fully debuged.
*/
/******************************************************************************/
/******************************************************************************/
/*!
  \brief
    Default ctor for BList class
*/
/******************************************************************************/
template <typename T, int Size>
BList<T, Size>::BList()
{
  //Initialize all members
  BLStats_ = BListStats(nodesize(), 0, Size, 0);
  head_ = NULL;
  tail_ = NULL;
  //The list is sorted when there is nothing on the list
  sorted_ = true;
}
/******************************************************************************/
/*!
  \brief
    Copy ctor for BList class
    
  \param rhs
    The original BList object
*/
/******************************************************************************/
template <typename T, int Size>
BList<T, Size>::BList(const BList &rhs)
{
  //Initialize the stats using the original stats
  BLStats_ = BListStats(nodesize(), rhs.BLStats_.NodeCount, Size, rhs.BLStats_.ItemCount);
  head_ = NULL;
  tail_ = NULL;
  BNode* walk = rhs.head_;
  
  while(walk)
  {
    //Call helper function to populate and maintain same structure as the original list
    PushBackBNode(*walk);
    
    walk = walk->next;
  }
  //Everything is copied as the original so sorted should be maintain
  if(rhs.sorted_ == true)
    sorted_ = true;
  else
    sorted_ = false;
}
/******************************************************************************/
/*!
  \brief
    Dtor for BList class
*/
/******************************************************************************/
template <typename T, int Size>
BList<T, Size>::~BList()
{
  //Call clear function to deallocate all BNodes on the list
  clear();
}
/******************************************************************************/
/*!
  \brief
    Assignment operator for BList class
    
  \param rhs
    The original BList object
    
  \return
    The reference of the current object
*/
/******************************************************************************/
template <typename T, int Size>
BList<T, Size>& BList<T, Size>::operator=(const BList &rhs)
{
  //Check for self assignment
  if (&rhs != this)
  {
    //Deallocate BNode if there is any
    clear();
    
    BLStats_ = BListStats(nodesize(), rhs.BLStats_.NodeCount, Size, rhs.BLStats_.ItemCount);
    BNode* walk = rhs.head_;
  
    while(walk)
    {
      //Call helper function to populate and maintain same structure as the original list
      PushBackBNode(*walk);
    
      walk = walk->next;
    }
    //Everything is copied as the original so sorted should be maintain
    if(rhs.sorted_ == true)
      sorted_ = true;
    else
      sorted_ = false;
  }
  
  return *this;
}
/******************************************************************************/
/*!
  \brief
    This function pushes a value into the front of the BList
    
  \param value
    The value to be pushed
*/
/******************************************************************************/
template <typename T, int Size>
void BList<T, Size>::push_front(const T& value)
{
  //Set flag to false for linear search and insert
  sorted_ = false;
  //If head is empty
  if(!head_)
  {
    //Make a new node and updates head and tail
    BNode* pnewnode = MakeBNode(value);
    head_ = pnewnode;
    tail_ = pnewnode;
  }
  else
  {
    //If head node not full
    if(head_->count < BLStats_.ArraySize)
    {
      //Shift all items to next space
      ShiftByOneAfter(*head_, 0);
      //Put item in front of the array
      head_->values[0] = value;
      //Update count
      ++head_->count;
      ++BLStats_.ItemCount;
    }
    else //If full
    {
      //Make a new node
      BNode* pnewnode = MakeBNode(value);
      //Attach both sides
      pnewnode->next = head_;
      head_->prev = pnewnode;
      //Update head_ to the new BNode
      head_ = pnewnode;
    }
  }
}
/******************************************************************************/
/*!
  \brief
    This function pushes a value into the back of the BList
    
  \param value
    The value to be pushed
*/
/******************************************************************************/
template <typename T, int Size>
void BList<T, Size>::push_back(const T& value)
{
  //Set flag to false for linear search and insert
  sorted_ = false;
  //If head is empty
  if(!head_)
  {
    //Make a new node
    BNode* pnewnode = MakeBNode(value);
    //Put item in front
    head_ = pnewnode;
    tail_ = pnewnode;
  }
  else
  {
    //If tail is not full
    if(tail_->count < BLStats_.ArraySize)
    {
      //Put item to after tail's last item
      tail_->values[tail_->count] = value;
      //Update count
      ++tail_->count;
      ++BLStats_.ItemCount;
    }
    else //If tail is full
    {
      //Make a new node
      BNode* pnewnode = MakeBNode(value);
      //Attach both sides
      tail_->next = pnewnode;
      pnewnode->prev = tail_;
      //Update tail_ to the new BNode
      tail_ = pnewnode;
    }
  }
}
/******************************************************************************/
/*!
  \brief
    This function creates one BNode when the list is empty, or determine which
    insert helper function to be called.
  
  \param value
    The value to be inserted.
*/  
/******************************************************************************/
template <typename T, int Size>
void BList<T, Size>::insert(const T& value)
{
  //Create a single element BNode when the list is empty
  if(!head_)
  {
    //Make a new node
    BNode* pnewnode = MakeBNode(value);
    head_ = pnewnode;
    tail_ = pnewnode;
  }
  else
  { //Call slightly optimized single element list helper function if Size is 1
    if(Size == 1)
    {
      InsertSingle(value);
    }
    else
    {
      InsertBList(value);
    }
  }
}
/******************************************************************************/
/*!
  \brief
    This function determine which remove helper function to be called.
  
  \param index
    The index of the item to be removed.
*/  
/******************************************************************************/
template <typename T, int Size>
void BList<T, Size>::remove(int index)
{
  //Call slightly optimized single element list helper function if Size is 1
  if(Size == 1)
  {
    RemoveSingle(index);
  }
  else
  {
    RemoveBList(index);
  }
}
/******************************************************************************/
/*!
  \brief
    This function determines which remove_by_value function to be called.
  \param value
    The value to be inserted.
*/  
/******************************************************************************/
template <typename T, int Size>
void BList<T, Size>::remove_by_value(const T& value)
{
  if(Size == 1)
  { //Both sorted and unsorted use same method when there is only 1 element in each BNode
    RemoveSingleValue(value);
  }
  else
  {
    //When the BList is sorted, more optimized function is called
    if(sorted_ == false)
    {
      RemoveBListValueUnsorted(value);
    }
    else
    {
      RemoveBListValueSorted(value);
    }
  }
}
/******************************************************************************/
/*!
  \brief
    This function determines which find function to be called.
  
  \param value
    The value to be inserted.
    
  \return
    Returns the index if the item is on the list else -1.
*/
/******************************************************************************/
template <typename T, int Size>
int BList<T, Size>::find(const T& value) const
{
  //If not sort
  if(sorted_ == false)
  {
    //Linear search
    return FindItemLinear(value);
  }
  else
    //Else Linear + Bineary search
  {
    return FindItem(value);
  }
}
/******************************************************************************/
/*!
  \brief
    [] operator for BList.
  
  \param index
    The index on the BList.
    
  \exception BListException
    Indicates the index is not valid
    
  \return
    Returns the ref to the T object on the BList.
*/
/******************************************************************************/
template <typename T, int Size>
T& BList<T, Size>::operator[](int index)
{
  BNode* walk = head_;
  //Check if the index is valid
  if(index < BLStats_.ItemCount && -1 < index)
  { //Loop each BNode
    while(walk)
    {
      //If the index is on the current BNode, break
      if(index < walk->count)
      {
        break;
      }
      //Dcrement each count if not on the right BNode yet
      index -= (walk->count);
      walk = walk->next;
    }
  }
  else
   throw(BListException(BListException::E_BAD_INDEX, "Index not exist"));
 
  return walk->values[index];
}
/******************************************************************************/
/*!
  \brief
    [] operator for BList for const T.
  
  \param index
    The index on the BList.
    
  \exception BListException
    Indicates the index is not valid
    
  \return
    Returns the ref to the const T object on the BList.
*/
/******************************************************************************/
template <typename T, int Size>
const T& BList<T, Size>::operator[](int index) const
{
  BNode* walk = head_;
  //Loop through each BNode if index is valid
  if(index < BLStats_.ItemCount && -1 < index)
  {
    while(walk)
    {
      //If the index is on the current BNode, break
      if(index < walk->count)
      {
        break;
      }
      //Dcrement each count if not on the right BNode yet
      index -= (walk->count);
      walk = walk->next;
    }
  }
  else
   throw(BListException(BListException::E_BAD_INDEX, "Index not exist"));
 
  return walk->values[index];
}
/******************************************************************************/
/*!
  \brief
    This function returns current item counts.
  
  \return
    Returns the total number of T on the BList.
*/
/******************************************************************************/
template <typename T, int Size>
size_t BList<T, Size>::size(void) const
{
  return BLStats_.ItemCount;
}
/******************************************************************************/
/*!
  \brief
    This function deallocates BNodes and set every member to default value.
*/
/******************************************************************************/
template <typename T, int Size>
void BList<T, Size>::clear(void)
{
  BNode* walk = head_;
  
  //Deaalocates each BNode on the list
  while(walk)
  {
    BNode* temp = walk->next;
    delete walk;
    walk = temp;
  }
  
  head_ = NULL;
  tail_ = NULL;
  sorted_ = true;
  BLStats_.NodeCount = 0;
  BLStats_.ItemCount = 0;
}
/******************************************************************************/
/*!
  \brief
    This function returns the size of a BNode.
  
  \return
    Returns the size of a BNode.
*/
/******************************************************************************/
template <typename T, int Size>
size_t BList<T, Size>::nodesize(void)
{
  return sizeof(BNode);
}
/******************************************************************************/
/*!
  \brief
    This function returns the head_ of the BList.
  
  \return
    Returns the head_ of the BList.
*/
/******************************************************************************/
template <typename T, int Size>
const typename BList<T, Size>::BNode* BList<T, Size>::GetHead() const
{
  return head_;
}
/******************************************************************************/
/*!
  \brief
    This function returns BLStats_.
  
  \return
    Returns BLstats_.
*/
/******************************************************************************/
template <typename T, int Size>
BListStats BList<T, Size>::GetStats() const
{
  return BLStats_;
}
/******************************************************************************/
/*!
  \brief
    This functions creates an empty BNode.
    
  \exception BListException
    Indicates if there is no memory available.
    
  \return
    Returns the pointer to the newly created BNode.
*/
/******************************************************************************/
template <typename T, int Size>
typename BList<T, Size>::BNode* BList<T, Size>::MakeBNode(void)
{
  BNode* pnewnode = NULL;
  //Rethorw the exception as BListException
  try
  {
    pnewnode = new BNode;
  }
  catch(const std::bad_alloc& e)
  {
    throw(BListException(BListException::E_NO_MEMORY, e.what()));
  }
  //BNode count should only be incremented inside this function
  ++BLStats_.NodeCount;
  
  return pnewnode;
}
/******************************************************************************/
/*!
  \brief
    This functions calls MakeBNode function and put the value onto the array.
    
  \param value
    The value to be inserted to index 0.
    
  \return
    Returns the pointer to the newly created BNode.
*/
/******************************************************************************/
template <typename T, int Size>
typename BList<T, Size>::BNode* BList<T, Size>::MakeBNode(const T& value)
{
  BNode* pnewnode = MakeBNode();
  
  pnewnode->values[0] = value;
  //Updates count
  ++pnewnode->count;
  ++BLStats_.ItemCount;
  
  return pnewnode;
}
/******************************************************************************/
/*!
  \brief
    This functions splits a fulled BNode and return a pointer to the newly 
    created BNode that contains the second half of the original BNode
    
  \param bnode
    The fulled BNode.
    
  \return
    Returns the pointer to the newly created BNode.
*/
/******************************************************************************/
template <typename T, int Size>
typename BList<T, Size>::BNode* BList<T, Size>::SplitBNode(BNode& bnode)
{
  //Create an empty BNode
  BNode* pnewnode = MakeBNode();
  //Check if there is a BNode after original
  if(bnode.next)
  {
    //Attach the origianl next to the new BNode
    pnewnode->next = bnode.next;
    bnode.next->prev = pnewnode;
  }
  else
  { //Make the new BNode the new tail
    tail_ = pnewnode;
  }
  //Attach the original and the new BNode
  bnode.next = pnewnode;
  pnewnode->prev = &bnode;
  
  //Assign second half of original BNode to new BNode's first half
  for(int i = 0; i < (bnode.count / 2); ++i)
  {
    pnewnode->values[i] = bnode.values[i + (bnode.count/2)];
  }
  //Update both BNode's count
  bnode.count /= 2;
  pnewnode->count = bnode.count;
  
  return pnewnode;
}
/******************************************************************************/
/*!
  \brief
    This function insert a T object to a BNode using a given index.
  
  \param value
    The value to be inserted.
  
  \param bnode
    The BNode to store the value.
    
  \param index
    The index of to be used on the BNode's static member array.
*/
/******************************************************************************/
template <typename T, int Size>
void BList<T,Size>::InsertToIndex(const T& value, BNode& bnode, int index)
{
  //Shift everything after index
  ShiftByOneAfter(bnode, index);
  bnode.values[index] = value;
  //Updates count
  ++bnode.count;
  ++BLStats_.ItemCount;
}
/******************************************************************************/
/*!
  \brief
    This function copies every value on the BNode includes the index and 
    after the index to one after.
    
  \param bnode
    The BNode that contains the array to be shifted.
    
  \param index
    The index and everything after the index will be shifted.
*/
/******************************************************************************/
template <typename T, int Size>
void BList<T, Size>::ShiftByOneAfter(BNode& bnode, int index)
{
  for(int i = bnode.count; i > index; --i)
  {
    bnode.values[i] = bnode.values[i - 1];
  }
}
/******************************************************************************/
/*!
  \brief
    This function assigns every value after the index to one index before.
    
  \param bnode
    The BNode that contains the array to be shifted.
    
  \param index
    The index and everything after the index will be shifted.
*/
/******************************************************************************/
template <typename T, int Size>
void BList<T, Size>::ShiftByOneBefore(BNode& bnode, int index)
{
  for(int i = index; i < bnode.count - 1; ++i)
  {
    bnode.values[i] = bnode.values[i + 1];
  }
}
/******************************************************************************/
/*!
  \brief
    This function does an O(n) linear search to find the position of the BNode
    previous to the inserting BNode.
  
  \param value
    The value to be inserted.
  
  \return 
    Returns a pointer pointing to the previous BNode in single element 
    instantiation, or the BNode to have the value to be inserted.
*/  
/******************************************************************************/
template <typename T, int Size>
typename BList<T, Size>::BNode* BList<T, Size>::FindWhereToInsert(const T& value) const
{
  //Check tail_ first for slight optimzation
  if(tail_ && tail_->values[0] < value)
  {
    return tail_;
  }
  
  BNode* walk = head_;
  
  while(walk)
  {
    //Only compare to one value of each BNode
    if(value < walk->values[0])
    {
      //Once found a BNode's first element larger than the given value, break
      break;
    }
    
    walk = walk->next;
  }
  
  return walk->prev;
}
/******************************************************************************/
/*!
  \brief
    This insert function handles the case when single element list 
    implementation is instantiated.
  
  \param value
    The value to be inserted.
*/  
/******************************************************************************/
template <typename T, int Size>
void BList<T, Size>::InsertSingle(const T& value)
{
  BNode* ppos;
  //Find where to insert
  ppos = FindWhereToInsert(value);
  //Construct a new element
  BNode* pnewnode = MakeBNode(value);
  if(ppos)
  {
    //Check to see if there is a BNode after the pos
    if(ppos->next)
    {
      ppos->next->prev = pnewnode;
      pnewnode->next = ppos->next;
    }
    else
    { //Need to update tail_ if the new BNode is the last
      tail_ = pnewnode;
    }
    
    ppos->next = pnewnode;
    pnewnode->prev = ppos;
  }
  else
  { //Need to update head_ if the new BNode is the new first
    pnewnode->next = head_;
    head_->prev = pnewnode;
    head_ = pnewnode;
  }
}
/******************************************************************************/
/*!
  \brief
    This insert function handles the case when multiple element list 
    implementation is instantiated.
  
  \param value
    The value to be inserted.
*/  
/******************************************************************************/
template <typename T, int Size>
void BList<T, Size>::InsertBList(const T& value)
{
  //Find where to insert
  BNode* ppos = FindWhereToInsert(value);

  if(ppos)
  {
    //Case 1: BNode A has space, value between values[0] to BNode B values[0]
    if(ppos->count < Size)
    {
      //Binary search to find the index on BNode A
      int index = FindIndex(value, *ppos);
      //Insert to index on BNode A
      InsertToIndex(value, *ppos, index);
    }
    //Case 2: BNode A has no space, B has space, value between A values[count-1] and B values[0]
    else if(ppos->count == Size && (ppos->values[ppos->count -1] < value)&& ppos->next != NULL && ppos->next->count < Size)
    {
      //Insert to front on BNode B
      InsertToIndex(value, *(ppos->next), 0);
    }
    //Case 3: BNode A and BNode B have no space or BNode B does not exist
    else
    {
      //Split BNode A to BNode A and BNode A'
      BNode* pnewnode = SplitBNode(*ppos);
      //Compare to BNode A' values[0], less choose BNode A, else BNode A'
      //Binary search to find index on A or A'
      if(value < pnewnode->values[0])
      {
        int index = FindIndex(value, *ppos);
        InsertToIndex(value, *ppos, index);
      }
      else
      {
        int index = FindIndex(value, *pnewnode);
        InsertToIndex(value, *pnewnode, index);
      }
    }
  }
  else //value is less than the first item on the BList
  {
    //Case 1: Null so no place to insert but next one (head_) has space
    //Insert to front of head_
    if(head_->count < Size)
    {
      InsertToIndex(value, *head_, 0);
    }
    //Case 2: Null and head_ has no space.
    else
    {
      //Split head_ to head_ and head_'
      SplitBNode(*head_);
      //Insert to head_
      InsertToIndex(value, *head_, 0);
    }
  }
}
/******************************************************************************/
/*!
  \brief
    This function separates the recursive find index for debugging and code 
    organization purpose.
  
  \param value
    The value to be found.
    
  \param bnode
    The BNode to be searched.
    
  \return
    The index to be used to insert the value or -1 indicating item is already
    on the BNode.
*/  
/******************************************************************************/
template <typename T, int Size>
int BList<T, Size>::FindIndex(const T& value, const BNode& bnode)
{
  return FindIndexToBeUsedBinRec(value, bnode, 0, bnode.count -1);
}
/******************************************************************************/
/*!
  \brief
    This function finds an item's index on the BNode member array 
    using O(log n) binary search.
  
  \param value
    The value to be searched.
  
  \param bnode
    The BNode contains the value.
  
  \param low
    The lower bound of the range.
  
  \param high
    The higher bound of the range.
  
  \return
    Returns the index to of the item, or -1 to inidicate the item is not on the
    member array.
*/  
/******************************************************************************/
template <typename T, int Size>
int BList<T, Size>::FindItemBinRec(const T& value, const BNode& bnode, const int low, const int high) const
{
  if(low > high)
    return -1;
  
  int mid = (low + high) / 2;
  
  if(bnode.values[mid] < value)
    return FindItemBinRec(value, bnode, mid + 1, high);
  else if(value < bnode.values[mid])
    return FindItemBinRec(value, bnode, low, mid - 1);
  else
    return mid;
}
/******************************************************************************/
/*!
  \brief
    This function finds the index of an item to be insert onto the BNode member
    array using O(log n) binary search.
  
  \param value
    The value to be inserted.
  
  \param bnode
    The BNode that to be inserted with the value.
  
  \param low
    The lower bound of the range.
  
  \param high
    The higher bound of the range.
  
  \return
    Returns the index to be used, or -1 to inidicate the item is already on the
    member array.
*/  
/******************************************************************************/
template <typename T, int Size>
int BList<T, Size>::FindIndexToBeUsedBinRec(const T& value, const BNode& bnode, const int low, const int high) const
{
  //When the recursion stops, low (higher) is the index to be returned.
  if(low > high)
    return low;
  
  int mid = (low + high) / 2;
  
  if(bnode.values[mid] < value)
    return FindIndexToBeUsedBinRec(value, bnode, mid + 1, high);
  else if(value < bnode.values[mid])
    return FindIndexToBeUsedBinRec(value, bnode, low, mid - 1);
  else
    return -1;
}
/******************************************************************************/
/*!
  \brief
    This function finds an item on the BList using O(n) linear search.
  
  \param value
    The value to be found.

  \return
    Returns the index of the item or -1 to inidicate the item is not on the
    BList.
*/  
/******************************************************************************/
template <typename T, int Size>
int BList<T, Size>::FindItemLinear(const T& value) const
{
  BNode* walk = head_;
  int indexcount = 0;
  bool found = false;
  //Loop through each BNode
  while(walk)
  {
    //Check each element on the static array
    for(int i = 0; i < walk->count; ++i)
    {
      if(walk->values[i] == value)
      {
        //Set flag to break the outer loop
        found = true;
        //Add the index on the array to the accumlated index
        indexcount += i;
        break;
      }
    }
    
    if(found)
      break;
    //Add item counts and move to next BNode if not found
    indexcount += walk->count;
    walk = walk->next;
  }

  if(found)
    return indexcount;
  else
    return -1;
}
/******************************************************************************/
/*!
  \brief
    This function finds an item on the BList using O(n) linear search to move
    to each BNode and does O(log n) bineary search on the sorted member
    array.
  
  \param value
    The value to be found.

  \return
    Returns the index of the item or -1 to inidicate the item is not on the
    BList.
*/  
/******************************************************************************/
template <typename T, int Size>
int BList<T, Size>::FindItem(const T& value) const
{
  BNode* walk = head_;
  int indexcount = 0;
  bool found = false;
  
  while(walk)
  {
    //Check the first element and the last element before calling helper find
    //operator <= may not be implemented for T
    //Case 1: First element
    if(value == walk->values[0])
    {
      found = true;
      break;
    }//Case 2: Last item
    else if(value == walk->values[walk->count - 1])
    {
      found = true;
      indexcount += walk->count -1;
      break;
    }//Case 3: In between
    else if(walk->values[0] < value && value < walk->values[walk->count -1])
    {
      int result = FindItemBinRec(value, *walk, 0, walk->count - 1);
      
      if(result != -1)
      {
        found = true;
        indexcount += result;
      }
      
      break;
    }//Move to next BNode if not found
    else
    {
      indexcount += walk->count;
      walk = walk-> next;
    }
  }
  
  if(found)
    return indexcount;
  else
    return -1;
}
/******************************************************************************/
/*!
  \brief
    This function removes the item on the single element BList at given index.
  
  \param index
    The logical index of the item.
    
  \exception BListException
    Indicating the index is not valid.
*/  
/******************************************************************************/
template <typename T, int Size>
void BList<T, Size>::RemoveSingle(int index)
{
  BNode* walk = head_;
  //Case 1: head_
  if(index == 0)
  {
    if(head_ == tail_)
    { //Just call clear if there is only one item
      clear();
    }
    else
    {
      walk->next->prev = NULL;
      head_ = walk->next;
      delete walk;
      --BLStats_.NodeCount;
      --BLStats_.ItemCount;
    }
  }
  else if(index == BLStats_.ItemCount - 1)
  {
    //Case 2: tail_
    BNode* temp = tail_->prev;
    temp->next = NULL;
    delete tail_;
    tail_ = temp;
    --BLStats_.NodeCount;
    --BLStats_.ItemCount;
  }
  else if(0 < index && index < BLStats_.ItemCount - 1)
  {
    //Case 3: In between
    while(walk)
    {
      if(index == 0)
      {
        //Attach both sides
        walk->prev->next = walk->next;
        walk->next->prev = walk->prev;
        
        delete walk;
        --BLStats_.NodeCount;
        --BLStats_.ItemCount;
        break;
      }
      //Only one item on each BNode
      --index;
      walk = walk->next;
    }
  }
  else //Case4: Out of bound
    throw(BListException(BListException::E_BAD_INDEX, "Index not exist"));
}
/******************************************************************************/
/*!
  \brief
    This function removes the item on the multeple element BList at given index.
  
  \param index
    The logical index of the item.
    
  \exception BListException
    Indicating the index is not valid.
*/  
/******************************************************************************/
template <typename T, int Size>
void BList<T, Size>::RemoveBList(int index)
{
  //Case 1: If there is only one item on one node
  if(BLStats_.ItemCount == 1 && BLStats_.NodeCount == 1 && index == 0)
  {
    clear();
  }
  //Case 2: 0 <= index < ItemCount -1, valid index
  else if((0 <= index) && (index < BLStats_.ItemCount))
  {
    BNode* walk = head_;
    
    while(walk)
    {
      if(index < walk->count)
      {
        //Case 2a: The last item(values[0]) and count = 1
        if(index == 0 && walk->count == 1)
        {
          //Attach prev and next, delete the node
          if(walk->prev)
            walk->prev->next = walk->next;
          if(walk->next)
            walk->next->prev = walk->prev;
        
          if(walk == head_)
            head_ = walk->next;
          else if(walk == tail_)
            tail_ = walk->prev;
        
          delete walk;
        
          --BLStats_.NodeCount;
          --BLStats_.ItemCount;
          break;
        }
        else
        {
          //Case 2b: Items from values[0] to values[count -1]
          //Not shifting items if the index is the count -1
          if(index != walk->count-1)
            ShiftByOneBefore(*walk, index);
          //Shift all items to one before and decrease the count
          --walk->count;
          --BLStats_.ItemCount;
          break;
        }
      }
      index -= walk->count;
      walk = walk->next;
    }
  }
  else //Case 3: index < 0, index > ItemCount -1, bad index
    throw(BListException(BListException::E_BAD_INDEX, "Index not exist"));
}
/******************************************************************************/
/*!
  \brief
    This function linear searches each 1 element BNode and removes the BNode.
  
  \param value
    The value to be removed.
*/  
/******************************************************************************/
template <typename T, int Size>
void BList<T, Size>::RemoveSingleValue(const T& value)
{
  BNode* walk = head_;
  
  //Case 1: Only one node exists and the value equals
  if(walk && value == walk->values[0] && BLStats_.NodeCount == 1)
  {
    clear();
  }
  //Case 2: If head_ has the value and there are other node
  else if(walk && value == walk->values[0] && BLStats_.NodeCount > 1)
  {
    //Detach next node's prev
    walk->next->prev = NULL;
    head_ = walk->next;
    delete walk;
    
    --BLStats_.NodeCount;
    --BLStats_.ItemCount;
  }
  //Case 3: Loop through to see if the value exists
  else 
  {
    while(walk)
    {
      if(value == walk->values[0])
      {
        walk->prev->next = walk->next;
        
        if(walk->next)
          walk->next->prev = walk->prev;
        else
          tail_ = walk->prev;
        
        delete walk;
        --BLStats_.NodeCount;
        --BLStats_.ItemCount;
        break;
      }
      
      walk = walk->next;
    }
    //Not found if it reaches the end.
  }
}
/******************************************************************************/
/*!
  \brief
    This function linear searches each element in BNode and removes the element
    and delete BNode if it is the last one.
  
  \param value
    The value to be removed.
*/  
/******************************************************************************/
template <typename T, int Size>
void BList<T, Size>::RemoveBListValueUnsorted(const T& value)
{
  //Case 1: Only one element and one node and equals the value
  if(BLStats_.ItemCount == 1 && BLStats_.NodeCount == 1 && value == head_->values[0])
  {
    clear();
  }
  else
  {
    //Case 2: Loop through each BNode
    {
      BNode* walk = head_;
      
      while(walk)
      {
        //Check each element on the member array
        for(int i = 0; i < walk->count; ++i)
        {
          if(value == walk->values[i])
          {
            //Case 2a: Last element on the BNode
            if(i == 0 && walk->count == 1)
            {
              //Attach prev and next, delete the node
              if(walk->prev)
                walk->prev->next = walk->next;
              if(walk->next)
                walk->next->prev = walk->prev;
        
              if(walk == head_)
                head_ = walk->next;
              else if(walk == tail_)
                tail_ = walk->prev;
        
              delete walk;
        
              --BLStats_.NodeCount;
              --BLStats_.ItemCount;
              return;
            }
            else
            {
              //Case 2b: Multiple elemets on the BNode
              if(i != walk->count -1)
                ShiftByOneBefore(*walk, i);
              //Shift all items to one before and decrease the count
              --walk->count;
              --BLStats_.ItemCount;
              return;
            }
          }
        }
        walk = walk->next;
      }
      //Not found if reaches the end
    }
  }
}
/******************************************************************************/
/*!
  \brief
    This function removes the first item on the BList with given value,
    optimized function when BList is sorted.
  
  \param value
    The value to be removed.
*/  
/******************************************************************************/
template <typename T, int Size>
void BList<T, Size>::RemoveBListValueSorted(const T& value)
{
  //Case 1: Only one element and one node and equals the value
  if(BLStats_.ItemCount == 1 && BLStats_.NodeCount == 1 && value == head_->values[0])
  {
    clear();
  }
  else
  {
    //Case 2: Loop through each BNode
    BNode* walk = head_;
    while(walk)
    {
      //Case 2a: values[0]
      if(walk->values[0] == value)
      {
        //Case 2aa: only one item on the BNode
        if(walk->count == 1)
        {
          //Attach prev and next, delete the node
          if(walk->prev)
            walk->prev->next = walk->next;
          if(walk->next)
            walk->next->prev = walk->prev;
        
          if(walk == head_)
            head_ = walk->next;
          else if(walk == tail_)
            tail_ = walk->prev;
        
          delete walk;
        
          --BLStats_.NodeCount;
        }
        else //Case 2ab: multiple items on the BNode
        {
          ShiftByOneBefore(*walk, 0);
          --walk->count;
        }
        
        --BLStats_.ItemCount;
        break;
      }
      //Case 2b: multiple items BNode and value is at the back
      else if(walk->values[walk->count -1] == value)
      {
        --walk->count;
        --BLStats_.ItemCount;
        break;
      }
      //Case 2c:value between values[0] and values[count-1]
      else if(walk->values[0] < value && value < walk->values[walk->count - 1])
      {
        int index = FindItemBinRec(value, *walk, 0, walk->count - 1);
        if(index != -1)
        {
          ShiftByOneBefore(*walk, index);
          --walk->count;
          --BLStats_.ItemCount;
        }
        break;
      }
      walk = walk->next;
    }
  }
}
/******************************************************************************/
/*!
  \brief
    This function copies and creates a new BNode to be pushed onto the BList.
    The purpose of this function is to effciently deep copy and
    recreates same structure as the original rhs from copy ctor or = operator.
  
  \param bnode
    The BNode to be copied.
    
  \attention
    This function must only be used on an empty BList when calling from copy
    ctor or = operator, otherwise it will break the BList logic severely.
*/  
/******************************************************************************/
template <typename T, int Size>
void BList<T, Size>::PushBackBNode(const BNode& bnode)
{
  //Case 1: fist BNode
  if(head_ == NULL)
  {
    head_ = new BNode(bnode);
    tail_ = head_;
    //Set the pointer correctly to NULL (Otherwise it still points at the rhs)
    head_->prev = NULL;
    head_->next = NULL;
  }
  else //Case 2: Every BNode after the first one
  {
    //Attach tail_ and the new BNode
    tail_->next = new BNode(bnode);
    tail_->next->prev = tail_;
    //Set tail_
    tail_ = tail_->next;
    //Point next to NULL (Otherwise it sill points at the rhs)
    tail_->next = NULL;
  }
}
