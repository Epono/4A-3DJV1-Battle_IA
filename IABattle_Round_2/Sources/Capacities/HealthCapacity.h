#ifndef HEALTHCAPACITY_H
#define HEALTHCAPACITY_H

#include "Capacity.h"

class HealthCapacity : public Capacity
{
    public:
        HealthCapacity(int);
        void computeValue();
        float getValueMax() const { return m_valueMax; }
    private:
        float m_valueMax;
};

#endif // HEALTHCAPACITY_H
