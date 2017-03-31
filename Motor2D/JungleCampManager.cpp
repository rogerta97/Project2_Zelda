#include "JungleCampManager.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Entity.h"
#include "j1Timer.h"
#include "Snakes.h"
#include "Skeleton.h"

#define SNAKE_RESPAWN_TIME 120
#define SKELETON_RESPAWN_TIME 180

JungleCampManager::JungleCampManager()
{	
}

JungleCampManager::~JungleCampManager()
{
}

bool JungleCampManager::Start()
{
	//first test
	SpawnSnake();
	SpawnSkeleton();

	return true;
}

bool JungleCampManager::Update(float dt)
{
	bool ret = true;
	
	if (respawn.ReadSec() > SNAKE_RESPAWN_TIME)
	{
		SpawnSnake();

	}
	if (s_respawn.ReadSec() > SKELETON_RESPAWN_TIME)
	{
		SpawnSkeleton();
	}

	return ret;
}

bool JungleCampManager::CleanUp()
{
	for (std::list<Entity*>::const_iterator item = snakes.begin(); item != snakes.end(); ++item)
	{
		if ((*item) != nullptr)
		{
			App->entity->DeleteEntity(*item);
		}
	}

	snakes.clear();

	return true;
}

void JungleCampManager::SpawnSnake()
{
	if (!snakes.empty())
	{
		for (list<Entity*>::iterator it = snakes.begin(); it != snakes.end();)
		{
			if ((*it) != nullptr)
			{
				App->entity->DeleteEntity(*it);
				it = snakes.erase(it);
			}
			else
				++it;
		}

		snakes.clear();
	}

	Snakes* s1 = (Snakes*)App->entity->CreateEntity(snake, { 2490,1740 });
	snakes.push_back(s1);
	Snakes* s2 = (Snakes*)App->entity->CreateEntity(snake, { 2530,1740 });
	snakes.push_back(s2);
	Snakes* s3 = (Snakes*)App->entity->CreateEntity(snake, { 2900,720 });
	snakes.push_back(s3);
	Snakes* s4 = (Snakes*)App->entity->CreateEntity(snake, { 2940,720 });
	snakes.push_back(s4);
}

void JungleCampManager::SpawnSkeleton()
{
	if (skltn == nullptr)
	{
		skltn = (Skeleton*)App->entity->CreateEntity(skeleton, {800,970});
	}

}

void JungleCampManager::KillJungleCamp(Entity * camp)
{
	if (!snakes.empty())
	{
		for (std::list<Entity*>::iterator it = snakes.begin(); it != snakes.end(); it++)
		{
			if (camp == (*it))
			{
				App->entity->DeleteEntity(camp);
				snakes.erase(it);
				respawn.Start();
				break;
			}
		}

		snakes.clear();
	}
	App->entity->DeleteEntity(camp);
	s_respawn.Start();
}
