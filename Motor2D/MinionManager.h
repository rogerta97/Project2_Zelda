#ifndef _MINION_MANAGER_H_
#define _MINION_MANAGER_H_

#include <list>
#include "p2Defs.h"
#include "p2Point.h"
#include "j1Timer.h"

class Minion;
class Entity;

class MinionManager 
{
public:
	MinionManager();
	~MinionManager();

	//Controll minion spawn
	bool Update();

	//Return a list that contains all team #team minions. 1 or 2 teams.
	const std::list<Minion*>& GetMinionList(uint team) const;

	//Delete a minion form their team list
	void KillMinion(Entity* minion);
	
	//Starts spawn timer
	void StartTimer();

	void AddMinions();

private:

public:

private:
	std::list<Minion*>		team1_minions;
	std::list<Minion*>		team2_minions;

	std::list<iPoint>		minions_path;

	j1Timer					spawn_timer;
	float					wave_time = 0.0f;
	float					minion_time_difference = 0.0f;
	uint					minions_wave = 0.0f;
	float					first_wave_delay = 0.0f;

	bool					spawn_wave = false;
	bool					first_wave = true;

	iPoint					team1_spawn = NULLPOINT;
	iPoint					team2_spawn = NULLPOINT;
};

#endif // !_MINION_MANAGER_H_

