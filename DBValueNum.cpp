#include "DBValueNum.h"
#include <stdio.h>

namespace prophetstor
{
namespace ociwrapper
{

DBValueNum::DBValueNum()
    :num_(0)
{
}

DBValueNum::~DBValueNum()
{
}

void* DBValueNum::getBuffer()
{
    return &num_;
}

int DBValueNum::getBufferSize() const
{
    return sizeof(num_);
}

std::string DBValueNum::getString() const
{
    char buf[128] = {0};
    snprintf(buf, sizeof(buf), "%ld", num_);

    return buf;
}

long DBValueNum::getLong() const
{
    return num_;
}


} /* namespace ociwrapper */
} /* namespace prophetstor */
