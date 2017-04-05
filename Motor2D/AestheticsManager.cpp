#include "AestheticsManager.h"
#include "j1Map.h"
#include "Trees.h"
#include "GameObject.h"
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
		bushes_entity.push_back((Bush*)App->entity->CreateEntity(bush, iPoint(bushes_nodes.at(z)->bush_pos.x, bushes_nodes.at(z)->bush_pos.y)));

		switch (bushes_nodes.at(z)->color)
		{
		case green_bush:
			bushes_entity.at(z)->game_object->SetPos(fPoint(bushes_entity.at(z)->game_object->GetPos().x, bushes_entity.at(z)->game_object->GetPos().y - 5));
			bushes_entity.at(z)->SetBushColor("green");
			bushes_entity.at(z)->SetMiddle(green_bush);
			break;

		case purple_bush:
			bushes_entity.at(z)->game_object->SetPos(fPoint(bushes_entity.at(z)->game_object->GetPos().x, bushes_entity.at(z)->game_object->GetPos().y - 5));
			bushes_entity.at(z)->SetBushColor("purple");
			bushes_entity.at(z)->SetMiddle(purple_bush);
			break;

		case green_half_bush:
			bushes_entity.at(z)->SetBushColor("green_half");
			bushes_entity.at(z)->SetMiddle(green_half_bush);
			break;

		case purple_half_bush:
			bushes_entity.at(z)->SetBushColor("purple_half");
			bushes_entity.at(z)->SetMiddle(purple_half_bush);
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

	//Waterfall
	App->entity->CreateEntity(entity_name::waterfall, App->map->GetWaterfallPosition());
	// -----

}

void AestheticsManager::CleanUp()
{
	//Clear Trees
	if (!trees_entity.empty())
	{
		for (std::vector<Tree*>::iterator it = trees_entity.begin(); it != trees_entity.end();)
		{
			App->entity->DeleteEntity(*it);
			it = trees_entity.erase(it);
		}
	}
	// -----

	//Clear Eyes
	if (!eyes.empty())
	{
		for (std::vector<Eyes*>::iterator it = eyes.begin(); it != eyes.end(); ++it)
		{
			App->entity->DeleteEntity(*it);
			it = eyes.erase(it);
		}
	}
	// -----

	//Clear bushes
	if (!bushes_entity.empty())
	{
		for (std::vector<Bush*>::iterator it = bushes_entity.begin(); it != bushes_entity.end();)
		{
			App->entity->DeleteEntity(*it);
			it = bushes_entity.erase(it);
		}
	}

	if (!trees_nodes.empty())
	{
		for (vector<TreeNode*>::iterator it = trees_nodes.begin(); it != trees_nodes.end();)
		{
			RELEASE(*it);
			it = trees_nodes.erase(it);
		}
	}

	if (!bushes_nodes.empty())
	{
		for (vector<BushNode*>::iterator it = bushes_nodes.begin(); it != bushes_nodes.end();)
		{
			RELEASE(*it);
			it = bushes_nodes.erase(it);
		}
	}

	App->entity->DeleteEntity(trunk_entity);
}


