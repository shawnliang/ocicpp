#ifndef PROPHETSTOR_OCI_WRAPPER_STATEMENT_H_
#define PROPHETSTOR_OCI_WRAPPER_STATEMENT_H_

#include "oci.h"

namespace prophetstor
{
namespace ociwrapper
{

class DBDataReader;
class DBConnection;
class DBStatementImp;

class DBStatement
{
public:
    DBStatement(DBConnection& connection, const char* statementText);
    ~DBStatement();

    /**
     * prepare sql statement
     * @throw OCIException
     */
    void prepare();

    /**
     *
     */
    int execute();

    /**
     * Executes sql command and builds a DBDataReader.
     * @throw OCIException
     */
    DBDataReader* executeReader();


    OCIStmt* getHandle();

private:
    // copy of db statement object is not allowed
    DBStatement(const DBStatement&);
    DBStatement& operator=(const DBStatement&);

private:
    DBStatementImp* imp;
};

}
}

#endif //PROPHETSTOR_OCI_WRAPPER_STATEMENT_H_
