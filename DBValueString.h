#ifndef DBVALUESTRING_H_
#define DBVALUESTRING_H_

#include "oci.h"
#include "DBValue.h"

namespace prophetstor
{
namespace ociwrapper
{

class DBValueString : public DBValue
{
public:
    DBValueString(int maxSize);
    virtual ~DBValueString();

    inline int getSQLType() const { return SQLT_STR; }
    void* getBuffer();
    int getBufferSize() const;

    long getLong() const;
    std::string getString() const;

private:
    char* buffer_;
    int bufferSize_;
};

} /* namespace ociwrapper */
} /* namespace prophetstor */

#endif /* DBVALUESTRING_H_ */
