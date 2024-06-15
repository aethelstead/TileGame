#ifndef UID_H
#define UID_H

#include <cstdint>

using UniqueID = uint64_t;

class UniqueIDGenerator
{
public:
    static UniqueID Next();

private:
    static UniqueID m_prevID;
};

#endif
