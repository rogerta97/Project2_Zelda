#include "TowerManager.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Entity.h"
#include "Tower.h"

TowerManager::TowerManager()
{
	Tower* t = (Tower*)App->entity->CreateEntity(tower, { 511,735 });
	t->SetTeam(2);
	team2_towers.push_back(t);
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
