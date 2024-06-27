/******************************************************************************/
// file: ListLibrarians.xaml.cs
// author: Richard Shen
// date: 08/10/14
// brief: This file contains the implementation of ListLibrarians page
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
  /// Interaction logic for ListLibrarians.xaml
  /// </summary>
  public partial class ListLibrarians : Page
  {
    public ListLibrarians()
    {
      InitializeComponent();
      LoadLibrarians();
    }

    private void LoadLibrarians()
    {
      LibrariansListView.DataContext = LibraryUtility.GetAllLibrarians();
    }
  }
}
