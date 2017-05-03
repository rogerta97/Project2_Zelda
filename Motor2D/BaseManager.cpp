#include "BaseManager.h"
#include "j1Entity.h"
#include "j1App.h"
#include "Base.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "MainScene.h"
#include "TowerManager.h"

BaseManager::BaseManager()
{
	team1 = (Base*)App->entity->CreateEntity(base, App->map->GetBasePosition(1));
	team1->SetTeam(1);
	b_Invulnerable(team1);

	team2 = (Base*)App->entity->CreateEntity(base, App->map->GetBasePosition(2));
	team2->SetTeam(2);
	b_Invulnerable(team2);
}

BaseManager::~BaseManager()
{
}

bool BaseManager::Update()
{
	bool ret = true;


	if (team1->invulnerable == true)
	{
		if (!App->scene->main_scene->tower_manager->TowersAlive(1))
		{
			team1->invulnerable = false;
		}
	}
	if (team2->invulnerable == true)
	{
		if (!App->scene->main_scene->tower_manager->TowersAlive(2))
		{
			team2->invulnerable = false;
		}
	}

	return ret;
}

bool BaseManager::CleanUp()
{
	bool ret = true;

	if(team1 != nullptr)
		App->entity->DeleteEntity(team1);

	if (team2 != nullptr)
		App->entity->DeleteEntity(team2);

	return ret;
}

void BaseManager::KillBase(Base* base)
{
	if (base == team1)
	{
		App->entity->DeleteEntity(team1);
	}
	else if (base == team2)
	{
		App->entity->DeleteEntity(team2);
	}
}


Entity * BaseManager::GetBase(int team) const
{
	if (team == 1)
		return team1;
	else
		return team2;
}

void BaseManager::b_Invulnerable(Base * base)
{
	base->SetInvulnerable();
}
