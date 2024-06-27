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

namespace LibraryDebugApplication
{
  /// <summary>
  /// Interaction logic for MainWindow.xaml
  /// </summary>
  public partial class MainWindow : Window
  {
    //LibraryInformation2Entities _li = new LibraryInformation2Entities();
    SystemPersonCollection _spc = new SystemPersonCollection();
    SystemBookCollection _sbc = new SystemBookCollection();
    SystemCheckOutLogCollection _scolc = new SystemCheckOutLogCollection();


    public MainWindow()
    {
      InitializeComponent();
      LoadSystem();
    }

    private void LoadSystem()
    {
      LoadSystemLibrarian();
      LoadSystemCardholder();
      LoadSystemAuthor();
      _spc.Sort();
      PersonListBox.DataContext = _spc.People();

      LoadSystemBook();
      _sbc.Sort();
      BookListBox.DataContext = _sbc.Books();

      LoadSystemCheckOutLog();
      _scolc.Sort();
      CheckOutLogListBox.DataContext = _scolc.CheckOutLogs();
    }

    //private IEnumerable<Librarian> LoadLibrarian()
    //{
    //  //using (var li = new LibraryInformation2Entities())
    //  //{
    //    return from lib in _li.Librarians
    //      select lib;
    //  //}
    //}

    //private IEnumerable<Cardholder> LoadCardholder()
    //{
    //  //using (var li = new LibraryInformation2Entities())
    //  //{
    //    return from ch in _li.Cardholders
    //           select ch;
    //  //}
    //}

    //private IEnumerable<Author> LoadAuthor()
    //{
    //  //using (var li = new LibraryInformation2Entities())
    //  //{
    //    return from au in _li.Authors
    //      select au;
    //  //}
    //}

    //private IEnumerable<Book> LoadBook()
    //{
    //  //using (var li = new LibraryInformation2Entities())
    //  //{
    //    return from bo in _li.Books
    //      select bo;
    //  //}
    //}

    //private IEnumerable<CheckOutLog> LoadCheckOutLog()
    //{
    //  //using (var li = new LibraryInformation2Entities())
    //  //{
    //    return from col in _li.CheckOutLogs
    //      select col;
    //  //}
    //}

    private void LoadSystemLibrarian()
    {
      using (var li = new LibraryInformation2Entities())
      {
      //List<Librarian> liList = new List<Librarian>();
      //liList = LoadLibrarian().ToList();
      var liList = (from libr in li.Librarians select libr);

      foreach (Librarian lib in liList)
      {
        _spc.Add(new SystemLibrarian(lib.Person.PersonID, lib.Person.FirstName, 
                                     lib.Person.LastName, lib.Phone, lib.UserID, lib.Password));
      }
      }
    }

    private void LoadSystemCardholder()
    {
      using (var li = new LibraryInformation2Entities())
      {
      //List<Cardholder> chList = new List<Cardholder>();
      //chList = LoadCardholder().ToList();
      var  chList = (from ch in li.Cardholders
                 select ch);

      foreach (Cardholder ch in chList)
      {
        _spc.Add(new SystemCardholder(ch.Person.PersonID, ch.Person.FirstName, ch.Person.LastName,
                                      ch.Phone, ch.LibraryCardID));
      }
      }
    }

    private void LoadSystemAuthor()
    {
      using (var li = new LibraryInformation2Entities())
      {
      //List<Author> auList = new List<Author>();
      //auList = LoadAuthor().ToList();
      var auList = (from au in li.Authors select au);

      foreach (Author au in auList)
      {
        _spc.Add(new SystemAuthor(au.ID, au.Person.FirstName, au.Person.LastName, au.Bio));
      }
      }
    }

    private void LoadSystemBook()
    {
      using (var li = new LibraryInformation2Entities())
      {
      //List<Book> boList = LoadBook().ToList();
        var boList = (from bo in li.Books select bo);

      foreach (Book bo in boList)
      {
        _sbc.Add(new SystemBook(bo.BookID, bo.ISBN, bo.Title, bo.AuthorID, bo.NumPages, bo.Subject, bo.Description,
                                bo.Publisher, bo.YearPublished, bo.Language, bo.NumberOfCopies));
      }
      }
    }

    private void LoadSystemCheckOutLog()
    {
      using (var li = new LibraryInformation2Entities())
      {
      //List<CheckOutLog> colList = LoadCheckOutLog().ToList();
        var colList = from col in li.CheckOutLogs
          select col;

      foreach (CheckOutLog col in colList)
      {
        SystemCheckOutLog tempCOL = new SystemCheckOutLog(col.CheckOutLogID, col.CardholderID, col.BookID, col.CheckOutDate,
          col.CheckInDate);

        tempCOL.CheckedOutBook = new SystemBook(col.Book.BookID, col.Book.ISBN, col.Book.Title, col.Book.AuthorID,
                                            col.Book.NumPages, col.Book.Subject, col.Book.Description,
                                            col.Book.Publisher, col.Book.YearPublished, col.Book.Language, col.Book.NumberOfCopies);
        tempCOL.CheckedOutCardholder = new SystemCardholder(col.Cardholder.Person.PersonID, col.Cardholder.Person.FirstName, col.Cardholder.Person.LastName,
                                      col.Cardholder.Phone, col.Cardholder.LibraryCardID);

        _scolc.Add(tempCOL);
      }
      }
    }
  }
}
