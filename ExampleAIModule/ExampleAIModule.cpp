#include "ExampleAIModule.h"
#include <limits>

using namespace BWAPI;
using namespace std;

string stateName(State state) {
	switch (state) {
		case flee:
			return "flee";
		case fight:
			return "fight";
		case default_state:
			return "default_state";
		default:
			return "penis";
	}
}

void ExampleAIModule::onStart()
{
	Broodwar->printf("The map is %s, a %d player map",Broodwar->mapName().c_str(),Broodwar->getStartLocations().size());
	// Enable some cheat flags
	Broodwar->enableFlag(Flag::UserInput);

	this->center = Position((Broodwar->mapWidth() * TILE_SIZE)/2 , (Broodwar->mapHeight() * TILE_SIZE)/2 );
	this->unitData = map< Unit*, UnitData >();

	for(set<Unit*>::const_iterator i = Broodwar->self()->getUnits().begin();
	    i != Broodwar->self()->getUnits().end();
	    i++) {
		
		Unit* unit = *i;
		unit->attackMove(this->center);  
		UnitData unitData;
		unitData.state = default_state;
		unitData.fleeCounter = 0;
		this->unitData.insert(make_pair(unit, unitData));
		
		Broodwar->printf("Initial hit points: %d", unit->getType().maxHitPoints());
	}
}

void ExampleAIModule::onEnd(bool isWinner)
{
  if (isWinner)
  {
    //log win to file
  }
}

void ExampleAIModule::onFrame()
{
	drawUnitInfo();

	map< Unit*, int > * attacking = this->getAttackerCount();
	
	this->printAttackerInfo(attacking);
	this->decideActions(attacking);

	delete attacking;
}

void ExampleAIModule::printAttackerInfo(map<Unit*, int>* attacking) {
	for (map<Unit*, int>::const_iterator iter = attacking->begin(); iter != attacking->end(); iter++) {
		Position pos = iter->first->getPosition();
		Broodwar->drawTextMap(pos.x() - 16, pos.y() - 26, "%d", iter->second);
	}
}

void ExampleAIModule::decideActions(map<Unit*, int>* attacking) {
	for(set<Unit*>::const_iterator u = Broodwar->getAllUnits().begin(); u != Broodwar->getAllUnits().end(); u++) {
		Unit* unit = *u;

		this->handleFlee(unit, attacking);
		this->handleAttack(unit);
	}
}

void ExampleAIModule::handleFlee(Unit* unit, map<Unit*, int>* attacking) {
	UnitData* data = &unitData[unit];

	if (data->state == flee) {
		if (data->fleeCounter > 0) {
			data->fleeCounter--;
			return;
		}
		else {
			data->state = default_state;
		}
	}

	// TODO add smarter flee
	// Parameterize on unit type
	if ((*attacking)[unit] > 1) {
		TilePosition current = unit->getTilePosition();
		TilePosition runTo = current - TilePosition(5, 5);
		
		data->state = flee;
		data->fleeCounter = 25;

		unit->rightClick(runTo);
	}
}

void ExampleAIModule::handleAttack(Unit* unit) {
	UnitData data = this->unitData[unit];

	//TODO: units on low HP should assist some other unit,
	//i.e. pick closest friendly unit and attack its target
	//TODO: attack weakest enemy in range?
	if (data.state == default_state && !isAttackingEnemy(unit)) {		
		set<Unit*> enemies = Broodwar->enemy()->getUnits();

		if (!enemies.empty())
			unit->attackUnit(getClosestEnemy(unit, enemies));
	}
	else {
		//TODO: figure out where to move
		//more global AI should deside movement related issues
	}	
}

Unit* ExampleAIModule::getClosestEnemy(Unit* unit, set<Unit*> enemies) {		
	Unit* closestEnemy = NULL;
	double minDistance = numeric_limits<double>::infinity();
	
	for (set<Unit*>::const_iterator iter = enemies.begin(); iter != enemies.end(); iter++) {
		Unit* enemy = *iter;
		double distance = unit->getDistance(enemy);
				
		if (distance < minDistance) {
			minDistance = distance;
			closestEnemy = enemy;
		}
	}

	return closestEnemy;	
}

bool ExampleAIModule::isAttackingEnemy(Unit* unit) {
	Unit* other = unit->getOrderTarget();
	return other && other->getPlayer() == Broodwar->enemy();
}

map< Unit*, int > * ExampleAIModule::getAttackerCount() {
	map< Unit*, int > * attacking = new map< Unit*, int >();

	set<Unit*> myUnits = Broodwar->self()->getUnits();

	for (set<Unit*>::const_iterator iter = myUnits.begin(); iter != myUnits.end(); iter++) {
		Unit* u = *iter;
		(*attacking)[u] = 0;
	}

	set<Unit*> enemyUnits = Broodwar->enemy()->getUnits();

	for(set<Unit*>::const_iterator u = enemyUnits.begin(); u != enemyUnits.end(); u++) {
		Unit* unit = *u;							
		Unit* target = unit->getOrderTarget();

		if (target && target->getPlayer() == Broodwar->self() && this->isInAttackRange(unit, target))
			(*attacking)[target]++;								
	}

	return attacking;
}

bool ExampleAIModule::isInAttackRange(Unit* attacker, Unit* target) {
	return attacker->getDistance(target) <= attacker->getType().groundWeapon()->maxRange();
}	


void ExampleAIModule::drawUnitInfo()
{
	for(set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
	{
		Position pos = (*i)->getPosition();
		Unit* targetUnit = (*i)->getOrderTarget();
		Position target = (*i)->getTargetPosition();
		Color color;

		if (targetUnit)
			color = Color(Colors::Red);
		else
			color = Color(Colors::Green);
			
		Broodwar->drawLineMap(pos.x(), pos.y(), target.x(), target.y(), color);

		Broodwar->drawTextMap(pos.x() - 16, pos.y() - 16, "\x05(%d, %d)", pos.x(), pos.y());

		Broodwar->drawTextMap(pos.x() - 16, pos.y() - 26, "%s", stateName(this->getUnitData(*i).state).c_str());
  }
}

UnitData ExampleAIModule::getUnitData(Unit* unit) {
	return this->unitData.find(unit)->second;
}

void ExampleAIModule::onUnitCreate(BWAPI::Unit* unit)
{

}

void ExampleAIModule::onUnitDestroy(BWAPI::Unit* unit)
{

}

void ExampleAIModule::onUnitMorph(BWAPI::Unit* unit)
{

}

void ExampleAIModule::onUnitShow(BWAPI::Unit* unit)
{

}

void ExampleAIModule::onUnitHide(BWAPI::Unit* unit)
{

}

void ExampleAIModule::onUnitRenegade(BWAPI::Unit* unit)
{

}

void ExampleAIModule::onPlayerLeft(BWAPI::Player* player)
{

}

void ExampleAIModule::onNukeDetect(BWAPI::Position target)
{

}

bool ExampleAIModule::onSendText(string text)
{
  if (text=="/show players")
  {
    return false;
  }
  return true;
}