//------------------------------------------------------------------------------
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
    using System.Collections.Generic;
    
    public partial class Librarian
    {
        public int ID { get; set; }
        public string Phone { get; set; }
        public string UserID { get; set; }
        public string Password { get; set; }
    
        public virtual Person Person { get; set; }
    }
}
