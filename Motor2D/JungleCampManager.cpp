#include "JungleCampManager.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Entity.h"
#include "j1Timer.h"
#include "Snakes.h"
#include "Skeleton.h"

#define SNAKE_RESPAWN_TIME 120
#define SKELETON_RESPAWN_TIME 180

#define HALFMAP 81*32

JungleCampManager::JungleCampManager()
{	
}

JungleCampManager::~JungleCampManager()
{
}

bool JungleCampManager::Start()
{
	//first test

	SpawnSkeleton(0);
	SpawnSnake(0);


	return true;
}

bool JungleCampManager::Update(float dt)
{
	bool ret = true;
	

	if (snakes_camp1.empty() && !snakes_timer_camp1.IsActive())
	{
		snakes_timer_camp1.Start();
	}

	if (snakes_camp2.empty() && !snakes_timer_camp2.IsActive())
	{
		snakes_timer_camp2.Start();
	}

	if (snakes_timer_camp1.ReadSec() > SNAKE_RESPAWN_TIME)
	{
		SpawnSnake(1);
		snakes_timer_camp1.Stop();
	}

	if (snakes_timer_camp2.ReadSec() > SNAKE_RESPAWN_TIME)
	{
		SpawnSnake(2);
		snakes_timer_camp2.Stop();

	}

	if (skeleton_camp1.empty() && !skeleton_timer_camp1.IsActive())
	{
		skeleton_timer_camp1.Start();
	}

	if (skeleton_camp2.empty() && !skeleton_timer_camp2.IsActive())
	{
		skeleton_timer_camp2.Start();
	}

	if (skeleton_timer_camp1.ReadSec() > SKELETON_RESPAWN_TIME)
	{
		SpawnSkeleton(1);
		skeleton_timer_camp1.Stop();
	}

	if (skeleton_timer_camp2.ReadSec() > SKELETON_RESPAWN_TIME)
	{
		SpawnSkeleton(2);
		skeleton_timer_camp2.Stop();

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

	snakes_camp1.clear();
	snakes_camp2.clear();

	for (std::list<Entity*>::const_iterator item = skeleton_camp1.begin(); item != skeleton_camp1.end();)
	{
		App->entity->DeleteEntity(*item);
		item = skeleton_camp1.erase(item);
	}

	skeleton_camp1.clear();

	for (std::list<Entity*>::const_iterator item = skeleton_camp2.begin(); item != skeleton_camp2.end();)
	{
		App->entity->DeleteEntity(*item);
		item = skeleton_camp2.erase(item);
	}

	skeleton_camp1.clear();
	skeleton_camp2.clear();

	return true;
}

void JungleCampManager::SpawnSnake(uint camp)
{
	switch (camp)
	{
	case 0:
	{
		std::vector<iPoint> snake_positions = App->map->GetSnakesSpawns();
		Snakes* s1 = (Snakes*)App->entity->CreateEntity(snake, snake_positions[0]);
		Snakes* s2 = (Snakes*)App->entity->CreateEntity(snake, snake_positions[1]);

		snakes_camp1.push_back(s1);
		snakes_camp1.push_back(s2);


		Snakes* s3 = (Snakes*)App->entity->CreateEntity(snake, snake_positions[2]);
		Snakes* s4 = (Snakes*)App->entity->CreateEntity(snake, snake_positions[3]);

		snakes_camp2.push_back(s3);
		snakes_camp2.push_back(s4);
		break; 
	}
	case 1:
	{
		std::vector<iPoint> snake_positions = App->map->GetSnakesSpawns();
		Snakes* s1 = (Snakes*)App->entity->CreateEntity(snake, snake_positions[0]);
		Snakes* s2 = (Snakes*)App->entity->CreateEntity(snake, snake_positions[1]);


		snakes_camp1.push_back(s1);
		snakes_camp1.push_back(s2);
		break;
	}
	case 2:
	{
		std::vector<iPoint> snake_positions = App->map->GetSnakesSpawns();
		Snakes* s3 = (Snakes*)App->entity->CreateEntity(snake, snake_positions[2]);
		Snakes* s4 = (Snakes*)App->entity->CreateEntity(snake, snake_positions[3]);

		snakes_camp2.push_back(s3);
		snakes_camp2.push_back(s4);
		break;
	}
	default:
		break;
	}

}

void JungleCampManager::SpawnSkeleton(uint camp)
{
	switch (camp)
	{
	case 0:
	{
		std::vector<iPoint> skeleton_positions = App->map->GetSkeletonSpawns();
		Skeleton* sk1 = (Skeleton*)App->entity->CreateEntity(skeleton, skeleton_positions[0]);
		Skeleton* sk2 = (Skeleton*)App->entity->CreateEntity(skeleton, skeleton_positions[1]);


		skeleton_camp1.push_back(sk1);
		skeleton_camp2.push_back(sk2);
		break;
	}
	case 1:
	{
		std::vector<iPoint> skeleton_positions = App->map->GetSkeletonSpawns();
		Skeleton* sk1 = (Skeleton*)App->entity->CreateEntity(skeleton, skeleton_positions[0]);

		skeleton_camp1.push_back(sk1);
		break;
	}
	case 2:
	{
		std::vector<iPoint> skeleton_positions = App->map->GetSkeletonSpawns();
		Skeleton* sk2 = (Skeleton*)App->entity->CreateEntity(skeleton, skeleton_positions[1]);

		skeleton_camp2.push_back(sk2);
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
		if (camp->GetPos().x < HALFMAP)
		{
			snakes_camp1.remove(camp);
		}
		else
		{
			snakes_camp2.remove(camp);
		}
	}
	if (camp->type == skeleton)
	{
		if (camp->GetPos().x < HALFMAP)
		{
			skeleton_camp1.remove(camp);
		}
		else
		{
			skeleton_camp2.remove(camp);
		}
	}
	App->entity->DeleteEntity(camp);

}
