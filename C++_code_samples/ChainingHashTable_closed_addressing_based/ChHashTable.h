/******************************************************************************/
/*!
\file   ChHashTable.h
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS280
\par    Assignment 5
\date   04/02/2017
\brief  
    This file contains the interface of class ChHashTable<T> and other
    necessary class for the assignment.

  Hours spent on this assignment: 8

  Specific portions that gave you the most trouble:
    
*/
/******************************************************************************/
//---------------------------------------------------------------------------
#ifndef OAHASHTABLEH
#define OAHASHTABLEH
//---------------------------------------------------------------------------
#include <string> // strcmp, strcpy
#include "ObjectAllocator.h"
#include "support.h" // GetClosestPrime

// client-provided hash function: takes a key and table size,
// returns an index in the table.
typedef unsigned (*HASHFUNC)(const char *, unsigned);

// Max length of our "string" keys
const unsigned MAX_KEYLEN = 10;

class HashTableException
{
  private:  
    int error_code_;
    std::string message_;

  public:
    HashTableException(int ErrCode, const std::string& Message) :
        error_code_(ErrCode), message_(Message) {};

    virtual ~HashTableException() {
    }

    virtual int code() const { 
      return error_code_; 
    }

    virtual const char *what() const {
      return message_.c_str();
    }
    enum HASHTABLE_EXCEPTION {E_ITEM_NOT_FOUND, E_DUPLICATE, E_NO_MEMORY};
};


// HashTable statistical info
struct HTStats
{
  HTStats() : Count_(0), TableSize_(0), Probes_(0), Expansions_(0),
                    HashFunc_(0), Allocator_(0) {};
  unsigned Count_;      // Number of elements in the table
  unsigned TableSize_;  // Size of the table (total slots)
  unsigned Probes_;     // Number of probes performed
  unsigned Expansions_; // Number of times the table grew
  HASHFUNC HashFunc_;   // Pointer to primary hash function
  ObjectAllocator *Allocator_; // The allocator in use (may be 0)
};

/******************************************************************************/
/*!
  \class ChHashTable
  \brief  
    The ChHashTable class implements a chaing hash table data structure

    Operations include:

    - Ctor
    - Dtor
    - insert
    - remove
    - find
    - clear
    - GetStat
    - GetTable
*/
/******************************************************************************/
template <typename T>
class ChHashTable
{
  public:

    typedef void (*FREEPROC)(T); // client-provided free proc (we own the data)

    struct HTConfig
    {
        HTConfig(unsigned InitialTableSize, 
                 HASHFUNC HashFunc, 
                 double MaxLoadFactor = 3.0,
                 double GrowthFactor = 2.0, 
                 FREEPROC FreeProc = 0) :

        InitialTableSize_(InitialTableSize), 
        HashFunc_(HashFunc), 
        MaxLoadFactor_(MaxLoadFactor), 
        GrowthFactor_(GrowthFactor),
        FreeProc_(FreeProc) {}

      unsigned InitialTableSize_;
      HASHFUNC HashFunc_; 
      double MaxLoadFactor_;
      double GrowthFactor_;
      FREEPROC FreeProc_;
    };
      
      // Nodes that will hold the key/data pairs
    struct ChHTNode
    {
      char Key[MAX_KEYLEN]; // Key is a string
      T Data;               // Client data
      ChHTNode *Next;
      ChHTNode(const T& data) : Data(data), Next(0) {}; // constructor
    };

      // Each list has a special head pointer
    struct ChHTHeadNode
    {
      ChHTNode *Nodes;
      ChHTHeadNode() : Nodes(0), Count(0) {};
      int Count; // For testing
    };

      // Ctor
    ChHashTable(const HTConfig& Config, ObjectAllocator* allocator = 0);
    ChHashTable(const ChHashTable& rhs) = delete;
    ChHashTable& operator=(const ChHashTable& rhs) = delete;
      // Dtor
    ~ChHashTable();

      // Insert a key/data pair into table. Throws an exception if the
      // insertion is unsuccessful.
    void insert(const char *Key, const T& Data);

      // Delete an item by key. Throws an exception if the key doesn't exist.
    void remove(const char *Key);

      // Find and return data by key. Throws an exception if the
      // key doesn't exist.  
    const T& find(const char *Key) const;

      // Removes all items from the table (Doesn't deallocate table)
    void clear();

      // Allow the client to peer into the data
    HTStats GetStats() const;
    const ChHTHeadNode *GetTable() const;

  private:

    // Any private members ...
    // HTConfig to store config settings
    HTConfig          htconfig_;
    // Pointer to ObjectAllocator (Self owned or provided)
    ObjectAllocator*  oa_;
    // bool to determine if need to free object allocator
    bool              freeOA_;
    // Array to store heads of lists
    ChHTHeadNode*     table_;
    // A mutable variable  to store stats (in order to modify by method as const (find)
    mutable HTStats   stats_;
    
    // Create a node
    ChHTNode* make_node(const T& Data);
    // Free a node
    void free_node(ChHTNode* Node);
    // Reinsert to new table
    void reinsert(ChHTHeadNode* NewTable, unsigned NewSize);

};

#include "ChHashTable.cpp"

#endif
