using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using LibrarySystemClasses;
using ListNavigationDebug;
using NUnit.Framework;

namespace SystemUnitTests
{
    [TestFixture]
    public class Tests
    {
      //[Test]
      //void AbstractClassShouldNotBeCreated()
      //{
      //  SystemPerson sp = new SystemPerson();
      //}

      //[Test]
      //public void SystemAuthorShouldBeCreated()
      //{
      //  SystemAuthor sa = new SystemAuthor();
      //  Assert.IsNotNull(sa);
      //}

      [Test]
      public void SystemAuthorPropertyShouldNotBeNull()
      {
        int id = 3;
        string firstName = "J.R.R.";
        string lastName = "Tolkien";
        string bio = "Born 3 January 1892";

        SystemAuthor sa = new SystemAuthor(id, firstName, lastName, bio);

        Assert.IsNotNull(sa.PersonID);
        Assert.IsNotNull(sa.FirstName);
        Assert.IsNotNull(sa.LastName);
        Assert.IsNotNull(sa.Bio);
      }

      [Test]
      public void SystemAuthorToStringFormatShouldBeCorrect()
      {
        int id = 3;
        string firstName = "J.R.R.";
        string lastName = "Tolkien";
        string bio = "Born 3 January 1892";

        SystemAuthor sa = new SystemAuthor(id, firstName, lastName, bio);
        string expected = lastName + ", " + firstName;

        Assert.AreEqual(expected, sa.ToString());
      }

      [Test]
      public void SystemAuthorShouldBeSortedInACollectionContainsSystemPerson()
      {
        int id = 3;
        string firstName = "J.R.R.";
        string lastName = "Tolkien";
        string bio = "Born 3 January 1892";

        SystemAuthor tj = new SystemAuthor(id, firstName, lastName, bio);
        
        id = 4;
        firstName = "Arthur C.";
        lastName = "Clarke";
        bio = "16 December 1917";

        SystemAuthor ca = new SystemAuthor(id, firstName, lastName, bio);

        id = 5;
        firstName = "H. G.";
        lastName = "Wells";
        bio = "21 September 1866";

        SystemAuthor wh = new SystemAuthor(id, firstName, lastName, bio);

        List<SystemPerson> spList = new List<SystemPerson>();

        spList.Add(tj);
        spList.Add(ca);
        spList.Add(wh);

        StringBuilder sbExpected = new StringBuilder();
        StringBuilder sbActual = new StringBuilder();

        sbExpected.Append(ca);
        sbExpected.Append("\n");
        sbExpected.Append(tj);
        sbExpected.Append("\n");
        sbExpected.Append(wh);
        sbExpected.Append("\n");

        spList.Sort();

        foreach (SystemPerson sp in spList)
        {
          sbActual.Append(sp);
          sbActual.Append("\n");
        }

        Assert.AreEqual(sbExpected.ToString(), sbActual.ToString());
      }

      [Test]
      public void SystemAuthorShouldBeSortedInACollectionContainsSystemAuthor()
      {
        int id = 3;
        string firstName = "J.R.R.";
        string lastName = "Tolkien";
        string bio = "Born 3 January 1892";

        SystemAuthor tj = new SystemAuthor(id, firstName, lastName, bio);

        id = 4;
        firstName = "Arthur C.";
        lastName = "Clarke";
        bio = "16 December 1917";

        SystemAuthor ca = new SystemAuthor(id, firstName, lastName, bio);

        id = 5;
        firstName = "H. G.";
        lastName = "Wells";
        bio = "21 September 1866";

        SystemAuthor wh = new SystemAuthor(id, firstName, lastName, bio);

        List<SystemAuthor> spList = new List<SystemAuthor>();

        spList.Add(tj);
        spList.Add(ca);
        spList.Add(wh);

        StringBuilder sbExpected = new StringBuilder();
        StringBuilder sbActual = new StringBuilder();

        sbExpected.Append(ca);
        sbExpected.Append("\n");
        sbExpected.Append(tj);
        sbExpected.Append("\n");
        sbExpected.Append(wh);
        sbExpected.Append("\n");

        spList.Sort();

        foreach (SystemAuthor sa in spList)
        {
          sbActual.Append(sa);
          sbActual.Append("\n");
        }

        Assert.AreEqual(sbExpected.ToString(), sbActual.ToString());
      }

      [Test]
      public void SystemCardholderShouldNotBeNull()
      {
        int id = 6;
        string firstName = "John";
        string lastName = "Doe";
        string phone = "4251111111";

        Guid guid = Guid.NewGuid();
        string libraryCardID = guid.GetHashCode().ToString();

        SystemCardholder sc = new SystemCardholder(id, firstName, lastName, phone, libraryCardID);

        Assert.AreEqual(id, sc.PersonID);
        Assert.AreEqual(firstName, sc.FirstName);
        Assert.AreEqual(lastName, sc.LastName);
        Assert.AreEqual(phone, sc.Phone);
        Assert.AreEqual(libraryCardID, sc.LibraryCardID);
      }

      [Test]
      public void SystemCardholderToStringFormatShouldBeCorrect()
      {
        int id = 6;
        string firstName = "John";
        string lastName = "Doe";
        string phone = "4251111111";

        Guid guid = Guid.NewGuid();
        string libraryCardID = guid.GetHashCode().ToString();

        SystemCardholder sc = new SystemCardholder(id, firstName, lastName, phone, libraryCardID);
        string expected = lastName + ", " + firstName;

        Assert.AreEqual(expected, sc.ToString());
      }

      [Test]
      public void SystemCardholderShouldBeSortedInACollectionContainsSystemPerson()
      {
        int id = 6;
        string firstName = "John";
        string lastName = "Doe";
        string phone = "4251111111";

        string libraryCardID = Guid.NewGuid().GetHashCode().ToString();
        SystemCardholder djo = new SystemCardholder(id, firstName, lastName, phone, libraryCardID);

        id = 7;
        firstName = "Mary";
        lastName = "Sue";
        phone = "4252222222";

        libraryCardID = Guid.NewGuid().GetHashCode().ToString();
        SystemCardholder sm = new SystemCardholder(id, firstName, lastName, phone, libraryCardID);

        id = 8;
        firstName = "Jane";
        lastName = "Doe";
        phone = "4251111111";

        libraryCardID = Guid.NewGuid().GetHashCode().ToString();
        SystemCardholder dja = new SystemCardholder(id, firstName, lastName, phone, libraryCardID);

        List<SystemPerson> spList = new List<SystemPerson>();

        spList.Add(djo);
        spList.Add(sm);
        spList.Add(dja);

        StringBuilder sbExpected = new StringBuilder();
        StringBuilder sbActual = new StringBuilder();

        sbExpected.Append(dja);
        sbExpected.Append("\n");
        sbExpected.Append(djo);
        sbExpected.Append("\n");
        sbExpected.Append(sm);
        sbExpected.Append("\n");

        spList.Sort();

        foreach (SystemPerson sp in spList)
        {
          sbActual.Append(sp);
          sbActual.Append("\n");
        }

        Assert.AreEqual(sbExpected.ToString(), sbActual.ToString());
      }

      [Test]
      public void SystemCardholderShouldBeSortedInACollectionContainsSystemCardHolder()
      {
        int id = 6;
        string firstName = "John";
        string lastName = "Doe";
        string phone = "4251111111";

        string libraryCardID = Guid.NewGuid().GetHashCode().ToString();
        SystemCardholder djo = new SystemCardholder(id, firstName, lastName, phone, libraryCardID);

        id = 7;
        firstName = "Mary";
        lastName = "Sue";
        phone = "4252222222";

        libraryCardID = Guid.NewGuid().GetHashCode().ToString();
        SystemCardholder sm = new SystemCardholder(id, firstName, lastName, phone, libraryCardID);

        id = 8;
        firstName = "Jane";
        lastName = "Doe";
        phone = "4251111111";

        libraryCardID = Guid.NewGuid().GetHashCode().ToString();
        SystemCardholder dja = new SystemCardholder(id, firstName, lastName, phone, libraryCardID);

        List<SystemCardholder> spList = new List<SystemCardholder>();

        spList.Add(djo);
        spList.Add(sm);
        spList.Add(dja);

        StringBuilder sbExpected = new StringBuilder();
        StringBuilder sbActual = new StringBuilder();

        sbExpected.Append(dja);
        sbExpected.Append("\n");
        sbExpected.Append(djo);
        sbExpected.Append("\n");
        sbExpected.Append(sm);
        sbExpected.Append("\n");

        spList.Sort();

        foreach (SystemCardholder sp in spList)
        {
          sbActual.Append(sp);
          sbActual.Append("\n");
        }

        Assert.AreEqual(sbExpected.ToString(), sbActual.ToString());
      }

      [Test]
      public void SystemLibrarianShouldNotBeNull()
      {
        int id = 9;
        string firstName = "Ben";
        string lastName = "Franklin";
        string phone = "4253333333";

        string userID = "BFrank1785";
        string password = "boston1706";

        SystemLibrarian sl = new SystemLibrarian(id, firstName, lastName, phone, userID, password);

        Assert.AreEqual(id, sl.PersonID);
        Assert.AreEqual(firstName, sl.FirstName);
        Assert.AreEqual(lastName, sl.LastName);
        Assert.AreEqual(phone, sl.Phone);
        Assert.AreEqual(userID, sl.UserID);
        Assert.AreEqual(password, sl.Password);
      }

      [Test]
      public void SystemLibrarianToStringFormatShouldBeCorrect()
      {
        int id = 9;
        string firstName = "Ben";
        string lastName = "Franklin";
        string phone = "4253333333";

        string userID = "BFrank1785";
        string password = "boston1706";

        SystemLibrarian sl = new SystemLibrarian(id, firstName, lastName, phone, userID, password);
        string expected = lastName + ", " + firstName;

        Assert.AreEqual(expected, sl.ToString());
      }

      [Test]
      public void SystemLibrarianShouldBeSortedInACollectionContainsSystemPerson()
      {
        int id = 9;
        string firstName = "Ben";
        string lastName = "Franklin";
        string phone = "4253333333";

        string userID = "BFrank1785";
        string password = "boston1706";

        SystemLibrarian fb = new SystemLibrarian(id, firstName, lastName, phone, userID, password);

        id = 10;
        firstName = "Melvil";
        lastName = "Dewey";
        phone = "4254444444";
        userID = "MDewey";
        password = "admascenter1851";

        SystemLibrarian dm = new SystemLibrarian(id, firstName, lastName, phone, userID, password);

        id = 11;
        firstName = "Mohammad";
        lastName = "Khatami";
        phone = "4255555555";
        userID = "MKhatami";
        password = "ardakan1943";

        SystemLibrarian km = new SystemLibrarian(id, firstName, lastName, phone, userID, password);

        List<SystemPerson> spList = new List<SystemPerson>();

        spList.Add(fb);
        spList.Add(dm);
        spList.Add(km);

        StringBuilder sbExpected = new StringBuilder();
        StringBuilder sbActual = new StringBuilder();

        sbExpected.Append(dm);
        sbExpected.Append("\n");
        sbExpected.Append(fb);
        sbExpected.Append("\n");
        sbExpected.Append(km);
        sbExpected.Append("\n");

        spList.Sort();

        foreach (SystemPerson sp in spList)
        {
          sbActual.Append(sp);
          sbActual.Append("\n");
        }

        Assert.AreEqual(sbExpected.ToString(), sbActual.ToString());
      }

      [Test]
      public void SystemLibrarianShouldBeSortedInACollectionContainsSystemLibrarian()
      {
        int id = 9;
        string firstName = "Ben";
        string lastName = "Franklin";
        string phone = "4253333333";

        string userID = "BFrank1785";
        string password = "boston1706";

        SystemLibrarian fb = new SystemLibrarian(id, firstName, lastName, phone, userID, password);

        id = 10;
        firstName = "Melvil";
        lastName = "Dewey";
        phone = "4254444444";
        userID = "MDewey";
        password = "admascenter1851";

        SystemLibrarian dm = new SystemLibrarian(id, firstName, lastName, phone, userID, password);

        id = 11;
        firstName = "Mohammad";
        lastName = "Khatami";
        phone = "4255555555";
        userID = "MKhatami";
        password = "ardakan1943";

        SystemLibrarian km = new SystemLibrarian(id, firstName, lastName, phone, userID, password);

        List<SystemLibrarian> spList = new List<SystemLibrarian>();

        spList.Add(fb);
        spList.Add(dm);
        spList.Add(km);

        StringBuilder sbExpected = new StringBuilder();
        StringBuilder sbActual = new StringBuilder();

        sbExpected.Append(dm);
        sbExpected.Append("\n");
        sbExpected.Append(fb);
        sbExpected.Append("\n");
        sbExpected.Append(km);
        sbExpected.Append("\n");

        spList.Sort();

        foreach (SystemPerson sp in spList)
        {
          sbActual.Append(sp);
          sbActual.Append("\n");
        }

        Assert.AreEqual(sbExpected.ToString(), sbActual.ToString());
      }

      [Test]
      public void CheckInDateCanBeNull()
      {
        int checkOutLogID = 1;
        int cardholderID = Guid.NewGuid().GetHashCode();
        int bookID = Guid.NewGuid().GetHashCode();
        DateTime checkOutDate = DateTime.Now;

        SystemCheckOutLog scol = new SystemCheckOutLog(checkOutLogID, cardholderID, bookID,
          checkOutDate, null);

        Assert.IsNull(scol.CheckInDate);
      }

      [Test]
      public void CheckInDateCanBeAssigned()
      {
        int checkOutLogID = 1;
        int cardholderID = Guid.NewGuid().GetHashCode();
        int bookID = Guid.NewGuid().GetHashCode();
        DateTime checkOutDate = DateTime.Now;
        DateTime checkInDate = DateTime.Now.AddDays(21);

        SystemCheckOutLog scol = new SystemCheckOutLog(checkOutLogID, cardholderID, bookID,
          checkOutDate, checkInDate);

        Assert.IsNotNull(scol.CheckInDate);
      }

      [Test]
      public void PersonCollectionCanBeLoad()
      {
        SystemPersonCollection spc = new SystemPersonCollection();

        int id = 3;
        string firstName = "J.R.R.";
        string lastName = "Tolkien";
        string bio = "Born 3 January 1892";

        SystemAuthor tj = new SystemAuthor(id, firstName, lastName, bio);

        id = 4;
        firstName = "Arthur C.";
        lastName = "Clarke";
        bio = "16 December 1917";

        SystemAuthor ca = new SystemAuthor(id, firstName, lastName, bio);

        id = 5;
        firstName = "H. G.";
        lastName = "Wells";
        bio = "21 September 1866";

        SystemAuthor wh = new SystemAuthor(id, firstName, lastName, bio);

        id = 6;
        firstName = "John";
        lastName = "Doe";
        string phone = "4251111111";

        string libraryCardID = Guid.NewGuid().GetHashCode().ToString();

        SystemCardholder djo = new SystemCardholder(id, firstName, lastName, phone, libraryCardID);

        id = 7;
        firstName = "Mary";
        lastName = "Sue";
        phone = "4252222222";

        libraryCardID = Guid.NewGuid().GetHashCode().ToString();
        SystemCardholder sm = new SystemCardholder(id, firstName, lastName, phone, libraryCardID);

        id = 8;
        firstName = "Jane";
        lastName = "Doe";
        phone = "4251111111";

        libraryCardID = Guid.NewGuid().GetHashCode().ToString();
        SystemCardholder dja = new SystemCardholder(id, firstName, lastName, phone, libraryCardID);

        id = 9;
        firstName = "Ben";
        lastName = "Franklin";
        phone = "4253333333";

        string userID = "BFrank1785";
        string password = "boston1706";

        SystemLibrarian fb = new SystemLibrarian(id, firstName, lastName, phone, userID, password);

        id = 10;
        firstName = "Melvil";
        lastName = "Dewey";
        phone = "4254444444";
        userID = "MDewey";
        password = "admascenter1851";

        SystemLibrarian dm = new SystemLibrarian(id, firstName, lastName, phone, userID, password);

        id = 11;
        firstName = "Mohammad";
        lastName = "Khatami";
        phone = "4255555555";
        userID = "MKhatami";
        password = "ardakan1943";

        SystemLibrarian km = new SystemLibrarian(id, firstName, lastName, phone, userID, password);

        spc.Add(tj);
        spc.Add(ca);
        spc.Add(wh);
        spc.Add(djo);
        spc.Add(sm);
        spc.Add(dja);
        spc.Add(fb);
        spc.Add(dm);
        spc.Add(km);

        spc.Sort();

        StringBuilder sbExpected = new StringBuilder();
        StringBuilder sbActual = new StringBuilder();

        sbExpected.Append(ca);
        sbExpected.Append("\n");
        sbExpected.Append(dm);
        sbExpected.Append("\n");
        sbExpected.Append(dja);
        sbExpected.Append("\n");
        sbExpected.Append(djo);
        sbExpected.Append("\n");
        sbExpected.Append(fb);
        sbExpected.Append("\n");
        sbExpected.Append(km + "\n");
        sbExpected.Append(sm + "\n");
        sbExpected.Append(tj + "\n");
        sbExpected.Append(wh + "\n");

        foreach (SystemPerson sp in spc)
        {
          sbActual.Append(sp);
          sbActual.Append("\n");
        }

        Assert.AreEqual(sbExpected.ToString(), sbActual.ToString());
      }

      [Test]
      public void PersonIDShouldNotBeDuplicate()
      {
        int newID = Guid.NewGuid().GetHashCode();

        LibraryUtility.VerifyPersonIDDuplicate(newID);
      }
    }
}
