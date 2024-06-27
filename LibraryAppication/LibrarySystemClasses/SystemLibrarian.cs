/******************************************************************************/
// file: SystemLibrarian.cs
// author: Richard Shen
// date: 07/15/14
// brief: This file contains the derivied class SystemLibrarian
/******************************************************************************/

namespace LibrarySystemClasses
{
  public class SystemLibrarian : SystemPerson
  {
        public SystemLibrarian(int id, string firstName, 
                            string lastName, string phone, string userID, string password)
    {
      PersonID = id;
      FirstName = firstName;
      LastName = lastName;
      Phone = phone;
      UserID = userID;
      Password = password;
    }

    public string Phone { get; set; }
    public string UserID { get; set; }
    public string Password { get; set; }

    public override string Display
    {
      get { return "[L] - " + ToString(); }
    }
  }
}
