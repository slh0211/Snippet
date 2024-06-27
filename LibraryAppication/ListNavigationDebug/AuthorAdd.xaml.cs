/******************************************************************************/
// file: AuthorAdd.xaml.cs
// author: Richard Shen
// date: 07/30/14
// brief: This file contains the implementation of AuthorAdd page
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
  /// Interaction logic for AuthorAdd.xaml
  /// </summary>
  public partial class AuthorAdd : Page
  {
    public AuthorAdd()
    {
      InitializeComponent();
    }

    private void submitButton_Click(object sender, RoutedEventArgs e)
    {
      if (LastNameTextBox.Text != null && FirstNameTextBox.Text != null && BioTextBox.Text != null)
      {
        // Check if all fields are not empty
        if (LastNameTextBox.Text.Length != 0 && FirstNameTextBox.Text.Length != 0 && BioTextBox.Text.Length != 0)
        {
          try
          {
            int newID = Guid.NewGuid().GetHashCode();
            LibraryUtility.VerifyPersonIDDuplicate(newID);
            SystemAuthor tempAuthor = new SystemAuthor(newID, FirstNameTextBox.Text, LastNameTextBox.Text,
              BioTextBox.Text);

            NavigationService svc = NavigationService.GetNavigationService(SubmitButton);
            if (svc != null)
            {
              svc.Navigate(new AuthorConfirm(tempAuthor));
            }
          }
          catch (ArgumentException ae)
          {
            ErrorMessageLabel.Content = ae.Message;
          }
        }
        else
        {
          ErrorMessageLabel.Content = "Error: All fields are required";
        }
      }
      else
      {
        ErrorMessageLabel.Content = "Error: User Input Error";
      }
    }

    private void cancelButton_Click(object sender, RoutedEventArgs e)
    {
      NavigationService svc = NavigationService.GetNavigationService(CancelButton);
      if (svc != null)
      {
        svc.Navigate(new AuthorVerify());
      }
    }

    private void clearButton_Click(object sender, RoutedEventArgs e)
    {
      FirstNameTextBox.Text = null;
      LastNameTextBox.Text = null;
      BioTextBox.Text = null;
      ErrorMessageLabel.Content = null;
    }
  }
}
