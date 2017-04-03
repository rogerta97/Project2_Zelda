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
#include "AestheticsManager.h"
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
#include "Functions.h"
#include "ShopManager.h"
#include "MenuScene.h"
#include "ZeldaManager.h"
#include "BaseManager.h"
#include "JungleCampManager.h"
#include "j1Audio.h"

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

	//Create UI ---------
	SDL_Rect screen = App->view->GetViewportRect(1);

	iPoint rupiees_pos = { screen.w / 50 + 15 , screen.h / 40 + 5 };
	SDL_Rect rupiees_rect = { 32, 0, 16, 16 };

	iPoint minimap_pos = { screen.w - 58, 5 };
	SDL_Rect minimap_rect = { 472, 588, 58, 80 };

	iPoint win_text_pos = { int(screen.w*0.5f) - 50, int(screen.h*0.5f) - 100 };

	// Player1
	main_window_1 = App->gui->UI_CreateWin(iPoint(0, 0), screen.w, screen.h, 0, true);
	main_window_1->viewport = 1;
	progress_bar_1 = main_window_1->CreateImage(iPoint(screen.w / 4 - 30, screen.h / 40), { 0, 28, 385, 24 });
	princess_1 = main_window_1->CreateImage(iPoint(progress_bar_1->rect.x + (progress_bar_1->rect.w / 2) - 15, progress_bar_1->rect.y - 5), { 0,0,32,28 });
	rupiees_img_1 = main_window_1->CreateImage(rupiees_pos, rupiees_rect);
	minimap_icon_1 = main_window_1->CreateImage(minimap_pos, minimap_rect);
	win_text_1 = main_window_1->CreateText(win_text_pos,App->font->game_font_20);
	win_text_1->enabled = false;

	// Player2
	main_window_2 = App->gui->UI_CreateWin(iPoint(0, 0), screen.w, screen.h, 0, true);
	main_window_2->viewport = 2;
	progress_bar_2 = main_window_2->CreateImage(iPoint(screen.w / 4 - 30, screen.h / 40), { 0, 28, 385, 24 });
	princess_2 = main_window_2->CreateImage(iPoint(progress_bar_2->rect.x + (progress_bar_2->rect.w / 2) - 15, progress_bar_2->rect.y - 5), { 0,0,32,28 });
	rupiees_img_2 = main_window_2->CreateImage(rupiees_pos, rupiees_rect);
	minimap_icon_2 = main_window_2->CreateImage(minimap_pos, minimap_rect);
	win_text_2 = main_window_2->CreateText(win_text_pos, App->font->game_font_20);
	win_text_2->enabled = false;

	// Player3
	main_window_3 = App->gui->UI_CreateWin(iPoint(0, 0), screen.w, screen.h, 0, true);
	main_window_3->viewport = 3;
	progress_bar_3 = main_window_3->CreateImage(iPoint(screen.w / 4 - 30, screen.h / 40), { 0, 28, 385, 24 });
	princess_3 = main_window_3->CreateImage(iPoint(progress_bar_1->rect.x + (progress_bar_3->rect.w / 2) - 15, progress_bar_3->rect.y - 5), { 0,0,32,28 });
	rupiees_img_3 = main_window_3->CreateImage(rupiees_pos, rupiees_rect);
	minimap_icon_3 = main_window_3->CreateImage(minimap_pos, minimap_rect);
	win_text_3 = main_window_3->CreateText(win_text_pos, App->font->game_font_20);
	win_text_3->enabled = false;

	// Player4
	main_window_4 = App->gui->UI_CreateWin(iPoint(0, 0), screen.w, screen.h, 0, true);
	main_window_4->viewport = 4;
	progress_bar_4 = main_window_4->CreateImage(iPoint(screen.w / 4 - 30, screen.h / 40), { 0, 28, 385, 24 });
	princess_4 = main_window_4->CreateImage(iPoint(progress_bar_4->rect.x + (progress_bar_4->rect.w / 2) - 15, progress_bar_4->rect.y - 5), { 0,0,32,28 });
	rupiees_img_4 = main_window_4->CreateImage(rupiees_pos, rupiees_rect);
	minimap_icon_4 = main_window_4->CreateImage(minimap_pos, minimap_rect);
	win_text_4 = main_window_4->CreateText(win_text_pos, App->font->game_font_20);
	win_text_4->enabled = false;

	// ------------------

	//Load Map
	if (App->map->Load("zelda_moba.tmx"))
	{
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	CreateMapCollisions();

	// Shop Manager
	shop_manager = new ShopManager();
	shop_manager->Start();

	// Loading Players
	player_manager = new PlayerManager();
	player_manager->Start();

	LOG("Loading Players");
	bool def = false;
	for (int i = 0; i < 4; i++)
	{
		if (App->scene->players[i].character == e_n_null)
		{
			def = true;
			break;
		}
	}
	if (!def)
	{
		Player* p1 = player_manager->AddPlayer(App->scene->players[0].character, iPoint(300, 700), App->scene->players[0].gamepad, App->scene->players[0].viewport, App->scene->players[0].team,1);
		Player* p2 = player_manager->AddPlayer(App->scene->players[1].character, iPoint(300, 700), App->scene->players[1].gamepad, App->scene->players[1].viewport, App->scene->players[1].team,1);
		Player* p3 = player_manager->AddPlayer(App->scene->players[2].character, iPoint(300, 700), App->scene->players[2].gamepad, App->scene->players[2].viewport, App->scene->players[2].team,2);
		Player* p4 = player_manager->AddPlayer(App->scene->players[3].character, iPoint(300, 700), App->scene->players[3].gamepad, App->scene->players[3].viewport, App->scene->players[3].team,2);
	}
	else
	{
		Player* p1 = player_manager->AddPlayer(entity_name::link, iPoint(300, 700), 1, 1, 1, 1);
		Player* p2 = player_manager->AddPlayer(entity_name::link, iPoint(300, 700), 2, 2, 1, 2);
		Player* p3 = player_manager->AddPlayer(entity_name::link, iPoint(300, 700), 3, 3, 2, 1);
		Player* p4 = player_manager->AddPlayer(entity_name::link, iPoint(300, 700), 4, 4, 2, 2);
	}

	// Disable player input until level is loaded
	player_manager->DisableInput(0);
	// ----

	//Test Jungle Camp
	jungleCamp_manager = new JungleCampManager();
	jungleCamp_manager->Start();

	//Test Minion
	LOG("Creating minion manager");
	minion_manager = new MinionManager();

	//Test Tower
	LOG("Creating tower manager");
	tower_manager = new TowerManager();
  
	// Zelda manager
	zelda_manager = new ZeldaManager();
  
	// Aesth manager
	aest_manager = new AestheticsManager(); 
	aest_manager->Start(); 

	// Base manager
	base_manager = new BaseManager();

	//Creating quests
	quest_manager = new QuestManager();
	quest_manager->CreateQuest(string("Test"), 1);
	quest_manager->CreateQuest(string("Test"), 2);
	quest_manager->CreateQuest(string("Test"), 3);
	quest_manager->CreateQuest(string("Test"), 4);

	// Allow player input once the level is loaded
	player_manager->AllowInput(0);
	// ----

	game_timer.Start();

	App->console->AddText("viewports.set 4", Input);

	App->console->AddCommand("scene.set_player_gamepad", App->scene, 2, 2, "Set to player the gampad number. Min_args: 2. Max_args: 2. Args: 1, 2, 3, 4");
	App->console->AddCommand("scene.set_player_camera", App->scene, 2, 2, "Set to player the camera number. Min_args: 2. Max_args: 2. Args: 1, 2, 3, 4");

	App->audio->ChangeVolume(25);
	App->audio->PlayMusic("Audio/Music/overworld.ogg");

	return ret;
}

bool MainScene::PreUpdate()
{
	bool ret = true;

	if (player_manager != nullptr)
		player_manager->PreUpdate();

	return ret;
}

bool MainScene::Update(float dt)
{
	bool ret = true;

	// Draw map
	App->map->Draw();

	// Update Managers
	if(minion_manager != nullptr)
		minion_manager->Update();
	if(shop_manager != nullptr)
		shop_manager->Update();
	if (quest_manager != nullptr)
		quest_manager->Update(dt);
	if(player_manager != nullptr)
		player_manager->Update(dt);
	if (jungleCamp_manager != nullptr)
		jungleCamp_manager->Update(dt);
	// ------

	// Update progress bar
	if (zelda_manager != nullptr && winner == 0)
		UpdateProgressBar();
	// ------

	// End Game
	if (winner != 0 && game_timer.ReadSec() > end_delay)
	{
		App->scene->ChangeScene((Scene*)App->scene->menu_scene);
		App->view->SetViews(1);
	}

	// Test
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_REPEAT)
	{
		App->scene->ChangeScene((Scene*)App->scene->menu_scene);
		App->view->SetViews(1);
	}
	// ------

	//DrawScreenSeparation();

	return ret;
}

bool MainScene::PostUpdate()
{
	bool ret = true;

	if (player_manager != nullptr)
		player_manager->PostUpdate();

	return ret;
}

bool MainScene::CleanUp()
{
	bool ret = true;

	// Unload Managers
	shop_manager->CleanUp(); 	   RELEASE(shop_manager)
	zelda_manager->CleanUp(); 	   RELEASE(zelda_manager);
	base_manager->CleanUp(); 	   RELEASE(base_manager);
	minion_manager->CleanUp(); 	   RELEASE(minion_manager);
	quest_manager->CleanUp(); 	   RELEASE(quest_manager);
	tower_manager->CleanUp(); 	   RELEASE(tower_manager);
	aest_manager->CleanUp(); 	   RELEASE(aest_manager);
	player_manager->CleanUp();	   RELEASE(player_manager);
	jungleCamp_manager->CleanUp(); RELEASE(jungleCamp_manager);

	App->map->CleanUp();
	App->entity->ClearEntities();
	App->spell->ClearSpells();

	// Free UI
	if (App->scene->GetCurrentScene() != App->scene->main_scene)	
	{
		App->gui->DeleteElement(main_window_1);
		App->gui->DeleteElement(main_window_2);
		App->gui->DeleteElement(main_window_3);
		App->gui->DeleteElement(main_window_4);
	}
	// -------

	// Delete Map Collisions
	for (std::vector<PhysBody*>::iterator it = map_collisions.begin(); it != map_collisions.end(); it++)
	{
		App->physics->DeleteBody(*it);
	}
	map_collisions.clear();
	// -------

	winner = 0;

	for (int i = 0; i < 4; i++)
	{
		App->scene->players[i].character = e_n_null;
	}

	//Stop Music
	App->audio->StopMusic();

	return ret;
}

j1Timer * MainScene::GetGameTimer()
{
	return &game_timer;
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

void MainScene::EndGame(int _winner)
{
	player_manager->DisableInput(0);

	winner = _winner;

	SetWinnerText(winner);

	game_timer.Start();
}

void MainScene::UpdateProgressBar()
{
	iPoint zelda_pos = App->map->WorldToMap(zelda_manager->GetZeldaPos().x, zelda_manager->GetZeldaPos().y);

	float percentage = (zelda_pos.x-27) * 100 / 95;
	percentage /= 100;

	int delta = (progress_bar_1->rect.w * percentage) - princess_1->rect.w/2;

	princess_1->SetPos({ progress_bar_1->GetPos().x + delta, progress_bar_1->GetPos().y - 5 });
	princess_2->SetPos({ progress_bar_2->GetPos().x + delta, progress_bar_2->GetPos().y - 5 });
	princess_3->SetPos({ progress_bar_3->GetPos().x + delta, progress_bar_3->GetPos().y - 5 });
	princess_4->SetPos({ progress_bar_4->GetPos().x + delta, progress_bar_4->GetPos().y - 5 });
}

void MainScene::CreateMapCollisions()
{
	pugi::xml_document doc;
	App->LoadXML("MapCollisions.xml", doc);
	pugi::xml_node collisions = doc.child("collisions");
	
	for(pugi::xml_node chain = collisions.child("chain");chain != NULL;chain = chain.next_sibling("chain"))
	{
		string points_string = chain.child_value();
		int num_points = chain.attribute("vertex").as_int();
		int* points = new int[num_points];

		std::list<string> points_list;
		Tokenize(points_string, ',', points_list);

		int i = 0;
		for (std::list<string>::iterator it = points_list.begin(); it != points_list.end(); it++)
		{
			if (i >= num_points)
				break;

			if (*it != "")
			{
				*(points + i) = stoi(*it);
				i++;
			}
		}
		PhysBody* b = App->physics->CreateStaticChain(0, 0, points, num_points, 1, 0, 0.0f, App->cf->CATEGORY_SCENERY, App->cf->MASK_SCENERY);
		b->type = pbody_type::p_t_world;

		map_collisions.push_back(b);
		RELEASE_ARRAY(points);
	}

	
}

void MainScene::DrawScreenSeparation()
{
	uint win_w, win_h;
	App->win->GetWindowSize(win_w, win_h);

	for (uint y = 0; y < win_h - 2; y += 2)
	{
		App->view->LayerBlit(20, App->gui->atlas, iPoint(win_w / 4 - 2, y), { 130,0,4,2 });
	}

	for (uint x = 0; x < win_w - 2; x += 2)
	{
		App->view->LayerBlit(20, App->gui->atlas, iPoint(x, win_h / 4 - 4), { 130,2,2,4 });
	}

	App->view->LayerBlit(20, App->gui->atlas, iPoint(win_w / 4 - 2, win_h / 4 - 6), { 130,0,6,6 });
}

void MainScene::SetWinnerText(uint winner)
{
	SDL_Color win_color = { 46,150,255,255 };
	SDL_Color lose_color = { 255,0,0,255 };

	switch (winner)
	{
	case 1:	
		win_text_1->color = win_color;
		win_text_1->SetText("VICTORY");	

		win_text_3->color = win_color;
		win_text_3->SetText("VICTORY");

		win_text_2->color = lose_color;
		win_text_2->SetText("DEFEAT");

		win_text_4->color = lose_color;
		win_text_4->SetText("DEFEAT");
		break;
	case 2:
		win_text_2->color = win_color;
		win_text_2->SetText("VICTORY");
		
		win_text_4->color = win_color;
		win_text_4->SetText("VICTORY");
		
		win_text_1->color = lose_color;
		win_text_1->SetText("DEFEAT");
		
		win_text_3->color = lose_color;
		win_text_3->SetText("DEFEAT");
		break;
	default:
		break;
	}

	win_text_1->enabled = true;
	win_text_2->enabled = true;
	win_text_3->enabled = true;
	win_text_4->enabled = true;
}

