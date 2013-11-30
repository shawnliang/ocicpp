ocicpp
======

ocicpp is an OCI c++ wrapper which provides simple and friendly interfaces similiar to ADO.Net.


Introduction
------------

To programmatically access oracle database thru C++ code, oracle provides OCCI (oracle C++ call interface). OCCI is powerful and versatile. But for developer who just want to do simple queries, ocicpp is an alternative. ocicpp is simple 
and lightweight. More importantly, it is very similar to ADO.NET interface which is extrmely easy to use.


Usage
-----

    #include "ociwrapper.h"
  
    namespace db = prophetstor::ociwrapper;
  
    db::DBConnection conn("user", "pwd", "dbhost/sid");
    conn.connect();
  
    db::DBStatement stmt(conn, "select name, age from Employee");
    stmt.prepare();
    db::DBDataReader* reader = stmt.executeReader();
    while (reader->hasNext())
    {
      cout << "name: " << reader->get(0).getString() << endl
           << "age : " << reader->get(1).getLong() << endl;
      
    }
  
