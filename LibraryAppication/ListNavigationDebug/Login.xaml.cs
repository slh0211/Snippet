/******************************************************************************/
// file: Login.xaml.cs
// author: Richard Shen
// date: 07/29/14
// brief: This file contains the implementation of Login page
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
  /// Interaction logic for Login.xaml
  /// </summary>
  public partial class Login : Page
  {
    public Login()
    {
      InitializeComponent();
      UserIDTextBox.Focus();
    }

    private void loginButton_Click(object sender, RoutedEventArgs e)
    {
      if (UserIDTextBox.Text != null && PasswordBox.Password != null)
      {
        bool userFound = LibraryUtility.VerifyUser(UserIDTextBox.Text, PasswordBox.Password);

        if (userFound)
        {
          NavigationService svc = NavigationService.GetNavigationService(LoginButton);
          if (svc != null)
          {
            svc.Navigate(new LibrarianMenu());
          }
        }
        else
        {
          UserIDTextBox.Text = null;
          PasswordBox.Password = null;
          ErrorMessageLabel.Content = "Incorrect UserID or Password";
        }
      }
    }
  }
}
