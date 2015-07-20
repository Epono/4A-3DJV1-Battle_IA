#include "UnitAI.h"

Action& UnitAI::operator()(Unit * u, Army * ally, Army * ennemy) const
{
	Action *action = nullptr;
	Point * uPosition = &(u->getPosition());

	if (u->getWeaponSpeed().getValue() == 0) // If we can shoot
	{
		Unit *uEnnemy = nullptr;
		switch (u->getAICode()) //Choice of the target to shoot
		{
		case LD:
			uEnnemy = &(ennemy->getNearestUnit(*uPosition));
			break;
		case HD:
			uEnnemy = &(ennemy->getFurthestUnit(*uPosition));
			break;
		case L0:
			uEnnemy = &(ennemy->getLowestUnit(0));
			break;
		case H0:
			uEnnemy = &(ennemy->getHighestUnit(0));
			break;
		case L1:
			uEnnemy = &(ennemy->getLowestUnit(1));
			break;
		case H1:
			uEnnemy = &(ennemy->getHighestUnit(1));
			break;
		case L2:
			uEnnemy = &(ennemy->getLowestUnit(2));
			break;
		case H2:
			uEnnemy = &(ennemy->getHighestUnit(2));
			break;
		case L3:
			uEnnemy = &(ennemy->getLowestUnit(3));
			break;
		case H3:
			uEnnemy = &(ennemy->getHighestUnit(3));
			break;
		case L4:
			uEnnemy = &(ennemy->getLowestUnit(4));
			break;
		case H4:
			uEnnemy = &(ennemy->getHighestUnit(4));
			break;
		case L5:
			uEnnemy = &(ennemy->getLowestUnit(5));
			break;
		case H5:
			uEnnemy = &(ennemy->getHighestUnit(4));
			break;
		case L6:
			uEnnemy = &(ennemy->getLowestUnit(6));
			break;
		case H6:
			uEnnemy = &(ennemy->getHighestUnit(6));
			break;
		default:
			throw std::string("Unexpected AICode.");
		}
		if (Point::getDistance(*uPosition, uEnnemy->getPosition()) > u->getWeaponRange().getValue()) //Move to the target
			action = new ActionMove(u, uPosition->getNextPosWithSpeed(uEnnemy->getPosition(), u->getSpeed().getValue()));
		else
			action = new ActionShoot(u, uEnnemy);
	}
	else
		action = new ActionMove(u, uPosition->getNextPosWithSpeed(ennemy->getNearestUnit(*uPosition).getPosition() , -(u->getSpeed().getValue())));

	return *action;
}