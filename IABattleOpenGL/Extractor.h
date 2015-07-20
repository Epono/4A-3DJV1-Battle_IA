#include "Unit.h"
#include "Army.h"
#include "Utils\Point.h"
#include <set>

template<typename T>
class Extractor
{
public:
	virtual T& get(Unit&, Army&, Army&) const = 0;
};

class ExtractorA : public Extractor < Army >
{
public:
	Army& get(Unit& u, Army& a, Army& o) const { return a; }
};

class ExtractorO : public Extractor < Army >
{
public:
	Army& get(Unit& u, Army& a, Army& o) const { return o; }
};

class ExtractorU : public Extractor< Unit >
{
public:
	Unit& get(Unit& u, Army& a, Army& o) const {	return u; }
};

/* Capacity value extractor */

class ExtractorC : public Extractor < float >
{
protected:
	Extractor< Unit >* _eu;
public:
	ExtractorC(Extractor< Unit >* eu) : _eu(eu) {}
	virtual float& get(Unit& u, Army& a, Army& o) const = 0;
};

class ExtractorC0 : public ExtractorC
{
public:
	ExtractorC0(Extractor< Unit >* eu) : ExtractorC(eu) {}
	float& get(Unit& u, Army& a, Army& o) const { _eu->get(u, a, o).getSpeed().getValue(); }
};

class ExtractorC1 : public ExtractorC
{
public:
	ExtractorC1(Extractor< Unit >* eu) : ExtractorC(eu) {}
	float& get(Unit& u, Army& a, Army& o) const { _eu->get(u, a, o).getHealth().getValue(); }
};

class ExtractorC2 : public ExtractorC
{
public:
	ExtractorC2(Extractor< Unit >* eu) : ExtractorC(eu) {}
	float& get(Unit& u, Army& a, Army& o) const { _eu->get(u, a, o).getArmor().getValue(); }
};

class ExtractorC3 : public ExtractorC
{
public:
	ExtractorC3(Extractor< Unit >* eu) : ExtractorC(eu) {}
	float& get(Unit& u, Army& a, Army& o) const { _eu->get(u, a, o).getRegen().getValue(); }
};

class ExtractorC4 : public ExtractorC
{
public:
	ExtractorC4(Extractor< Unit >* eu) : ExtractorC(eu) {}
	float& get(Unit& u, Army& a, Army& o) const { _eu->get(u, a, o).getWeaponDamage().getValue(); }
};

class ExtractorC5 : public ExtractorC
{
public:
	ExtractorC5(Extractor< Unit >* eu) : ExtractorC(eu) {}
	float& get(Unit& u, Army& a, Army& o) const { _eu->get(u, a, o).getWeaponRange().getValue(); }
};

class ExtractorC6 : public ExtractorC
{
public:
	ExtractorC6(Extractor< Unit >* eu) : ExtractorC(eu) {}
	float& get(Unit& u, Army& a, Army& o) const { _eu->get(u, a, o).getWeaponSpeed().getValue(); }
};

/* Distance Extractor */
class ExtractorD : public Extractor < double >
{
private:
	Extractor < Unit >* _eu;
	Extractor < Point >* _ep;
public:
	ExtractorD(Extractor < Unit >* eu, Extractor < Point >* ep)
		: _eu(eu), _ep(ep) {}
	double& get(Unit& u, Army& a, Army& o) const 
	{ 
		Point::getDistance(_eu->get(u, a, o).getPosition(), _ep->get(u, a, o));
	}
};

class ExtractorP : public Extractor < Point >
{
public:
	Point& get(Unit& u, Army& a, Army& o) const { return u.getPosition(); }
};

/* max/min/valeur moyenne de capacité d’une armee */
class ExtractorM0 : public Extractor< float >
{
private:
	Extractor< Army >* _ea;
public:
	ExtractorM0(Extractor< Army >* ea) : _ea(ea) {}
	float get(Unit& u, Army& a, Army& o)
	{
		float result = _ea->get(u, a, o).getUnitList().front()->getSpeed().getValue();
		for (auto& it : _ea->get(u, a, o).getUnitList())
		{
			float val = it->getSpeed().getValue();
			if (val < result) 
				result = val;
		}
		return result;
	}
};

class Extractorm0 : public Extractor< float >
{
private:
	Extractor< Army >* _ea;
public:
	Extractorm0(Extractor< Army >* ea) : _ea(ea) {}
	float get(Unit& u, Army& a, Army& o)
	{
		float result = _ea->get(u, a, o).getUnitList().front()->getSpeed().getValue();
		for (auto& it : _ea->get(u, a, o).getUnitList())
		{
			float val = it->getSpeed().getValue();
			if (val > result)
				result = val;
		}
		return result;
	}
};

class Extractora0 : public Extractor< float >
{
private:
	Extractor< Army >* _ea;
public:
	Extractora0(Extractor< Army >* ea) : _ea(ea) {}
	float get(Unit& u, Army& a, Army& o)
	{
		float result = 0.f;
		for (auto& it : _ea->get(u, a, o).getUnitList())
			result += it->getSpeed().getValue();

		return result / _ea->get(u, a, o).getUnitList().size();
	}
};

class ExtractorM1 : public Extractor< float >
{
private:
	Extractor< Army >* _ea;
public:
	ExtractorM1(Extractor< Army >* ea) : _ea(ea) {}
	float get(Unit& u, Army& a, Army& o)
	{
		float result = _ea->get(u, a, o).getUnitList().front()->getHealth().getValue();
		for (auto& it : _ea->get(u, a, o).getUnitList())
		{
			float val = it->getHealth().getValue();
			if (val < result)
				result = val;
		}
		return result;
	}
};

class Extractorm1 : public Extractor< float >
{
private:
	Extractor< Army >* _ea;
public:
	Extractorm1(Extractor< Army >* ea) : _ea(ea) {}
	float get(Unit& u, Army& a, Army& o)
	{
		float result = _ea->get(u, a, o).getUnitList().front()->getHealth().getValue();
		for (auto& it : _ea->get(u, a, o).getUnitList())
		{
			float val = it->getHealth().getValue();
			if (val > result)
				result = val;
		}
		return result;
	}
};

class Extractora1 : public Extractor< float >
{
private:
	Extractor< Army >* _ea;
public:
	Extractora1(Extractor< Army >* ea) : _ea(ea) {}
	float get(Unit& u, Army& a, Army& o)
	{
		float result = 0.f;
		for (auto& it : _ea->get(u, a, o).getUnitList())
			result += it->getHealth().getValue();

		return result / _ea->get(u, a, o).getUnitList().size();
	}
};

class ExtractorM2 : public Extractor< float >
{
private:
	Extractor< Army >* _ea;
public:
	ExtractorM2(Extractor< Army >* ea) : _ea(ea) {}
	float get(Unit& u, Army& a, Army& o)
	{
		float result = _ea->get(u, a, o).getUnitList().front()->getArmor().getValue();
		for (auto& it : _ea->get(u, a, o).getUnitList())
		{
			float val = it->getArmor().getValue();
			if (val < result)
				result = val;
		}
		return result;
	}
};

class Extractorm2 : public Extractor< float >
{
private:
	Extractor< Army >* _ea;
public:
	Extractorm2(Extractor< Army >* ea) : _ea(ea) {}
	float get(Unit& u, Army& a, Army& o)
	{
		float result = _ea->get(u, a, o).getUnitList().front()->getArmor().getValue();
		for (auto& it : _ea->get(u, a, o).getUnitList())
		{
			float val = it->getArmor().getValue();
			if (val > result)
				result = val;
		}
		return result;
	}
};

class Extractora2 : public Extractor< float >
{
private:
	Extractor< Army >* _ea;
public:
	Extractora2(Extractor< Army >* ea) : _ea(ea) {}
	float get(Unit& u, Army& a, Army& o)
	{
		float result = 0.f;
		for (auto& it : _ea->get(u, a, o).getUnitList())
			result += it->getArmor().getValue();

		return result / _ea->get(u, a, o).getUnitList().size();
	}
};

class ExtractorM3 : public Extractor< float >
{
private:
	Extractor< Army >* _ea;
public:
	ExtractorM3(Extractor< Army >* ea) : _ea(ea) {}
	float get(Unit& u, Army& a, Army& o)
	{
		float result = _ea->get(u, a, o).getUnitList().front()->getRegen().getValue();
		for (auto& it : _ea->get(u, a, o).getUnitList())
		{
			float val = it->getRegen().getValue();
			if (val < result)
				result = val;
		}
		return result;
	}
};

class Extractorm3 : public Extractor< float >
{
private:
	Extractor< Army >* _ea;
public:
	Extractorm3(Extractor< Army >* ea) : _ea(ea) {}
	float get(Unit& u, Army& a, Army& o)
	{
		float result = _ea->get(u, a, o).getUnitList().front()->getRegen().getValue();
		for (auto& it : _ea->get(u, a, o).getUnitList())
		{
			float val = it->getRegen().getValue();
			if (val > result)
				result = val;
		}
		return result;
	}
};

class Extractora3 : public Extractor< float >
{
private:
	Extractor< Army >* _ea;
public:
	Extractora3(Extractor< Army >* ea) : _ea(ea) {}
	float get(Unit& u, Army& a, Army& o)
	{
		float result = 0.f;
		for (auto& it : _ea->get(u, a, o).getUnitList())
			result += it->getRegen().getValue();

		return result / _ea->get(u, a, o).getUnitList().size();
	}
};

class ExtractorM4 : public Extractor< float >
{
private:
	Extractor< Army >* _ea;
public:
	ExtractorM4(Extractor< Army >* ea) : _ea(ea) {}
	float get(Unit& u, Army& a, Army& o)
	{
		float result = _ea->get(u, a, o).getUnitList().front()->getWeaponDamage().getValue();
		for (auto& it : _ea->get(u, a, o).getUnitList())
		{
			float val = it->getWeaponDamage().getValue();
			if (val < result)
				result = val;
		}
		return result;
	}
};

class Extractorm4 : public Extractor< float >
{
private:
	Extractor< Army >* _ea;
public:
	Extractorm4(Extractor< Army >* ea) : _ea(ea) {}
	float get(Unit& u, Army& a, Army& o)
	{
		float result = _ea->get(u, a, o).getUnitList().front()->getWeaponDamage().getValue();
		for (auto& it : _ea->get(u, a, o).getUnitList())
		{
			float val = it->getWeaponDamage().getValue();
			if (val > result)
				result = val;
		}
		return result;
	}
};

class Extractora4 : public Extractor< float >
{
private:
	Extractor< Army >* _ea;
public:
	Extractora4(Extractor< Army >* ea) : _ea(ea) {}
	float get(Unit& u, Army& a, Army& o)
	{
		float result = 0.f;
		for (auto& it : _ea->get(u, a, o).getUnitList())
			result += it->getWeaponDamage().getValue();

		return result / _ea->get(u, a, o).getUnitList().size();
	}
};

class ExtractorM5 : public Extractor< float >
{
private:
	Extractor< Army >* _ea;
public:
	ExtractorM5(Extractor< Army >* ea) : _ea(ea) {}
	float get(Unit& u, Army& a, Army& o)
	{
		float result = _ea->get(u, a, o).getUnitList().front()->getWeaponRange().getValue();
		for (auto& it : _ea->get(u, a, o).getUnitList())
		{
			float val = it->getWeaponRange().getValue();
			if (val < result)
				result = val;
		}
		return result;
	}
};

class Extractorm5 : public Extractor< float >
{
private:
	Extractor< Army >* _ea;
public:
	Extractorm5(Extractor< Army >* ea) : _ea(ea) {}
	float get(Unit& u, Army& a, Army& o)
	{
		float result = _ea->get(u, a, o).getUnitList().front()->getWeaponRange().getValue();
		for (auto& it : _ea->get(u, a, o).getUnitList())
		{
			float val = it->getWeaponRange().getValue();
			if (val > result)
				result = val;
		}
		return result;
	}
};

class Extractora5 : public Extractor< float >
{
private:
	Extractor< Army >* _ea;
public:
	Extractora5(Extractor< Army >* ea) : _ea(ea) {}
	float get(Unit& u, Army& a, Army& o)
	{
		float result = 0.f;
		for (auto& it : _ea->get(u, a, o).getUnitList())
			result += it->getWeaponRange().getValue();

		return result / _ea->get(u, a, o).getUnitList().size();
	}
};

class ExtractorM6 : public Extractor< float >
{
private:
	Extractor< Army >* _ea;
public:
	ExtractorM6(Extractor< Army >* ea) : _ea(ea) {}
	float get(Unit& u, Army& a, Army& o)
	{
		float result = _ea->get(u, a, o).getUnitList().front()->getWeaponSpeed().getValue();
		for (auto& it : _ea->get(u, a, o).getUnitList())
		{
			float val = it->getWeaponSpeed().getValue();
			if (val < result)
				result = val;
		}
		return result;
	}
};

class Extractorm6 : public Extractor< float >
{
private:
	Extractor< Army >* _ea;
public:
	Extractorm6(Extractor< Army >* ea) : _ea(ea) {}
	float get(Unit& u, Army& a, Army& o)
	{
		float result = _ea->get(u, a, o).getUnitList().front()->getWeaponSpeed().getValue();
		for (auto& it : _ea->get(u, a, o).getUnitList())
		{
			float val = it->getWeaponSpeed().getValue();
			if (val > result)
				result = val;
		}
		return result;
	}
};

class Extractora6 : public Extractor< float >
{
private:
	Extractor< Army >* _ea;
public:
	Extractora6(Extractor< Army >* ea) : _ea(ea) {}
	float get(Unit& u, Army& a, Army& o)
	{
		float result = 0.f;
		for (auto& it : _ea->get(u, a, o).getUnitList())
			result += it->getWeaponSpeed().getValue();

		return result / _ea->get(u, a, o).getUnitList().size();
	}
};

/*distance minimale/maximale/moyenne d’un ensemble a un point M/m/aD<set><point>*/
class ExtractorMD : public Extractor < double >
{
private:
	Extractor< Army >* _ea;
	Extractor< Point >* _ep;
public:
	ExtractorMD(Extractor< Army >* ea, Extractor< Point >* ep)
		: _ea(ea), _ep(ep) {}
	double get(Unit& u, Army& a, Army& o)
	{
		double result = Point::getDistance(_ea->get(u, a, o).getUnitList().front()->getPosition(),
											_ep->get(u, a, o));
		Point p = _ep->get(u, a, o);
		for (auto& it : _ea->get(u, a, o).getUnitList())
		{
			double dist = Point::getDistance(it->getPosition(), p);
			if (dist < result)
				result = dist;
		}
		return result;
	}
};

class ExtractormD : public Extractor < double >
{
private:
	Extractor< Army >* _ea;
	Extractor< Point >* _ep;
public:
	ExtractormD(Extractor< Army >* ea, Extractor< Point >* ep)
		: _ea(ea), _ep(ep) {}
	double get(Unit& u, Army& a, Army& o)
	{
		double result = Point::getDistance(_ea->get(u, a, o).getUnitList().front()->getPosition(),
			_ep->get(u, a, o));
		Point p = _ep->get(u, a, o);
		for (auto& it : _ea->get(u, a, o).getUnitList())
		{
			double dist = Point::getDistance(it->getPosition(), p);
			if (dist > result)
				result = dist;
		}
		return result;
	}
};

class ExtractoraD : public Extractor < double >
{
private:
	Extractor< Army >* _ea;
	Extractor< Point >* _ep;
public:
	ExtractoraD(Extractor< Army >* ea, Extractor< Point >* ep)
		: _ea(ea), _ep(ep) {}
	double get(Unit& u, Army& a, Army& o)
	{
		double result = 0.0;
		Point p = _ep->get(u, a, o);
		for (auto& it : _ea->get(u, a, o).getUnitList())
			result += Point::getDistance(it->getPosition(), p);
		
		return result / _ea->get(u, a, o).getUnitList().size();
	}
};

//////////////////////////////////////
//									//
//				TODO				//
//////////////////////////////////////

class ExtractorLD : public Extractor< Unit >
{
private:
	ExtractorO* _eo;
	ExtractorP* _ep;
public:
	ExtractorLD(ExtractorO* eo, ExtractorP* ep)
		: _eo(eo), _ep(ep) 
	{}
	Unit& get(Unit& u, Army& a, Army& o) const
	{
		return _eo->get(u, a, o).getNearestUnit(_ep->get(u, a, o));
	}
};

class ExtractorHD : public Extractor< Unit >
{
private:
	Extractor< Army >* _ea;
	Extractor< Point >* _ep;
public:
	ExtractorHD(Extractor< Army >* ea, Extractor< Point >* ep)
		: _ea(ea), _ep(ep)
	{}
	Unit& get(Unit& u, Army& a, Army& o) const
	{
		return _ea->get(u, a, o).getNearestUnit(_ep->get(u, a, o));
	}
};

