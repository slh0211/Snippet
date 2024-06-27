/******************************************************************************/
// file: SearchMethods.cs
// author: Richard Shen
// date: 07/28/14
// brief: This file contains the implementation of SearchMethods static class
/******************************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Animation;
using LibrarySystemClasses;

namespace ListNavigationDebug
{
  public static class SearchMethods
  {
    public static SystemBookCollection SearchByTitle(string searchTerm)
    {
      using (LibraryInformation2Entities li = new LibraryInformation2Entities())
      {
        //Get a list of books contain searhterm in title
        var tempList = li.Books.Where(bk => bk.Title.Contains(searchTerm));

        return LibraryUtility.ConvertBooks(tempList);
      }
    }

    public static SystemBookCollection SearchByISBN(string isbn)
    {
      using (LibraryInformation2Entities li = new LibraryInformation2Entities())
      {
        // ISBN should be exact match
        var tempList = li.Books.Where(bk => bk.ISBN == isbn);

        return LibraryUtility.ConvertBooks(tempList);
      }
    }

    public static SystemBookCollection SearchByAuthor(string searchTerm)
    {
      using (LibraryInformation2Entities li = new LibraryInformation2Entities())
      {
        // Either first name or last name contains searchTerm is qualified
        var tempList = li.Books.Where(bk => bk.Author.Person.LastName.Contains(searchTerm) 
                                            || bk.Author.Person.FirstName.Contains(searchTerm));

        return LibraryUtility.ConvertBooks(tempList);
      }
    }

    public static SystemBookCollection SearchBySubject(string searchTerm)
    {
      using (LibraryInformation2Entities li = new LibraryInformation2Entities())
      {
        // Search by subject
        var tempList = li.Books.Where(bk => bk.Subject.Contains(searchTerm));

        return LibraryUtility.ConvertBooks(tempList);
      }
    }

    public static SystemAuthorCollection SearchAuthor(string lastName, string firstName)
    {
      using (LibraryInformation2Entities li = new LibraryInformation2Entities())
      {
        var tempList = li.Authors.Where(au => au.Person.LastName.Contains(lastName)
                                              && au.Person.FirstName.Contains(firstName));

        return LibraryUtility.ConvertAuthors(tempList);
      }
    }

    public static bool ISBNNotExist(string isbn)
    {
      using (LibraryInformation2Entities li = new LibraryInformation2Entities())
      {
        return !(li.Books.Any(bo => bo.ISBN == isbn));
      }
    }

    private static bool SearchKeywords(string keyword, List<string> elements)
    {
      bool found = false;

      foreach (string e in elements)
      {

          //Check if the element string is empty first
          if (e != null && e.ToLower().Contains(keyword.ToLower()))
          {
            found = true;
            //Short circuit to increase efficiency
            break;
          }
      }

      return found;
    }

    // Break a book into multiple strings
    private static List<string> ConvertToStrings(Book bk)
    {
      List<string> tempList = new List<string>();

      tempList.Add(bk.Title);
      tempList.Add(bk.Author.Person.FirstName);
      tempList.Add(bk.Author.Person.LastName);
      tempList.Add(bk.Subject);
      tempList.Add(bk.Description);
      tempList.Add(bk.Publisher);
      tempList.Add(bk.YearPublished.ToString());
      tempList.Add(bk.Language);

      return tempList;
    }

    public static SystemBookCollection SearchByAnything(string searchTerm)
    {
      using (LibraryInformation2Entities li = new LibraryInformation2Entities())
      {
        //Get a list of books contain searhterm in title
        var tempList = li.Books.Select(bk => bk);
        List<Book> newList = new List<Book>();

        foreach (Book bk in tempList)
        {
          if (SearchKeywords(searchTerm, ConvertToStrings(bk)))
          {
            newList.Add(bk);
          }
        }

        return LibraryUtility.ConvertBooks(newList);
      }
    }
  }
}
