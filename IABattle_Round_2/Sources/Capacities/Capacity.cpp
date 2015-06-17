#include "Capacity.h"

Capacity::Capacity(int level) : m_value(0.f), m_level(level) {}

void Capacity::upgrade()
{
    m_level++;
    computeValue();
}

void Capacity::downgrade()
{
    //On ne peut pas avoir une compétence en dessous du niveau 0
    if(m_level > 0)
    {
        m_level--;
        computeValue();
    }
}
