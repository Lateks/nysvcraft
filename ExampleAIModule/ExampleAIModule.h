#pragma once
#include <BWAPI.h>
#include <BWTA.h>
#include <windows.h>

enum State { flee, find_enemy, fight };

struct UnitData {
	State state;
};


class ExampleAIModule : public BWAPI::AIModule
{
public:
  virtual void onStart();
  virtual void onEnd(bool isWinner);
  virtual void onFrame();
  virtual bool onSendText(std::string text);
  virtual void onPlayerLeft(BWAPI::Player* player);
  virtual void onNukeDetect(BWAPI::Position target);
  virtual void onUnitCreate(BWAPI::Unit* unit);
  virtual void onUnitDestroy(BWAPI::Unit* unit);
  virtual void onUnitMorph(BWAPI::Unit* unit);
  virtual void onUnitShow(BWAPI::Unit* unit);
  virtual void onUnitHide(BWAPI::Unit* unit);
  virtual void onUnitRenegade(BWAPI::Unit* unit);
private:
  BWAPI::Position center;
  void drawUnitInfo();
  std::map< BWAPI::Unit*, int > * ExampleAIModule::getAttackerCount();
  std::map< BWAPI::Unit*, UnitData > unitData;
  UnitData getUnitData(BWAPI::Unit* unit);
};