#include "AestheticsManager.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1Entity.h"

AestheticsManager::AestheticsManager()
{
}

AestheticsManager::~AestheticsManager()
{
}

void AestheticsManager::Start()
{
	trunk_entity = App->entity->CreateEntity(trunk, App->map->GetTrunkPosition()); 

	App->map->GetTreesPosition(trees_pos); 

	int z = 0; 
	while(z < trees_pos.size())
	{
		switch (trees_pos.at(z)->type)
		{
		case greentree:
			break; 

		case greentree:
			break;

		case greentree:
			break;

		}
	}




}

void AestheticsManager::CleanUp()
{
}


