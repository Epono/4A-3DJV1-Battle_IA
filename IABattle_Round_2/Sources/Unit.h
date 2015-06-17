#ifndef UNIT_H
#define UNIT_H

#include <string>
#include "Capacities/SpeedCapacity.h"
#include "Capacities/HealthCapacity.h"
#include "Capacities/ArmorCapacity.h"
#include "Capacities/RegenCapacity.h"
#include "Capacities/WeaponDamageCapacity.h"
#include "Capacities/WeaponRangeCapacity.h"
#include "Capacities/WeaponSpeedCapacity.h"
#include "Utils/Point.h"

enum AICode { LD, HD, L0, H0, L1, H1, L2, H2, L3, H3, L4, H4, L5, H5, L6, H6 };

class Unit
{
    public:
		Unit();
        Unit(int);
        Unit(AICode, int, int, int, int, int, int, int);
		Unit(const Unit& unit);
		//~Unit();

		//Attributes Getters
        int getId() const { return m_unitId; }
        Point getPosition() const { return m_unitPosition; }
        int getLevel() const;
		AICode getAICode() const { return m_AICode; }
        //Capacity Getters
        SpeedCapacity& getSpeed() const { return dynamic_cast<SpeedCapacity&>(*m_capacities[0]); }
        HealthCapacity& getHealth() const { return dynamic_cast<HealthCapacity&>(*m_capacities[1]); }
        ArmorCapacity& getArmor() const { return dynamic_cast<ArmorCapacity&>(*m_capacities[2]); }
        RegenCapacity& getRegen() const { return dynamic_cast<RegenCapacity&>(*m_capacities[3]); }
        WeaponDamageCapacity& getWeaponDamage() const { return dynamic_cast<WeaponDamageCapacity&>(*m_capacities[4]); }
        WeaponRangeCapacity& getWeaponRange() const { return dynamic_cast<WeaponRangeCapacity&>(*m_capacities[5]); }
        WeaponSpeedCapacity& getWeaponSpeed() const { return dynamic_cast<WeaponSpeedCapacity&>(*m_capacities[6]); }

        void setPosition(Point& p) { m_unitPosition = p; };

        Capacity& operator[](unsigned int) const;

        void refresh();
        bool shoot();
        void takeDamage(float);
        bool isAlive() const;

		Unit& mutate() const;
		Unit& operator*(const Unit& unit) const;
    private:
        static unsigned int nbUnits;
        unsigned int m_unitId;
        Capacity *m_capacities[7];
        Point m_unitPosition;
		AICode m_AICode;
};

#endif // UNIT_H
