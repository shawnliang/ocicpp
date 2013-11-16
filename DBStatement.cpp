#include <string>
#include <stdint.h>
#include <vector>
#include "DBConnection.h"
#include "DBStatement.h"
#include "DBColumnDefinition.h"
#include "OCIException.h"
#include "DBDataReader.h"

namespace prophetstor
{
namespace ociwrapper
{


class DBStatementImp
{
public:
    DBStatementImp(DBConnection& connection, const char* commandText, DBStatement& stmt)
        : connection_(connection), commandText_(commandText), stmtExt_(stmt)
    {
        stmt_ = (OCIStmt*)connection.allocHandle(OCI_HTYPE_STMT);
    }

    ~DBStatementImp()
    {
        if (stmt_)
            OCIHandleFree(stmt_, OCI_HTYPE_STMT);
    }

    void prepare()
    {
        sword ret = OCIStmtPrepare2(connection_.getSvcCtxHandle(), &stmt_, connection_.getErrHandle(), (const OraText*)commandText_.c_str(), commandText_.length(), NULL, 0, OCI_NTV_SYNTAX, OCI_DEFAULT);
        if (ret != OCI_SUCCESS)
            throw OCIException((std::string("statement prepare failed: ") + commandText_) .c_str(), ret, connection_.getErrHandle());
    }

    DBDataReader* executeReader()
    {
        sword ret = OCIStmtExecute(connection_.getSvcCtxHandle(), stmt_, connection_.getErrHandle(), 0, 0, NULL, NULL, OCI_DEFAULT);
        if (ret != OCI_SUCCESS)
            throw OCIException("OCIStmtExecute failed", ret, connection_.getErrHandle());

        std::vector<DBColumnDefinition>* columns = readColumnDefinition();

        return new DBDataReader(stmtExt_, columns, connection_);
    }

    std::vector<DBColumnDefinition>* readColumnDefinition()
    {
        std::vector<DBColumnDefinition>* columns = new std::vector<DBColumnDefinition>();
        int colIndex = 1;
        OCIParam *column = getColumn(colIndex);

        while (column)
        {
            columns->push_back(DBColumnDefinition(column, connection_.getErrHandle()));

            colIndex ++;
            column = getColumn(colIndex);
        }

        return columns;
    }

    inline OCIStmt* getHandle()
    {
        return stmt_;
    }

private:
    inline OCIParam* getColumn(int index)
    {
        OCIParam* column = NULL;
        if (OCIParamGet(stmt_, OCI_HTYPE_STMT, connection_.getErrHandle(), (void **)&column, (ub4) index) == OCI_SUCCESS)
            return column;
        else
            return NULL;
    }


private:
    DBConnection& connection_;
    std::string commandText_;
    OCIStmt* stmt_;
    DBStatement& stmtExt_;
};




DBStatement::DBStatement(DBConnection& connection, const char* statementText)
{
    imp = new DBStatementImp(connection, statementText, *this);
}

void DBStatement::prepare()
{
    imp->prepare();
}

DBStatement::~DBStatement()
{
    delete imp;
}

DBDataReader* DBStatement::executeReader()
{
    return imp->executeReader();
}

OCIStmt* DBStatement::getHandle()
{
    return imp->getHandle();
}

}
}
