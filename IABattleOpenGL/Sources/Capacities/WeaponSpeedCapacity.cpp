#include "WeaponSpeedCapacity.h"

WeaponSpeedCapacity::WeaponSpeedCapacity(int level) : Capacity(level)
{
	this->m_value = -1;
    computeValue();
}

void WeaponSpeedCapacity::computeValue()
{
    this->m_valueMax = 5 / (1 + this->m_level);

    if(this->m_value == -1.f)
        this->m_value = 0;
    else
    {
        //If the current value of the weapon speed is higher
        //than the maxvalue we set the two at the same value
        if(this->m_valueMax < this->m_value)
            this->m_value = this->m_valueMax;
    }
}
