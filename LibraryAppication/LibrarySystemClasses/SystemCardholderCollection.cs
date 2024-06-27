/******************************************************************************/
// file: SystemCardholderCollection.cs
// author: Richard Shen
// date: 07/18/14
// brief: This file contains the class SystemCardholderCollection
/******************************************************************************/
using System;
using System.Collections.Generic;

namespace LibrarySystemClasses
{
  public class SystemCardholderCollection : IEnumerable<SystemCardholder>
  {
    private List<SystemCardholder> _collection = new List<SystemCardholder>();

    public void Sort()
    {
      _collection.Sort();
    }

    public List<SystemCardholder> Cardholders()
    {
      return _collection;
    }

    public void Add(SystemCardholder newCardholder)
    {
      _collection.Add(newCardholder);
    }

    public void Clear()
    {
      _collection.Clear();
    }

    IEnumerator<SystemCardholder> IEnumerable<SystemCardholder>.GetEnumerator()
    {
      foreach (SystemCardholder systemCardholder in _collection)
      {
        yield return systemCardholder;
      }
    }

    System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
    {
      foreach (SystemCardholder systemCardholder in _collection)
      {
        yield return systemCardholder;
      }
    }
  }
}
