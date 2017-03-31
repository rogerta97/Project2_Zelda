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

	bool Update();

	void SpawnSnake();

	void KillJungleCamp(Entity* camp);

private:

	std::list<Entity*>		snakes;

	j1Timer					respawn;

};

#endif // !_JUNGLE_CAMP_MANAGER_H_