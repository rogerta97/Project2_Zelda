#include "MainScene.h"
#include "j1Scene.h"
#include "j1Viewports.h"
#include "p2Log.h"
#include "j1Input.h"
#include "Functions.h"
#include "j1Physics.h"
#include "GameObject.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Console.h"
#include "Parallax.h"
#include "QuestManager.h"
#include "j1Entity.h"
#include "CollisionFilters.h"
#include "Link.h"
#include "j1Map.h"
#include "PlayerManager.h"
#include "j1Pathfinding.h"
#include "Minion.h"
#include "MinionManager.h"
#include "Scene.h"
#include "TowerManager.h"

MainScene::MainScene()
{
}

MainScene::~MainScene()
{
}

bool MainScene::Start()
{
	bool ret = true;

	LOG("Start MainScene");

	//Load Map
	if (App->map->Load("zelda_moba2.tmx"))
	{
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	LOG("Loading Players");
	Player* p1 = App->entity->player_manager->AddPlayer(entity_name::link, 1, iPoint(300, 700), 1);
	Player* p2 = App->entity->player_manager->AddPlayer(entity_name::link, 2, iPoint(400, 700), 2);
	Player* p3 = App->entity->player_manager->AddPlayer(entity_name::link, 3, iPoint(500, 700), 2);
	Player* p4 = App->entity->player_manager->AddPlayer(entity_name::link, 4, iPoint(600, 700), 1);

	//Test Minion
	LOG("Creating minion manager");
	minion_manager = new MinionManager();

	//Test Tower
	LOG("Creating tower manager");
	tower_manager = new TowerManager();

	//Create UI element
	SDL_Rect screen = App->view->GetViewportRect(1); 
	main_window = App->gui->UI_CreateWin(iPoint(0, 0), screen.w, screen.h, 0, true);

	progress_bar = main_window->CreateImage(iPoint(screen.w / 4 - 30, screen.h / 40), {0, 28, 385, 24 });
	princess = main_window->CreateImage(iPoint(progress_bar->rect.x + (progress_bar->rect.w / 2) - 15, progress_bar->rect.y - 5) , { 0,0,32,28 });
	rupiees_img = main_window->CreateImage(iPoint(screen.w /50 + 15 , screen.h / 40 + 5), { 32, 0, 16, 16});
	rupiees_numb = main_window->CreateText(iPoint(rupiees_img->GetPos().x, rupiees_img->GetPos().y + 30), App->font->game_font, 0, false); 
	rupiees_numb->SetText("0"); 
	minimap_icon = main_window->CreateImage(iPoint(screen.w - 50, 5), { 182, 78, 47, 47 });
	habilities.push_back(main_window->CreateImage(iPoint(screen.w  - 90 , screen.h - 100), { 182, 78, 35, 35 }));
	habilities.push_back(main_window->CreateImage(iPoint(screen.w - 90, screen.h - 60), { 182, 78, 35, 35 }));
	habilities.push_back(main_window->CreateImage(iPoint(screen.w / 50 + 30, screen.h - 100), { 182, 78, 35, 35 }));
	habilities.push_back(main_window->CreateImage(iPoint(screen.w / 50 + 30, screen.h - 60), { 182, 78, 35, 35 }));


	//Creating quests
	quest_manager = new QuestManager();
	quest_manager->CreateQuest(string("Test"), 1); 
	quest_manager->CreateQuest(string("Test"), 2);
	quest_manager->CreateQuest(string("Test"), 3);
	quest_manager->CreateQuest(string("Test"), 4);

	App->console->AddText("viewports.set 4", Input);

	App->console->AddCommand("scene.set_player_gamepad", App->scene, 2, 2, "Set to player the gampad number. Min_args: 2. Max_args: 2. Args: 1, 2, 3, 4");
	App->console->AddCommand("scene.set_player_camera", App->scene, 2, 2, "Set to player the camera number. Min_args: 2. Max_args: 2. Args: 1, 2, 3, 4");

	return ret;
}

bool MainScene::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool MainScene::Update(float dt)
{
	bool ret = true;

	App->map->Draw();
	quest_manager->Update(dt); 

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_REPEAT)
	{
		App->scene->ChangeScene((Scene*)App->scene->menu_scene);
		App->view->SetViews(1);
	}

	minion_manager->Update();

	return ret;
}

bool MainScene::PostUpdate()
{
	bool ret = true;


	return ret;
}

bool MainScene::CleanUp()
{
	bool ret = true;

	RELEASE(quest_manager);
	RELEASE(minion_manager);
	RELEASE(tower_manager);
	App->entity->player_manager->ClearPlayers();
	App->entity->ClearEntities();

	// Free UI
	if (App->scene->GetCurrentScene() != App->scene->main_scene)
	{
		App->gui->DeleteElement(main_window);
	}
	// -------

	return ret;
}

void MainScene::OnColl(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{

}

void MainScene::OnCommand(std::list<std::string>& tokens)
{
	switch (tokens.size())
	{
	case 3:
		if (tokens.front() == "scene.set_player_gamepad") {
			int player, gamepad;
			player = atoi((++tokens.begin())->c_str());
			gamepad = atoi(tokens.back().c_str());
			if (player > 0 && player <= 4 && gamepad > 0 && gamepad <= 4)
			{
				switch (player)
				{
				case 1:
					//test_player->SetGamePad(gamepad);
					break;
				}
			}
			else
			{
				LOG("Invalid player or gamepad number");
			}
		}
		else if (tokens.front() == "scene.set_player_camera") {
			int player, camera;
			player = atoi((++tokens.begin())->c_str());
			camera = atoi(tokens.back().c_str());
			if (player > 0 && player <= 4 && camera > 0 && camera <= 4)
			{
				switch (player)
				{
				case 1:
					//test_player->SetCamera(camera);
					break;
				}
			}
			else
			{
				LOG("Invalid player or camera number");
			}
		}
		break;
	default:
		break;
	}
}

