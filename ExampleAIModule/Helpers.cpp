#include "Helpers.h"

using namespace BWAPI;
using namespace std;

namespace helpers {
	Unit* getClosestEnemy(Unit* unit, set<Unit*> enemies) {		
		return getClosestUnitFrom(unit->getPosition(), enemies);
	}

	Unit* getClosestUnitFrom(Position &pos, set<Unit*> units) {
		Unit* closest = 0;
		double minDistance = numeric_limits<double>::infinity();
		
		for (set<BWAPI::Unit*>::const_iterator iter = units.begin(); iter != units.end(); iter++) {
			Unit* unit = *iter;
			double distance = unit->getPosition().getDistance(pos);
					
			if (distance < minDistance) {
				minDistance = distance;
				closest = unit;
			}
		}
		return closest;
	}

	set<Unit*> getAttackingAllies() {
		set<Unit*> allies = Broodwar->self()->getUnits();
		for (set<Unit*>::const_iterator iter = allies.begin(); iter != allies.end(); iter++) {
			Unit* ally = *iter;
			if (!isAttackingEnemy(ally)) {
				allies.erase(iter);
			}
		}
		return allies;
	}

	bool isAttackingEnemy(Unit* unit) {
		Unit* other = unit->getOrderTarget();
		return other && other->getPlayer() == Broodwar->enemy();
	}

	double angleBetween(Position p1, Position p2)
	{
		double len1 = p1.getLength();
		double len2 = p2.getLength();
		double p = (p1.x()*p2.x() + p1.y()*p2.y())/(len1*len2);
		return acos(p);
	}

	double angleBetween(Position p, double x, double y)
	{
		double len = sqrt(x*x + y*y);
		double v = (p.x()*x + p.y()*y)/(len*p.getLength());
		return acos(v);
	}

	Position vecFromAngle(double angle, int tiles)
	{
		int coef = TILE_SIZE*tiles;
		return Position(static_cast<int>(cos(angle)*coef), static_cast<int>(sin(angle)*coef));
	}

	int killedUnitCount() {
		int count = 0;

		for (int i = 0; i < typeCount; i++) {
			count += Broodwar->self()->killedUnitCount(UnitTypes::getUnitType(string(unitTypes[i])));
		}

		return count;
	}

	int deadUnitCount() {
		int count = 0;

		for (int i = 0; i < typeCount; i++) {
			count += Broodwar->self()->deadUnitCount(UnitTypes::getUnitType(string(unitTypes[i])));
		}

		return count;
	}
}
