/******************************************************************************/
// file: BookDetail.xaml.cs
// author: Richard Shen
// date: 07/28/14
// brief: This file contains the implementation of BookDetail page
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
  /// Interaction logic for BookDetail.xaml
  /// </summary>
  public partial class BookDetail : Page
  {
    private SystemBook _sb;

    public BookDetail(SystemBook sb, Mode md)
    {
      InitializeComponent();
      _sb = sb;
      if (md == Mode.Update)
      {
        EditButton.Visibility = Visibility.Visible;
      }
      DisplayBookDetail(sb);
    }

    private void DisplayBookDetail(SystemBook sb)
    {
      TitleTextBox.Text = sb.Title;
      AuthorTextBox.Text = sb.Author.ToString();
      PublisherTextBox.Text = sb.Publisher;
      SubjectTextBox.Text = sb.Subject;
      LanguagesTextBox.Text = sb.Language;
      YearTextBox.Text = sb.YearPublished.ToString();
      ISBNTextBox.Text = sb.ISBN;
      PagesTextBox.Text = sb.NumPages.ToString();
      AvailableTextBox.Text = sb.NumberOfAvailableCopies.ToString();
      TotalTextBox.Text = sb.NumberOfCopies.ToString();
      DescriptionTextBox.Text = sb.Description;
    }

    private void editButton_Click(object sender, RoutedEventArgs e)
    {
      NavigationService svc = NavigationService.GetNavigationService(EditButton);
      if (svc != null)
      {
          svc.Navigate(new BookEdit(_sb));
      }
    }
  }
}
