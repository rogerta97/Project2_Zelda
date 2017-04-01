#include "JungleCampManager.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Entity.h"
#include "j1Timer.h"
#include "Snakes.h"

#define S_RESPAWN_TIME 120

JungleCampManager::JungleCampManager()
{	
}

JungleCampManager::~JungleCampManager()
{
}

bool JungleCampManager::Start()
{
	//first test
	SpawnSnake(0);

	return true;
}

bool JungleCampManager::Update(float dt)
{
	bool ret = true;
	
	if (snakes_camp1.empty())
	{
		snakes_timer_camp1.Start();
	}

	if (snakes_camp2.empty())
	{
		snakes_timer_camp2.Start();
	}

	if (snakes_timer_camp1.ReadSec() > S_RESPAWN_TIME)
	{
		SpawnSnake(1);
		snakes_timer_camp1.Stop();
	}

	if (snakes_timer_camp2.ReadSec() > S_RESPAWN_TIME)
	{
		SpawnSnake(2);
		snakes_timer_camp2.Stop();
	}

	return ret;
}

bool JungleCampManager::CleanUp()
{
	for (std::list<Entity*>::const_iterator item = snakes_camp1.begin(); item != snakes_camp1.end();)
	{
		App->entity->DeleteEntity(*item);
		item = snakes_camp1.erase(item);
	}

	snakes_camp1.clear();

	for (std::list<Entity*>::const_iterator item = snakes_camp2.begin(); item != snakes_camp2.end();)
	{
		App->entity->DeleteEntity(*item);
		item = snakes_camp2.erase(item);
	}

	snakes_camp2.clear();

	return true;
}

void JungleCampManager::SpawnSnake(uint camp)
{
	switch (camp)
	{
	case 0:
	{
		Snakes* s1 = (Snakes*)App->entity->CreateEntity(snake, { 2490,1740 });
		Snakes* s2 = (Snakes*)App->entity->CreateEntity(snake, { 2530,1740 });

		snakes_camp1.push_back(s1);
		snakes_camp1.push_back(s2);

		Snakes* s3 = (Snakes*)App->entity->CreateEntity(snake, { 2900,720 });
		Snakes* s4 = (Snakes*)App->entity->CreateEntity(snake, { 2940,720 });

		snakes_camp2.push_back(s3);
		snakes_camp2.push_back(s4);
		break; 
	}
	case 1:
	{
		Snakes* s1 = (Snakes*)App->entity->CreateEntity(snake, { 2490,1740 });
		Snakes* s2 = (Snakes*)App->entity->CreateEntity(snake, { 2530,1740 });

		snakes_camp1.push_back(s1);
		snakes_camp1.push_back(s2);
		break;
	}
	case 2:
	{
		Snakes* s3 = (Snakes*)App->entity->CreateEntity(snake, { 2900,720 });
		Snakes* s4 = (Snakes*)App->entity->CreateEntity(snake, { 2940,720 });

		snakes_camp2.push_back(s3);
		snakes_camp2.push_back(s4);
		break;
	}
	default:
		break;
	}

}

void JungleCampManager::KillJungleCamp(Entity * camp)
{
	if (camp->type == snake)
	{
		if (camp->GetPos().x < 2750)
		{
			snakes_camp1.remove(camp);
		}
		else
		{
			snakes_camp2.remove(camp);
		}
	}
}
