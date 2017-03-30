#include "JungleCampManager.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Entity.h"
#include "j1Timer.h"
#include "Snakes.h"

#define RESPAWN_TIME 120

JungleCampManager::JungleCampManager()
{
	//first test
	Snakes* s1 = (Snakes*)App->entity->CreateEntity(snake, { 1000,960 });
	camps.push_back(s1);
	
}

JungleCampManager::~JungleCampManager()
{
	for (std::list<Entity*>::const_iterator item = camps.begin(); item != camps.end(); ++item)
	{
		App->entity->DeleteEntity(*item);
	}

	camps.clear();
}

bool JungleCampManager::Update()
{
	bool ret = true;
	if (respawn.ReadSec() > RESPAWN_TIME)
	{
		//spawcamp
	}

	return ret;
}

void JungleCampManager::KillJungleCamp(Entity * camp)
{
	for (std::list<Entity*>::iterator it = camps.begin(); it != camps.end(); it++)
	{
		if (camp == (*it))
		{
			App->entity->DeleteEntity(camp);
			camps.erase(it);
			respawn.Start();
			break;
		}
	}
	
}
