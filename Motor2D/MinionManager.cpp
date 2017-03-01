#include "MinionManager.h"
#include "j1App.h"
#include "j1Pathfinding.h"
#include "j1Map.h"
#include "j1Entity.h"
#include "Minion.h"

// Coment code, ty
MinionManager::MinionManager()
{
	pugi::xml_document doc;
	App->LoadXML("GameSettings.xml", doc);

	pugi::xml_node minions_node = doc.child("file").child("minions");

	wave_time = minions_node.child("wave_time").attribute("sec").as_float();
	minion_time_difference = minions_node.child("minion_time_difference").attribute("sec").as_float();
	minions_wave = minions_node.child("minions_wave").attribute("amount").as_uint();
	first_wave_delay = minions_node.child("first_wave_delay").attribute("sec").as_float();

	team1_spawn = App->map->GetMinionsSpawn(1);
	team2_spawn = App->map->GetMinionsSpawn(2);

	std::vector<iPoint> path_points = App->map->GetMinionPath();
	
	std::vector<iPoint> sorted_points;
	while (path_points.size() > 0)
	{
		int i = 0;
		int index = 0;
		int x_value = path_points.front().x;
		for (std::vector<iPoint>::iterator it = path_points.begin(); it != path_points.end(); it++) {
			if (it->x < x_value) {
				index = i;
				x_value = it->x;
			}
			i++;
		}
		sorted_points.push_back(path_points.at(index));
		path_points.erase(path_points.begin() + index);

	}

	int i = 1;
	minions_path.push_back(sorted_points.front());
	while (i < sorted_points.size())
	{
		if (App->pathfinding->CreatePath(sorted_points[i - 1], sorted_points[i]) > 0)
		{
			std::list<iPoint>::const_iterator it = App->pathfinding->GetLastPath()->begin();
			it++;
			for (int j = 1; j < App->pathfinding->GetLastPath()->size(); j++)
			{
				minions_path.push_back(*it);
				it++;
			}
		}
		i++;
	}

	AddMinions();

}

MinionManager::~MinionManager()
{
}

bool MinionManager::Update()
{
	bool ret = true;



	return ret;	return true;
}

const std::list<Minion*>& MinionManager::GetMinionList(uint team) const
{
	if (team == 1) 
	{
		return team1_minions;
	}
	else
	{
		return team2_minions;
	}
}

void MinionManager::KillMinion(Entity * minion)
{
}

void MinionManager::StartTimer()
{
}

void MinionManager::AddMinions()
{
	Minion* team1 = (Minion*)App->entity->CreateEntity(minion, team1_spawn);
	Minion* team2 = (Minion*)App->entity->CreateEntity(minion, team2_spawn);

	team1->SetTeam(1);
	team2->SetTeam(2);

	team1->SetBasePath(minions_path);
	minions_path.reverse();
	team2->SetBasePath(minions_path);
	minions_path.reverse();

	team1_minions.push_back(team1);
	team2_minions.push_back(team2);
}
