/******************************************************************************/
// file: SystemCheckOutLog.cs
// author: Richard Shen
// date: 07/18/14
// brief: This file contains the class CheckOutLog
/******************************************************************************/
using System;
namespace LibrarySystemClasses
{
  public class SystemCheckOutLog : IComparable<SystemCheckOutLog>
  {
    private const int OverdueDays = 30;

    public SystemCheckOutLog(int checkOutLogID, int cardholderID, int bookID,
      DateTime checkOutDate, DateTime? checkInDate)
    {
      CheckOutLogID = checkOutLogID;
      CardholderID = cardholderID;
      BookID = bookID;
      CheckOutDate = checkOutDate;
      CheckInDate = checkInDate;
    }

    public int CheckOutLogID { get; set; }
    public int CardholderID { get; set; }
    public int BookID { get; set; }
    public DateTime CheckOutDate { get; set; }
    public DateTime? CheckInDate { get; set; }

    public virtual SystemBook CheckedOutBook { get; set; }
    public virtual SystemCardholder CheckedOutCardholder { get; set; }

    public bool IsOverdue
    {
      get
      {
        if (CheckInDate != null)
        {
          return false;
        }
        else
        {
          return DateTime.Now > CheckOutDate.AddDays(OverdueDays);
        }
      }
    }

    public string Overdue
    {
      get
      {
        if (IsOverdue)
        {
          return "Overdue";
        }
        else
        {
          return "";
        }
      }
    }

    public string CheckOutDateShort
    {
      get { return CheckOutDate.ToShortDateString(); }
    }


    public override string ToString()
    {
      string cits = "Currently Out";

      if (CheckInDate != null)
      {
        cits = CheckInDate.Value.ToShortDateString();
      }
      return CheckedOutBook + " : " + CheckedOutCardholder + " : " + CheckOutDate.ToShortDateString() + " : " + cits;
    }

    public string DisplayOverdue
    {
      get
      {
        string display = "Book ID: " + CheckedOutBook.BookID + "   ISBN: " + CheckedOutBook.ISBN + "   Title: " +
                         CheckedOutBook.Title + "   Check Out Date: " + CheckOutDate.ToShortDateString();

        if (IsOverdue)
        {
          return  display + "   Overdue";
        }
        else
        {
          return display;
        }
      }
    }

    int IComparable<SystemCheckOutLog>.CompareTo(SystemCheckOutLog other)
    {
      int dateDiff = CheckOutDate.CompareTo(other.CheckOutDate);
      if (dateDiff != 0)
      {
        return dateDiff;
      }
      else
      {
        return this.CheckedOutBook.Title.CompareTo(other.CheckedOutBook.Title);
      }
    }
  }
}
