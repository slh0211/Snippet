/******************************************************************************/
// file: SystemPersonCollection.cs
// author: Richard Shen
// date: 07/18/14
// brief: This file contains the class SystemPersonCollection
/******************************************************************************/
using System;
using System.Collections.Generic;

namespace LibrarySystemClasses
{
  public class SystemPersonCollection : IEnumerable<SystemPerson>
  {
    private List<SystemPerson> _collection = new List<SystemPerson>();

    public void Sort()
    {
      _collection.Sort();
    }

    public List<SystemPerson> People()
    {
      return _collection;
    }

    public void Add(SystemPerson newPerson)
    {
      _collection.Add(newPerson);
    }

    public void Clear()
    {
      _collection.Clear();
    }

    IEnumerator<SystemPerson> IEnumerable<SystemPerson>.GetEnumerator()
    {
      foreach (SystemPerson systemPerson in _collection)
      {
        yield return systemPerson;
      }
    }

    System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
    {
      throw new NotImplementedException();
    }
  }
}
