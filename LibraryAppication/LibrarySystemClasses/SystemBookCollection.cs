/******************************************************************************/
// file: SystemBookCollection.cs
// author: Richard Shen
// date: 07/18/14
// brief: This file contains the class SystemBookCollection
/******************************************************************************/
using System;
using System.Collections.Generic;

namespace LibrarySystemClasses
{
  public class SystemBookCollection : IEnumerable<SystemBook>
  {
    private List<SystemBook> _collection = new List<SystemBook>();

    public void Sort()
    {
      _collection.Sort();
    }

    public List<SystemBook> Books()
    {
      return _collection;
    }

    public void Add(SystemBook newBook)
    {
      _collection.Add(newBook);
    }

    public void Clear()
    {
      _collection.Clear();
    }

    IEnumerator<SystemBook> IEnumerable<SystemBook>.GetEnumerator()
    {
      foreach (SystemBook systemBook in _collection)
      {
        yield return systemBook;
      }
    }

    System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
    {
      foreach (SystemBook systemBook in _collection)
      {
        yield return systemBook;
      }
    }
  }
}
