/******************************************************************************/
// file: ListsMenu.xaml.cs
// author: Richard Shen
// date: 08/10/14
// brief: This file contains the implementation of ListsMenu page
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

namespace ListNavigationDebug
{
  /// <summary>
  /// Interaction logic for ListsMenu.xaml
  /// </summary>
  public partial class ListsMenu : Page
  {
    public ListsMenu()
    {
      InitializeComponent();
    }

    private void librariansButton_Click(object sender, RoutedEventArgs e)
    {
      NavigationService svc = NavigationService.GetNavigationService(LibrariansButton);
      if (svc != null)
      {
        svc.Navigate(new ListLibrarians());
      }
    }

    private void cardholdersButton_Click(object sender, RoutedEventArgs e)
    {
      NavigationService svc = NavigationService.GetNavigationService(CardholdersButton);
      if (svc != null)
      {
        svc.Navigate(new ListCardholders());
      }
    }

    private void authorsButton_Click(object sender, RoutedEventArgs e)
    {
      NavigationService svc = NavigationService.GetNavigationService(AuthorsButton);
      if (svc != null)
      {
        svc.Navigate(new ListAuthors());
      }
    }

    private void booksOverdueButton_Click(object sender, RoutedEventArgs e)
    {
      NavigationService svc = NavigationService.GetNavigationService(BooksOverdueButton);
      if (svc != null)
      {
        svc.Navigate(new ListOverdues());
      }
    }
  }
}
