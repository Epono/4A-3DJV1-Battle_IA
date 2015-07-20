#include "WeaponRangeCapacity.h"

WeaponRangeCapacity::WeaponRangeCapacity(int level) : Capacity(level)
{
    computeValue();
}

void WeaponRangeCapacity::computeValue()
{
    this->m_value = (10 + this->m_level) * 2;
}
