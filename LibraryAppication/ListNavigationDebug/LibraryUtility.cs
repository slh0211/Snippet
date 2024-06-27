/******************************************************************************/
// file: LibraryUtility.cs
// author: Richard Shen
// date: 07/28/14
// brief: This file contains the implementation of LibraryUtility static class
/******************************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Documents;
using LibrarySystemClasses;

namespace ListNavigationDebug
{
  public static class LibraryUtility
  {
    // Verify Cardholder exist
    public static bool CardholderExist(string libraryCardID)
    {
      using (LibraryInformation2Entities li = new LibraryInformation2Entities())
      {
        foreach (Cardholder ch in li.Cardholders)
        {
          if (ch.LibraryCardID == libraryCardID)
          {
            return true;
          }
        }
        return false;
      }
    }


    //public static IEnumerable<CheckOutLog> GetCheckOutBookLog(Cardholder chd)
    //{
    //  return chd.CheckOutLogs.Where(col => col.CheckInDate == null);
    //}

    // Convert CheckOutLogs into SystemCheckOutLogCollection
    public static SystemCheckOutLogCollection ConvertCheckOutLogs(IEnumerable<CheckOutLog> tempCols)
    {
      var tempScols = new SystemCheckOutLogCollection();

      foreach (CheckOutLog col in tempCols)
      {
        var tempScol = new SystemCheckOutLog(col.CheckOutLogID, col.CardholderID, col.BookID, col.CheckOutDate,
                                             col.CheckInDate);

        tempScol.CheckedOutBook = new SystemBook(col.Book.BookID, col.Book.ISBN, col.Book.Title, col.Book.AuthorID,
                                      col.Book.NumPages, col.Book.Subject, col.Book.Description,
                                      col.Book.Publisher, col.Book.YearPublished, col.Book.Language, col.Book.NumberOfCopies);

        tempScol.CheckedOutCardholder = new SystemCardholder(col.Cardholder.Person.PersonID, col.Cardholder.Person.FirstName, col.Cardholder.Person.LastName,
                                      col.Cardholder.Phone, col.Cardholder.LibraryCardID);

        tempScols.Add(tempScol);
      }
      tempScols.Sort();
      return tempScols;
    }

    // Convert Cardholders into SystemCardholderCollection
    public static SystemCardholderCollection ConvertCardholders(IEnumerable<Cardholder> tempChs)
    {
      var tempSchs = new SystemCardholderCollection();
      
      foreach (Cardholder ch in tempChs)
      {
        var newCardholder = new SystemCardholder(ch.Person.PersonID, ch.Person.FirstName, ch.Person.LastName,
                                                 ch.Phone, ch.LibraryCardID);

        newCardholder.UserCheckOutLogs = ConvertCheckOutLogs(ch.CheckOutLogs);
        
        tempSchs.Add(newCardholder);
      }

      tempSchs.Sort();
      return tempSchs;
    }

    // Return all cardholders
    public static SystemCardholderCollection GetAllCardholders()
    {
      using (LibraryInformation2Entities li = new LibraryInformation2Entities())
      {
        var tempList = li.Cardholders.Select(ch => ch);
        return ConvertCardholders(tempList);
      }
    }

    // Return all librarians
    public static SystemLibrarianCollection GetAllLibrarians()
    {
      using (LibraryInformation2Entities li = new LibraryInformation2Entities())
      {
        var tempList = li.Librarians.Select(lib => lib);
        return ConvertLibrarians(tempList);
      }
    }

    // Convert Librarians into SystemLibrarianCollection
    private static SystemLibrarianCollection ConvertLibrarians(IEnumerable<Librarian> tempList)
    {
      var tempLibs = new SystemLibrarianCollection();

      foreach (Librarian lib in tempList)
      {
        tempLibs.Add(new SystemLibrarian(lib.ID, lib.Person.FirstName, lib.Person.LastName, lib.Phone, lib.UserID, lib.Password));
      }

      tempLibs.Sort();
      return tempLibs;
    }

    // Convert Authors into SystemAuthorCollection
    public static SystemAuthorCollection ConvertAuthors(IEnumerable<Author> tempList)
    {
      SystemAuthorCollection tempCollection = new SystemAuthorCollection();

      foreach (Author au in tempList)
      {
        var tempAuthor = new SystemAuthor(au.ID, au.Person.FirstName, au.Person.LastName, au.Bio);
        tempAuthor.BooksAuthored = ConvertBooks(au.Books);
        tempCollection.Add(tempAuthor);
      }

      tempCollection.Sort();
      return tempCollection;
    }

    // Return all authors
    public static SystemAuthorCollection GetAllAuthors()
    {
      using (LibraryInformation2Entities li = new LibraryInformation2Entities())
      {
        var tempList = li.Authors.Select(au => au);
        return ConvertAuthors(tempList);
      }
    }

    // Convert Books into SystemBookCollection
    public static SystemBookCollection ConvertBooks(IEnumerable<Book> tempList)
    {
      SystemBookCollection tempCollection = new SystemBookCollection();

      foreach (Book bo in tempList)
      {
        SystemBook newBook = new SystemBook(bo.BookID, bo.ISBN, bo.Title, bo.AuthorID, bo.NumPages, bo.Subject,
          bo.Description,
          bo.Publisher, bo.YearPublished, bo.Language, bo.NumberOfCopies);

        //newBook.NumberOfAvailableCopies = LibraryUtility.NumberOfAvailableCopies(bo);
        newBook.BookCheckOutLogs = LibraryUtility.ConvertCheckOutLogs(bo.CheckOutLogs);
        newBook.Author = new SystemAuthor(bo.Author.ID, bo.Author.Person.FirstName, bo.Author.Person.LastName, bo.Author.Bio);

        tempCollection.Add(newBook);
      }

      tempCollection.Sort();
      return tempCollection;
    }

    // Return all books
    public static SystemBookCollection GetAllBooks()
    {
      using (LibraryInformation2Entities li = new LibraryInformation2Entities())
      {
        var tempList = li.Books.Select(bk => bk);
        return ConvertBooks(tempList);
      }
    }

    // Return all checkoutlogs
    public static SystemCheckOutLogCollection GetAllCheckOutLogs()
    {
      using (LibraryInformation2Entities li = new LibraryInformation2Entities())
      {
        return ConvertCheckOutLogs(li.CheckOutLogs.Select(col => col));
      }
    }

    public static bool VerifyUser(string userID, string password)
    {
      using (LibraryInformation2Entities li = new LibraryInformation2Entities())
      {
        foreach (Librarian lib in li.Librarians)
        {
          if (lib.UserID == userID)
          {
            if (lib.Password == password)
            {
              return true;
            }
          }
        }
        return false;
      }
    }

    public static bool PersonIDExist(int id)
    {
      using (LibraryInformation2Entities li = new LibraryInformation2Entities())
      {
        return li.People.Any(p => p.PersonID == id);
      }
    }

    public static void VerifyPersonIDDuplicate(int id)
    {
      if (PersonIDExist(id))
      {
        throw new ArgumentException("Duplicate PersonID");
      }
    }

    public static bool BookIDExist(int id)
    {
      using (LibraryInformation2Entities li = new LibraryInformation2Entities())
      {
        return li.Books.Any(b => b.BookID == id);
      }
    }

    public static void VerifyBookIDDuplicate(int id)
    {
      if (BookIDExist(id))
      {
        throw new ArgumentException("Duplicate BookID");
      }
    }

    public static bool CheckOutLogIDExist(int id)
    {
      using (LibraryInformation2Entities li = new LibraryInformation2Entities())
      {
        return li.CheckOutLogs.Any(c => c.CheckOutLogID == id);
      }
    }

    public static void VerifyCheckOutLogIDDuplicate(int id)
    {
      if (CheckOutLogIDExist(id))
      {
        throw new ArgumentException("Duplicate CheckOutLogID");
      }
    }
  }
}
