#include <string>
#include <stdint.h>
#include "oci.h"
#include "DBConnection.h"
#include "OCIException.h"

namespace prophetstor
{
namespace ociwrapper
{


class DBConnectionImp
{
private:
    DBConnectionImp()
        : env_(NULL), err_(NULL), server_(NULL), session_(NULL), svcctx_(NULL)
    {
    }

public:
    DBConnectionImp(const std::string& user, const std::string& password, const std::string& connectionString)
        : user_(user), password_(password), connectionString_(connectionString)
    {
        sword ret = 0;

        ret = OCIEnvCreate(&env_, OCI_THREADED, NULL, NULL, NULL, NULL, 0, NULL);
        if (ret != OCI_SUCCESS)
            throw OCIException("OCIEnvCreate failed", ret);

        err_ = (OCIError*)allocHandle(OCI_HTYPE_ERROR);
        server_ = (OCIServer*)allocHandle(OCI_HTYPE_SERVER);
        svcctx_ = (OCISvcCtx*)allocHandle(OCI_HTYPE_SVCCTX);
        session_ = (OCISession*)allocHandle(OCI_HTYPE_SESSION);
    }

    ~DBConnectionImp()
    {
        if (env_)
            OCIHandleFree(env_, OCI_HTYPE_ENV);
    }

    void connect()
    {
        sword ret = 0;

        ret = OCIServerAttach(server_, err_, (const OraText*) connectionString_.c_str(), connectionString_.length(), OCI_DEFAULT);
        if (ret != OCI_SUCCESS)
            throw OCIException("OCIServerAttach failed", ret);

        ret = OCIAttrSet((void*) svcctx_, OCI_HTYPE_SVCCTX, server_, 0, OCI_ATTR_SERVER, err_);
        if (ret != OCI_SUCCESS)
            throw OCIException("OCIAttrSet attaching server to svcctx failed", ret);

        ret = OCIAttrSet((void*) session_, OCI_HTYPE_SESSION, (void*) user_.c_str(), (ub4) user_.length(), OCI_ATTR_USERNAME, err_);
        if (ret != OCI_SUCCESS)
            throw OCIException("OCIAttrSet attaching username to session failed", ret);

        ret = OCIAttrSet((void*) session_, OCI_HTYPE_SESSION, (void*) password_.c_str(), (ub4) password_.length(), OCI_ATTR_PASSWORD, err_);
        if (ret != OCI_SUCCESS)
            throw OCIException("OCIAttrSet attaching password to session failed", ret);

        ret = OCISessionBegin(svcctx_, err_, session_, OCI_CRED_RDBMS, OCI_SYSDBA);
        if (ret != OCI_SUCCESS)
            throw OCIException("OCISessionBegin failed", ret);

        ret = OCIAttrSet(svcctx_, OCI_HTYPE_SVCCTX, session_, 0, OCI_ATTR_SESSION, err_);
        if (ret != OCI_SUCCESS)
            throw OCIException("OCIAttrSet attaching session to server failed", ret);
    }

    void* allocHandle(uint32_t oracleHandleType)
    {
        void* handle = NULL;
        sword ret = OCIHandleAlloc(env_, &handle, oracleHandleType, 0, NULL);

        if (ret != OCI_SUCCESS)
            throw OCIException("OCIHandleAlloc failed", ret, err_);

        return handle;
    }

    inline OCIEnv* getEnvHandle()
    {
        return env_;
    }

    inline OCIError* getErrHandle()
    {
        return err_;
    }

    inline OCISvcCtx* getSvcCtxHandle()
    {
        return svcctx_;
    }
private:
    OCIEnv* env_;
    OCIError *err_;
    OCIServer *server_;
    OCISession *session_;
    OCISvcCtx *svcctx_;

    std::string user_;
    std::string password_;
    std::string connectionString_;
};


DBConnection::DBConnection(const std::string& user, const std::string& password, const std::string& connectionString)
{
    imp = new DBConnectionImp(user, password, connectionString);
}

DBConnection::~DBConnection()
{
    delete imp;
}

void DBConnection::connect()
{
    imp->connect();
}

void* DBConnection::allocHandle(uint32_t oracleHandleType)
{
    return imp->allocHandle(oracleHandleType);
}

OCIEnv* DBConnection::getEnvHandle()
{
    return imp->getEnvHandle();
}

OCIError* DBConnection::getErrHandle()
{
    return imp->getErrHandle();
}

OCISvcCtx* DBConnection::getSvcCtxHandle()
{
    return imp->getSvcCtxHandle();
}

}
}
