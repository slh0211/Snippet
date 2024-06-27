/******************************************************************************/
// file: SystemAuthor.cs
// author: Richard Shen
// date: 07/12/14
// brief: This file contains the derivied class SystemAuthor
/******************************************************************************/
namespace LibrarySystemClasses
{
  public class SystemAuthor : SystemPerson
  {
    public SystemAuthor(int id, string firstName, string lastName, string bio)
    {
      PersonID = id;
      FirstName = firstName;
      LastName = lastName;
      Bio = bio;
      BooksAuthored = new SystemBookCollection();
    }

    public string Bio { get; set; }
    public SystemBookCollection BooksAuthored { get; set; }

    public string DisplayDetail
    {
      get { return "Last Name: " + LastName + "   First Name: " + FirstName + "   Bio: " + Bio; }
    }

    public override string Display
    {
      get { return "[A] - " + ToString(); }
    }
  }
}
