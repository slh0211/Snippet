/******************************************************************************/
// file: AuthorConfirm.xaml.cs
// author: Richard Shen
// date: 07/30/14
// brief: This file contains the implementation of AuthorConfirm page
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
  /// Interaction logic for AuthorConfirm.xaml
  /// </summary>
  public partial class AuthorConfirm : Page
  {
    private SystemAuthor _sa;
    private bool _confirmed = false;

    public AuthorConfirm(SystemAuthor sa)
    {
      InitializeComponent();
      _sa = sa;
      LoadAuthor();
    }

    private void LoadAuthor()
    {
      LastNameTextBox.Text = _sa.LastName;
      FirstNameTextBox.Text = _sa.FirstName;
      BioTextBox.Text = _sa.Bio;
    }

    private void confirmButton_Click(object sender, RoutedEventArgs e)
    {
      if (_confirmed == false)
      {
        Person newPerson = new Person();
        newPerson.PersonID = _sa.PersonID;
        newPerson.LastName = _sa.LastName;
        newPerson.FirstName = _sa.FirstName;

        Author newAuthor = new Author();
        newAuthor.Bio = _sa.Bio;
        newAuthor.Person = newPerson;

        using (LibraryInformation2Entities li = new LibraryInformation2Entities())
        {
          li.People.Add(newPerson);
          li.Authors.Add(newAuthor);
          li.SaveChanges();
        }

        ErrorMessageLabel.Content = _sa+ " added to the system";
        _confirmed = true;
        NavigationService svc = NavigationService.GetNavigationService(ConfirmButton);
        if (svc != null)
        {
          svc.Navigate(new AuthorVerify());
        }
      }
      else
      {
        ErrorMessageLabel.Content = "The author has been added to the system";
      }
    }
  }
}
