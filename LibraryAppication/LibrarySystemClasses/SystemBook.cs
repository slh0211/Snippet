/******************************************************************************/
// file: SystemBook.cs
// author: Richard Shen
// date: 07/18/14
// brief: This file contains the class SystemBook
/******************************************************************************/

using System;
using System.CodeDom;
using System.Linq;

namespace LibrarySystemClasses
{
  public class SystemBook : IComparable<SystemBook>
  {
    public SystemBook(int bookID, string isbn, string title, int authorID, int numPages,
      string subject, string description, string publisher, int yearPublished,
      string language, int numberOfCopies)
    {
      BookID = bookID;
      ISBN = isbn;
      Title = title;
      AuthorID = authorID;
      NumPages = numPages;
      Subject = subject;
      Description = description;
      Publisher = publisher;
      YearPublished = yearPublished;
      Language = language;
      NumberOfCopies = numberOfCopies;
      BookCheckOutLogs = new SystemCheckOutLogCollection();
    }

    public int BookID { get; set; }
    public string ISBN { get; set; }
    public string Title { get; set; }
    public int AuthorID { get; set; }
    public int NumPages { get; set; }
    public string Subject { get; set; }
    public string Description { get; set; }
    public string Publisher { get; set; }
    public int YearPublished { get; set; }
    public string Language { get; set; }
    public int NumberOfCopies { get; set; }

    public int NumberOfAvailableCopies
    {
      get
      {
        int checkedOutCopies = BookCheckOutLogs.CheckOutLogs().Count(col => col.CheckInDate == null);

        return NumberOfCopies - checkedOutCopies;
      }
    }
    public SystemAuthor Author { get; set; }
    public SystemCheckOutLogCollection BookCheckOutLogs { get; set; }

    public bool AvailableForCheckOut
    {
      get
      {
        if (NumberOfAvailableCopies > 0)
        {
          return true;
        }
        else
        {
          return false;
        }
      }
    }

    public string Display
    {
      get { return "Book ID: " + BookID + "   Title: " + Title + "   Author: " + Author.LastName + ", " + Author.FirstName +
        "   Publisher: " + Publisher + "   Year Published: " + YearPublished + "   Available Copies: " + NumberOfAvailableCopies; }
    }

    public string DisplayISBNTitle
    {
      get { return ISBN + ", " + Title; }
    }

    public override string ToString()
    {
      return Title;
    }

    int IComparable<SystemBook>.CompareTo(SystemBook other)
    {
      return (Title + ISBN).CompareTo(other.Title + other.ISBN);
    }
  }
}
