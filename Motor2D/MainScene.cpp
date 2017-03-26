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
#include "Functions.h"
#include "ShopManager.h"

#include "ZeldaManager.h"

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
	if (App->map->Load("zelda_moba.tmx"))
	{
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	// Shop Manager
	shop_manager = new ShopManager();
	shop_manager->Start();

	// Loading Players
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
		Player* p1 = App->entity->player_manager->AddPlayer(App->scene->players[0].character, iPoint(300, 700), App->scene->players[0].gamepad, App->scene->players[0].viewport, App->scene->players[0].team);
		Player* p2 = App->entity->player_manager->AddPlayer(App->scene->players[1].character, iPoint(300, 700), App->scene->players[1].gamepad, App->scene->players[1].viewport, App->scene->players[1].team);
		Player* p3 = App->entity->player_manager->AddPlayer(App->scene->players[2].character, iPoint(300, 700), App->scene->players[2].gamepad, App->scene->players[2].viewport, App->scene->players[2].team);
		Player* p4 = App->entity->player_manager->AddPlayer(App->scene->players[3].character, iPoint(300, 700), App->scene->players[3].gamepad, App->scene->players[3].viewport, App->scene->players[3].team);
	}
	else
	{
		Player* p1 = App->entity->player_manager->AddPlayer(entity_name::link, iPoint(300, 700), 1, 1, 1);
		Player* p2 = App->entity->player_manager->AddPlayer(entity_name::link, iPoint(300, 700), 2, 2, 1);
		Player* p3 = App->entity->player_manager->AddPlayer(entity_name::link, iPoint(300, 700), 3, 3, 2);
		Player* p4 = App->entity->player_manager->AddPlayer(entity_name::link, iPoint(300, 700), 4, 4, 2);
	}

	//Test Minion
	LOG("Creating minion manager");
	minion_manager = new MinionManager();

	//Test Tower
	LOG("Creating tower manager");
	tower_manager = new TowerManager();

	//Create UI ---------
	SDL_Rect screen = App->view->GetViewportRect(1);
	iPoint ability1_pos = { screen.w  - 120 , screen.h - 126};
	iPoint ability2_pos = { (screen.w / 50), screen.h - 126 };
	iPoint ability3_pos = { screen.w - 90, screen.h - 76 };
	iPoint ability4_pos = { (screen.w) / 50, screen.h - 76 };

	iPoint rupiees_pos = { screen.w / 50 + 15 , screen.h / 40 + 5 };
	SDL_Rect rupiees_rect = { 32, 0, 16, 16 };

	iPoint minimap_pos = { screen.w - 58, 5 };
	SDL_Rect minimap_rect = { 472, 588, 58, 80 };

	// Player1
	main_window_1 = App->gui->UI_CreateWin(iPoint(0, 0), screen.w, screen.h, 0, true);
	main_window_1->viewport = 1;
	progress_bar_1 = main_window_1->CreateImage(iPoint(screen.w / 4 - 30, screen.h / 40), {0, 28, 385, 24 });
	princess_1 = main_window_1->CreateImage(iPoint(progress_bar_1->rect.x + (progress_bar_1->rect.w / 2) - 15, progress_bar_1->rect.y - 5) , { 0,0,32,28 });
	rupiees_img_1 = main_window_1->CreateImage(rupiees_pos, rupiees_rect);
	minimap_icon_1 = main_window_1->CreateImage(minimap_pos, minimap_rect);

	habilities_1.push_back(main_window_1->CreateImage(ability1_pos, { 182, 78, 35, 35 }));
	habilities_1.push_back(main_window_1->CreateImage(ability2_pos, { 182, 78, 35, 35 }));
	habilities_1.push_back(main_window_1->CreateImage(ability3_pos, { 182, 78, 35, 35 }));
	habilities_1.push_back(main_window_1->CreateImage(ability4_pos, { 182, 78, 35, 35 }));

	// Player2
	main_window_2 = App->gui->UI_CreateWin(iPoint(0, 0), screen.w, screen.h, 0, true);
	main_window_2->viewport = 2;
	progress_bar_2 = main_window_2->CreateImage(iPoint(screen.w / 4 - 30, screen.h / 40), { 0, 28, 385, 24 });
	princess_2 = main_window_2->CreateImage(iPoint(progress_bar_2->rect.x + (progress_bar_2->rect.w / 2) - 15, progress_bar_2->rect.y - 5), { 0,0,32,28 });
	rupiees_img_2 = main_window_2->CreateImage(rupiees_pos, rupiees_rect);
	minimap_icon_2 = main_window_2->CreateImage(minimap_pos, minimap_rect);

	habilities_2.push_back(main_window_2->CreateImage(ability1_pos, { 182, 78, 35, 35 }));
	habilities_2.push_back(main_window_2->CreateImage(ability2_pos, { 182, 78, 35, 35 }));
	habilities_2.push_back(main_window_2->CreateImage(ability3_pos, { 182, 78, 35, 35 }));
	habilities_2.push_back(main_window_2->CreateImage(ability4_pos, { 182, 78, 35, 35 }));

	// Player3
	main_window_3 = App->gui->UI_CreateWin(iPoint(0, 0), screen.w, screen.h, 0, true);
	main_window_3->viewport = 3;
	progress_bar_3 = main_window_3->CreateImage(iPoint(screen.w / 4 - 30, screen.h / 40), { 0, 28, 385, 24 });
	princess_3 = main_window_3->CreateImage(iPoint(progress_bar_1->rect.x + (progress_bar_3->rect.w / 2) - 15, progress_bar_3->rect.y - 5), { 0,0,32,28 });
	rupiees_img_3 = main_window_3->CreateImage(rupiees_pos, rupiees_rect);
	minimap_icon_3 = main_window_3->CreateImage(minimap_pos, minimap_rect);

	habilities_3.push_back(main_window_3->CreateImage(ability1_pos, { 182, 78, 35, 35 }));
	habilities_3.push_back(main_window_3->CreateImage(ability2_pos, { 182, 78, 35, 35 }));
	habilities_3.push_back(main_window_3->CreateImage(ability3_pos, { 182, 78, 35, 35 }));
	habilities_3.push_back(main_window_3->CreateImage(ability4_pos, { 182, 78, 35, 35 }));

	// Player4
	main_window_4 = App->gui->UI_CreateWin(iPoint(0, 0), screen.w, screen.h, 0, true);
	main_window_4->viewport = 4;
	progress_bar_4 = main_window_4->CreateImage(iPoint(screen.w / 4 - 30, screen.h / 40), { 0, 28, 385, 24 });
	princess_4 = main_window_4->CreateImage(iPoint(progress_bar_4->rect.x + (progress_bar_4->rect.w / 2) - 15, progress_bar_4->rect.y - 5), { 0,0,32,28 });
	rupiees_img_4 = main_window_4->CreateImage(rupiees_pos, rupiees_rect);
	minimap_icon_4 = main_window_4->CreateImage(minimap_pos, minimap_rect);

	habilities_4.push_back(main_window_4->CreateImage(ability1_pos, { 182, 78, 35, 35 }));
	habilities_4.push_back(main_window_4->CreateImage(ability2_pos, { 182, 78, 35, 35 }));
	habilities_4.push_back(main_window_4->CreateImage(ability3_pos, { 182, 78, 35, 35 }));
	habilities_4.push_back(main_window_4->CreateImage(ability4_pos, { 182, 78, 35, 35 }));

	// ------------------

	//Creating quests
	quest_manager = new QuestManager();
	quest_manager->CreateQuest(string("Test"), 1); 
	quest_manager->CreateQuest(string("Test"), 2);
	quest_manager->CreateQuest(string("Test"), 3);
	quest_manager->CreateQuest(string("Test"), 4);

	App->console->AddText("viewports.set 4", Input);

	App->console->AddCommand("scene.set_player_gamepad", App->scene, 2, 2, "Set to player the gampad number. Min_args: 2. Max_args: 2. Args: 1, 2, 3, 4");
	App->console->AddCommand("scene.set_player_camera", App->scene, 2, 2, "Set to player the camera number. Min_args: 2. Max_args: 2. Args: 1, 2, 3, 4");

	CreateMapCollisions();



	//Testing
	game_timer.Start();
	
	zelda_manager = new ZeldaManager();


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
	shop_manager->Update();

	// UI Control -----------

	for (int i = 0; i < App->entity->player_manager->players.size(); i++)
	{
		Player* curr_player = App->entity->player_manager->players.at(i);

		switch (curr_player->viewport)
		{
		case 1:
			if (curr_player->entity->GetAbility(0)->CdCompleted())
			{
				habilities_1.at(0)->ChangeImage(curr_player->entity->GetAbility(0)->ablility_avaliable);
			}
			else
			{
				habilities_1.at(0)->ChangeImage(NULLRECT);
			}
			if (curr_player->entity->GetAbility(1)->CdCompleted())
			{
				habilities_1.at(1)->ChangeImage(curr_player->entity->GetAbility(1)->ablility_avaliable);
			}
			else
			{
				habilities_1.at(1)->ChangeImage(NULLRECT);
			}
			if (curr_player->entity->GetAbility(2)->CdCompleted())
			{
				habilities_1.at(2)->ChangeImage(curr_player->entity->GetAbility(2)->ablility_avaliable);
			}
			else
			{
				habilities_1.at(2)->ChangeImage(NULLRECT);
			}
			if (curr_player->entity->GetAbility(3)->CdCompleted())
			{
				habilities_1.at(3)->ChangeImage(curr_player->entity->GetAbility(3)->ablility_avaliable);
			}
			else
			{
				habilities_1.at(3)->ChangeImage(NULLRECT);
			}
			break;
		case 2:
			if (curr_player->entity->GetAbility(0)->CdCompleted())
			{
				habilities_2.at(0)->ChangeImage(curr_player->entity->GetAbility(0)->ablility_avaliable);
			}
			else
			{
				habilities_2.at(0)->ChangeImage(NULLRECT);
			}
			if (curr_player->entity->GetAbility(1)->CdCompleted())
			{
				habilities_2.at(1)->ChangeImage(curr_player->entity->GetAbility(1)->ablility_avaliable);
			}
			else
			{
				habilities_2.at(1)->ChangeImage(NULLRECT);
			}
			if (curr_player->entity->GetAbility(2)->CdCompleted())
			{
				habilities_2.at(2)->ChangeImage(curr_player->entity->GetAbility(2)->ablility_avaliable);
			}
			else
			{
				habilities_2.at(2)->ChangeImage(NULLRECT);
			}
			if (curr_player->entity->GetAbility(3)->CdCompleted())
			{
				habilities_2.at(3)->ChangeImage(curr_player->entity->GetAbility(3)->ablility_avaliable);
			}
			else
			{
				habilities_2.at(3)->ChangeImage(NULLRECT);
			}
			break;
		case 3:
			if (curr_player->entity->GetAbility(0)->CdCompleted())
			{
				habilities_3.at(0)->ChangeImage(curr_player->entity->GetAbility(0)->ablility_avaliable);
			}
			else
			{
				habilities_3.at(0)->ChangeImage(NULLRECT);
			}
			if (curr_player->entity->GetAbility(1)->CdCompleted())
			{
				habilities_3.at(1)->ChangeImage(curr_player->entity->GetAbility(1)->ablility_avaliable);
			}
			else
			{
				habilities_3.at(1)->ChangeImage(NULLRECT);
			}
			if (curr_player->entity->GetAbility(2)->CdCompleted())
			{
				habilities_3.at(2)->ChangeImage(curr_player->entity->GetAbility(2)->ablility_avaliable);
			}
			else
			{
				habilities_3.at(2)->ChangeImage(NULLRECT);
			}
			if (curr_player->entity->GetAbility(3)->CdCompleted())
			{
				habilities_3.at(3)->ChangeImage(curr_player->entity->GetAbility(3)->ablility_avaliable);
			}
			else
			{
				habilities_3.at(3)->ChangeImage(NULLRECT);
			}
			break;
		case 4:
			if (curr_player->entity->GetAbility(0)->CdCompleted())
			{
				habilities_4.at(0)->ChangeImage(curr_player->entity->GetAbility(0)->ablility_avaliable);
			}
			else
			{
				habilities_4.at(0)->ChangeImage(NULLRECT);
			}
			if (curr_player->entity->GetAbility(1)->CdCompleted())
			{
				habilities_4.at(1)->ChangeImage(curr_player->entity->GetAbility(1)->ablility_avaliable);
			}
			else
			{
				habilities_4.at(1)->ChangeImage(NULLRECT);
			}
			if (curr_player->entity->GetAbility(2)->CdCompleted())
			{
				habilities_4.at(2)->ChangeImage(curr_player->entity->GetAbility(2)->ablility_avaliable);
			}
			else
			{
				habilities_4.at(2)->ChangeImage(NULLRECT);
			}
			if (curr_player->entity->GetAbility(3)->CdCompleted())
			{
				habilities_4.at(3)->ChangeImage(curr_player->entity->GetAbility(3)->ablility_avaliable);
			}
			else
			{
				habilities_4.at(3)->ChangeImage(NULLRECT);
			}
			break;
		}
	}

	// --------------

	//DrawScreenSeparation();

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
	shop_manager->CleanUp();

	RELEASE(quest_manager);
	RELEASE(minion_manager);
	RELEASE(tower_manager);
	RELEASE(shop_manager);

	App->entity->player_manager->ClearPlayers();
	App->entity->ClearEntities();

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

