/******************************************************************************/
// file: BookAddRemove.xaml.cs
// author: Richard Shen
// date: 07/30/14
// brief: This file contains the implementation of BookAddRemove page
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
  /// Interaction logic for BookAddRemove.xaml
  /// </summary>
  public partial class BookAddRemove : Page
  {
    private SystemBook _sb;

    public BookAddRemove(SystemBook sb)
    {
      InitializeComponent();
      _sb = sb;
      LoadSystemBook();
    }

    private void LoadSystemBook()
    {
      ISBNTextBox.Text = _sb.ISBN;
      TitleTextBox.Text = _sb.Title;
      AuthorTextBox.Text = _sb.Author.ToString();
      PagesTextBox.Text = _sb.NumPages.ToString();
      SubjectTextBox.Text = _sb.Subject;
      DescriptionTextBox.Text = _sb.Description;
      PublisherTextBox.Text = _sb.Publisher;
      YearTextBox.Text = _sb.YearPublished.ToString();
      LanguagesTextBox.Text = _sb.Language;
      TotalTextBox.Text = _sb.NumberOfCopies.ToString();
      AvailableTextBox.Text = _sb.NumberOfAvailableCopies.ToString();
    }

    private void addButton_Click(object sender, RoutedEventArgs e)
    {
      int temp;
      bool success = Int32.TryParse(CopiesTextBox.Text, out temp);
      if (success && temp > 0)
      {
        NavigationService svc = NavigationService.GetNavigationService(AddButton);
        if (svc != null)
        {
          svc.Navigate(new BookAddRemoveConfirm(_sb, temp));
        }
      }
      else
      {
        ErrorMessageLabel.Content = "Error: User Input Error";
      }
    }

    private void removeButton_Click(object sender, RoutedEventArgs e)
    {
      int temp;
      bool success = Int32.TryParse(CopiesTextBox.Text, out temp);
      if (success && temp > 0)
      {
        // Only remove available copies
        if (temp <= _sb.NumberOfAvailableCopies)
        {
          NavigationService svc = NavigationService.GetNavigationService(AddButton);
          if (svc != null)
          {
            svc.Navigate(new BookAddRemoveConfirm(_sb, -temp));
          }
        }
        else
        {
          ErrorMessageLabel.Content = "Error: Can not remove checked out copies";
        }
      }
      else
      {
        ErrorMessageLabel.Content = "Error: User Input Error";
      }
    }

    private void removeAllButton_Click(object sender, RoutedEventArgs e)
    {
      // Only remove all when no book checked out
      if (_sb.NumberOfCopies == _sb.NumberOfAvailableCopies)
      {
        NavigationService svc = NavigationService.GetNavigationService(AddButton);
        if (svc != null)
        {
          svc.Navigate(new BookAddRemoveConfirm(_sb, -_sb.NumberOfCopies));
        }
      }
      else
      {
        ErrorMessageLabel.Content = "Error: Can not remove all books";
      }
    }
  }
}
