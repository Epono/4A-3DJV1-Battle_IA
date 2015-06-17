#ifndef WEAPONSPEEDCAPACITY_H
#define WEAPONSPEEDCAPACITY_H

#include "Capacity.h"

class WeaponSpeedCapacity : public Capacity
{
    public:
        WeaponSpeedCapacity(int);
        void computeValue();
        unsigned int getValueMax() const { return m_valueMax; }
    private:
        unsigned int m_valueMax;
};

#endif // WEAPONSPEEDCAPACITY_H
