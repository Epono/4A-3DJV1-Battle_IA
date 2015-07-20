#include "Action.h"
#include <iostream>
#include <algorithm>

void ActionMove::execute()
{
	this->m_from->setPosition(m_position);
	//std::cout << "Unite " << m_from->getId() << " (Armee A) bouge en position " << m_position << std::endl;
}

void ActionShoot::execute()
{
	this->m_from->shoot();
	this->m_target->takeDamage(this->m_from->getWeaponDamage().getValue());
	//std::cout << "Unite " << m_from->getId()
	//	<< " (Armee A) attaque Unite " << m_target->getId()
	//	<< " (Armee B) qui n'a plus que " << std::max(m_target->getHealth().getValue(), 0.f) << " HP"
	//	<< std::endl;
}