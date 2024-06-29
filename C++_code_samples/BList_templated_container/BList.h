/******************************************************************************/
/*!
\file   BList.h
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS280
\par    Assignment 2
\date   01/27/2017
\brief  
    This file contains the interface of class BList<T, Size> and other
    necessary class for the assignment.

  Hours spent on this assignment: 16

  Specific portions that gave you the most trouble:
    The different conditions of insert function took some time to fully debuged.
*/
/******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
#ifndef BLIST_H
#define BLIST_H
////////////////////////////////////////////////////////////////////////////////

#include <string>     // error strings

class BListException : public std::exception
{
  private:  
    int m_ErrCode;
    std::string m_Description;

  public:
    BListException(int ErrCode, const std::string& Description) :
    m_ErrCode(ErrCode), m_Description(Description) {};

    virtual int code(void) const { 
      return m_ErrCode; 
    }

    virtual const char *what(void) const throw() {
      return m_Description.c_str();
    }

    virtual ~BListException() throw() {
    }

    enum BLIST_EXCEPTION {E_NO_MEMORY, E_BAD_INDEX, E_DATA_ERROR};
};

struct BListStats
{

  BListStats() : NodeSize(0), NodeCount(0), ArraySize(0), ItemCount(0)  {};
  BListStats(size_t nsize, int ncount, int asize, int count) : 
     NodeSize(nsize), NodeCount(ncount), ArraySize(asize), ItemCount(count)  {};

  size_t NodeSize; // Size of a node
  int NodeCount;   // Number of nodes in the list
  int ArraySize;   // Max number of items in each node
  int ItemCount;   // Number of items in the entire list
};  
/******************************************************************************/
/*!
  \class BList
  \brief  
    The BList class implements a BList data structure

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
*/
/******************************************************************************/
template <typename T, int Size = 1>
class BList
{
 
  public:
    struct BNode
    {
      BNode *next;
      BNode *prev;
      int count; // number of items currently in the node
      T values[Size];
      BNode() : next(0), prev(0), count(0) {}
    };

    BList();                 // default constructor                        
    BList(const BList &rhs); // copy constructor
    ~BList();                // destructor                         

    BList& operator=(const BList &rhs);

      // arrays will be unsorted
    void push_back(const T& value);
    void push_front(const T& value);

      // arrays will be sorted
    void insert(const T& value);

    void remove(int index);
    void remove_by_value(const T& value); 

    int find(const T& value) const;       // returns index, -1 if not found

    T& operator[](int index);             // for l-values
    const T& operator[](int index) const; // for r-values

    size_t size(void) const;   // total number of items (not nodes)
    void clear(void);          // delete all nodes 

    static size_t nodesize(void); // so the driver knows the size

      // For debugging
    const BNode *GetHead() const;
    BListStats GetStats() const;

  private:
    BNode *head_;           // points to the first node
    BNode *tail_;           // points to the last node

    BListStats BLStats_;    // stores related BListStats
    bool sorted_;
    //Makes an empty BNode
    BNode* MakeBNode(void);
    //Makes an empty BNode and insert a value to the first spot
    BNode* MakeBNode(const T& value);
    //Split a fulled BNode
    BNode* SplitBNode(BNode& bnode);
    //Insert a value to the given index
    void InsertToIndex(const T& value, BNode& bnode, int index);
    //Shift elements to one after
    void ShiftByOneAfter(BNode& bnode, int index);
    //Shift elements to one before
    void ShiftByOneBefore(BNode& bnode, int index);
    //Find where to insert, return the BNode that should be in front of inserting BNode
    BNode* FindWhereToInsert(const T& value) const;
    //Insert element to single list
    void InsertSingle(const T& value);
    //Insert element to BList
    void InsertBList(const T& value);
    //Find index for a given value to be inserted on a given BNode
    int FindIndex(const T& value, const BNode& bnode);
    //Find an existed item's index on a give BNode
    int FindItemBinRec(const T& value, const BNode& bnode, const int low, const int high) const;
    //Find index for a given value to be inserted on a given BNode
    int FindIndexToBeUsedBinRec(const T& value, const BNode& bnode, const int low, const int high) const;
    //Find an existed item's index linearly
    int FindItemLinear(const T& value) const;
    //Find an existed item's index on a sorted BList
    int FindItem(const T& value) const;
    //Remove an item and BNode with given index on a single element list
    void RemoveSingle(int index);
    //Remove an item (and BNode) with given index on a multiple element list
    void RemoveBList(int index);
    //Remove an item and BNode with given value on single element list
    void RemoveSingleValue(const T& value);
    //Remove an item (and BNode) with given value on a unsorted multiple element list
    void RemoveBListValueUnsorted(const T& value);
    //Remove an item (and BNode) with given value on a sorted multiple element list
    void RemoveBListValueSorted(const T& value);
    //Makes a new copy of given BNode and pushes on to the list
    void PushBackBNode(const BNode& bnode);

};

#include "BList.cpp"

#endif // BLIST_H
