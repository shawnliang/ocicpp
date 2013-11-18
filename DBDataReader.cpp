#include <stdexcept>
#include <vector>
#include "DBDataReader.h"
#include "OCIException.h"
#include "DBStatement.h"
#include "DBConnection.h"
#include "DBColumnDefinition.h"
#include "DBValueString.h"
#include "DBValueNum.h"

namespace prophetstor
{
namespace ociwrapper
{
class DBStatementImp;

class DBDataReaderImp
{
public:
    DBDataReaderImp(DBStatement& stmt, std::vector<DBColumnDefinition>* columns, DBConnection& connection)
        : stmt_(stmt), connection_(connection), rowFetched(false), columns_(columns)
    {
        for (int i=0; i<columns_->size(); i++)
        {
            DBColumnDefinition& col = columns_->at(i);

//            SQLT_DATE
//            SQLT_TIMESTAMP
//            SQLT_TIMESTAMP_TZ
//            SQLT_TIMESTAMP_LTZ
//            SQLT_INTERVAL_YM
//            SQLT_INTERVAL_DS
//            SQLT_CLOB
//            SQLT_BLOB
//            SQLT_INT
//            SQLT_UIN
//            SQLT_FLT
//            SQLT_PDN
//            SQLT_BIN
//            SQLT_NTY
//            SQLT_REF
//            SQLT_VST
//            SQLT_VNU

            DBValue* cell = NULL;

            switch(col.getDataType())
            {
            case SQLT_CHR:
                cell = new DBValueString(col.getWidth()+1);
                break;
            case SQLT_NUM:
                cell = new DBValueNum();
                break;
            default:
                throw OCIException("Not implemented for this data type", col.getDataType());
            }

            cells_.push_back(cell);
            OCIDefine* def = NULL;

            sword ret = OCIDefineByPos(stmt_.getHandle(), &def, connection_.getErrHandle(), i+1, cell->getBuffer(), cell->getBufferSize(), (ub2)cell->getSQLType(), NULL, NULL, NULL, OCI_DEFAULT);
            if (ret != OCI_SUCCESS)
                throw OCIException("OCIDefineByPos error", ret, connection_.getErrHandle());

            defines_.push_back(def);
        }
    }

    ~DBDataReaderImp()
    {
        delete columns_;

        for(int i=0; i<cells_.size(); i++)
            delete cells_[i];
    }

    bool hasNext()
    {
        sword ret = OCIStmtFetch2(stmt_.getHandle(), connection_.getErrHandle(), 1, OCI_DEFAULT, 0, OCI_DEFAULT);
        if (ret == OCI_SUCCESS)
        {
            rowFetched = true;
            return true;
        }
        else if (ret == OCI_NO_DATA)
        {
            rowFetched = false;
            return false;
        }
        else
        {
            rowFetched = false;
            throw OCIException("OCIStmtFetch2 failed", ret, connection_.getErrHandle());
        }
    }

    const DBValue& get(int col) const
    {
        if (!rowFetched)
            throw std::domain_error("no row is avaliable or not fetched yet");

        int index = col - 1;

        if (index < 0 || index >= cells_.size())
            throw std::out_of_range("col is out of range");

        return *cells_[index];
    }

private:
    DBStatement& stmt_;
    DBConnection& connection_;

    bool rowFetched;

    std::vector<DBColumnDefinition>* columns_;
    std::vector<DBValue*> cells_;
    std::vector<OCIDefine*> defines_;
};



DBDataReader::DBDataReader(DBStatement& stmt, std::vector<DBColumnDefinition>* columns, DBConnection& connection)
{
    imp = new DBDataReaderImp(stmt, columns, connection);
}

DBDataReader::~DBDataReader()
{
    delete imp;
}

bool DBDataReader::hasNext()
{
    return imp->hasNext();
}

const DBValue& DBDataReader::get(int col) const
{
    return imp->get(col);
}

} /* namespace ociwrapper */
} /* namespace prophetstor */
