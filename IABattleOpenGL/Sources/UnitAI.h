#ifndef UNITAI_H
#define UNITAI_H

#include "Action.h"
#include "Army.h"
#include "Unit.h"

class UnitAI
{
public:
	Action& operator()(Unit * u, Army * ally, Army * ennemy) const;
private:
};

#endif // UNITAI_H
