/******************************************************************************/
// file: ListAuthors.xaml.cs
// author: Richard Shen
// date: 08/10/14
// brief: This file contains the implementation of ListAuthors page
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
  /// Interaction logic for ListAuthors.xaml
  /// </summary>
  public partial class ListAuthors : Page
  {
    public ListAuthors()
    {
      InitializeComponent();
      LoadAuthors();
    }

    private void LoadAuthors()
    {
      AuthorsListView.DataContext = LibraryUtility.GetAllAuthors();
    }

    private void authorsListView_SelectionChanged(object sender, SelectionChangedEventArgs e)
    {
      if (AuthorsListView.SelectedItem != null)
      {
        var sa = AuthorsListView.SelectedItem as SystemAuthor;
        if (sa != null)
        {
          BooksListView.DataContext = sa.BooksAuthored.Books();
        }
      }
    }
  }
}
