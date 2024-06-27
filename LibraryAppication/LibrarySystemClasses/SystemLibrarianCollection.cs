/******************************************************************************/
// file: SystemLibrarianCollection.cs
// author: Richard Shen
// date: 07/18/14
// brief: This file contains the class SystemLibrarianCollection
/******************************************************************************/
using System;
using System.Collections.Generic;

namespace LibrarySystemClasses
{
  public class SystemLibrarianCollection : IEnumerable<SystemLibrarian>
  {
    private List<SystemLibrarian> _collection = new List<SystemLibrarian>();

    public void Sort()
    {
      _collection.Sort();
    }

    public List<SystemLibrarian> Librarians()
    {
      return _collection;
    }

    public void Add(SystemLibrarian newLibrarian)
    {
      _collection.Add(newLibrarian);
    }

    public void Clear()
    {
      _collection.Clear();
    }

    IEnumerator<SystemLibrarian> IEnumerable<SystemLibrarian>.GetEnumerator()
    {
      foreach (SystemLibrarian systemLibrarian in _collection)
      {
        yield return systemLibrarian;
      }
    }

    System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
    {
      foreach (SystemLibrarian systemLibrarian in _collection)
      {
        yield return systemLibrarian;
      }
    }
  }
}
