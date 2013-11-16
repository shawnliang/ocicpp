#include "oci.h"
#include "DBColumnDefinition.h"
#include "OCIException.h"

ub2 prophetstor::ociwrapper::DBColumnDefinition::getWidth()
{
    if (width_ == 0)
    {
        sword ret;

        ret = OCIAttrGet(column_, OCI_DTYPE_PARAM, &isText_, NULL, OCI_ATTR_CHAR_USED, err_);
        if (ret != OCI_SUCCESS)
            throw OCIException("Get column is_char failed", ret, err_);

        if (isText_)
            /* Retrieve the column width in characters */
            OCIAttrGet(column_, OCI_DTYPE_PARAM, &width_, NULL, OCI_ATTR_CHAR_SIZE, err_);
        else
            /* Retrieve the column width in bytes */
            OCIAttrGet(column_, OCI_DTYPE_PARAM, &width_, NULL, OCI_ATTR_DATA_SIZE, err_);
    }

    return width_;
}

bool prophetstor::ociwrapper::DBColumnDefinition::isText()
{
    if (width_ == 0)
        getWidth();

    return isText_;
}

const std::string& prophetstor::ociwrapper::DBColumnDefinition::getName()
{
    if (name_.empty())
    {
        char* columnName;
        int   columnNameLen;

        sword ret = OCIAttrGet(column_, OCI_DTYPE_PARAM, &columnName, (ub4*)&columnNameLen, OCI_ATTR_NAME, err_);
        if (ret != OCI_SUCCESS)
            throw OCIException("Get column name failed", ret, err_);

        name_.assign(columnName, columnNameLen);
    }

    return name_;
}

ub2 prophetstor::ociwrapper::DBColumnDefinition::getDataType()
{
    if (datatype_ == 0)
    {
        sword ret = OCIAttrGet(column_, OCI_DTYPE_PARAM, (void*)&datatype_, NULL, OCI_ATTR_DATA_TYPE, err_);
        if (ret != OCI_SUCCESS)
            throw OCIException("Get column data type failed", ret, err_);
    }

    return datatype_;
}
