
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
class DBValue;

class DBDataReader
{
public:
    DBDataReader(DBStatement& stmt, std::vector<DBColumnDefinition>* columns, DBConnection& connection);
    ~DBDataReader();

    bool hasNext();
    const DBValue& get(int col) const;

private:
    DBDataReaderImp* imp;
};

} /* namespace ociwrapper */
} /* namespace prophetstor */

#endif /* PROPHETSTOR_OCI_WRAPPER_DBDATAREADER_H_ */
