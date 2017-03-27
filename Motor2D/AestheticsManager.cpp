#include "AestheticsManager.h"
#include "j1Map.h"
#include "Trees.h"
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
		trees_entity.push_back((Tree*)App->entity->CreateEntity(tree, trees_nodes.at(z)->tree_pos)); 

 		switch (trees_nodes.at(z)->color) 
		{
		case green_tree:
			trees_entity.at(z)->SetTreeColor("green"); 
			break; 

		case yellow_tree:
			trees_entity.at(z)->SetTreeColor("yellow");
			break;

		case purple_tree:
			trees_entity.at(z)->SetTreeColor("purple");
			break;

		}

		z++; 	
	}




}

void AestheticsManager::CleanUp()
{
}


