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

}

void AestheticsManager::CleanUp()
{
}


