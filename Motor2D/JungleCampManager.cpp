#include "JungleCampManager.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Entity.h"
#include "j1Timer.h"
#include "Snakes.h"
#include "Skeleton.h"
#include "MageSkeleton.h"
#include "Guards.h"
#include "p2Log.h"
#include "j1Audio.h"

#define SNAKE_RESPAWN_TIME 60
#define SKELETON_RESPAWN_TIME 100
#define MAGESKELETON_RESPAWN_TIME 60
#define GUARD_RESPAWN_TIME 80

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

	// Stopping timers
	snakes_timer_camp1 = App->AddGameplayTimer(); snakes_timer_camp1->Stop();
	snakes_timer_camp2 = App->AddGameplayTimer(); snakes_timer_camp2->Stop();

	skeleton_timer_camp1 = App->AddGameplayTimer(); skeleton_timer_camp1->Stop();
	skeleton_timer_camp2 = App->AddGameplayTimer(); skeleton_timer_camp2->Stop();

	mageskeleton_timer_camp1 = App->AddGameplayTimer(); mageskeleton_timer_camp1->Stop();
	mageskeleton_timer_camp2 = App->AddGameplayTimer(); mageskeleton_timer_camp2->Stop();

	guards_timer_camp1.Stop();
	guards_timer_camp2.Stop();

	// Spawning jungle camps
	SpawnSkeleton(0);
	SpawnSnake(0);
	SpawnMageSkeleton(0);
	SpawnGuard(0);

	death_sound_effect = App->audio->LoadFx("Audio/FX/Entities/Enemies/LTTP_Enemy_Kill.wav");

	return ret;
}

bool JungleCampManager::Update(float dt)
{
	bool ret = true;

	if (snakes_camp1.empty() && !snakes_timer_camp1->IsActive())
	{
		snakes_timer_camp1->Start();
	}

	if (snakes_camp2.empty() && !snakes_timer_camp2->IsActive())
	{
		snakes_timer_camp2->Start();
	}

	if (snakes_timer_camp1->ReadSec() > SNAKE_RESPAWN_TIME)
	{
		SpawnSnake(1);
		snakes_timer_camp1->Stop();
	}

	if (snakes_timer_camp2->ReadSec() > SNAKE_RESPAWN_TIME)
	{
		SpawnSnake(2);
		snakes_timer_camp2->Stop();
	}

	if (skeleton_camp1 == nullptr && !skeleton_timer_camp1->IsActive())
	{
		skeleton_timer_camp1->Start();
	}

	if (skeleton_camp2 == nullptr && !skeleton_timer_camp2->IsActive())
	{
		skeleton_timer_camp2->Start();
	}

	if (skeleton_timer_camp1->ReadSec() > SKELETON_RESPAWN_TIME)
	{
		SpawnSkeleton(1);
		skeleton_timer_camp1->Stop();
	}

	if (skeleton_timer_camp2->ReadSec() > SKELETON_RESPAWN_TIME)
	{
		SpawnSkeleton(2);
		skeleton_timer_camp2->Stop();
	}

	if (mageskeleton_camp1.empty() && !mageskeleton_timer_camp1->IsActive())
	{
		mageskeleton_timer_camp1->Start();
	}

	if (mageskeleton_camp2.empty() && !mageskeleton_timer_camp2->IsActive())
	{
		mageskeleton_timer_camp2->Start();
	}

	if (mageskeleton_timer_camp1->ReadSec() > MAGESKELETON_RESPAWN_TIME)
	{
		SpawnMageSkeleton(1);
		mageskeleton_timer_camp1->Stop();
	}

	if (mageskeleton_timer_camp2->ReadSec() > MAGESKELETON_RESPAWN_TIME)
	{
		SpawnMageSkeleton(2);
		mageskeleton_timer_camp2->Stop();
	}

	if (guards_camp1.empty() && !guards_timer_camp1.IsActive())
	{
		guards_timer_camp1.Start();
	}

	if (guards_camp2.empty() && !guards_timer_camp2.IsActive())
	{
		guards_timer_camp2.Start();
	}

	if (guards_timer_camp1.ReadSec() > GUARD_RESPAWN_TIME)
	{
		SpawnGuard(1);
		guards_timer_camp1.Stop();
	}

	if (guards_timer_camp2.ReadSec() > GUARD_RESPAWN_TIME)
	{
		SpawnGuard(2);
		guards_timer_camp2.Stop();
	}

	return ret;
}

bool JungleCampManager::CleanUp()
{
	LOG("Unloading JungleCampManager");

	// Cleaning snakes
	for (int i = 0; i < snakes_camp1.size(); i++)
	{
		App->entity->DeleteEntity(snakes_camp1[i]);
	}
	snakes_camp1.clear();

	for (int i = 0; i < snakes_camp2.size(); i++)
	{
		App->entity->DeleteEntity(snakes_camp2[i]);
	}
	snakes_camp2.clear();
	// ------

	// Cleaning skeletons
	if(skeleton_camp1 != nullptr)
	{
		App->entity->DeleteEntity(skeleton_camp1);
	}

	if(skeleton_camp2 != nullptr)
	{
		App->entity->DeleteEntity(skeleton_camp2);
	}
	// ------

	// Cleaning megaeskeletons
	for (int i = 0; i < mageskeleton_camp1.size(); i++)
	{
		App->entity->DeleteEntity(mageskeleton_camp1[i]);
	}
	mageskeleton_camp1.clear();

	for (int i = 0; i<mageskeleton_camp2.size(); i++)
	{
		App->entity->DeleteEntity(mageskeleton_camp2[i]);
	}
	mageskeleton_camp2.clear();
	// ------
	//Cleaning guards
	for (int i = 0; i < guards_camp1.size(); i++)
	{
		App->entity->DeleteEntity(guards_camp1[i]);
	}
	guards_camp1.clear();

	for (int i = 0; i < guards_camp2.size(); i++)
	{
		App->entity->DeleteEntity(guards_camp2[i]);
	}
	guards_camp2.clear();

	App->DeleteGameplayTimer(snakes_timer_camp1);
	App->DeleteGameplayTimer(snakes_timer_camp2);

	App->DeleteGameplayTimer(skeleton_timer_camp1);
	App->DeleteGameplayTimer(skeleton_timer_camp2);

	App->DeleteGameplayTimer(mageskeleton_timer_camp1);
	App->DeleteGameplayTimer(mageskeleton_timer_camp2);
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

void JungleCampManager::SpawnMageSkeleton(uint camp)
{
	switch (camp)
	{
	case 0:
	{
		std::vector<iPoint> mskeleton_positions = App->map->GetMageSkeletonSpawns();
		MageSkeleton* s1 = (MageSkeleton*)App->entity->CreateEntity(mskeleton, mskeleton_positions[0]);
		MageSkeleton* s2 = (MageSkeleton*)App->entity->CreateEntity(mskeleton, mskeleton_positions[1]);
		MageSkeleton* s3 = (MageSkeleton*)App->entity->CreateEntity(mskeleton, mskeleton_positions[2]);
		MageSkeleton* s4 = (MageSkeleton*)App->entity->CreateEntity(mskeleton, mskeleton_positions[3]);

		mageskeleton_camp1.push_back(s1);
		mageskeleton_camp1.push_back(s2);
		mageskeleton_camp1.push_back(s3);
		mageskeleton_camp1.push_back(s4);


		MageSkeleton* s5 = (MageSkeleton*)App->entity->CreateEntity(mskeleton, mskeleton_positions[4]);
		MageSkeleton* s6 = (MageSkeleton*)App->entity->CreateEntity(mskeleton, mskeleton_positions[5]);
		MageSkeleton* s7 = (MageSkeleton*)App->entity->CreateEntity(mskeleton, mskeleton_positions[6]);
		MageSkeleton* s8 = (MageSkeleton*)App->entity->CreateEntity(mskeleton, mskeleton_positions[7]);

		mageskeleton_camp2.push_back(s5);
		mageskeleton_camp2.push_back(s6);
		mageskeleton_camp2.push_back(s7);
		mageskeleton_camp2.push_back(s8);

		break;
	}
	case 1:
	{
		std::vector<iPoint> mskeleton_positions = App->map->GetMageSkeletonSpawns();
		MageSkeleton* s1 = (MageSkeleton*)App->entity->CreateEntity(mskeleton, mskeleton_positions[0]);
		MageSkeleton* s2 = (MageSkeleton*)App->entity->CreateEntity(mskeleton, mskeleton_positions[1]);
		MageSkeleton* s3 = (MageSkeleton*)App->entity->CreateEntity(mskeleton, mskeleton_positions[2]);
		MageSkeleton* s4 = (MageSkeleton*)App->entity->CreateEntity(mskeleton, mskeleton_positions[3]);

		mageskeleton_camp1.push_back(s1);
		mageskeleton_camp1.push_back(s2);
		mageskeleton_camp1.push_back(s3);
		mageskeleton_camp1.push_back(s4);

		break;
	}
	break;
	case 2:
	{
		std::vector<iPoint> mskeleton_positions = App->map->GetMageSkeletonSpawns();
		MageSkeleton* s5 = (MageSkeleton*)App->entity->CreateEntity(mskeleton, mskeleton_positions[4]);
		MageSkeleton* s6 = (MageSkeleton*)App->entity->CreateEntity(mskeleton, mskeleton_positions[5]);
		MageSkeleton* s7 = (MageSkeleton*)App->entity->CreateEntity(mskeleton, mskeleton_positions[6]);
		MageSkeleton* s8 = (MageSkeleton*)App->entity->CreateEntity(mskeleton, mskeleton_positions[7]);

		mageskeleton_camp2.push_back(s5);
		mageskeleton_camp2.push_back(s6);
		mageskeleton_camp2.push_back(s7);
		mageskeleton_camp2.push_back(s8);

		break;
	}
	default:
		break;
	}
}

void JungleCampManager::SpawnGuard(uint camp)
{
	switch (camp)
	{
	case 0:
	{
		std::vector<iPoint> guard_positions = App->map->GetGuardsSpawns();
		Guards* g1 = (Guards*)App->entity->CreateEntity(guards, guard_positions[0]);
		Guards* g2 = (Guards*)App->entity->CreateEntity(guards, guard_positions[1]);
		Guards* g3 = (Guards*)App->entity->CreateEntity(guards, guard_positions[2]);

		guards_camp1.push_back(g1);
		guards_camp1.push_back(g2);
		guards_camp1.push_back(g3);


		Guards* g4 = (Guards*)App->entity->CreateEntity(guards, guard_positions[3]);
		Guards* g5 = (Guards*)App->entity->CreateEntity(guards, guard_positions[4]);
		Guards* g6 = (Guards*)App->entity->CreateEntity(guards, guard_positions[5]);

		guards_camp2.push_back(g4);
		guards_camp2.push_back(g5);
		guards_camp2.push_back(g6);
		break;
	}
	case 1:
	{
		std::vector<iPoint> guard_positions = App->map->GetGuardsSpawns();
		Guards* g1 = (Guards*)App->entity->CreateEntity(guards, guard_positions[0]);
		Guards* g2 = (Guards*)App->entity->CreateEntity(guards, guard_positions[1]);
		Guards* g3 = (Guards*)App->entity->CreateEntity(guards, guard_positions[2]);

		guards_camp1.push_back(g1);
		guards_camp1.push_back(g2);
		guards_camp1.push_back(g3);
		break;
	}
	case 2:
	{
		std::vector<iPoint> guard_positions = App->map->GetGuardsSpawns();
		Guards* g4 = (Guards*)App->entity->CreateEntity(guards, guard_positions[3]);
		Guards* g5 = (Guards*)App->entity->CreateEntity(guards, guard_positions[4]);
		Guards* g6 = (Guards*)App->entity->CreateEntity(guards, guard_positions[5]);

		guards_camp2.push_back(g4);
		guards_camp2.push_back(g5);
		guards_camp2.push_back(g6);
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
	switch (camp->type)
	{
	case snake:
	{
		if (camp->GetPos().x > HALFMAP)
		{
			for (std::vector<Entity*>::iterator it = snakes_camp1.begin(); it != snakes_camp1.end(); it++)
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
			for (std::vector<Entity*>::iterator it = snakes_camp2.begin(); it != snakes_camp2.end(); it++)
			{
				if (camp == *it)
				{
					snakes_camp2.erase(it);
					break;
				}
			}
		}

		break;
	}
	case skeleton:
	{
		if (camp->GetPos().x < HALFMAP)
		{
			skeleton_camp1 = nullptr;
		}
		else
		{
			skeleton_camp2 = nullptr;
		}

		break;
	}
	case mskeleton:
	{
		if (camp->GetPos().x < HALFMAP)
		{
			for (std::vector<Entity*>::iterator it = mageskeleton_camp1.begin(); it != mageskeleton_camp1.end(); it++)
			{
				if (camp == *it)
				{
					mageskeleton_camp1.erase(it);
					break;
				}
			}
		}
		else
		{
			for (std::vector<Entity*>::iterator it = mageskeleton_camp2.begin(); it != mageskeleton_camp2.end(); it++)
			{
				if (camp == *it) {
					mageskeleton_camp2.erase(it);
					break;
				}
			}
		}

		break;
	}
	case guards:
	{
		if (camp->GetPos().x > HALFMAP)
		{
			for (std::vector<Entity*>::iterator it = guards_camp1.begin(); it != guards_camp1.end(); it++)
			{
				if (camp == *it)
				{
					guards_camp1.erase(it);
					break;
				}
			}
		}
		else
		{
			for (std::vector<Entity*>::iterator it = guards_camp2.begin(); it != guards_camp2.end(); it++)
			{
				if (camp == *it)
				{
					guards_camp2.erase(it);
					break;
				}
			}
		}

		break;
	}
	}

	App->entity->DeleteEntity(camp);
	App->audio->PlayFx(death_sound_effect, 0);
}
