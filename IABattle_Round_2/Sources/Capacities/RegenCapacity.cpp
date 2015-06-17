#include "RegenCapacity.h"

RegenCapacity::RegenCapacity(int level) : Capacity(level)
{
    computeValue();
}

void RegenCapacity::computeValue()
{
    this->m_value = this->m_level / 1000.f;
}
