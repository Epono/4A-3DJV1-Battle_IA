#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <random>
#include <vector>
#include "Unit.h"

unsigned int Unit::nbUnits = 0;

Unit::Unit() : m_unitId(++nbUnits) {

}

Unit::Unit(int levelTotal) : m_unitId(++nbUnits)
{
	std::random_device rd;
	std::default_random_engine e1(rd());
	std::uniform_int_distribution<> uniform_dist(0.f, 100.f);
	
	m_unitPosition = Point(uniform_dist(e1), uniform_dist(e1));

	uniform_dist = std::uniform_int_distribution<>(0, 15);
	m_AICode = static_cast<AICode>(uniform_dist(e1)); //Generate the IACode randomly
	
	m_capacities[0] = new SpeedCapacity(0);
	m_capacities[1] = new HealthCapacity(0);
	m_capacities[2] = new ArmorCapacity(0);
	m_capacities[3] = new RegenCapacity(0);
	m_capacities[4] = new WeaponDamageCapacity(0);
	m_capacities[5] = new WeaponRangeCapacity(0);
	m_capacities[6] = new WeaponSpeedCapacity(0);
	// Fill the vector for the random indexes
	uniform_dist = std::uniform_int_distribution<>(0, 6);
	for (int i = 0; i < levelTotal; ++i)
		m_capacities[uniform_dist(e1)]->upgrade();
}

Unit::Unit(AICode AICode
           , int speedLevel
           , int healthLevel
           , int armorLevel
           , int regenLevel
           , int weaponDamageLevel
           , int weaponRangeLevel
           , int weaponSpeedLevel
		   ) : m_unitId(++nbUnits), m_AICode(AICode)
{
	std::random_device rd;
	std::default_random_engine e1(rd());
	std::uniform_int_distribution<> uniform_dist(0.f, 100.f);

	Point m_unitPosition(uniform_dist(e1), uniform_dist(e1));

    m_capacities[0] = new SpeedCapacity(speedLevel);
    m_capacities[1] = new HealthCapacity(healthLevel);
    m_capacities[2] = new ArmorCapacity(armorLevel);
    m_capacities[3] = new RegenCapacity(regenLevel);
    m_capacities[4] = new WeaponDamageCapacity(weaponDamageLevel);
    m_capacities[5] = new WeaponRangeCapacity(weaponRangeLevel);
    m_capacities[6] = new WeaponSpeedCapacity(weaponSpeedLevel);
}

Unit::Unit(const Unit& unit) : m_unitId(++nbUnits) 
{
	m_AICode = unit.m_AICode;
	m_unitPosition = unit.m_unitPosition;

	m_capacities[0] = new SpeedCapacity(unit.getSpeed().getLevel());
	m_capacities[1] = new HealthCapacity(unit.getHealth().getLevel());
	m_capacities[2] = new ArmorCapacity(unit.getArmor().getLevel());
	m_capacities[3] = new RegenCapacity(unit.getRegen().getLevel());
	m_capacities[4] = new WeaponDamageCapacity(unit.getWeaponDamage().getLevel());
	m_capacities[5] = new WeaponRangeCapacity(unit.getWeaponRange().getLevel());
	m_capacities[6] = new WeaponSpeedCapacity(unit.getWeaponSpeed().getLevel());
}

//Unit::~Unit() 
//{
//	for(int i = 0; i < 7; ++i)
//		delete m_capacities[i];
//}

Capacity& Unit::operator[](unsigned int i) const
{
	try
	{
		return dynamic_cast<Capacity&>(*m_capacities[i]);
	}
	catch (const std::out_of_range& oor)
	{
		std::cerr << "Out of range error: " << oor.what() << std::endl;
	}
}

int Unit::getLevel() const
{
    int levelTotal = 0;

    for(int i = 0; i < 7; i++)
    {
        levelTotal += m_capacities[i]->getLevel();
    }

    return levelTotal;
}

void Unit::refresh()
{
    HealthCapacity& health = this->getHealth();
    RegenCapacity& regen = this->getRegen();
    WeaponSpeedCapacity& weaponSpeed = this->getWeaponSpeed();

    //Life value can't be more than the max health
	health.setValue(std::min(health.getValue() + regen.getValue(), health.getValueMax()));

    if(weaponSpeed.getValue() > 0)
        weaponSpeed.setValue(weaponSpeed.getValue() - 1);
}

bool Unit::shoot()
{
    WeaponSpeedCapacity& weaponSpeed = this->getWeaponSpeed();
    if(weaponSpeed.getValue() == 0)
    {
        weaponSpeed.setValue(weaponSpeed.getValueMax());
        return true;
    }
    return false;
}

void Unit::takeDamage(float value)
{
	float damageTaken = std::max(1.0f, value - this->getArmor().getValue());
    float newHealth = this->getHealth().getValue() - damageTaken;
    this->operator[](1).setValue(newHealth);
}

bool Unit::isAlive() const
{
    return this->getHealth().getValue() > 0;
}

Unit& Unit::mutate() const 
{
	Unit* newUnit = new Unit(*this);

	int capacityToDecrease = -1;
	int capacityToDecreaseLevel = 0;

	while(capacityToDecrease == -1) {
		int tempCapacityToDecrease = std::rand() % 7;
		int tempCapacityToDecreaseLevel = m_capacities[tempCapacityToDecrease]->getValue();
		if(tempCapacityToDecreaseLevel > 0) {
			capacityToDecrease = tempCapacityToDecrease;
			capacityToDecreaseLevel = tempCapacityToDecreaseLevel;
		}
	}

	int capacityToIncrease = -1;

	while(capacityToIncrease == -1) {
		int tempCapacityToIncrease = std::rand() % 7;
		if(tempCapacityToIncrease != capacityToDecrease) {
			capacityToIncrease = tempCapacityToIncrease;
		}
	}

	int randomMutation = std::rand() % capacityToDecreaseLevel;

	for(int i = 0; i < randomMutation; i++) {
		newUnit->m_capacities[capacityToDecrease]->downgrade();
		newUnit->m_capacities[capacityToIncrease]->upgrade();
	}

	return *newUnit;
}

Unit& Unit::operator*(const Unit& unit) const {
	Unit* newUnit = new Unit(0);

	newUnit->m_AICode = std::rand() % 2 == 0 ? this->m_AICode : unit.m_AICode;

	int newUnitCapacityPoints = 0;
	int otherUnitLevel = unit.getLevel();
	int currentUnitLevel = this->getLevel();

	if(otherUnitLevel == currentUnitLevel) {
		newUnitCapacityPoints = currentUnitLevel;
	}
	else {
		int levelDifference = abs(otherUnitLevel - currentUnitLevel);
		int minimumLevel = std::min(otherUnitLevel, currentUnitLevel);
		newUnitCapacityPoints = (std::rand() % levelDifference) + minimumLevel;
	}

	//newUnit->m_capacities[0] = new SpeedCapacity(0);
	//newUnit->m_capacities[1] = new HealthCapacity(0);
	//newUnit->m_capacities[2] = new ArmorCapacity(0);
	//newUnit->m_capacities[3] = new RegenCapacity(0);
	//newUnit->m_capacities[4] = new WeaponDamageCapacity(0);
	//newUnit->m_capacities[5] = new WeaponRangeCapacity(0);
	//newUnit->m_capacities[6] = new WeaponSpeedCapacity(0);

	while(newUnitCapacityPoints > 0) {
		int capacityID = std::rand() % 7;

		int myCapacityLevel = this->m_capacities[capacityID]->getLevel();
		int otherUnitCapacityLevel = unit.m_capacities[capacityID]->getLevel();
		int newUnitCapacityLevel = newUnit->m_capacities[capacityID]->getLevel();

		int capacityMaximumLevel = std::max(myCapacityLevel, otherUnitCapacityLevel);

		if(newUnitCapacityLevel < capacityMaximumLevel) {
			newUnit->m_capacities[capacityID]->upgrade();
			newUnitCapacityPoints--;
		}
	}

	return *newUnit;
}
