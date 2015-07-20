#include "ArmorCapacity.h"

ArmorCapacity::ArmorCapacity(int level) : Capacity(level)
{
    computeValue();
}

void ArmorCapacity::computeValue()
{
    this->m_value = this->m_level / 2;
}
