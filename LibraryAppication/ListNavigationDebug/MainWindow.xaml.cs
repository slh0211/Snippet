/******************************************************************************/
// file: MainWindox.xaml.cs
// author: Richard Shen
// date: 07/27/14
// brief: This file contains the implementation of MainWindow window
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
  /// Interaction logic for MainWindow.xaml
  /// </summary>
  public partial class MainWindow : Window
  {
    public MainWindow()
    {
      InitializeComponent();
      _mainFrame.NavigationService.Navigate(new MainMenu());
    }

    private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
    {
      CloseDialog dialog = new CloseDialog();

      dialog.Owner = this;
      if (dialog.ShowDialog() == true)
      {
        Application.Current.ShutdownMode = ShutdownMode.OnExplicitShutdown;
        Application.Current.Shutdown();
      }
      else
      {
        e.Cancel = true;
      }
    }
  }
}
