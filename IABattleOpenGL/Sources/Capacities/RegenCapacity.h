#ifndef REGENCAPACITY_H
#define REGENCAPACITY_H

#include "Capacity.h"

class RegenCapacity : public Capacity
{
    public:
        RegenCapacity(int);
        void computeValue();
};

#endif // REGENCAPACITY_H
