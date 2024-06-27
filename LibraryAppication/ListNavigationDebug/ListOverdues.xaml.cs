/******************************************************************************/
// file: ListOverdues.xaml.cs
// author: Richard Shen
// date: 08/10/14
// brief: This file contains the implementation of ListOverdues page
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
  /// Interaction logic for ListOverdues.xaml
  /// </summary>
  public partial class ListOverdues : Page
  {
    public ListOverdues()
    {
      InitializeComponent();
      LoadCheckOutLogs();
    }

    private void LoadCheckOutLogs()
    {
      CheckOutLogsListView.DataContext = LibraryUtility.GetAllCheckOutLogs().Where(scol => scol.IsOverdue);
    }

    private void checkOutLogsListView_SelectionChanged(object sender, SelectionChangedEventArgs e)
    {
      if (CheckOutLogsListView.SelectedItem != null)
      {
        var ch = CheckOutLogsListView.SelectedItem as SystemCheckOutLog;

        if (ch != null)
        {
          var tempChs = new SystemCheckOutLogCollection();
          tempChs.Add(ch);
          CardholdersListView.DataContext = tempChs;
        }
      }
    }
  }
}
