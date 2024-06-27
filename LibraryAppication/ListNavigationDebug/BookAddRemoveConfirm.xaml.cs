/******************************************************************************/
// file: BookAddRemoveConfirm.xaml.cs
// author: Richard Shen
// date: 07/30/14
// brief: This file contains the implementation of BookAddRemoveConfirm page
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
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using LibrarySystemClasses;

namespace ListNavigationDebug
{
  /// <summary>
  /// Interaction logic for BookAddRemoveConfirm.xaml
  /// </summary>
  public partial class BookAddRemoveConfirm : Page
  {
    private SystemBook _sb;
    private int _copies;

    public BookAddRemoveConfirm(SystemBook sb, int copies)
    {
      InitializeComponent();
      _sb = sb;
      LoadSystemBook(copies);
    }

    private void LoadSystemBook(int copies)
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
      TotalTextBox.Text = (_sb.NumberOfCopies + copies).ToString();
      AvailableTextBox.Text = (_sb.NumberOfAvailableCopies + copies).ToString();
      _copies = copies;
    }

    private void confirmButton_Click(object sender, RoutedEventArgs e)
    {
      UpdateBook(_copies);
      NavigationService svc = NavigationService.GetNavigationService(ConfirmButton);
      if (svc != null)
      {
        svc.Navigate(new ISBNVerify("The total copies of " + _sb.ISBN + " is updated"));
      }
    }

    private void UpdateBook(int cps)
    {
      Book bk;
      using (LibraryInformation2Entities li = new LibraryInformation2Entities())
      {
        bk = li.Books.FirstOrDefault(b => b.BookID == _sb.BookID);
      }

      if (bk != null)
      {
        bk.NumberOfCopies += cps;
      }

      using (LibraryInformation2Entities li = new LibraryInformation2Entities())
      {
        li.Entry(bk).State = EntityState.Modified;
        li.SaveChanges();
      }
    }
  }
}
