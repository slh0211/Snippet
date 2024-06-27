/******************************************************************************/
// file: BookCheckInOut.xaml.cs
// author: Richard Shen
// date: 08/10/14
// brief: This file contains the implementation of BookCheckInOut page
/******************************************************************************/
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using LibrarySystemClasses;

namespace ListNavigationDebug
{
  /// <summary>
  /// Interaction logic for CheckInOut.xaml
  /// </summary>
  public partial class CheckInOut : Page
  {
    private SystemBookCollection _tempbooks ;
    public CheckInOut()
    {
      InitializeComponent();
      _tempbooks = LibraryUtility.GetAllBooks();
      LoadCardholders();
      LoadBooks();
    }

    private void LoadCardholders()
    {
      CardholderComboBox.DataContext = LibraryUtility.GetAllCardholders();
    }

    private void cardholderComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
    {
      var temp = CardholderComboBox.SelectedItem as SystemCardholder;
      if (temp != null)
      {
        CheckOutLogListView.DataContext = temp.UserCheckOutLogs.CheckOutLogs().Where(bk => bk.CheckInDate == null);
      }
    }

    private void LoadBooks()
    {
      BooksComboBox.DataContext = LibraryUtility.GetAllBooks();
    }

    private void booksComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
    {
      var temp = BooksComboBox.SelectedItem as SystemBook;
      if (temp != null)
      {
        var tempc = new SystemBookCollection();
        tempc.Add(temp);
        BooksListView.DataContext = tempc;
      }
    }

    private void checkInButton_Click(object sender, RoutedEventArgs e)
    {
      if (CheckOutLogListView.SelectedItem != null)
      {
        var tempScol = CheckOutLogListView.SelectedItem as SystemCheckOutLog;

        // Save check in item's index in BooksListView
        int bindex = -1;
        if (tempScol != null)
        {
          bindex = _tempbooks.Books().FindIndex(bk => bk.BookID == tempScol.BookID);
        }

        CheckInBook();
        int index = CardholderComboBox.SelectedIndex;

        // Reload for display
        LoadBooks();
        LoadCardholders();

        // Move to previous selectd item
        CardholderComboBox.SelectedIndex = index;
        // Display for checked in item
        BooksComboBox.SelectedIndex = bindex;
        CheckOutLogListView.SelectedItem = null;
        ErrorMessageLabel.Content = "Item has been Checked In";
      }
      else
      {
        ErrorMessageLabel.Content = "Error: No item selected";
      }
    }

    private void CheckInBook()
    {
      var tempCol = CheckOutLogListView.SelectedItem as SystemCheckOutLog;

      CheckOutLog col;
      using (var li = new LibraryInformation2Entities())
      {
        col = li.CheckOutLogs.FirstOrDefault(c => c.CheckOutLogID == tempCol.CheckOutLogID);
      }

      if (col != null)
      {
        col.CheckInDate = DateTime.Now;
      }

      using (var li = new LibraryInformation2Entities())
      {
        li.Entry(col).State = EntityState.Modified;
        li.SaveChanges();
      }
    }

    private void checkOutButton_Click(object sender, RoutedEventArgs e)
    {
      var sch = CardholderComboBox.SelectedItem as SystemCardholder;
      var sb = BooksComboBox.SelectedItem as SystemBook;
      
      // The logic break if this happen
      if (sch == null || sb == null)
      {
        ErrorMessageLabel.Content = "Error: Select a Cardholder and a Book";
        return;
      }

      // Make sure private objects are set before doing any check
      if (CardholderComboBox.SelectedItem != null && BooksComboBox.SelectedItem != null && sch.HasBookOverdue == false && sb.AvailableForCheckOut == true)
      {
        try
        {
          CheckOutBook();
          ErrorMessageLabel.Content = sb.Title + " is checked out by " + sch.LibraryCardID;
        }
        catch (ArgumentException ae)
        {
          ErrorMessageLabel.Content = ae.Message;
        }
      }
      else if (sch.HasBookOverdue == true)
      {
        ErrorMessageLabel.Content = sch.LibraryCardID + " has book overdue";
      }
      else if (sb.AvailableForCheckOut == false)
      {
        ErrorMessageLabel.Content = sb.Title + " currently is not available";
      }
      else
      {
        ErrorMessageLabel.Content = "Error: Select a Cardholder and a Book";
      }
    }

    private void CheckOutBook()
    {
      var sch = CardholderComboBox.SelectedItem as SystemCardholder;
      var sb = BooksComboBox.SelectedItem as SystemBook;

      // The logic break if this happen
      if (sch == null || sb == null)
      {
        return;
      }

      var newCheckOutLog = new CheckOutLog();
      int newID = Guid.NewGuid().GetHashCode();
      LibraryUtility.VerifyCheckOutLogIDDuplicate(newID);
      newCheckOutLog.CheckOutLogID = newID;
      newCheckOutLog.CardholderID = sch.PersonID;
      newCheckOutLog.BookID = sb.BookID;
      newCheckOutLog.CheckOutDate = DateTime.Now;

      using (LibraryInformation2Entities li = new LibraryInformation2Entities())
      {
        var ch = li.Cardholders.FirstOrDefault(c => c.Person.PersonID == sch.PersonID);
        var bo = li.Books.FirstOrDefault(b => b.BookID == sb.BookID);

        newCheckOutLog.Cardholder = ch;
        newCheckOutLog.Book = bo;

        if (ch != null) ch.CheckOutLogs.Add(newCheckOutLog);
        if (bo != null) bo.CheckOutLogs.Add(newCheckOutLog);
        li.CheckOutLogs.Add(newCheckOutLog);
        li.SaveChanges();
      }

      // Save the index
      int schindex = CardholderComboBox.SelectedIndex;
      int sbindex = BooksComboBox.SelectedIndex;

      LoadBooks();
      LoadCardholders();
      // Select previous selectd items to refresh ListView
      BooksComboBox.SelectedIndex = sbindex;
      CardholderComboBox.SelectedIndex = schindex;
    }
  }
}
