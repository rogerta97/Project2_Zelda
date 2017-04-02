#ifndef _TOWER_MANAGER_H_
#define _TOWER_MANAGER_H_

#include <list>
#include "p2Defs.h"
#include "p2Point.h"

class Tower;
class Entity;

class TowerManager
{
public:
	TowerManager();
	~TowerManager();

	bool Update();
	bool CleanUp();

	//Return a list that contains all team #team towers. 1 or 2 teams.
	std::list<Tower*>& GetTowerList(uint team);

	//Delete a tower from their team list
	void KillTower(Entity* tower);

private:

private:
	std::list<Tower*> team1_towers;
	std::list<Tower*> team2_towers;

};

#endif // !_TOWER_MANAGER_H_

