#ifndef PROPHETSTOR_OCI_WRAPPER_DBCONN_H_
#define PROPHETSTOR_OCI_WRAPPER_DBCONN_H_

#include "oci.h"
#include "stdint.h"

namespace prophetstor
{
namespace ociwrapper
{

class DBConnectionImp;
class DBStatement;

class DBConnection
{
public:
    ~DBConnection();
    DBConnection(const std::string& user, const std::string& password, const std::string& connectionString);

    void connect();
    void close();
    void* allocHandle(uint32_t oracleHandleType);

    OCIEnv* getEnvHandle();
    OCIError* getErrHandle();
    OCISvcCtx* getSvcCtxHandle();

private:
    // copy of db conneciton is not allowed
    DBConnection(const DBConnection&);
    DBConnection& operator=(const DBConnection&);

private:
    DBConnectionImp* imp;
};


}
}


#endif // PROPHETSTOR_OCI_WRAPPER_DBCONN_H_
