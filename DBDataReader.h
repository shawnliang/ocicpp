
#ifndef PROPHETSTOR_OCI_WRAPPER_DBDATAREADER_H_
#define PROPHETSTOR_OCI_WRAPPER_DBDATAREADER_H_

#include <stdint.h>
#include <vector>
#include "oci.h"

namespace prophetstor
{
namespace ociwrapper
{

class DBDataReaderImp;
class DBStatement;
class DBColumnDefinition;
class DBConnection;
class DBValueString;

class DBDataReader
{
public:
    DBDataReader(DBStatement& stmt, std::vector<DBColumnDefinition>* columns, DBConnection& connection);
    ~DBDataReader();

    bool hasNext();

    bool isNull(const char* columnName);
    bool isNull(int col);

    const DBValueString& get(int col) const;

private:
    DBDataReaderImp* imp;
};

} /* namespace ociwrapper */
} /* namespace prophetstor */

#endif /* PROPHETSTOR_OCI_WRAPPER_DBDATAREADER_H_ */
