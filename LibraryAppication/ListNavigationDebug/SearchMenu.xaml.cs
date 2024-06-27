/******************************************************************************/
// file: SearchMenu.xaml.cs
// author: Richard Shen
// date: 07/28/14
// brief: This file contains the implementation of SearchMenu page
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
  /// Interaction logic for SearchMenu.xaml
  /// </summary>
  public partial class SearchMenu : Page
  {
    private Mode _md;
    private bool _searched = false;

    // Receive argument to show which mode is using
    public SearchMenu(Mode md, string message)
    {
      InitializeComponent();
      _md = md;
      ErrorMessageLabel.Content = message;
      SearchModeLabel.Content = "Book Search Mode: " + md;
    }

    private SystemBookCollection SearchBooks(string searchTerm)
    {
      // Check if user made error in input before search
      if ((searchTerm.Length == 0) || (searchTerm.Length !=0 && searchTerm.Replace("\t", "").Replace(" ", "").Length == 0))
      {
        throw new ArgumentException("User Input Error");
      }

      SystemBookCollection tempBookCollection = new SystemBookCollection();
      if (TitleRadioButton.IsChecked == true)
      {
        tempBookCollection = SearchMethods.SearchByTitle(searchTerm);
        _searched = true;
      }
      else if (ISBNRadioButton.IsChecked == true)
      {
        tempBookCollection = SearchMethods.SearchByISBN(searchTerm);
        _searched = true;
      }
      else if (AuthorRadioButton.IsChecked == true)
      {
        tempBookCollection = SearchMethods.SearchByAuthor(searchTerm);
        _searched = true;
      }
      else if (SubjectRadioButton.IsChecked == true)
      {
        tempBookCollection = SearchMethods.SearchBySubject(searchTerm);
        _searched = true;
      }
      else if (AnythingRadioButton.IsChecked == true)
      {
        tempBookCollection = SearchMethods.SearchByAnything(searchTerm);
        _searched = true;
      }
      else
      {
        ErrorMessageLabel.Content = "Tip: Select a search method";
        _searched = false;
      }

      return tempBookCollection;
    }

    private void searchButton_Click(object sender, RoutedEventArgs e)
    {
      // Catch errors from search methods
      try
      {
        // Get tempBookCollection from SearchBooks
        SystemBookCollection tempBookCollection = SearchBooks(SearchTermTextBox.Text);
        // Patron does not display book has 0 copy
        if (_md == Mode.Patron)
        {
          var temp = tempBookCollection.Books().Where(bk => bk.NumberOfCopies != 0);
          ResultListView.DataContext = temp;
          if(_searched)
          {
            ErrorMessageLabel.Content = "Found " + temp.ToList().Count + " items";
          }
        }
        else
        {
          ResultListView.DataContext = tempBookCollection.Books();
          if (_searched)
          {
            ErrorMessageLabel.Content = "Found " + tempBookCollection.Books().Count + " items";
          }
        }
      }
      catch (ArgumentException exce)
      {
        ErrorMessageLabel.Content = "Error: " + exce.Message;
      }
    }

    private void clearButton_Click(object sender, RoutedEventArgs e)
    {
      ResultListView.SelectedItem = null;
      ErrorMessageLabel.Content = null;
      SearchTermTextBox.Text = null;
      ResultListView.DataContext = null;
      TitleRadioButton.IsChecked = false;
      ISBNRadioButton.IsChecked = false;
      AuthorRadioButton.IsChecked = false;
      SubjectRadioButton.IsChecked = false;
      AnythingRadioButton.IsChecked = false;
    }

    private void detailButton_Click(object sender, RoutedEventArgs e)
    {
      NavigationService svc = NavigationService.GetNavigationService(DetailButton);
      if (svc != null)
      {
        if (ResultListView.SelectedItem != null)
        {
          SystemBook sb = ResultListView.SelectedItem as SystemBook;

          svc.Navigate(new BookDetail(sb , _md));
        }
        else
        {
          ErrorMessageLabel.Content = "Tip: Select a book to view detail";
        }
      }
    }

    private void backButton_Click(object sender, RoutedEventArgs e)
    {
      NavigationService svc = NavigationService.GetNavigationService(BackButton);
      if (svc != null)
      {
        if (_md == Mode.Patron)
        {
          svc.Navigate(new MainMenu());
        }
        else
        {
          svc.Navigate(new LibrarianMenu());
        }
      }
    }
  }
}
