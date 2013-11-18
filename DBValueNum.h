#ifndef DBVALUENUM_H_
#define DBVALUENUM_H_

#include "oci.h"
#include "DBValue.h"

namespace prophetstor
{
namespace ociwrapper
{

class DBValueNum : public DBValue
{
public:
    DBValueNum();
    virtual ~DBValueNum();

    inline int getSQLType() const { return SQLT_INT; }
    void* getBuffer();
    int getBufferSize() const;

    std::string getString() const;
    long getLong() const;

private:
    long num_;
};

} /* namespace ociwrapper */
} /* namespace prophetstor */

#endif /* DBVALUENUM_H_ */
