#include "ZeldaManager.h"
#include "j1Scene.h"
#include "MainScene.h"
#include "j1Entity.h"
#include "Zelda.h"
#include "j1Map.h"

ZeldaManager::ZeldaManager()
{
	iPoint pos(App->map->GetZeldaPosition());
	zelda = (Zelda*)App->entity->CreateEntity(entity_name::zelda, { pos.x + 16, pos.y + 30 });
	zelda->SetTimer(App->scene->main_scene->GetGameTimer());

	// Set Zelda paths
	zelda->SetInitialPath(App->map->GetZeldaInitPath());

	vector<iPoint> path_unsorted = App->map->GetZeldaPath();
	vector<iPoint> path_sorted;

	while (path_unsorted.size() > 0)
	{
		int i = 0;
		int index = 0;
		int x_value = path_unsorted.front().x;
		for (std::vector<iPoint>::iterator it = path_unsorted.begin(); it != path_unsorted.end(); it++) {
			if (it->x < x_value) {
				index = i;
				x_value = it->x;
			}
			i++;
		}
		path_sorted.push_back(path_unsorted.at(index));
		path_unsorted.erase(path_unsorted.begin() + index);

	}

	zelda->SetPath(path_sorted);
	//-----

}

ZeldaManager::~ZeldaManager()
{
}
