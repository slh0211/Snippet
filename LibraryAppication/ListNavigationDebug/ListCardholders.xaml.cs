/******************************************************************************/
// file: ListCardholders.xaml.cs
// author: Richard Shen
// date: 08/10/14
// brief: This file contains the implementation of Cardholders page
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
  /// Interaction logic for ListCardholders.xaml
  /// </summary>
  public partial class ListCardholders : Page
  {
    public ListCardholders()
    {
      InitializeComponent();
      LoadCardholders();
    }

    private void LoadCardholders()
    {
      CardholdersListView.DataContext = LibraryUtility.GetAllCardholders();
    }

    private void cardholdersListView_SelectionChanged(object sender, SelectionChangedEventArgs e)
    {
      if (CardholdersListView.SelectedItem != null)
      {
        var holder = CardholdersListView.SelectedItem as SystemCardholder;

        // Only load checked out books (no check in date)
        if (holder != null)
        {
          CheckOutLogsListView.DataContext = holder.UserCheckOutLogs.CheckOutLogs().Where(ch => ch.CheckInDate == null);
        }
      }
    }
  }
}
