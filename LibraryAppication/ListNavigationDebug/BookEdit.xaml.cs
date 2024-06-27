/******************************************************************************/
// file: BookEditxaml.cs
// author: Richard Shen
// date: 08/01/14
// brief: This file contains the implementation of BookEdit page
/******************************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using LibrarySystemClasses;

namespace ListNavigationDebug
{
  /// <summary>
  /// Interaction logic for BookEdit.xaml
  /// </summary>
  public partial class BookEdit : Page
  {
    private SystemBook _sb;
    private string newISBN;

    public BookEdit(SystemBook sb)
    {
      InitializeComponent();
      _sb = sb;
      newISBN = _sb.ISBN;
      LoadSystemBook();
    }

    private void LoadSystemBook()
    {
      ISBNTextBox.Text = _sb.ISBN;
      TitleTextBox.Text = _sb.Title;
      AuthorTextBox.Text = _sb.Author.ToString();
      PagesTextBox.Text = _sb.NumPages.ToString();
      SubjectTextBox.Text = _sb.Subject;
      DescriptionTextBox.Text = _sb.Description;
      PublisherTextBox.Text = _sb.Publisher;
      YearTextBox.Text = _sb.YearPublished.ToString();
      LanguagesTextBox.Text = _sb.Language;
      TotalTextBox.Text = _sb.NumberOfCopies.ToString();
      AvailableTextBox.Text = _sb.NumberOfAvailableCopies.ToString();
    }

    private void reloadBook_Click(object sender, RoutedEventArgs e)
    {
      LoadSystemBook();
    }

    private void submitButton_Click(object sender, RoutedEventArgs e)
    {
      // Verify newly entered ISBN does not exist in the system
      if (newISBN != _sb.ISBN)
      {
        if (!SearchMethods.ISBNNotExist(newISBN))
        {
          ErrorMessageLabel.Content = "Error: New ISBN already exist";
          return;
        }
      }

      if (!BookAbleToDecrease())
      {
        ErrorMessageLabel.Content = "Error: Not able to remove checked out copies";
        return;
      }

      if (NoEmptyField())
      {
        NavigationService svc = NavigationService.GetNavigationService(SubmitButton);
        if (svc != null)
        {
          try
          {
            SystemBook sb = CreateNewBook();
            sb.Author = _sb.Author;
            sb.BookCheckOutLogs = _sb.BookCheckOutLogs;
            svc.Navigate(new BookEditConfirm(sb));
          }
          catch (Exception exce)
          {
            ErrorMessageLabel.Content = exce.Message;
          }
        }
      }
      else
      {
        ErrorMessageLabel.Content = "Error: User Input Error";
      }
    }

    private bool NoEmptyField()
    {
      if (TitleTextBox.Text.Length != 0 && ISBNTextBox.Text.Length != 0 && PagesTextBox.Text.Length != 0 &&
          SubjectTextBox.Text.Length != 0 && DescriptionTextBox.Text.Length != 0 && PublisherTextBox.Text.Length != 0
          && YearTextBox.Text.Length != 0 && LanguagesTextBox.Text.Length != 0 && TotalTextBox.Text.Length != 0)
      {
        return true;
      }
      else
      {
        return false;
      }
    }

    private SystemBook CreateNewBook()
    {
      int bkID = _sb.BookID;
      
      string isbn = ISBNTextBox.Text;
      string title = TitleTextBox.Text;
      
      int auID = _sb.Author.PersonID;
      
      int pgs = Int32.Parse(PagesTextBox.Text);
      string subject = SubjectTextBox.Text;
      string description = DescriptionTextBox.Text;
      string publisher = PublisherTextBox.Text;
      int year = Int32.Parse(YearTextBox.Text);
      string languages = LanguagesTextBox.Text;
      int copies = Int32.Parse(TotalTextBox.Text);

      return new SystemBook(bkID, isbn, title, auID, pgs, subject, description, publisher, year, languages, copies);
    }


    private void isbn_TextChanged(object sender, TextChangedEventArgs e)
    {
      newISBN = ISBNTextBox.Text;
    }

    private bool BookAbleToDecrease()
    {
      int newNumberOfCopies = Int32.Parse(TotalTextBox.Text);

      if (newNumberOfCopies < (_sb.NumberOfCopies - _sb.NumberOfAvailableCopies))
      {
        return false;
      }
      else
      {
        return true;
      }
    }
  }
}
