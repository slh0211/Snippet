/******************************************************************************/
// file: SystemPerson.cs
// author: Richard Shen
// date: 07/12/14
// brief: This file contains the abstract base class SystemPerson
/******************************************************************************/
using System;

namespace LibrarySystemClasses
{
  public abstract class SystemPerson : IComparable<SystemPerson>
  {
    public int PersonID { get; set; }
    public string FirstName { get; set; }
    public string LastName { get; set; }

    public override string ToString()
    {
      return LastName + ", " + FirstName;
    }

    public abstract string Display { get; }

    int IComparable<SystemPerson>.CompareTo(SystemPerson other)
    {
      return (LastName + FirstName).CompareTo((other.LastName + other.FirstName));
    }
  }
}