/******************************************************************************/
// file: AuthorVerify.xaml.cs
// author: Richard Shen
// date: 07/30/14
// brief: This file contains the implementation of AuthorVerify page
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
  /// Interaction logic for AuthorVerify.xaml
  /// </summary>
  public partial class AuthorVerify : Page
  {
    public AuthorVerify()
    {
      InitializeComponent();
      ErrorMessageLabel.Content = "Select an author to add a new book";
      LoadAuthor();
    }

    private void LoadAuthor()
    {
      AuthorsComboBox.DataContext = LibraryUtility.GetAllAuthors();
    }

    private void addAuthorButton_Click(object sender, RoutedEventArgs e)
    {
      NavigationService svc = NavigationService.GetNavigationService(AddAuthorButton);
      if (svc != null)
      {
        svc.Navigate(new AuthorAdd());
      }
    }

    private void addBookButon_Click(object sender, RoutedEventArgs e)
    {
      if (AuthorsComboBox.SelectedItem != null)
      {
        SystemAuthor sa = AuthorsComboBox.SelectedItem as SystemAuthor;

        NavigationService svc = NavigationService.GetNavigationService(AddBookButton);
        if (svc != null)
        {
          svc.Navigate(new BookAdd(sa));
        }
      }
      else
      {
        ErrorMessageLabel.Content = "Error: No author selected";
      }
    }

    private void isbnVerifyButton_Click(object sender, RoutedEventArgs e)
    {
      NavigationService svc = NavigationService.GetNavigationService(ISBNVerifyButton);
      if (svc != null)
      {
        svc.Navigate(new ISBNVerify("Select a book to add/remove"));
      }
    }

    private void librarianMenuButton_Click(object sender, RoutedEventArgs e)
    {
      NavigationService svc = NavigationService.GetNavigationService(LibrarianMenuButton);
      if (svc != null)
      {
        svc.Navigate(new LibrarianMenu());
      }
    }
  }
}
