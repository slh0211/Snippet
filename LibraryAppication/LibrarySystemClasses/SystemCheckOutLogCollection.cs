/******************************************************************************/
// file: SystemCheckOutLogCollection.cs
// author: Richard Shen
// date: 07/18/14
// brief: This file contains the class SystemCheckOutLogCollection
/******************************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;

namespace LibrarySystemClasses
{
  public class SystemCheckOutLogCollection : IEnumerable<SystemCheckOutLog>
  {
    private List<SystemCheckOutLog> _collection = new List<SystemCheckOutLog>();

    public bool HasBookOverdue
    {
      get
      {
        return _collection.Any(scol => scol.IsOverdue);
      }
    }

    public void Sort()
    {
      _collection.Sort();
    }

    public List<SystemCheckOutLog> CheckOutLogs()
    {
      return _collection;
    }

    public void Add(SystemCheckOutLog newCheckOutLog)
    {
      _collection.Add(newCheckOutLog);
    }

    public void Clear()
    {
      _collection.Clear();
    }

    IEnumerator<SystemCheckOutLog> IEnumerable<SystemCheckOutLog>.GetEnumerator()
    {
      foreach (SystemCheckOutLog systemCheckOutLog in _collection)
      {
        yield return systemCheckOutLog;
      }
    }

    System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
    {
      foreach (SystemCheckOutLog systemCheckOutLog in _collection)
      {
        yield return systemCheckOutLog;
      }
    }
  }
}
