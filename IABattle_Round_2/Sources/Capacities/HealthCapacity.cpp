#include "HealthCapacity.h"

HealthCapacity::HealthCapacity(int level) : Capacity(level)
{
	this->m_value = -1.f;
    computeValue();
    this->m_valueMax = this->m_value;
}

void HealthCapacity::computeValue()
{
	float healthPercentage = 0;

	if (this->m_value != -1.f)
		healthPercentage = this->m_value / this->m_valueMax;
	else
		healthPercentage = 1;
	this->m_valueMax = (1.f + this->m_level) * 10.f;
	this->m_value = healthPercentage * this->m_valueMax;
}
