#ifndef ACTION_H
#define ACTION_H

#include "Utils/Point.h"
#include "Unit.h"

class Action
{
public:
	Action(Unit * from) : m_from(from) {}
	virtual void execute() = 0;
protected:
	Unit * m_from;
};

class ActionMove : public Action
{
public:
	ActionMove(Unit * from, Point target) : Action(from), m_position(target){}
	Point getPosition(){ return m_position; }
	void execute();
private:
	Point m_position;
};

class ActionShoot : public Action
{
public:
	ActionShoot(Unit * from, Unit * target) : Action(from), m_target(target){}
	Unit * getTarget(){ return m_target; }
	void execute();
private:
	Unit * m_target;
};

#endif //ACTION_H
