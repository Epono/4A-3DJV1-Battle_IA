#include "Point.h"
#include <ostream>
#include <iomanip>
#include <algorithm>

Point::Point() : m_x(0), m_y(0) {}

Point::Point(float x, float y) : m_x(x), m_y(y) {}

Point Point::getNextPosWithSpeed(const Point& position, float speed) const
{
	// Return the Position we can go 
	double distance = Point::getDistance(*this, position);
	if (distance > speed)
	{
		float xTemp = (speed / distance) * (position.m_x - this->m_x) + this->m_x;
		float yTemp = (speed / distance) * (position.m_y - this->m_y) + this->m_y;

		float x = round(std::min(std::max(xTemp, X_MIN), X_MAX));
		float y = round(std::min(std::max(yTemp, Y_MIN), Y_MAX));
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