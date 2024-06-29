/******************************************************************************/
/*!
\file   ObjectAllocator.cpp
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS280
\par    Assignment 1
\date   01/12/2017
\brief  
    This file contains the interface of class ObjectAllocator and other
    necessary class for the assignment.

  Hours spent on this assignment: 12

  Specific portions that gave you the most trouble:
    The error checking for the Free function
    Free empty pages took some hours to work
*/
/******************************************************************************/
#include "ObjectAllocator.h" // OjectAllocator, OAException, OAConfig
                             // OAStats, GenericObject
#include <cstring> //strlen, strcpy
/******************************************************************************/
/*!
  \brief
    Non-default ctor for ObjectAllocator class
    
  \param ObjectSize
    The size of each block
    
  \param config
    The configuration struct for the allocator
*/
/******************************************************************************/
ObjectAllocator::ObjectAllocator(size_t ObjectSize, const OAConfig& config)
    : Config_(config), pPageList_(NULL), pFreeList_(NULL)
{ // Through when using default C++ new and delete
  if(Config_.UseCPPMemManager_ == false)
  {
    // Set up object size
    OAStats_.ObjectSize_ = ObjectSize;

    // Calculate alignment if it is needed
    if(Config_.Alignment_ != 0)
    {
      // Left most block in a page
      unsigned leftblock = GetLeftBlockSize();
      // Blocks in between each objects
      unsigned interblock = GetInterBlockSize();
      // Calculate needed alignment for specific bytes
      // Check if the left most block's size can be mod
      if((leftblock % Config_.Alignment_) != 0)
      {
        Config_.LeftAlignSize_ = Config_.Alignment_ - (leftblock % Config_.Alignment_);
      }
      // Check if the inter blocks' size can be mod
      if((interblock % Config_.Alignment_) != 0)
      {
        Config_.InterAlignSize_ = Config_.Alignment_ - (interblock % Config_.Alignment_);
      }
    }
    // Set up page size by calculating how many object and other padded bytes is
    // needed for a page. sizeof(void *) to suit both 32 and 64 bit machines
    OAStats_.PageSize_   = Config_.ObjectsPerPage_ * 
                           (Config_.HBlockInfo_.size_ + Config_.PadBytes_ * 2  + ObjectSize) +
                           sizeof(void*) + Config_.LeftAlignSize_ + 
                           (Config_.ObjectsPerPage_ - 1) * Config_.InterAlignSize_;

    AllocatePage();
  }
  else
  {
    // To match the result of DisableOA
    OAStats_.ObjectSize_ = ObjectSize;
    OAStats_.PageSize_ = ObjectSize * Config_.ObjectsPerPage_ + sizeof(void *);
  }
}
/******************************************************************************/
/*!
  \brief
    Dtor for ObjectAllocator class
*/
/******************************************************************************/
ObjectAllocator::~ObjectAllocator()
{
  // Through when using using C++ default new and delete
  if(Config_.UseCPPMemManager_ == false)
  {
    // Walk through page list to delete each allocated page
    while(pPageList_ != 0)
    {
      // Store temp page for next page before deleting current one
      GenericObject* tempPage = pPageList_->Next;
      // Extra routine when exteranl header used
      if(Config_.HBlockInfo_.type_ == OAConfig::hbExternal)
      {
        unsigned char* pagefront = reinterpret_cast<unsigned char *>(pPageList_);
        unsigned leftblock = GetLeftBlockSize();
        unsigned interblock = GetInterBlockSize();
       
        // Check each object
        for(unsigned i = 0; i < Config_.ObjectsPerPage_;  i++)
        {
          unsigned char* block = pagefront + leftblock + interblock * i;
          // If an object is not on free list it is used and not freed by client
          if(IsOnFreeList(block) == false)
          {
            //Cast the header block to MemBlockInfo pointer
            MemBlockInfo* pmbinfo = reinterpret_cast<MemBlockInfo *>(*reinterpret_cast<MemBlockInfo **>(block - Config_.PadBytes_ - Config_.HBlockInfo_.size_));

            if(pmbinfo)
            {
              //Clean up the label
              if(pmbinfo->label != NULL)
              {
                delete [] pmbinfo->label;
              }
              //Clean up dynamic alloacated MemBlockInfo
              delete pmbinfo;;
            }
          }
        }
      }
      
      delete [] pPageList_ ;

      pPageList_ = tempPage;
    }
  }
}
/******************************************************************************/
/*!
  \brief
    Allocating a page for client

  \return
    void pointer point to a free object
  
  \exception OAException
    Indicates what memory issue
*/
/******************************************************************************/
void* ObjectAllocator::Allocate(const char *label)
{
  // Return a normal dynamic allocated array when using default C++ new
  if(Config_.UseCPPMemManager_ == true)
  {
    ++OAStats_.Allocations_;
    ++OAStats_.MostObjects_;
    unsigned char* object;
    try
    {
      object = new unsigned char[OAStats_.ObjectSize_];
    }
    catch(const std::bad_alloc& e)
    {
      throw OAException(OAException::E_NO_MEMORY, "No memory available");
    }
    return object;
  }
  
  // Check if OAStats_ setting allow client to have more pages (MaxPages_ == 0 gives no limit)
  if(OAStats_.FreeObjects_== 0 && (OAStats_.PagesInUse_ < Config_.MaxPages_ || Config_.MaxPages_ == 0))
  {
    AllocatePage();
  }
  else if(OAStats_.FreeObjects_ == 0 && OAStats_.PagesInUse_ == Config_.MaxPages_ && Config_.MaxPages_ != 0)
  {
    throw OAException(OAException::E_NO_PAGES, "NO MORE PAGES");
  }

  // Create a temp pointer to store current head of free list
  GenericObject *tempPFList = pFreeList_;
  // Move the head of current free list to the next free object
  pFreeList_ = pFreeList_->Next;
  // Updates alloctaion number before assigns it to header block
  ++OAStats_.Allocations_;
  // Set the bit pattern before returning it to client if debug is on
  if(Config_.DebugOn_ == true)
  {
    unsigned char *block = reinterpret_cast<unsigned char *>(tempPFList);
    memset(block, ALLOCATED_PATTERN, OAStats_.ObjectSize_);
    
    switch (Config_.HBlockInfo_.type_)
    {
      case OAConfig::hbNone:
        break;
      case OAConfig::hbBasic:
        //Set the last byte to true as in-use
        memset(block - Config_.PadBytes_ - sizeof(char), true, 1);
        //Cast sizeof(unsigned) sized block and assigns updated allocation number
        *reinterpret_cast<unsigned *>(block - Config_.PadBytes_ - 1 - sizeof(unsigned)) = OAStats_.Allocations_;
        break;
      case OAConfig::hbExtended:
        //Set the last byte to true as in-use
        memset(block - Config_.PadBytes_ - sizeof(char), true, 1);
        //Cast sizeof(unsigned) sized block and assigns updated allocation number
        *reinterpret_cast<unsigned *>(block - Config_.PadBytes_ - sizeof(char) - sizeof(unsigned)) = OAStats_.Allocations_;
        //Cast sizeof(unsigned short) sized block and increment the count
        ++*reinterpret_cast<unsigned short *>(block - Config_.PadBytes_ - sizeof(char) - sizeof(unsigned) - sizeof(unsigned short));
        break;
      case OAConfig::hbExternal:
        //Create a pointer and allocate memoryfor header block
        MemBlockInfo* pmbinfo;
        try
        {
          pmbinfo = new MemBlockInfo;
        }
        catch(const std::bad_alloc& e)
        {
          throw OAException(OAException::E_NO_MEMORY, "No memory available");
        }
        
        //Set in_use flag to true
        pmbinfo->in_use = true;
        //Create a pointer to new label char array
        char* newlabel = NULL;
        //Only allocate memory if label is provided
        if(label != NULL)
        {
          try
          {
            //Create a dynamic char array
            newlabel = new char[strlen(label) + 1];
          }
          catch(const std::bad_alloc& e)
          {
            throw OAException(OAException::E_NO_MEMORY, "No memory available");
          }
          //Deep copy the old array to new array
          strcpy(newlabel, label);
        }
        //Set pointer to newlabel
        pmbinfo->label = newlabel;
        //Assign current allocation number
        pmbinfo->alloc_num = OAStats_.Allocations_;
        //Header block is a pointer, only can modify the block through a pointer, so reinterpret_cast **
        *reinterpret_cast<MemBlockInfo **>(block - Config_.PadBytes_ - Config_.HBlockInfo_.size_) = pmbinfo;

        break;
    }
  }
  
  // Increment counts and return the object to client
  --OAStats_.FreeObjects_;
  ++OAStats_.ObjectsInUse_;
  ++OAStats_.MostObjects_;
  return tempPFList;
}
/******************************************************************************/
/*!
  \brief
    Allocation helper function to be used for both ctor and Allocate (when 
    more page is needed)
    
  \exception OAException
    Indicates what memory issue
*/
/******************************************************************************/
void ObjectAllocator::AllocatePage()
{
  // Allocating a single page (char array)
  unsigned char* page;
  try
  {
    page = new unsigned char[OAStats_.PageSize_];
  }
  catch(const std::bad_alloc& e)
  {
    throw OAException(OAException::E_NO_MEMORY, "No memory available");
  }

  // Calculate the distance between each block
  unsigned leftblock = GetLeftBlockSize();
  unsigned interblock = GetInterBlockSize();

  // Set bit pattern when debug is on
  if(Config_.DebugOn_ == true)
  {
    // Set entire chunk to 0xaa
    memset(page, UNALLOCATED_PATTERN, OAStats_.PageSize_);
    // Set pads (both sides) bytes to 0xdd
    if(Config_.PadBytes_ != 0)
    {
      for(unsigned i = 0; i < Config_.ObjectsPerPage_; ++i)
      {
        memset((page + leftblock + i * interblock - Config_.PadBytes_), PAD_PATTERN, Config_.PadBytes_);
        memset((page + leftblock + OAStats_.ObjectSize_ + i * interblock), PAD_PATTERN, Config_.PadBytes_);
      }
    }
    // Set all types header blocks to 0x00
    if(Config_.HBlockInfo_.size_ != 0)
    {
      for(unsigned i = 0; i < Config_.ObjectsPerPage_; ++i)
      {
        memset((page + leftblock + i * interblock - Config_.PadBytes_ - Config_.HBlockInfo_.size_), 0, Config_.HBlockInfo_.size_);
      }
    }
    // Set the left most alignment block to 0xee
    if(Config_.LeftAlignSize_ != 0)
    {
      memset(page + sizeof(void *), ALIGN_PATTERN, Config_.LeftAlignSize_);
    }
    // Set the rest alignment block to 0xee
    if(Config_.InterAlignSize_ != 0)
    {
      for(unsigned i = 0; i < Config_.ObjectsPerPage_ - 1; ++i)
      {
        memset((page + leftblock + i * interblock +OAStats_.ObjectSize_ +Config_.PadBytes_), ALIGN_PATTERN, Config_.InterAlignSize_);
      }
    }
  }
  // Check if there is a existed page on list
  if(pPageList_ == 0)
  {
    // Set the next to null to be the end of the list
    pPageList_ = reinterpret_cast<GenericObject *>(page);
    pPageList_->Next = 0;
  }
  // If there existed pages
  else if(pPageList_ != 0)
  {
    // Put the new allocated page in front of existed pages
    GenericObject* tempPage = pPageList_;
    pPageList_ = reinterpret_cast<GenericObject *>(page);
    pPageList_->Next = tempPage;
  }
  // Check if free list is empty
  if(pFreeList_ == 0)
  {
    // Put the first block on to the free list and point to null
    pFreeList_ = reinterpret_cast<GenericObject *>(page + leftblock);
    pFreeList_->Next = 0;
    ++OAStats_.FreeObjects_;
  }
  else if(pFreeList_ != 0)
  {
    // Put the first block on to the free list and point to existed free list
    GenericObject* tempFree = pFreeList_;
    pFreeList_ = reinterpret_cast<GenericObject *>(page + leftblock);
    pFreeList_->Next = tempFree;
    ++OAStats_.FreeObjects_;
  }
  // Put rest free blocks on the new allocated page to the free list
  for(unsigned i = 1; i < Config_.ObjectsPerPage_; ++i)
  {
    GenericObject *tempPFList = pFreeList_;
    pFreeList_ = reinterpret_cast<GenericObject *>(page + interblock * i + leftblock);
    pFreeList_->Next = tempPFList;
    ++OAStats_.FreeObjects_;
  }
  // Increment page in use count
  ++OAStats_.PagesInUse_;
}
/******************************************************************************/
/*!
  \brief
    Put the item given by client back on to free list, do error checking
    accordingly when debug is on
    
  \param Object
    The object to be freed.
  
  \exception OAException
    Indicates what memory issue
*/
/******************************************************************************/
void ObjectAllocator::Free(void *Object)
{
  // Just delete the object when using default C++ feature
  if(Config_.UseCPPMemManager_ == true)
  {
    unsigned char* array = reinterpret_cast<unsigned char *>(Object);
    delete [] array;
    ++OAStats_.Deallocations_;
    return;
  }

  // Only do check when debug is on
  if(Config_.DebugOn_ == true)
  {
    // check if the object is on the free list
    if(IsOnFreeList(Object) == true)
      throw OAException(OAException::E_MULTIPLE_FREE, "Free Twice");
    // walk through page list
    GenericObject* tempPageList = pPageList_;
    while(tempPageList)
    { // if found the right page
      if(IsOnCurrentPage(Object, tempPageList) == true)
      { // check the boundary
        if(IsAtRightBoundary(Object, tempPageList) == false)
        {
          throw OAException(OAException::E_BAD_BOUNDARY, "Bad Boundary");
        }
        // Break the loop if found the item and is on the right boundary
        break;
      } // if no more page after this one and the object is neither on the current page
      else if(IsOnCurrentPage(Object, tempPageList) == false && tempPageList->Next == 0)
        throw OAException(OAException::E_BAD_BOUNDARY, "Bad Address");

      tempPageList = tempPageList->Next;
    }

    // Check if pad bytes are overwritten by client when debug is on and pad is used
    int result = ValidPad(Object);
    if(result == UNDER_RUN)
      throw OAException(OAException::E_CORRUPTED_BLOCK, "Block before is corrupted");
    else if(result == OVER_RUN)
      throw OAException(OAException::E_CORRUPTED_BLOCK, "Block after is corrupted");
  }

  // tempFlist to store current head of free list
  GenericObject* tempFlist = pFreeList_;
  // Put the Object given back by client on to the free list
  pFreeList_ = reinterpret_cast<GenericObject *>(Object);
  // Set bytes pattern if debug is on
  if(Config_.DebugOn_ == true)
  {
    unsigned char *block = reinterpret_cast<unsigned char *>(Object);
    memset(block, FREED_PATTERN, OAStats_.ObjectSize_);
    
    switch(Config_.HBlockInfo_.type_)
    {
      case OAConfig::hbNone: //Do nothing
        break;
      case OAConfig::hbBasic: //Set all bytes to 0x00
        //in_use byte
        memset(block - Config_.PadBytes_ - sizeof(char), 0, 1);
        //Allocation number bytes
        memset(block - Config_.PadBytes_ - sizeof(char) - sizeof(unsigned), 0, sizeof(unsigned));
        break;
      case OAConfig::hbExtended: //Only alloc count and flag set to 0x00
        //in_use byte
        memset(block - Config_.PadBytes_ - sizeof(char), 0, 1);
        //Allocation number bytes
        memset(block - Config_.PadBytes_ - sizeof(char) - sizeof(unsigned), 0, sizeof(unsigned));
        break;
      case OAConfig::hbExternal: //Set sizeof(void *) to 0x00
        //Cast twice to use correct pointer type
        MemBlockInfo* pmbinfo = reinterpret_cast<MemBlockInfo *>(*reinterpret_cast<MemBlockInfo **>(block - Config_.PadBytes_ - Config_.HBlockInfo_.size_));
        //Check if not NULL
        if(pmbinfo)
        { //Clean up if label is used
          if(pmbinfo->label != NULL)
          {
            delete [] pmbinfo->label;
          } //Clean up external header block
          delete pmbinfo;
        }
        //External header block pointer
        memset(block - Config_.PadBytes_ - Config_.HBlockInfo_.size_, 0, Config_.HBlockInfo_.size_);
        break;
    }
  }
  
  // Point to the old free list
  pFreeList_->Next = tempFlist;
  // Update the Stats
  ++OAStats_.FreeObjects_;
  ++OAStats_.Deallocations_;
  --OAStats_.ObjectsInUse_;
}
/******************************************************************************/
/*!
  \brief
    Check if the object is on free list, optimized when header is used
  \param Object
    The specfic object to be checked
  \return true or false
    Return true if it is on free list else false
*/  
/******************************************************************************/
bool ObjectAllocator::IsOnFreeList(void *Object) const
{
  // Walk the free list if header is not used
  if(Config_.HBlockInfo_.size_ == 0)
  {
    GenericObject* tempList = pFreeList_;
    // Walk through the entire free list
    while(tempList)
    {
      // If it is already on the free list, return true
      if(tempList == Object)
        return true;
      tempList = tempList->Next;
    }
    // If found nothing return false
    return false;
  }
  else // If basic or extended header is used just check the last byte(first before padding or actual block
  {
    unsigned char* block = reinterpret_cast<unsigned char *>(Object);

    if(Config_.HBlockInfo_.type_ != OAConfig::hbExternal)
    {
      if(*(block - Config_.PadBytes_ - sizeof(char)) == 0x00)
      {
        return true;
      }
      else
      {
        return false;
      }
    }
    else  // external header check the flag through the external header
    { //Cast twice to get correct type
      MemBlockInfo* pmbinfo = reinterpret_cast<MemBlockInfo *>(*reinterpret_cast<MemBlockInfo **>(block - Config_.PadBytes_ - Config_.HBlockInfo_.size_));
      //Make sure pmbinfo points to actual block before checking in_use
      if(pmbinfo && pmbinfo->in_use == true)
        return false;
      else
        return true;
    }
  }
}
/******************************************************************************/
/*!
  \brief
    Check if the object is on given page
  \param Object
    The specfic object to be checked
  \param Page
    The specifc page to be checked
  \return true or false
    Return true if it is on the page else false
*/  
/******************************************************************************/
bool ObjectAllocator::IsOnCurrentPage(void *Object, void *Page) const
{
  // cast both into char * to compare with pointer math
  unsigned char* block     = reinterpret_cast<unsigned char *>(Object);
  unsigned char* pagefront  = reinterpret_cast<unsigned char *>(Page);

  if(block >= pagefront && block < pagefront + OAStats_.PageSize_)
    return true;
  else
    return false;
}
/******************************************************************************/
/*!
  \brief
    Check if the object is on given page's right boundary
  \param Object
    The specfic object to be checked
  \param Page
    The specifc page to be checked
  \return true or false
    Return true if it is on the right boundary else false
*/  
/******************************************************************************/
bool ObjectAllocator::IsAtRightBoundary(void *Object, void *Page) const
{
  // cast to char* to compare
  unsigned char* block     = reinterpret_cast<unsigned char *>(Object);
  unsigned char* pagefront  = reinterpret_cast<unsigned char *>(Page);
  unsigned leftblock = GetLeftBlockSize();
  unsigned interblock = GetInterBlockSize();
  // mod with the size of the interval between blocks (minus the left most block
  if((block - leftblock - pagefront) % (interblock) == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}
/******************************************************************************/
/*!
  \brief
    Check if the object's pad is over written
  \param Object
    The specfic object to be checked
  \return UNDER_RUN, 0, OVER_RUN
    Return UNDER_RUN (-1) if pad before is overwritten, OVER_RUN (1) if pad
    after is overwritten, 0 when pad is not overwritten or debug, pad is not
    used.
*/  
/******************************************************************************/
///////////////////////////////////////////////Need to rewrite without using new
int ObjectAllocator::ValidPad(void *Object) const
{
  if(Config_.DebugOn_ && Config_.PadBytes_)
  {
    //Set up the block
    unsigned char *block = reinterpret_cast<unsigned char *>(Object);
    //Check the front pad
    //Loop thorugh each byte of the front pad
    for(unsigned i = 1; i <= Config_.PadBytes_; i++)
    {
      //If found an overwritten byte (Not PAD_PATTERN)
      if(*(block - i) != PAD_PATTERN)
        return UNDER_RUN;
    }
    //Check the back pad
    for(unsigned i = 0; i < Config_.PadBytes_; i++)
    {
      //If found an overwritten byte (Not PAD_PATTERN)
      if(*(block + OAStats_.ObjectSize_ + i) != PAD_PATTERN)
        return OVER_RUN;
    }
    //Found nothing than return 0
    return 0;
  }
  else  // do nothing when debug is off or pad is not used
    return 0;
}
/******************************************************************************/
/*!
  \brief
    Set debug state
  \param State
    true when on, false when off
*/  
/******************************************************************************/
void ObjectAllocator::SetDebugState(bool State)
{
  Config_.DebugOn_ = State;
}
/******************************************************************************/
/*!
  \brief
    Return the head of free list
  \return pFreeList_
    Pointer to the free list
*/  
/******************************************************************************/
const void *ObjectAllocator::GetFreeList() const
{
  return pFreeList_;
}
/******************************************************************************/
/*!
  \brief
    Return the head of page list
  \return pFreeList_
    Pointer to the page list
*/  
/******************************************************************************/
const void *ObjectAllocator::GetPageList() const
{
  return pPageList_;
}
/******************************************************************************/
/*!
  \brief
    Return the config of ObjectAllocator
  \return Config_
    The Config struct of OA
*/
/******************************************************************************/
OAConfig ObjectAllocator::GetConfig() const
{
  return Config_;
}
/******************************************************************************/
/*!
  \brief
    Return the stats of ObjectAllocator
  \return OAStats_
    The OAStats struct of OA
*/
/******************************************************************************/
OAStats ObjectAllocator::GetStats() const
{
  return OAStats_;
}
/******************************************************************************/
/*!
  \brief
    Free all empty pages (all the object on the page is on free list)
  \return PageCount
    Total pages freed
*/
/******************************************************************************/
unsigned ObjectAllocator::FreeEmptyPages()
{
  // A pointer to walk the list and a pointer to store prev for removing
  GenericObject* temp  = pPageList_;
  GenericObject* prev  = temp;
  unsigned pagecount = 0;

  while(temp)
  {
    unsigned char* pagefront = reinterpret_cast<unsigned char *>(temp);
    // Call helper function to check if all object is on free list
    if(IsAnEmptyPage(pagefront) == true)
    {  // Check if the first item is to be removed
      if(temp == pPageList_)
      {
        // Take all free object of the free list
        RemoveAllFreeObject(pagefront);
        // Update the head and the temp pointer
        pPageList_ = temp->Next;
        temp       = temp->Next;
        // Delete unused page
        delete [] pagefront;
        // Update stats
        ++pagecount;
        --OAStats_.PagesInUse_;
      }
      else
      {
        // Take all free object of the free list
        RemoveAllFreeObject(pagefront);
        // Update the prev and the temp pointer
        prev->Next = temp->Next;
        temp       = temp->Next;
        // Delete unused page
        delete [] pagefront;
        // Update stats
        ++pagecount;
        --OAStats_.PagesInUse_;
      }
    }
    else
    {
      // Move to next page if not an empty page
      prev = temp;
      temp = temp->Next;
    }
  }
  // Return the count
  return pagecount;

}
/******************************************************************************/
/*!
  \brief
    Call RemoveFromFreeList for each object on the page
  \param Page
    The page to be used
*/
/******************************************************************************/
void ObjectAllocator::RemoveAllFreeObject(void *Page)
{
  // Calculate the block to do pointer math
  unsigned leftblock = GetLeftBlockSize();
  unsigned interblock = GetInterBlockSize();
  unsigned char* pagefront = reinterpret_cast<unsigned char *>(Page);

  for(unsigned i = 0; i < Config_.ObjectsPerPage_;  i++)
  {
    RemoveFromFreeList(pagefront + leftblock + interblock * i);
  }
}
/******************************************************************************/
/*!
  \brief
    Remove given object from free list
  \param Object
    The object to be removed
*/
/******************************************************************************/
void ObjectAllocator::RemoveFromFreeList(void *Object)
{
  // Two pointer for walking list and storing prev
  GenericObject* temp  = pFreeList_;
  GenericObject* prev  = temp;
  GenericObject* block = reinterpret_cast<GenericObject *>(Object);
  // Walk the list to see if the object is on the list
  while(temp)
  {
    if(temp == block)
    {
      if(temp == pFreeList_)
      {
        pFreeList_ = pFreeList_->Next;
        // update after each removal
        --OAStats_.FreeObjects_;
        return;
      }
      else
      {
        prev->Next = temp->Next;
        // update after each removal
        --OAStats_.FreeObjects_;
        return;
      }
    }
    else
    {
      prev = temp;
      temp = temp->Next;
    }
  }
}
/******************************************************************************/
/*!
  \brief
    Check if given page is empty (all object are on the free list)
  \param Page
    The page to be checked
  \return true, false
    Return true if the page is empty else false
*/
/******************************************************************************/
bool ObjectAllocator::IsAnEmptyPage(void *Page) const
{
  // Calculate pointer math
  unsigned leftblock = GetLeftBlockSize();
  unsigned interblock = GetInterBlockSize();
  unsigned char* pagefront = reinterpret_cast<unsigned char *>(Page);

  for(unsigned i = 0; i < Config_.ObjectsPerPage_;  i++)
  {
    // return immedately if any object is used
    if(IsOnFreeList((pagefront + leftblock + interblock * i)) == false)
    {
      return false;
    }
  }

  return true;
}
/******************************************************************************/
/*!
  \brief
    Walk through page list and check each object if it is used
  \param fn
    The callback function to dump memory
  \return GetStats().ObjectsInUse_
    Return the number of used object
*/
/******************************************************************************/
unsigned ObjectAllocator::DumpMemoryInUse(DUMPCALLBACK fn) const
{
  // Do nothing when debug is off
  if(Config_.DebugOn_ == false)
    return 0;
  // Calculate pointer math
  unsigned leftblock = GetLeftBlockSize();
  unsigned interblock = GetInterBlockSize();
  GenericObject* pList = pPageList_;
  // Walk the page list to check each object
  while(pList)
  {
    unsigned char* pagefront = reinterpret_cast<unsigned char *>(pList);
    // Check each object
    for(unsigned i = 0; i < Config_.ObjectsPerPage_;  i++)
    {
      // If an object is not on free list it is used
      if(IsOnFreeList((pagefront + leftblock + interblock * i)) == false)
      {
        fn(pagefront + leftblock + interblock * i, OAStats_.ObjectSize_);
      }
    }
    pList = pList->Next;
  }
  return GetStats().ObjectsInUse_;
}
/******************************************************************************/
/*!
  \brief
    Walk through page list and check each object if its pad byte is valid
  \param fn
    The callback function to dump memory
  \return Corrupted
    Return the number of corrupted object
*/
/******************************************************************************/
unsigned ObjectAllocator::ValidatePages(VALIDATECALLBACK fn) const
{
  // do nothing if pad is not used or debug is off
  if(Config_.PadBytes_ == 0 || Config_.DebugOn_ == false)
    return 0;

  unsigned corrupted = 0;
  // Pointer math calculation
  unsigned leftblock = GetLeftBlockSize();
  unsigned interblock = GetInterBlockSize();
  GenericObject* pList = pPageList_;
  // Walk the page list
  while(pList)
  {
    unsigned char* pagefront = reinterpret_cast<unsigned char *>(pList);
    // Call validPad for each object to see if it is corrupted
    for(unsigned i = 0; i < Config_.ObjectsPerPage_;  i++)
    {
      if(ValidPad(pagefront + leftblock + interblock * i))
      {
        fn(pagefront + leftblock + interblock * i, OAStats_.ObjectSize_);
        // increment count if found
        ++corrupted;
      }
    }
    pList = pList->Next;
  }
  return corrupted;
}
/******************************************************************************/
/*!
  \brief
    Calculate and return left most block's size
  \return sizeof(void *) + Config_.LeftAlignSize_ + Config_.HBlockInfo_.size_ + 
    Config_.PadBytes_;
    Return the size of left most block
*/
/******************************************************************************/
unsigned ObjectAllocator::GetLeftBlockSize() const
{
  //return sizeof(void *) + Config_.LeftAlignSize_ + Config_.HBlockInfo_.size_ + Config_.PadBytes_;
  return static_cast<unsigned int>(sizeof(void *) + Config_.LeftAlignSize_ + Config_.HBlockInfo_.size_ + Config_.PadBytes_);
}
/******************************************************************************/
/*!
  \brief
    Calculate and retunr inter block's size
  \return OAStats_.ObjectSize_ + Config_.PadBytes_ * 2 + 
    Config_.HBlockInfo_.size_ + Config_.InterAlignSize_;
    Return the size of inter block
*/
/******************************************************************************/
unsigned ObjectAllocator::GetInterBlockSize() const
{
  return static_cast<unsigned int>(OAStats_.ObjectSize_ + Config_.PadBytes_ * 2 + Config_.HBlockInfo_.size_ + Config_.InterAlignSize_);
}
/******************************************************************************/
/*!
  \brief
    Extra credit function
  \return EXTRA
    Return true if alignment and freeEmptyPages is implmented else false
*/
/******************************************************************************/
bool ObjectAllocator::ImplementedExtraCredit()
{
  return EXTRA;
}