/******************************************************************************/
// file: SystemCardholder.cs
// author: Richard Shen
// date: 07/12/14
// brief: This file contains the derivied class SystemCardholder
/******************************************************************************/
namespace LibrarySystemClasses
{
  public class SystemCardholder : SystemPerson
  {
    public SystemCardholder(int id, string firstName, 
                            string lastName, string phone, string libraryCardID)
    {
      PersonID = id;
      FirstName = firstName;
      LastName = lastName;
      Phone = phone;
      LibraryCardID = libraryCardID;
    }

    public string Phone { get; set; }
    public string LibraryCardID { get; set; }

    public bool HasBookOverdue
    {
      get { return UserCheckOutLogs.HasBookOverdue; }
    }
    public SystemCheckOutLogCollection UserCheckOutLogs { get; set; }

    public override string Display
    {
      get { return "[C] - " + ToString(); }
    }
  }
}
