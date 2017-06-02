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

	void SetHP(Entity* tower, uint hp);
	void t_Invulnerable(Entity* tower);

	bool TowersAlive(uint team);

private:
	std::list<Tower*> team1_towers;
	std::list<Tower*> team2_towers;

	uint			  tower_death_sound_effect_1_1 = 100;
	uint			  tower_death_sound_effect_1_2 = 100;
	uint			  tower_death_sound_effect_2_1 = 100;
	uint			  tower_death_sound_effect_2_2 = 100;
	uint			  tower_death_sound_effect_2_3 = 100;

	Tower*			  t1 = nullptr;
	Tower*			  t2 = nullptr;
	Tower*			  t3 = nullptr;
	Tower*			  t4 = nullptr;
};

#endif // !_TOWER_MANAGER_H_

