/*
 * main.cpp
 *
 *  Created on: Nov 12, 2013
 *      Author: shawn
 */

#include <oci.h>
#include <iostream>
#include <boost/program_options.hpp>

#include "OCIException.h"
#include "DBConnection.h"
#include "DBStatement.h"
#include "DBDataReader.h"
#include "DBValueString.h"

namespace po = boost::program_options;
const std::string STMT = "select name, name from v$tablespace";

namespace ow = prophetstor::ociwrapper;

int main(int argc, char **argv)
{
    po::options_description desc("Allow options");
    desc.add_options()
                        ("help", "show this help message")
                        ("user", po::value<std::string>(), "user name")
                        ("pass", po::value<std::string>(), "password")
                        ("conn", po::value<std::string>(), "connection string");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        std::cout << desc << std::endl;
        return 1;
    }

    if (!vm.count("user") || !vm.count("pass") || !vm.count("conn"))
    {
        std::cout << "Missing options!" << std::endl
                << desc << std::endl;

        return 1;
    }

    std::string user = vm["user"].as<std::string>();
    std::string password = vm["pass"].as<std::string>();
    std::string conn_string = vm["conn"].as<std::string>();

    try{
        ow::DBConnection conn(user, password, conn_string);
        conn.connect();

        ow::DBStatement stmt(conn, STMT.c_str());
        stmt.prepare();
        ow::DBDataReader* reader = stmt.executeReader();

        while (reader->hasNext())
        {
            std::cout << reader->get(1).getString() << "\t" << reader->get(2).getString() << std::endl;
        }
    }
    catch (const ow::OCIException& ex)
    {
        std::cout << ex.what() << std::endl;
    }

    return 0;
}

#if 0
int main(int argc, char** argv)
{
    po::options_description desc("Allow options");
    desc.add_options()
            ("help", "show this help message")
            ("user", po::value<std::string>(), "user name")
            ("pass", po::value<std::string>(), "password")
            ("conn", po::value<std::string>(), "connection string");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        std::cout << desc << std::endl;
        return 1;
    }

    if (!vm.count("user") || !vm.count("pass") || !vm.count("conn"))
    {
        std::cout << "Missing options!" << std::endl
                << desc << std::endl;

        return 1;
    }

    std::string user = vm["user"].as<std::string>();
    std::string password = vm["pass"].as<std::string>();
    std::string conn_string = vm["conn"].as<std::string>();
    ub4 sessionMode = OCI_DEFAULT;

    OCIEnv *env = NULL;
    OCIError *err = NULL;
    OCIServer *server = NULL;
    OCISession *session = NULL;
    OCISvcCtx *svcctx = NULL;
    OCIStmt *stmt = NULL;
    OCIDefine* def = NULL;

    char val1[10][30] = { {0} };
    char val2[10][30] = { {0} };

    ub4 nrow = 0;
    ub4 sizeof_nrow = 0;

    sword ret;

    ret = OCIEnvCreate(&env, OCI_THREADED, NULL, NULL, NULL, NULL, 0, NULL);
    if (ret != OCI_SUCCESS)
    {
        std::cout << "OCIEnvCreate: " << ret << std::endl;
        goto go_exit;
    }

    ret = OCIHandleAlloc(env, (void**)&err, OCI_HTYPE_ERROR, 0, NULL);
    if (ret != OCI_SUCCESS)
    {
        std::cout << "OCIHandleAlloc Err: " << ret << std::endl;
        goto go_exit;
    }

    ret = OCIHandleAlloc(env, (void**)&server, OCI_HTYPE_SERVER, 0, NULL);
    if (ret != OCI_SUCCESS)
    {
        std::cout << "OCIHandleAlloc Server: " << ret << std::endl;
        goto go_exit;
    }

    ret = OCIHandleAlloc(env, (void**)&svcctx, OCI_HTYPE_SVCCTX, 0, NULL);
    if (ret != OCI_SUCCESS)
    {
        std::cout << "OCIHandleAlloc svcctx: " << ret << std::endl;
        goto go_exit;
    }

    ret = OCIServerAttach(server, err, (const OraText*)conn_string.c_str(), conn_string.length(), OCI_DEFAULT);
    if (ret != OCI_SUCCESS)
    {
        std::cout << "OCIServerAttach " << ret << std::endl;
        goto go_exit;
    }


    ret = OCIHandleAlloc(env, (void**)&session, OCI_HTYPE_SESSION, 0, NULL);
    if (ret != OCI_SUCCESS)
    {
        std::cout << "OCIHandleAlloc Session: " << ret << std::endl;
        goto go_exit;
    }

    ret = OCIAttrSet((void*)svcctx, OCI_HTYPE_SVCCTX, server, 0, OCI_ATTR_SERVER, err);
    if (ret != OCI_SUCCESS)
    {
        std::cout << "attach server to svcCtx: " << ret << std::endl;
        goto go_exit;
    }

    ret = OCIAttrSet((void*)session, OCI_HTYPE_SESSION, (void*)user.c_str(), (ub4)user.length(), OCI_ATTR_USERNAME, err);
    if (ret != OCI_SUCCESS)
    {
        std::cout << "attach user name to svcCtx: " << ret << std::endl;
        goto go_exit;
    }

    ret = OCIAttrSet((void*)session, OCI_HTYPE_SESSION, (void*)password.c_str(), (ub4)password.length(), OCI_ATTR_PASSWORD, err);
    if (ret != OCI_SUCCESS)
    {
        std::cout << "attach password to svcCtx: " << ret << std::endl;
        goto go_exit;
    }



    if (user.find("sys") != std::string::npos ||
        user.find("SYS") != std::string::npos)
        sessionMode = OCI_SYSDBA;

    ret = OCISessionBegin(svcctx, err, session, OCI_CRED_RDBMS, sessionMode);
    if (ret != OCI_SUCCESS)
    {
        std::cout << "OCISessionBegin: " << ret << std::endl;
        goto go_exit;
    }

    ret = OCIAttrSet(svcctx, OCI_HTYPE_SVCCTX, session, 0, OCI_ATTR_SESSION, err);
    if (ret != OCI_SUCCESS)
    {
        std::cout << "OCIAttrSet session: " << ret << std::endl;
        goto go_exit;
    }




    ret = OCIHandleAlloc(env, (void**)&stmt, OCI_HTYPE_STMT, 0, NULL);
    if (ret != OCI_SUCCESS)
    {
        std::cout << "alloc stmt handle: " << ret << std::endl;
        goto go_exit;
    }



    //ret = OCIStmtPrepare(stmt, err, (const OraText*)STMT.c_str(), (ub4)STMT.length(), 0, OCI_DEFAULT);
    OCIStmtPrepare2(svcctx, &stmt, err, (const OraText*)STMT.c_str(), (ub4)STMT.length(), NULL, 0, OCI_NTV_SYNTAX, OCI_DEFAULT);
    if (ret != OCI_SUCCESS)
    {
        std::cout << "OCIStmtPrepare: " << ret << std::endl;
        goto go_exit;
    }

    ret = OCIDefineByPos(stmt, &def, err, 1,  val1, sizeof(val1[0]), SQLT_STR, NULL, NULL, NULL, OCI_DEFAULT);
    if (ret != OCI_SUCCESS)
    {
        std::cout << "OCIDefineByPos 1: " << ret << std::endl;
        goto go_exit;
    }

    ret = OCIDefineByPos(stmt, &def, err, 2,  val2, sizeof(val2[0]), SQLT_STR, NULL, NULL, NULL, OCI_DEFAULT);
    if (ret != OCI_SUCCESS)
    {
        std::cout << "OCIDefineByPos 2: " << ret << std::endl;
        goto go_exit;
    }

    ret = OCIStmtExecute(svcctx, stmt, err,  0, 0, NULL, NULL, OCI_DEFAULT);
    if (ret != OCI_SUCCESS)
    {
        std::cout << "OCIStmtExecute: " << ret << std::endl;
        goto go_exit;
    }

    ret = OCIStmtFetch2(stmt, err, 10, OCI_DEFAULT, 0, OCI_DEFAULT);
    if (ret != OCI_SUCCESS && ret != OCI_NO_DATA)
    {
        std::cout << "OCIStmtFetch2: " << ret << std::endl;
        goto go_exit;
    }

    sizeof_nrow = sizeof(nrow);
    ret = OCIAttrGet(stmt, OCI_HTYPE_STMT, &nrow, &sizeof_nrow, OCI_ATTR_ROWS_FETCHED, err);
    if (ret != OCI_SUCCESS)
    {
        std::cout << "get OCI_ATTR_ROWS_FETCHED: " << ret << std::endl;
        goto go_exit;
    }

    for(int i = 0; i<(int)nrow; i++)
    {
        std::cout << val1[i] << " : " << val2[i] << std::endl;
    }








go_exit:

    if (ret != OCI_SUCCESS)
    {
        sb4 errCode;
        OraText errText[2048] = {0};
        OCIErrorGet(err, 1, NULL, &errCode, &errText[0], sizeof(errText), OCI_HTYPE_ERROR);
        std::cout << errText << std::endl;
    }

    if (env != NULL)
        OCIHandleFree(env, OCI_HTYPE_ENV);

    return ret;
}
#endif
