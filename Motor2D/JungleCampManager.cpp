#include "JungleCampManager.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Entity.h"
#include "Snakes.h"

JungleCampManager::JungleCampManager()
{
	//first test
	Snakes* s1 = (Snakes*)App->entity->CreateEntity(snake, { 1000,1000 });
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


	return ret;
}

void JungleCampManager::KillJungleCamp(Entity * camp)
{
	App->entity->DeleteEntity(camp);
	camps.remove(camp);
}
