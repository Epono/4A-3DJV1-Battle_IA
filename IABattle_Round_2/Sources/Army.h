#ifndef ARMY_H
#define ARMY_H

#include <vector>
#include "Unit.h"

class Army
{
public:
	Army();
    Army(int, int);
    Army(const std::vector<Unit*>&);
	Army(const Army& army);
	//~Army();

    std::vector<Unit*> getUnitList() const { return m_unitList; }
	Unit& getUnit(int);
	Unit& getConstUnit(int) const;
    int size() const { return this->m_unitList.size(); }
    Unit& getNearestUnit(const Point&) const;
    Unit& getFurthestUnit(const Point&) const;
    Unit& getLowestUnit(int) const;
    Unit& getHighestUnit(int) const;

	int getGlobalLevel() const;
	void saveArmy() const;

	Army& mutate() const;
	Army& operator*(const Army&) const;

    void purge();

	int getArmyId() const {	return this->m_armyId; };
	int getScore() const { return this->m_score; };
	void setScore(int score) { this->m_score = score; };

	//Army& operator=(const Army& army);
	Unit& getRandomUnit() const;


	bool operator<(const Army&) const;
	bool operator>(const Army&) const;
private:
	static unsigned int nbArmies;
	unsigned int m_armyId;
    std::vector<Unit*> m_unitList;
	int m_score;
};

class FighterWrapper
{
public:
	FighterWrapper(Unit &u, Army &a1, Army &a2)
		: m_fighter(&u), m_allies(&a1), m_ennemies(&a2)
	{}
	FighterWrapper(const FighterWrapper& fw)
		: m_fighter(&(*fw.m_fighter))
		, m_allies(&(*fw.m_allies))
		, m_ennemies(&(*fw.m_ennemies))
	{}
	FighterWrapper& operator=(const FighterWrapper& fw)
	{
		if (this != &fw)
		{
			m_fighter = &(*fw.m_fighter);
			m_allies = &(*fw.m_allies);
			m_ennemies = &(*fw.m_ennemies);
		}
		return *this;
	}
	Unit * m_fighter = nullptr;
	Army * m_allies = nullptr;
	Army * m_ennemies = nullptr;
};

#endif // ARMY_H
