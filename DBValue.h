#ifndef PROPHETSTOR_OCIWRAPPER_DBVALUE_H_
#define PROPHETSTOR_OCIWRAPPER_DBVALUE_H_

#include <string>

namespace prophetstor
{
namespace ociwrapper
{

class DBValue
{
public:
    virtual ~DBValue() {}

    virtual int getSQLType() const = 0;
    virtual void* getBuffer() = 0;
    virtual int getBufferSize() const = 0;

    virtual std::string getString() const = 0;
    virtual long getLong() const = 0;

};


}
}


#endif /* PROPHETSTOR_OCIWRAPPER_DBVALUE_H_ */
