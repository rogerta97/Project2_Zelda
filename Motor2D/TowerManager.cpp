#include "TowerManager.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Entity.h"
#include "Tower.h"

TowerManager::TowerManager()
{
	//Team 1 towers
	std::vector<iPoint> tower_positions1 = App->map->GetTowerSpawns(1);
	Tower* t1 = (Tower*)App->entity->CreateEntity(tower, tower_positions1[0]);
	t1->SetTeam(1);
	team1_towers.push_back(t1);
	Tower* t2 = (Tower*)App->entity->CreateEntity(tower, tower_positions1[1]);
	t2->SetTeam(1);
	team1_towers.push_back(t2);
	//Team 2 towers
	std::vector<iPoint> tower_positions2 = App->map->GetTowerSpawns(2);
	Tower* t3 = (Tower*)App->entity->CreateEntity(tower, tower_positions2[0]);
	t3->SetTeam(2);
	team2_towers.push_back(t3);
	Tower* t4 = (Tower*)App->entity->CreateEntity(tower, tower_positions2[1]);
	t4->SetTeam(2);
	team2_towers.push_back(t4);

	//to-improve: using for loop to create towers inside
}

TowerManager::~TowerManager()
{
	for (std::list<Tower*>::const_iterator item = team1_towers.begin(); item != team1_towers.end(); ++item)
	{
		App->entity->DeleteEntity(*item);
	}

	team1_towers.clear();

	for (std::list<Tower*>::const_iterator item = team2_towers.begin(); item != team2_towers.end(); ++item)
	{
		App->entity->DeleteEntity(*item);
	}

	team2_towers.clear();
}

bool TowerManager::Update()
{
	bool ret = true;


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
		team1_towers.remove((Tower*)tower);
		App->entity->DeleteEntity(tower);
		break;
	case 2:
		team2_towers.remove((Tower*)tower);
		App->entity->DeleteEntity(tower);
		break;

	}
}
