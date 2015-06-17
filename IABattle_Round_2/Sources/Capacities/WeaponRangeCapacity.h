#ifndef WEAPONRANGECAPACITY_H
#define WEAPONRANGECAPACITY_H

#include "Capacity.h"

class WeaponRangeCapacity : public Capacity
{
    public:
        WeaponRangeCapacity(int);
        void computeValue();
};

#endif // WEAPONRANGECAPACITY_H
