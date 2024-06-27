/******************************************************************************/
// file: BookAddConfirm.xaml.cs
// author: Richard Shen
// date: 07/30/14
// brief: This file contains the implementation of BookAddConfirm page
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
  /// Interaction logic for BookAddConfirm.xaml
  /// </summary>
  public partial class BookAddConfirm : Page
  {
    private SystemBook _sb;
    private bool _confirmed = false;

    public BookAddConfirm(SystemBook sb)
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
    }

    private void confirmButton_Click(object sender, RoutedEventArgs e)
    {
      if (_confirmed == false)
      { 
        Book newBook = new Book();
        Author au;

        newBook.BookID = _sb.BookID;
        newBook.ISBN = _sb.ISBN;
        newBook.Title = _sb.Title;
        newBook.AuthorID = _sb.AuthorID;
        newBook.NumPages = _sb.NumPages;
        newBook.Subject = _sb.Subject;
        newBook.Description = _sb.Description;
        newBook.Publisher = _sb.Publisher;
        newBook.YearPublished = _sb.YearPublished;
        newBook.Language = _sb.Language;
        newBook.NumberOfCopies = _sb.NumberOfCopies;

        using (LibraryInformation2Entities li = new LibraryInformation2Entities())
        {
          au = li.Authors.FirstOrDefault(a => a.ID == _sb.AuthorID);
        }

        if (au != null)
        {
          newBook.Author = au;
        }

        using (LibraryInformation2Entities li = new LibraryInformation2Entities())
        {
          li.Entry(au).State = EntityState.Modified;
          li.Books.Add(newBook);
          li.SaveChanges();
        }

        _confirmed = true;

        NavigationService svc = NavigationService.GetNavigationService(ConfirmButton);
        if (svc != null)
        {
          svc.Navigate(new ISBNVerify("The book " + _sb.ISBN + " is added"));
        }
      }
    }
  }
}
