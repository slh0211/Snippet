/******************************************************************************/
// file: ISBNVerify.xaml.cs
// author: Richard Shen
// date: 07/29/14
// brief: This file contains the implementation of ISBNVerify page
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
  /// Interaction logic for ISBNVerify.xaml
  /// </summary>
  public partial class ISBNVerify : Page
  {
    public ISBNVerify(String message)
    {
      InitializeComponent();
      ErrorMessageLabel.Content = message;
      LoadBookToComboBox();
    }

    private void LoadBookToComboBox()
    {
      BooksComboBox.DataContext = LibraryUtility.GetAllBooks();
    }

    private void newBookButton_Click(object sender, RoutedEventArgs e)
    {
      NavigationService svc = NavigationService.GetNavigationService(NewBookButton);
      if (svc != null)
      {
        svc.Navigate(new AuthorVerify());
      }
    }

    private void addRemoveButton_Click(object sender, RoutedEventArgs e)
    {
      if (BooksComboBox.SelectedItem != null)
      {
        NavigationService svc = NavigationService.GetNavigationService(AddRemoveButton);
        if (svc != null)
        {
          SystemBook tempSb = BooksComboBox.SelectedItem as SystemBook;

          svc.Navigate(new BookAddRemove(tempSb));
        }
      }
      else
      {
        ErrorMessageLabel.Content = "Select a book to add or remove copies";
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
