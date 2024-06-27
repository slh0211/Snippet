using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LibraryInsertDebug
{
  class Program
  {
    static void Main(string[] args)
    {

      var newPerson = new Person();
      newPerson.FirstName = "John";
      newPerson.LastName = "Doe";
      newPerson.PersonID = 333;


      var newCardholder = new Cardholder();
      newCardholder.ID = 333;
      newCardholder.Person = newPerson;
      newCardholder.Phone = "2051234567";
      newCardholder.LibraryCardID = "jd1984";

      using (var li = new LibraryInformation2Entities())
      {
        li.People.Add(newPerson);
        li.Cardholders.Add(newCardholder);

        li.SaveChanges();

      }
    }
  }
}
