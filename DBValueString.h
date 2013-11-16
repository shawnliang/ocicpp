#ifndef DBVALUESTRING_H_
#define DBVALUESTRING_H_

namespace prophetstor
{
namespace ociwrapper
{

class DBValueString
{
public:
    DBValueString(int maxSize);
    virtual ~DBValueString();

    void* getBuffer();
    int getBufferSize() const;

    std::string getString() const;

private:
    char* buffer_;
    int bufferSize_;
};

} /* namespace ociwrapper */
} /* namespace prophetstor */

#endif /* DBVALUESTRING_H_ */
