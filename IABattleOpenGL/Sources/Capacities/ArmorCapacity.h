#ifndef ARMORCAPACITY_H
#define ARMORCAPACITY_H

#include "Capacity.h"

class ArmorCapacity : public Capacity
{
    public:
        ArmorCapacity(int);
        void computeValue();
};

#endif // ARMORCAPACITY_H
