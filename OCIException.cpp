#include <string>
#include <string.h>
#include <stdio.h>
#include "OCIException.h"

prophetstor::ociwrapper::OCIException::OCIException(const char* msg, int status, OCIError* err)
    : errorId_(0)
{
    bzero(buff_, sizeof(buff_));

    if (err != NULL)
    {
        char ocimsg[256] = {0};
        sb4 errCode = 0;
        OCIErrorGet(err, 1, NULL, &errorId_, (OraText*)ocimsg, sizeof(ocimsg), OCI_HTYPE_ERROR);

        snprintf(buff_, sizeof(buff_), "%s (%s) %s", msg, getStatusString(status), ocimsg);
    }
    else
    {
        strncpy(buff_, msg, sizeof(buff_));
        buff_[sizeof(buff_)-1] = 0;
    }
}

const char* prophetstor::ociwrapper::OCIException::getStatusString(int status)
{
    switch (status)
    {
    case OCI_SUCCESS:
        return "Success";
    case OCI_SUCCESS_WITH_INFO:
        return "OCI_SUCCESS_WITH_INFO";
    case OCI_NEED_DATA:
        return "OCI_NEED_DATA";
    case OCI_NO_DATA:
        return "OCI_NODATA";
    case OCI_ERROR:
        return "OCI_ERROR";
    case OCI_INVALID_HANDLE:
        return "OCI_INVALID_HANDLE";
    case OCI_STILL_EXECUTING:
        return "OCI_STILL_EXECUTE";
    case OCI_CONTINUE:
        return "OCI_CONTINUE";
    default:
        return "..unknown..";
    }
}
