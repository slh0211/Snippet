/******************************************************************************/
// file: BookAdd.xaml.cs
// author: Richard Shen
// date: 07/30/14
// brief: This file contains the implementation of BookAdd page
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
  /// Interaction logic for BookAdd.xaml
  /// </summary>
  public partial class BookAdd : Page
  {
    private SystemAuthor _sa;

    public BookAdd(SystemAuthor sa)
    {
      InitializeComponent();

      _sa = sa;
      LoadAuthor();
    }

    private void LoadAuthor()
    {
      AuthorTextBox.Text = _sa.ToString();
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

    private void submitButton_Click(object sender, RoutedEventArgs e)
    {
      if (NoEmptyField() && SearchMethods.ISBNNotExist(ISBNTextBox.Text))
      {
        try
        {
          SystemBook sb = CreateNewBook();
          sb.Author = _sa;
          NavigationService svc = NavigationService.GetNavigationService(SubmitButton);
          if (svc != null)
          {
            svc.Navigate(new BookAddConfirm(sb));
          }
        }
        catch (Exception exce)
        {
          ErrorMessageLabel.Content = exce.Message;
        }
      }
      else if (SearchMethods.ISBNNotExist(ISBNTextBox.Text) == false)
      {
        ErrorMessageLabel.Content = "Error: ISBN already exist";
      }
      else if (NoEmptyField())
      {
        ErrorMessageLabel.Content = "Error: User Input Error";
      }
      else
      {
        ErrorMessageLabel.Content = "Error: User Input Error";
      }
    }

    private SystemBook CreateNewBook()
    {
      int bkID = Guid.NewGuid().GetHashCode();
      LibraryUtility.VerifyBookIDDuplicate(bkID);
      string isbn = ISBNTextBox.Text;
      string title = TitleTextBox.Text;
      int auID = _sa.PersonID;
      int pgs = Int32.Parse(PagesTextBox.Text);
      string subject = SubjectTextBox.Text;
      string description = DescriptionTextBox.Text;
      string publisher = PublisherTextBox.Text;
      int year = Int32.Parse(YearTextBox.Text);
      string languages = LanguagesTextBox.Text;
      int copies = Int32.Parse(TotalTextBox.Text);

      return new SystemBook(bkID, isbn, title, auID, pgs, subject, description, publisher, year, languages, copies);
    }

    private void clearButton_Click(object sender, RoutedEventArgs e)
    {
      ISBNTextBox.Text = null;
      TitleTextBox.Text = null;
      PagesTextBox.Text = null;
      SubjectTextBox.Text = null;
      DescriptionTextBox.Text = null;
      PublisherTextBox.Text = null;
      YearTextBox.Text = null;
      LanguagesTextBox.Text = null;
      TotalTextBox.Text = null;
    }
  }
}
