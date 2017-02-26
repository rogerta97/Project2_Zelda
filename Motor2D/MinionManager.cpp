#include "MinionManager.h"
#include "j1App.h"
#include "j1Pathfinding.h"

MinionManager::MinionManager()
{
	pugi::xml_document doc;
	App->LoadXML("GameSettings.xml", doc);

	pugi::xml_node minions_node = doc.child("file").child("minions");

	wave_time = minions_node.child("wave_time").attribute("sec").as_float();
	minion_time_difference = minions_node.child("minion_time_difference").attribute("sec").as_float();
	minions_wave = minions_node.child("minions_wave").attribute("amount").as_uint();
	first_wave_delay = minions_node.child("first_wave_delay").attribute("sec").as_float();



}

MinionManager::~MinionManager()
{
}

bool MinionManager::Update()
{
	return false;
}

const std::list<Minion*>& MinionManager::GetMinionList(uint team) const
{
	if (team == 1) 
	{
		return team1_minons;
	}
	else
	{
		return team2_minons;
	}
}

void MinionManager::KillMinion(Entity * minion)
{
}

void MinionManager::StartTimer()
{
}
