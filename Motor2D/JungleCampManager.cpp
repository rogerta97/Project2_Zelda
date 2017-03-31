#include "JungleCampManager.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Entity.h"
#include "j1Timer.h"
#include "Snakes.h"

#define S_RESPAWN_TIME 120

JungleCampManager::JungleCampManager()
{
	//first test
	Snakes* s1 = (Snakes*)App->entity->CreateEntity(snake, { 2490,1740 });
	snakes.push_back(s1);
	Snakes* s2 = (Snakes*)App->entity->CreateEntity(snake, { 2530,1740 });
	snakes.push_back(s2);
	Snakes* s3 = (Snakes*)App->entity->CreateEntity(snake, { 2900,720 });
	snakes.push_back(s3);
	Snakes* s4 = (Snakes*)App->entity->CreateEntity(snake, { 2940,720 });
	snakes.push_back(s4);
	
}

JungleCampManager::~JungleCampManager()
{
	for (std::list<Entity*>::const_iterator item = snakes.begin(); item != snakes.end(); ++item)
	{
		App->entity->DeleteEntity(*item);
	}

	snakes.clear();
}

bool JungleCampManager::Update()
{
	bool ret = true;
	
	if (respawn.ReadSec() > S_RESPAWN_TIME)
	{
		SpawnSnake();
	}

	return ret;
}

void JungleCampManager::SpawnSnake()
{
	for (std::list<Entity*>::iterator it = snakes.begin(); it != snakes.end(); it++)
	{
		if ((*it)!= nullptr)
		{
			App->entity->DeleteEntity(*it);
			snakes.erase(it);
		}
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

void JungleCampManager::KillJungleCamp(Entity * camp)
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
	
}
