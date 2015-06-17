#include "WeaponDamageCapacity.h"

WeaponDamageCapacity::WeaponDamageCapacity(int level) : Capacity(level)
{
    computeValue();
}

void WeaponDamageCapacity::computeValue()
{
    this->m_value = (1 + this->m_level) * 1.5;
}
