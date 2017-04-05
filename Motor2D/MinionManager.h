#ifndef _MINION_MANAGER_H_
#define _MINION_MANAGER_H_

#include <list>
#include "p2Defs.h"
#include "p2Point.h"
#include "j1Timer.h"

class Minion;
class Entity;

enum minion_manager_state
{
	null,
	game_start,
	spawn_minions,
	wait_between_minions,
	wait_wave,
};

class MinionManager 
{
public:
	MinionManager();
	~MinionManager();

	//Controll minion spawn
	bool Update();

	bool CleanUp();

	//Return a list that contains all team #team minions. 1 or 2 teams.
	std::list<Minion*>& GetMinionList(uint team);

	//Delete a minion form their team list
	void KillMinion(Entity* minion);
	
	//Starts spawn timer
	void StartTimer();

	void AddMinions();

	void StunMinions();

private:

public:

private:
	std::list<Minion*>	 team1_minions;
	std::list<Minion*>	 team2_minions;

	std::list<iPoint>	 minions_path;

	j1Timer				 spawn_timer;
	float				 wave_time = 0.0f;
	float				 minion_time_difference = 0.0f;
	uint				 minions_wave = 0.0f;
	float				 first_wave_delay = 0.0f;

	int					 minion_num = 0;

	iPoint				 team1_spawn = NULLPOINT;
	iPoint				 team2_spawn = NULLPOINT;

	minion_manager_state state = null;
};

#endif // !_MINION_MANAGER_H_

