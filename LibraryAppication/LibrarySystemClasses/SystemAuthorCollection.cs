/******************************************************************************/
// file: SystemAuthorCollection.cs
// author: Richard Shen
// date: 07/18/14
// brief: This file contains the class SystemAuthorCollection
/******************************************************************************/
using System;
using System.Collections.Generic;

namespace LibrarySystemClasses
{
  public class SystemAuthorCollection : IEnumerable<SystemAuthor>
  {
    private List<SystemAuthor> _collection = new List<SystemAuthor>();

    public void Sort()
    {
      _collection.Sort();
    }

    public List<SystemAuthor> Authors()
    {
      return _collection;
    }

    public void Add(SystemAuthor newAuthor)
    {
      _collection.Add(newAuthor);
    }

    public void Clear()
    {
      _collection.Clear();
    }

    IEnumerator<SystemAuthor> IEnumerable<SystemAuthor>.GetEnumerator()
    {
      foreach (SystemAuthor systemAuthor in _collection)
      {
        yield return systemAuthor;
      }
    }

    System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
    {
      foreach (SystemAuthor systemAuthor in _collection)
      {
        yield return systemAuthor;
      }
    }
  }
}
