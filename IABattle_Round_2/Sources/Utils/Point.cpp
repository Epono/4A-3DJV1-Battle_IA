#include "Point.h"
#include <ostream>
#include <iomanip>
#include <cmath>



Point::Point() : m_x(0), m_y(0) {}

Point::Point(float x, float y) : m_x(x), m_y(y)
{}

Point Point::getNextPosWithSpeed(const Point& position, float speed) const
{
	// Return the Position we can go 
	double distance = Point::getDistance(*this, position);
	if (distance > speed)
	{
		float x = fmod((speed / distance) * (position.m_x - this->m_x) + this->m_x, 100.0);
		float y = fmod((speed / distance) * (position.m_y - this->m_y) + this->m_y, 100.0);
		return Point(x, y);
	}
	return Point(position);
}

Point& Point::operator=(const Point& p)
{
	if (this != &p)
	{
		this->m_x = p.m_x;
		this->m_y = p.m_y;
	}
	return *this;
}

std::ostream & operator<<(std::ostream &out, const Point& p)
{
	return out << "(" << std::fixed << std::setprecision(0) << p.getX() << ", " << p.getY() << ")";
}