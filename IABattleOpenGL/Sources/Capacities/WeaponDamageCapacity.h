#ifndef WEAPONDAMAGECAPACITY_H
#define WEAPONDAMAGECAPACITY_H

#include "Capacity.h"

class WeaponDamageCapacity : public Capacity
{
    public:
        WeaponDamageCapacity(int);
        void computeValue();
};

#endif // WEAPONDAMAGECAPACITY_H
