﻿//------------------------------------------------------------------------------
// <auto-generated>
//    This code was generated from a template.
//
//    Manual changes to this file may cause unexpected behavior in your application.
//    Manual changes to this file will be overwritten if the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

namespace ListNavigationDebug
{
    using System;
    using System.Data.Entity;
    using System.Data.Entity.Infrastructure;
    
    public partial class LibraryInformation2Entities : DbContext
    {
        public LibraryInformation2Entities()
            : base("name=LibraryInformation2Entities")
        {
        }
    
        protected override void OnModelCreating(DbModelBuilder modelBuilder)
        {
            throw new UnintentionalCodeFirstException();
        }
    
        public DbSet<Author> Authors { get; set; }
        public DbSet<Book> Books { get; set; }
        public DbSet<Cardholder> Cardholders { get; set; }
        public DbSet<CheckOutLog> CheckOutLogs { get; set; }
        public DbSet<Librarian> Librarians { get; set; }
        public DbSet<Person> People { get; set; }
    }
}
