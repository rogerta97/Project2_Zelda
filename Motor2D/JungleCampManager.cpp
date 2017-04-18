#include "JungleCampManager.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Entity.h"
#include "j1Timer.h"
#include "Snakes.h"
#include "Skeleton.h"
#include "p2Log.h"
#include "j1Audio.h"

#define SNAKE_RESPAWN_TIME 60
#define SKELETON_RESPAWN_TIME 100

#define HALFMAP 81*32

JungleCampManager::JungleCampManager()
{	
}

JungleCampManager::~JungleCampManager()
{
}

bool JungleCampManager::Start()
{
	bool ret = true;

	//stopping timers
	snakes_timer_camp1.Stop();
	snakes_timer_camp2.Stop();
	skeleton_timer_camp1.Stop();
	skeleton_timer_camp2.Stop();

	//spawning jungle camps
	SpawnSkeleton(0);
	SpawnSnake(0);

	death_sound_effect = App->audio->LoadFx("Audio/FX/Entities/Enemies/LTTP_Enemy_Kill.wav");

	return ret;
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

	if (skeleton_camp1 == nullptr && !skeleton_timer_camp1.IsActive())
	{
		skeleton_timer_camp1.Start();
	}

	if (skeleton_camp2 == nullptr && !skeleton_timer_camp2.IsActive())
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
	LOG("Unloading JungleCampManager");

	for (int i = 0;i<snakes_camp1.size();i++)
	{
		App->entity->DeleteEntity(snakes_camp1[i]);
	}

	snakes_camp1.clear();

	for (int i = 0; i<snakes_camp2.size(); i++)
	{
		App->entity->DeleteEntity(snakes_camp1[i]);
	}
	snakes_camp2.clear();

	if(skeleton_camp1 != nullptr)
	{
		App->entity->DeleteEntity(skeleton_camp1);
	}

	if(skeleton_camp2 != nullptr)
	{
		App->entity->DeleteEntity(skeleton_camp2);
	}

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
		skeleton_camp1 = (Skeleton*)App->entity->CreateEntity(skeleton, skeleton_positions[0]);
		skeleton_camp2 = (Skeleton*)App->entity->CreateEntity(skeleton, skeleton_positions[1]);

		break;
	}
	case 1:
	{
		std::vector<iPoint> skeleton_positions = App->map->GetSkeletonSpawns();
		skeleton_camp1 = (Skeleton*)App->entity->CreateEntity(skeleton, skeleton_positions[0]);

		break;
	}
	case 2:
	{
		std::vector<iPoint> skeleton_positions = App->map->GetSkeletonSpawns();
		skeleton_camp2 = (Skeleton*)App->entity->CreateEntity(skeleton, skeleton_positions[1]);

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
		if (camp->GetPos().x > HALFMAP)
		{
			for (std::vector<Entity*>::iterator it = snakes_camp1.begin(); it != snakes_camp1.end(); ++it)
			{
				if (camp == *it) 
				{
					snakes_camp1.erase(it);
					break;
				}
			}
		}
		else
		{
			for (std::vector<Entity*>::iterator it = snakes_camp2.begin(); it != snakes_camp2.end(); ++it)
			{
				if (camp == *it) {
					snakes_camp2.erase(it);
					break;
				}
			}
		}
	}
	App->entity->DeleteEntity(camp);

	if (camp->type == skeleton)
	{
		if (camp->GetPos().x < HALFMAP)
		{
			skeleton_camp1 = nullptr;
		}
		else
		{
			skeleton_camp2 = nullptr;
		}
	}
	
	App->audio->PlayFx(death_sound_effect, 0);
}
