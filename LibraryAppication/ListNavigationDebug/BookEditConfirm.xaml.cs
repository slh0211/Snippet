/******************************************************************************/
// file: BookEditConfirm.xaml.cs
// author: Richard Shen
// date: 08/01/14
// brief: This file contains the implementation of BookEditConfirm page
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
  /// Interaction logic for BookEditConfirm.xaml
  /// </summary>
  public partial class BookEditConfirm : Page
  {
    private SystemBook _sb;
    private bool _confirmed = false;

    public BookEditConfirm(SystemBook sb)
    {
      InitializeComponent();
      _sb = sb;
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

    private void confirmButton_Click(object sender, RoutedEventArgs e)
    {
      if (_confirmed == false)
      {
        Book bk;

        using (LibraryInformation2Entities li = new LibraryInformation2Entities())
        {
          bk = li.Books.FirstOrDefault(b => b.BookID == _sb.BookID);
        }

        if (bk != null)
        {
          bk.BookID = _sb.BookID;
          bk.Title = _sb.Title;
          bk.ISBN = _sb.ISBN;
          bk.Publisher = _sb.Publisher;
          bk.NumPages = _sb.NumPages;
          bk.Subject = _sb.Subject;
          bk.NumberOfCopies = _sb.NumberOfCopies;
          bk.Description = _sb.Description;
          bk.YearPublished = _sb.YearPublished;
        }

        using (LibraryInformation2Entities li = new LibraryInformation2Entities())
        {
          li.Entry(bk).State = EntityState.Modified;
          li.SaveChanges();
        }

        _confirmed = true;

        NavigationService svc = NavigationService.GetNavigationService(ConfirmButton);
        if (svc != null)
        {
          svc.Navigate(new SearchMenu(Mode.Update, "ISBN: " + _sb.ISBN + " is updated"));
        }
      }
      else
      {
        ErrorMessageLabel.Content = "Error: already updated";
      }
    }
  }
}
