#ifndef _JUNGLE_CAMP_MANAGER_H_
#define _JUNGLE_CAMP_MANAGER_H_

#include <vector>
#include "p2Defs.h"
#include "p2Point.h"
#include "j1Timer.h"

class Skeleton;
class j1Timer;
class Snakes;
class Entity;
class MageSkeleton;
class Guards;

class JungleCampManager
{
public:
	JungleCampManager();
	~JungleCampManager();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void SpawnSkeleton(uint camp);
	void SpawnSnake(uint camp);
	void SpawnMageSkeleton(uint camp);
	void SpawnGuard(uint camp);
	void KillJungleCamp(Entity* camp);

public:
	std::vector<Entity*> snakes_camp1;
	std::vector<Entity*> snakes_camp2;

	Skeleton*		     skeleton_camp1 = nullptr;
	Skeleton*			 skeleton_camp2 = nullptr;

	std::vector<Entity*> mageskeleton_camp1;
	std::vector<Entity*> mageskeleton_camp2;

	std::vector<Entity*> guards_camp1;
	std::vector<Entity*> guards_camp2;

private:
	j1Timer*			 snakes_timer_camp1 = nullptr;
	j1Timer*			 snakes_timer_camp2 = nullptr;

	j1Timer*			 skeleton_timer_camp1 = nullptr;
	j1Timer*			 skeleton_timer_camp2 = nullptr;

	j1Timer*			 mageskeleton_timer_camp1 = nullptr;
	j1Timer*			 mageskeleton_timer_camp2 = nullptr;

	j1Timer				 guards_timer_camp1;
	j1Timer				 guards_timer_camp2;

	uint				 death_sound_effect = 100;
};

#endif // !_JUNGLE_CAMP_MANAGER_H_