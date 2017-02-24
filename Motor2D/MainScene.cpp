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

	App->entity->player_manager->AddPlayer(entity_name::link, 1, iPoint(300,20));
	//App->entity->player_manager->AddPlayer(entity_name::link, 2);

	App->console->AddCommand("scene.set_player_gamepad", App->scene, 2, 2, "Set to player the gampad number. Min_args: 2. Max_args: 2. Args: 1, 2, 3, 4");
	App->console->AddCommand("scene.set_player_camera", App->scene, 2, 2, "Set to player the camera number. Min_args: 2. Max_args: 2. Args: 1, 2, 3, 4");

	//Load Map
	App->map->Load("zelda_test2.tmx");

	//Create UI element
	SDL_Rect screen = App->view->GetViewportRect(1); 
	main_window = App->gui->UI_CreateWin(iPoint(0, 0), screen.w, screen.h, 0);

	exp_bar = main_window->CreateImage(iPoint(screen.w / 4 - 30, screen.h/40), SDL_Rect{ 1, 32, 385, 21 });
	rupiees = main_window->CreateImage(iPoint(screen.w /50 + 15, screen.h / 40), SDL_Rect{ 127, 1, 18, 18 });

	//Creating quests
	quest_ball = main_window->CreateImage(iPoint(screen.w - 10, screen.h - 10), SDL_Rect{112, 2, 12, 12});

	quest_manager = new QuestManager();

	quest_manager->CreateQuest(nullptr, quest_ball, SDL_Rect{ 91, 11, 19, 18 }, 1);
	quest_manager->CreateQuest(nullptr, quest_ball, SDL_Rect{ 91, 11, 19, 18 }, 2);
	quest_manager->CreateQuest(nullptr, quest_ball, SDL_Rect{ 91, 11, 19, 18 }, 3);
	quest_manager->CreateQuest(nullptr, quest_ball, SDL_Rect{ 91, 11, 19, 18 }, 4);
	quest_manager->CreateQuest(nullptr, quest_ball, SDL_Rect{ 91, 11, 19, 18 }, 5);

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

