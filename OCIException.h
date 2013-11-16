#ifndef PROPHETSTOR_OCIWRAPPER_OCIEXCEPTION_H_
#define PROPHETSTOR_OCIWRAPPER_OCIEXCEPTION_H_

#include "oci.h"

namespace prophetstor
{
namespace ociwrapper
{

class OCIException : std::bad_exception
{
public:
    OCIException(const char* msg, int status, OCIError* err = NULL);

    inline const char* what() const throw() { return buff_; }
    inline int getOracleErrorId() const { return errorId_; }

private:
    static const char* getStatusString(int status);

private:
    char buff_[1024];
    int errorId_;
};


}
}


#endif /* PROPHETSTOR_OCIWRAPPER_OCIEXCEPTION_H_ */
