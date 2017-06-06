#include "ZeldaManager.h"
#include "j1Scene.h"
#include "MainScene.h"
#include "j1Entity.h"
#include "Zelda.h"
#include "j1Map.h"
#include "j1Audio.h"



ZeldaManager::ZeldaManager()
{
	//Anouncers
	zelda_anouncer = App->audio->LoadFx("Audio/Voice act/be_prepared_1.wav");
	zelda_intro_1 = App->audio->LoadFx("Audio/Voice act/intro_1.wav");
	zelda_intro_2 = App->audio->LoadFx("Audio/Voice act/intro_2.wav");


	iPoint pos(App->map->GetZeldaPosition());
	zelda = (Zelda*)App->entity->CreateEntity(entity_name::zelda, { pos.x + 16, pos.y + 30 });
	zelda->SetTimer(App->scene->main_scene->GetGameTimer());
	spawn_time = zelda->GetActivationTime();

	// Set Zelda paths
	zelda->SetInitialPath(App->map->GetZeldaInitPath());

	vector<iPoint> path_unsorted = App->map->GetZeldaPath();
	vector<iPoint> path_sorted;

	while (path_unsorted.size() > 0)
	{
		int i = 0;
		int index = 0;
		int x_value = path_unsorted.front().x;
		for (std::vector<iPoint>::iterator it = path_unsorted.begin(); it != path_unsorted.end(); it++) 
		{
			if (it->x < x_value) 
			{
				index = i;
				x_value = it->x;
			}
			i++;
		}

		path_sorted.push_back(path_unsorted.at(index));
		path_unsorted.erase(path_unsorted.begin() + index);
	}

	zelda->SetPath(path_sorted);
	// -----
}

ZeldaManager::~ZeldaManager()
{
}

bool ZeldaManager::Update()
{
	bool ret = true;

	if (App->scene->main_scene->GetGameTimer()->ReadSec() > (GetSpawnTime() - 60) && anounced == false)
	{
		anounced = true;
		App->audio->PlayFx(zelda_anouncer,0);
	}
	if (App->scene->main_scene->GetGameTimer()->ReadSec() > GetSpawnTime() && anounced == true)
	{
		App->audio->PlayFx(GetRandomValue(zelda_intro_1,zelda_intro_2), 0);
	}

	return ret;
}

void ZeldaManager::CleanUp()
{
	App->entity->DeleteEntity(zelda);
	anounced = false;
}

iPoint ZeldaManager::GetZeldaPos()
{
	return zelda->GetPos();
}

Zelda * ZeldaManager::GetZelda()
{
	Zelda* ret = nullptr;

	if(zelda!= nullptr)
		ret = zelda;

	return ret;
}

int ZeldaManager::GetSpawnTime()
{
	Zelda* zelda = GetZelda(); 

	if (zelda != nullptr)
		return spawn_time;

	else
		return -1; 
}

zelda_states ZeldaManager::GetZeldaState()
{
	return zelda->GetZeldaState();
}
