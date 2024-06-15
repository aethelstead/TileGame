#include "UniqueID.h"

UniqueID UniqueIDGenerator::m_prevID = UINT64_MAX;

UniqueID UniqueIDGenerator::Next()
{
    return --m_prevID;
}
