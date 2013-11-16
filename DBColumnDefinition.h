#ifndef PROPHETSTOR_OCIWRAPPER_DBCOLUMNDEFINITION_H_
#define PROPHETSTOR_OCIWRAPPER_DBCOLUMNDEFINITION_H_

#include "oci.h"
#include <string>

namespace prophetstor
{
namespace ociwrapper
{

class DBColumnDefinition
{
public:
    DBColumnDefinition(OCIParam* column, OCIError* err)
        : column_(column), err_(err), datatype_(0), width_(0), isText_(false)
    {
    }

    ub2 getDataType();
    const std::string& getName();
    ub2 getWidth();
    bool isText();

private:
    OCIParam* column_;
    OCIError* err_;
    ub2 datatype_;
    std::string name_;
    ub2 width_;
    int isText_;
};


}
}



#endif /* PROPHETSTOR_OCIWRAPPER_DBCOLUMNDEFINITION_H_ */
