#include "TowerManager.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Entity.h"
#include "Tower.h"
#include "j1Audio.h"

TowerManager::TowerManager()
{
	//Team 1 towers
	std::vector<iPoint> tower_positions1 = App->map->GetTowerSpawns(1);
	t1 = (Tower*)App->entity->CreateEntity(tower, tower_positions1[0]);
	t1->SetTeam(1);
	team1_towers.push_back(t1);
	t2 = (Tower*)App->entity->CreateEntity(tower, tower_positions1[1]);
	t2->SetTeam(1);
	t_Invulnerable(t2);
	SetHP(t2, t2->stats.base_hp + 100);
	team1_towers.push_back(t2);

	//Team 2 towers
	std::vector<iPoint> tower_positions2 = App->map->GetTowerSpawns(2);
	t3 = (Tower*)App->entity->CreateEntity(tower, tower_positions2[0]);
	t3->SetTeam(2);
	team2_towers.push_back(t3);
	t4 = (Tower*)App->entity->CreateEntity(tower, tower_positions2[1]);
	t4->SetTeam(2);
	t_Invulnerable(t4);
	SetHP(t4, t4->stats.base_hp + 100);
	team2_towers.push_back(t4);

	//to-improve: using for loop to create towers inside
	death_sound_effect = App->audio->LoadFx("Audio/FX/Entities/Enemies/LTTP_Enemy_Kill.wav");
}

TowerManager::~TowerManager()
{
}

bool TowerManager::Update()
{
	bool ret = true;


	return ret;
}

bool TowerManager::CleanUp()
{
	bool ret = true;

	// Cleaning towers 1
	if (!team1_towers.empty())
	{
		for (std::list<Tower*>::const_iterator it = team1_towers.begin(); it != team1_towers.end();)
		{
			App->entity->DeleteEntity(*it);
			it = team1_towers.erase(it);
		}

		team1_towers.clear();
	}

	// Cleaning towers 2
	if (!team2_towers.empty())
	{
		for (std::list<Tower*>::const_iterator it = team2_towers.begin(); it != team2_towers.end();)
		{
			App->entity->DeleteEntity(*it);
			it = team2_towers.erase(it);
		}

		team2_towers.clear();
	}

	return ret;
}

std::list<Tower*>& TowerManager::GetTowerList(uint team) 
{
	switch (team)
	{
	case 1:
		return team1_towers;
		break;
	case 2:
		return team2_towers;
		break;
	default:
		return team1_towers;
		break;
	}	
}

void TowerManager::KillTower(Entity * tower)
{
	switch (tower->GetTeam())
	{
	case 1:
		for (std::list<Tower*>::const_iterator it = team1_towers.begin(); it != team1_towers.end();)
		{
			if (tower == (*it))
			{
				if (tower == t1)
				{
					t2->show_life_bar = true;
					t2->invulnerable = false;
				}
				team1_towers.erase(it);
				break;
			}
			else
				++it;
		}

		App->entity->DeleteEntity(tower);
		break;
	case 2:
		for (std::list<Tower*>::const_iterator it = team2_towers.begin(); it != team2_towers.end();)
		{
			if (tower == (*it))
			{
				if (tower = t3)
				{
					t4->show_life_bar = true;
					t4->invulnerable = false;
				}
				team2_towers.erase(it);
				break;
			}
			else
				++it;
		}

		App->entity->DeleteEntity(tower);
		break;
	}

	//App->audio->PlayFx(death_sound_effect, 0);
}

void TowerManager::SetHP(Entity * tower, uint hp)
{
	tower->stats.base_hp = tower->stats.max_life = tower->stats.life = hp;
}

void TowerManager::t_Invulnerable(Entity * tower)
{
	tower->SetInvulnerable();
}

bool TowerManager::TowersAlive(uint team)
{
	bool ret = true;

	switch (team)
	{
	case 1:
		if (team1_towers.empty())
			ret = false;
		break;
	case 2:
		if (team2_towers.empty())
			ret = false;
		break;		
	default:
		ret = true;
		break;
	}

	return ret;
}
