#include "AestheticsManager.h"
#include "j1Map.h"
#include "Trees.h"
#include "Bushes.h"
#include "j1App.h"
#include "j1Entity.h"
#include "Eyes.h"

AestheticsManager::AestheticsManager()
{
}

AestheticsManager::~AestheticsManager()
{
}

void AestheticsManager::Start()
{
	//Trunk
	trunk_entity = App->entity->CreateEntity(trunk, App->map->GetTrunkPosition()); 
	// -----

	//Trees
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
	// -----

	// Bushes 
	App->map->GetBushesPosition(bushes_nodes);

	 z = 0;
	while (z < bushes_nodes.size())
	{
		bushes_entity.push_back((Bush*)App->entity->CreateEntity(bush, bushes_nodes.at(z)->bush_pos));

		switch (bushes_nodes.at(z)->color)
		{
		case green_bush:
			bushes_entity.at(z)->SetBushColor("green");
			break;

		case purple_bush:
			bushes_entity.at(z)->SetBushColor("purple");
			break;
		}
		z++;
	}
	// -----

	//Eyes
	std::vector<iPoint> eyes_positions = App->map->GetEyesPositions();

	for (std::vector<iPoint>::iterator it = eyes_positions.begin(); it != eyes_positions.end(); ++it)
	{
		eyes.push_back((Eyes*)App->entity->CreateEntity(entity_name::eyes, *it));
	}
	// -----

}

void AestheticsManager::CleanUp()
{
	//Clear Trees
	for (std::vector<Tree*>::iterator it = trees_entity.begin(); it != trees_entity.end();)
	{
		App->entity->DeleteEntity(*it);
		it = trees_entity.erase(it);
	}
	// -----

	//Clear Eyes
	for (std::vector<Eyes*>::iterator it = eyes.begin(); it != eyes.end(); ++it)
	{
		App->entity->DeleteEntity(*it);
		it = eyes.erase(it);
	}
	// -----
}


