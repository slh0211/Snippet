/******************************************************************************/
// file: LibrarianMenu.xaml.cs
// author: Richard Shen
// date: 07/27/14
// brief: This file contains the implementation of LibrarianMenu page
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
  /// Interaction logic for LibrarianMenu.xaml
  /// </summary>
  public partial class LibrarianMenu : Page
  {
    public LibrarianMenu()
    {
      InitializeComponent();
    }

    private void displayButton_Click(object sender, RoutedEventArgs e)
    {
      NavigationService svc = NavigationService.GetNavigationService(DisplayButton);
      if (svc != null)
      {
        svc.Navigate(new ListsMenu());
      }
    }

    private void searchModeButton_Click(object sender, RoutedEventArgs e)
    {
      NavigationService svc = NavigationService.GetNavigationService(SearchModeButton);
      if (svc != null)
      {
        svc.Navigate(new SearchMenu(Mode.Librarian, ""));
      }
    }

    private void checkOutButton_Click(object sender, RoutedEventArgs e)
    {
      NavigationService svc = NavigationService.GetNavigationService(CheckOutButton);
      if (svc != null)
      {
        svc.Navigate(new CheckInOut());
      }
    }

    private void addBookButton_Click(object sender, RoutedEventArgs e)
    {
      NavigationService svc = NavigationService.GetNavigationService(AddBookButton);
      if (svc != null)
      {
        svc.Navigate(new ISBNVerify("Select a book to add/remove"));
      }
    }

    private void mainMenu_Click(object sender, RoutedEventArgs e)
    {
      NavigationService svc = NavigationService.GetNavigationService(MainMenuButton);
      if (svc != null)
      {
        svc.Navigate(new MainMenu());
      }
    }

    private void exitButton_Click(object sender, RoutedEventArgs e)
    {
      CloseDialog dialog = new CloseDialog();

      dialog.Owner = Window.GetWindow(this);
      if (dialog.ShowDialog() == true)
      {
        Application.Current.ShutdownMode = ShutdownMode.OnExplicitShutdown;
        Application.Current.Shutdown();
      }
    }

    private void updateBookButton_Click(object sender, RoutedEventArgs e)
    {
      NavigationService svc = NavigationService.GetNavigationService(UpdateBookButton);
      if (svc != null)
      {
        svc.Navigate(new SearchMenu(Mode.Update, ""));
      }
    }
  }
}
