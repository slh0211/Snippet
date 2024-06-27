/******************************************************************************/
// file: MainMenu.xaml.cs
// author: Richard Shen
// date: 07/29/14
// brief: This file contains the implementation of MainMenu page
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
  /// Interaction logic for MainMenu.xaml
  /// </summary>
  public partial class MainMenu : Page
  {
    public MainMenu()
    {
      InitializeComponent();
    }

    private void searchModeButton_Click(object sender, RoutedEventArgs e)
    {
      NavigationService svc = NavigationService.GetNavigationService(SearchModeButton);
      if (svc != null)
      {
        svc.Navigate(new SearchMenu(Mode.Patron, ""));
      }
    }

    private void librarianLoginButton_Click(object sender, RoutedEventArgs e)
    {
      NavigationService svc = NavigationService.GetNavigationService(LibrarianLoginButton);
      if (svc != null)
      {
        svc.Navigate(new Login());
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
  }
}
