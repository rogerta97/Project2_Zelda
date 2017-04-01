#ifndef _JUNGLE_CAMP_MANAGER_H_
#define _JUNGLE_CAMP_MANAGER_H_

#include <list>
#include "p2Defs.h"
#include "p2Point.h"
#include "j1Timer.h"

class j1Timer;
class Snakes;
class Entity;

class JungleCampManager
{
public:
	JungleCampManager();
	~JungleCampManager();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void SpawnSnake(uint camp);

	void KillJungleCamp(Entity* camp);

private:

	std::list<Entity*>		snakes_camp1;
	std::list<Entity*>		snakes_camp2;

	j1Timer					snakes_timer_camp1;
	j1Timer					snakes_timer_camp2;

};

#endif // !_JUNGLE_CAMP_MANAGER_H_