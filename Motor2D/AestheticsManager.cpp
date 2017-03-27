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
	
	App->map->GetTreesPosition(trees_nodes); 
	
	int z = 0; 
	while(z < trees_nodes.size())
	{

 		switch (trees_nodes.at(z)->color) 
		{
		case green_tree:
			
			break; 

		case yellow_tree:
			break;

		case purple_tree:
			break;

		}

		z++; 	
	}




}

void AestheticsManager::CleanUp()
{
}


