#include "MainScene.h"
#include "j1Scene.h"
#include "j1Viewports.h"
#include "p2Log.h"
#include "j1Input.h"
#include "j1Physics.h"
#include "GameObject.h"
#include "j1App.h"
#include "j1Gui.h"
#include "AestheticsManager.h"
#include "j1Console.h"
#include "CollisionFilters.h"
#include "Link.h"
#include "j1Map.h"
#include "PlayerManager.h"
#include "j1Pathfinding.h"
#include "MinionManager.h"
#include "Scene.h"
#include "TowerManager.h"
#include "Functions.h"
#include "ShopManager.h"
#include "MenuScene.h"
#include "ZeldaManager.h"
#include "BaseManager.h"
#include "Quest_Manager.h"
#include "JungleCampManager.h"
#include "EventThrower.h"
#include "j1Audio.h"
#include "j1XMLLoader.h"

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

	iPoint win_text_pos = { int(screen.w*0.5f) - 170, int(screen.h*0.5f) - 100 };

	SDL_Rect minimap_img_rect = {0, 588, 472, 313}; 
	iPoint minimap_img_pos = { screen.w / 2 - minimap_img_rect.w / 2, screen.h / 2 - minimap_img_rect.h / 2 };

	SDL_Rect stats_back_img_rect = { 0, 900, 410, 83 }; 
	iPoint stats_back_img_pos = { screen.w / 2 - stats_back_img_rect.w / 2 - 10, screen.h - 90 };

	iPoint first_text_pos = { stats_back_img_pos.x + 20, stats_back_img_pos.y + 15 }; 

	MainSceneViewport curr_viewport;

	for (int i = 0; i < 4;i++)
	{
		curr_viewport.main_window = App->gui->UI_CreateWin(iPoint(0, 0), screen.w, screen.h, 0, true);
		curr_viewport.main_window->viewport = i + 1; 
		curr_viewport.progress_bar = curr_viewport.main_window->CreateImage(iPoint(screen.w / 2 - 192, screen.h / 40), { 0, 28, 385, 24 });
		curr_viewport.princess = curr_viewport.main_window->CreateImage(iPoint(curr_viewport.progress_bar->rect.x + (curr_viewport.progress_bar->rect.w / 2) - 15, curr_viewport.progress_bar->rect.y - 5), { 0,0,32,28 });
		curr_viewport.rupiees_img = curr_viewport.main_window->CreateImage(rupiees_pos, rupiees_rect);
		curr_viewport.minimap_icon = curr_viewport.main_window->CreateImage(minimap_pos, minimap_rect);
		curr_viewport.win_text = curr_viewport.main_window->CreateImage(win_text_pos, NULLRECT);
		curr_viewport.win_text->enabled = false;
		curr_viewport.minimapstate.minimap = curr_viewport.main_window->CreateImage(minimap_img_pos, minimap_img_rect);
		curr_viewport.minimapstate.stats_back_image = curr_viewport.main_window->CreateImage(stats_back_img_pos, stats_back_img_rect);
		curr_viewport.minimapstate.hp_text = curr_viewport.main_window->CreateText(iPoint(first_text_pos.x, first_text_pos.y), App->font->game_font_12);
		curr_viewport.minimapstate.power_text = curr_viewport.main_window->CreateText(iPoint(first_text_pos.x + 130 , first_text_pos.y), App->font->game_font_12);
		curr_viewport.minimapstate.speed_text = curr_viewport.main_window->CreateText(iPoint(first_text_pos.x + 280, first_text_pos.y), App->font->game_font_12);
		curr_viewport.minimapstate.kills_text = curr_viewport.main_window->CreateText(iPoint(first_text_pos.x + 60, first_text_pos.y + 25), App->font->game_font_12);
		curr_viewport.minimapstate.minions_text = curr_viewport.main_window->CreateText(iPoint(first_text_pos.x + 210, first_text_pos.y + 25), App->font->game_font_12);
		curr_viewport.minimapstate.Disable(); 

		ui_viewports.push_back(curr_viewport);
	}
	// ------------------


	App->console->AddText("viewports.set 4", Input);
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

	//Quest manager
	quest_manager = new QuestManager();


	//Load Victory/Defeat Animations
	pugi::xml_document gs;

	App->xml->LoadXML("GameSettings.xml", gs);

	defeat = new Animator();
	victory = new Animator();

	defeat->LoadAnimationsFromXML(gs, "defeat_animations");
	victory->LoadAnimationsFromXML(gs, "victory_animations");

	defeat->SetAnimation("idle");
	victory->SetAnimation("idle");

	// Allow player input once the level is loaded
	player_manager->AllowInput(0);
	// ----

	game_timer.Start();
	quest_timer.Start();
	first_quest_completed = false;
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

	quest_manager->update_progress();

	// Update Managers
	if(minion_manager != nullptr)
		minion_manager->Update();
	if(shop_manager != nullptr)
		shop_manager->Update();
	if(player_manager != nullptr)
		player_manager->Update(dt);
	if (jungleCamp_manager != nullptr)
		jungleCamp_manager->Update(dt);
	if (quest_manager != nullptr)
		quest_manager->Update();
	// ------

	// Update progress bar
	if (zelda_manager != nullptr && winner == 0)
		UpdateProgressBar();
	// ------

	// End Game
	if (winner != 0 && game_timer.ReadSec() > end_delay)
	{
		App->scene->ChangeScene((Scene*)App->scene->final_screen);
		App->view->SetViews(1);
	}

	//Update Victory/Defeat animation
	if (winner != 0)
		UpdateWinnerAnim(winner, dt);

	// Test
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_REPEAT)
	{
		EndGame(1);
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
	tower_manager->CleanUp(); 	   RELEASE(tower_manager);
	aest_manager->CleanUp(); 	   RELEASE(aest_manager);
	player_manager->CleanUp();	   RELEASE(player_manager);
	jungleCamp_manager->CleanUp(); RELEASE(jungleCamp_manager);
	quest_manager->CleanUp();	   RELEASE(quest_manager);
	App->map->CleanUp();
	App->entity->ClearEntities();
	App->spell->ClearSpells();

	//Release Animators
	victory->CleanUp();
	defeat->CleanUp();
	RELEASE(victory);
	RELEASE(defeat);

	// Free UI
	if (App->scene->GetCurrentScene() != App->scene->main_scene)	
	{
		for (vector<MainSceneViewport>::iterator it = ui_viewports.begin(); it != ui_viewports.end(); it++) 
		{
			App->gui->DeleteElement(it->main_window);
		}	
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

	//Reset cameras position
	App->view->ResetCameras();

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

	for (vector<MainSceneViewport>::iterator it = ui_viewports.begin(); it != ui_viewports.end(); it++)
	{
		it->win_text->enabled = true;
	}

	UpdateWinnerAnim(winner,0.0f);

	App->audio->ChangeVolume(75);
	App->audio->PlayMusic("Audio/Music/triforce_chamber.ogg");

	game_timer.Start();

	minion_manager->StunMinions();
}

void MainScene::UpdateProgressBar()
{
	iPoint zelda_pos = App->map->WorldToMap(zelda_manager->GetZeldaPos().x, zelda_manager->GetZeldaPos().y);

	float percentage = (zelda_pos.x-36) * 100 / 95;
	percentage /= 100;

	int delta = (ui_viewports[0].progress_bar->rect.w * percentage) - ui_viewports[0].progress_bar->rect.w/2;

	for (vector<MainSceneViewport>::iterator it = ui_viewports.begin(); it != ui_viewports.end(); it++)
	{
		//(*it)->princess->SetPos({ (*it)->progress_bar->GetPos().x + delta, (*it)->progress_bar->GetPos().y - 4});
	}
	

}

void MainScene::ListenEvent(int type, EventThrower * origin, int id)
{
	event_type etype = static_cast<event_type>(type);

	switch (etype)
	{
	case e_t_null:
		break;
	case e_t_death:
		break;
	case e_t_end_game:
		break;
	default:
		break;
	}
}

void MainScene::CreateMapCollisions()
{
	pugi::xml_document doc;
	App->xml->LoadXML("MapCollisions.xml", doc);
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
		SDL_Rect rect = { 130,0,4,2 };
		App->render->Blit(App->gui->atlas, win_w / 2 - 2, y, &rect);
	}

	for (uint x = 0; x < win_w - 2; x += 2)
	{
		SDL_Rect rect2 = { 130,2,2,4 };
		App->render->Blit(App->gui->atlas, x, win_h / 2 - 4, &rect2);
	}

	SDL_Rect rect3 = { 130,0,6,6 };
	App->render->Blit(App->gui->atlas, win_w / 2 - 2, win_h / 2 - 6, &rect3);
}

void MainScene::UpdateWinnerAnim(uint winner, float dt)
{
	SDL_Rect win_rect = victory->GetCurrentAnimation()->GetAnimationFrame(dt);
	SDL_Rect lose_rect = defeat->GetCurrentAnimation()->GetAnimationFrame(dt);

	switch (winner)
	{
	case 1:	
		ui_viewports.at(0).win_text->image = win_rect;	

		ui_viewports.at(1).win_text->image = win_rect;

		ui_viewports.at(2).win_text->image = lose_rect;

		ui_viewports.at(3).win_text->image = lose_rect;
		break;
	case 2:
		ui_viewports.at(0).win_text->image = win_rect;

		ui_viewports.at(1).win_text->image = win_rect;

		ui_viewports.at(2).win_text->image = lose_rect;

		ui_viewports.at(3).win_text->image = lose_rect;
		break;
	default:
		break;
	}
}

void MinimapState::Enable()
{
	minimap->enabled = true; 
	stats_back_image->enabled = true;

	hp_text->SetText("HP: 300");
	hp_text->enabled = true;

	power_text->SetText("POWER: 300");
	power_text->enabled = true;

	speed_text->SetText("SPEED: 300");
	speed_text->enabled = true;

	kills_text->SetText("KILLS: 4/7");
	kills_text->enabled = true;

	minions_text->SetText("MINIONS: 5");
	minions_text->enabled = true;
}

void MinimapState::Disable()
{
	minimap->enabled = false;
	stats_back_image->enabled = false;
	hp_text->enabled = false;
	power_text->enabled = false;
	speed_text->enabled = false;
	kills_text->enabled = false;
	minions_text->enabled = false;
}
