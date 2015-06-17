#include "Army.h"
#include <fstream>
#include <iostream>
#include <algorithm>

unsigned int Army::nbArmies = 0;

Army::Army() : m_armyId(++nbArmies), m_score(0) 
{
}

Army::Army(int nbUnit, int levelUnit) : m_armyId(++nbArmies), m_score(0)
{
    for(int i = 0; i < nbUnit; i++)
        this->m_unitList.push_back(new Unit(levelUnit));
}

Army::Army(const std::vector<Unit*>& unitList) :m_armyId(++nbArmies), m_unitList(unitList), m_score(0)
{
}

Army::Army(const Army& army) : m_armyId(army.m_armyId), m_score(army.m_score)
{
	for(int i = 0; i < army.m_unitList.size(); i++) {
		this->m_unitList.push_back(new Unit(*army.m_unitList[i]));
	}
}

//Army::~Army() 
//{
//	for(auto it = this->m_unitList.begin(); it != this->m_unitList.end(); ++it)
//	{
//		delete *it;
//	}
//}

Unit& Army::getUnit(int idUnit) 
{
	for(auto it = this->m_unitList.begin(); it != this->m_unitList.end(); ++it)
		if((*it)->getId() == idUnit)
			return **it;

	throw std::runtime_error("ID not in list.");
}

Unit& Army::getConstUnit(int idUnit) const{
	for(auto it = this->m_unitList.begin(); it != this->m_unitList.end(); ++it)
		if((*it)->getId() == idUnit)
			return **it;

	throw std::runtime_error("ID not in list.");
}

Unit& Army::getRandomUnit() const
{
	return *(m_unitList.at(std::rand() % size()));
}

Unit& Army::getNearestUnit(const Point& p) const
{
    Unit *u = this->m_unitList.front();
    double dist = Point::getDistance(u->getPosition(), p);

	for (auto it = this->m_unitList.begin(); it != this->m_unitList.end(); ++it)
	{
		double tmp;
		if ((tmp = Point::getDistance((*it)->getPosition(), p)) < dist)
		{
			dist = tmp;
			u = *it;
		}
	}
    return *u;
}

Unit& Army::getFurthestUnit(const Point& p) const
{
    Unit * u = this->m_unitList.front();
    double dist = Point::getDistance(u->getPosition(), p);

	for (auto it = this->m_unitList.begin(); it != this->m_unitList.end(); ++it)
	{
		double tmp;
		if ((tmp = Point::getDistance((*it)->getPosition(), p)) > dist)
		{
			dist = tmp;
			u = *it;
		}
	}
    return *u;
}

Unit& Army::getLowestUnit(int capacityIndex) const
{
    Unit * u = this->m_unitList.front();
	int level = u->operator[](capacityIndex).getLevel();

	for (auto it = this->m_unitList.begin(); it != this->m_unitList.end(); ++it)
	{
		double tmp;
		if ((tmp = (*it)->operator[](capacityIndex).getLevel()) < level)
		{
			level = tmp;
			u = *it;
		}
	}
    return *u;
}

Unit& Army::getHighestUnit(int capacityIndex) const
{
    Unit * u = this->m_unitList.front();
    int level = u->operator[](capacityIndex).getLevel();

	for (auto it = this->m_unitList.begin(); it != this->m_unitList.end(); ++it)
	{
		double tmp;
		if ((tmp = (*it)->operator[](capacityIndex).getLevel()) > level)
		{
			level = tmp;
			u = *it;
		}
	}
    return *u;
}

void Army::purge()
{
	for (auto it = this->m_unitList.begin(); it != this->m_unitList.end();)
	{
		if (!(*it)->isAlive())
			it = m_unitList.erase(it);
		else
			it++;
	}
}

int  Army::getGlobalLevel() const {
	int globalLevel = 0;

	for each (Unit* unit in m_unitList) {
		globalLevel += unit->getLevel();
	}

	return globalLevel;
}

void Army::saveArmy() const {
	std::string fileName = "army_" + std::to_string(size()) + "_" + std::to_string(getGlobalLevel()) + ".save";

	std::ofstream armySaveFile(fileName, std::ios::out | std::ios::trunc);

	if(!armySaveFile) {
		throw 1;
	}
	else {
		for each (Unit* unit in m_unitList) {
			armySaveFile << unit->getSpeed().getLevel() << " " << unit->getHealth().getLevel() << " " << unit->getArmor().getLevel() << " " << unit->getRegen().getLevel() << " " << unit->getWeaponDamage().getLevel() << " " << unit->getWeaponRange().getLevel() << " " << unit->getWeaponSpeed().getLevel() << " " << unit->getAICode() << " " << std::endl;
		}
		armySaveFile.close();
	}
}

Army& Army::mutate() const {
	Army* newArmy = new Army(*this);
	newArmy->m_armyId = ++nbArmies;
	int armyLength = newArmy->m_unitList.size();

	int rand1 = 0;
	int rand2 = 0;

	while(rand1 == rand2) {
		rand1 = std::rand() % armyLength;
		rand2 = std::rand() % armyLength;
		//std::cout << rand1 << " - " << rand2 << std::endl;
		//std::cout << armyLength << std::endl;
	}

	int startMutation = std::min(rand1, rand2);
	int endMutation = std::max(rand1, rand2);

	for(int i = startMutation; i < endMutation; i++) {
		newArmy->m_unitList[i] = new Unit(newArmy->m_unitList[i]->mutate());
	}
	
	return *newArmy;
}

Army& Army::operator*(const Army& army) const{
	Army* newArmy = new Army();
	int newArmySize = this->size();

	int numberUnitMutation = std::rand() % newArmySize;
	for(int i = 0; i < numberUnitMutation; i++) {
		newArmy->m_unitList.push_back(new Unit(this->getRandomUnit() * army.getRandomUnit()));
	}

	int numberUnitCopy = newArmySize - numberUnitMutation;
	while(numberUnitCopy > 0) {
		if(std::rand() % 2) {
			newArmy->m_unitList.push_back(new Unit(*(this->m_unitList[std::rand() % newArmySize])));
		}
		else {
			newArmy->m_unitList.push_back(new Unit(*(army.m_unitList[std::rand() % newArmySize])));
		}
		numberUnitCopy--;
	}

	return *newArmy;
}

bool Army::operator>(const Army& otherArmy) const {
	return (this->m_score > otherArmy.m_score);
}

bool Army::operator<(const Army& otherArmy) const {
	return (this->m_score < otherArmy.m_score);
}

//Army& Army::operator=(const Army& army) {
//	for(int i = 0; i < army.m_unitList.size(); i++) {
//		this->m_unitList.push_back(new Unit(*army.m_unitList[i]));
//	}
//}