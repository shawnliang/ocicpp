#include <string.h>
#include <string>
#include "DBValueString.h"
#include "OCIException.h"

namespace prophetstor
{
namespace ociwrapper
{

DBValueString::DBValueString(int maxSize)
    : buffer_(new char[maxSize]), bufferSize_(maxSize)
{
    bzero(buffer_, maxSize);
}

void* DBValueString::getBuffer()
{
    return buffer_;
}

int DBValueString::getBufferSize() const
{
    return bufferSize_;
}

std::string DBValueString::getString() const
{
    return buffer_;
}

long DBValueString::getLong() const
{
    throw OCIException("DBValueString::getLong() not implemented", -1);
}

DBValueString::~DBValueString()
{
    delete buffer_;
}

} /* namespace ociwrapper */
} /* namespace prophetstor */
