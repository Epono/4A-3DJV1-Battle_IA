#ifndef SPEEDCAPACITY_H
#define SPEEDCAPACITY_H

#include "Capacity.h"

class SpeedCapacity : public Capacity
{
    public:
        SpeedCapacity(int);
        void computeValue();
};

#endif // SPEEDCAPACITY_H
