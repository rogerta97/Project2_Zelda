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
#include "MinimapManager.h"
#include "MinimapManager.h"
#include "RemapingScene.h"

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

	game_timer = App->AddGameplayTimer();
	quest_timer = App->AddGameplayTimer();

	//Create UI ---------
	SDL_Rect screen = App->view->GetViewportRect(1);

	iPoint rupiees_pos = { screen.w / 50 + 15 , screen.h / 40 + 5 };
	SDL_Rect rupiees_rect = { 32, 0, 16, 16 };


	iPoint minimap_pos = { screen.w - 58, 5 };
	SDL_Rect minimap_rect;

	iPoint win_text_pos = { int(screen.w*0.5f) - 170, int(screen.h*0.5f) - 100 };

	SDL_Rect minimap_img_rect = {0, 588, 472, 313}; 
	iPoint minimap_img_pos = { screen.w / 2 - minimap_img_rect.w / 2, screen.h / 2 - minimap_img_rect.h / 2 };

	SDL_Rect stats_back_img_rect = { 0, 900, 410, 83 }; 
	iPoint stats_back_img_pos = { screen.w / 2 - stats_back_img_rect.w / 2 - 10, screen.h - 90 };
	
	iPoint items_back_image_pos = { minimap_img_pos.x - 70, minimap_img_pos.y + 40 };

	iPoint hp_text_pos = { stats_back_img_pos.x + 25, stats_back_img_pos.y + 15 }; 
	iPoint power_text_pos = { hp_text_pos.x + 125 , hp_text_pos.y };
	iPoint speed_text_pos = { hp_text_pos.x + 275, hp_text_pos.y };
	iPoint kills_text_pos = { hp_text_pos.x + 55, hp_text_pos.y + 25 };
	iPoint items_text_pos = { items_back_image_pos.x + 15, items_back_image_pos.y + 15 };
	iPoint minions_text_pos = { hp_text_pos.x + 175, hp_text_pos.y + 25 };

	MainSceneViewport curr_viewport;

	LOG("Creating players UI");
	int count = 0; 
	for (int i = 0; i < 4;i++)
	{
		MainSceneViewport curr_viewport;

		if (count == 2)
			minimap_pos.y += 10; 

		// Player UI
		curr_viewport.viewport_window = App->gui->UI_CreateWin(iPoint(0, 0), screen.w, screen.h, 0, true);
		curr_viewport.viewport_window->viewport = i + 1;
		curr_viewport.rupiees_img = curr_viewport.viewport_window->CreateImage(rupiees_pos, rupiees_rect);
		curr_viewport.win_text = curr_viewport.viewport_window->CreateImage(win_text_pos, NULLRECT);
		curr_viewport.win_text->enabled = false;

		// Minimap UI
		curr_viewport.minimapstate.stats_back_image = curr_viewport.viewport_window->CreateImage(stats_back_img_pos, stats_back_img_rect);
		curr_viewport.minimapstate.stats_back_image->blit_layer = MINIMAP_LAYER;

		curr_viewport.minimapstate.hp_text = curr_viewport.viewport_window->CreateText(iPoint(hp_text_pos.x, hp_text_pos.y), App->font->game_font_25);
		curr_viewport.minimapstate.hp_text->blit_layer = MINIMAP_LAYER;
		curr_viewport.minimapstate.hp_text->SetText("HP:");
		curr_viewport.minimapstate.hp_num = curr_viewport.viewport_window->CreateText(iPoint(hp_text_pos.x + 30, hp_text_pos.y), App->font->game_font_25);
		curr_viewport.minimapstate.hp_num->blit_layer = MINIMAP_LAYER;

		curr_viewport.minimapstate.power_text = curr_viewport.viewport_window->CreateText(iPoint(power_text_pos.x , power_text_pos.y), App->font->game_font_25);
		curr_viewport.minimapstate.power_text->blit_layer = MINIMAP_LAYER;
		curr_viewport.minimapstate.power_text->SetText("POWER:"); 
		curr_viewport.minimapstate.power_num = curr_viewport.viewport_window->CreateText(iPoint(power_text_pos.x + 65, power_text_pos.y), App->font->game_font_25);
		curr_viewport.minimapstate.power_num->blit_layer = MINIMAP_LAYER;

		curr_viewport.minimapstate.speed_text = curr_viewport.viewport_window->CreateText(iPoint(speed_text_pos.x , speed_text_pos.y), App->font->game_font_25);
		curr_viewport.minimapstate.speed_text->blit_layer = MINIMAP_LAYER;
		curr_viewport.minimapstate.speed_text->SetText("SPEED:");
		curr_viewport.minimapstate.speed_num = curr_viewport.viewport_window->CreateText(iPoint(speed_text_pos.x + 60, speed_text_pos.y), App->font->game_font_25);
		curr_viewport.minimapstate.speed_num->blit_layer = MINIMAP_LAYER;

		curr_viewport.minimapstate.kills_text = curr_viewport.viewport_window->CreateText(iPoint(kills_text_pos.x , kills_text_pos.y), App->font->game_font_25);
		curr_viewport.minimapstate.kills_text->blit_layer = MINIMAP_LAYER;
		curr_viewport.minimapstate.kills_text->SetText("KILLS:"); 
		curr_viewport.minimapstate.kills_num = curr_viewport.viewport_window->CreateText(iPoint(kills_text_pos.x + 60, kills_text_pos.y), App->font->game_font_25);
		curr_viewport.minimapstate.kills_num->blit_layer = MINIMAP_LAYER;

		curr_viewport.minimapstate.minions_text = curr_viewport.viewport_window->CreateText(iPoint(minions_text_pos.x + 10, minions_text_pos.y), App->font->game_font_25);
		curr_viewport.minimapstate.minions_text->blit_layer = MINIMAP_LAYER;
		curr_viewport.minimapstate.minions_text->SetText("MINIONS:");

		curr_viewport.minimapstate.minions_num = curr_viewport.viewport_window->CreateText(iPoint(minions_text_pos.x +100, minions_text_pos.y), App->font->game_font_25);
		curr_viewport.minimapstate.minions_num->blit_layer = MINIMAP_LAYER;

		curr_viewport.minimapstate.items_background = curr_viewport.viewport_window->CreateImage(items_back_image_pos, { 327, 983, 83, 216 });
		curr_viewport.minimapstate.items_background->blit_layer = MINIMAP_LAYER;

		curr_viewport.minimapstate.items_text = curr_viewport.viewport_window->CreateText(items_text_pos, App->font->game_font_25);
		curr_viewport.minimapstate.items_text->blit_layer = MINIMAP_LAYER;
		curr_viewport.minimapstate.items_text->SetText("ITEMS");

		curr_viewport.minimapstate.item_1_img = curr_viewport.viewport_window->CreateImage(iPoint(items_back_image_pos.x + 26, items_back_image_pos.y + 50), { 0,0,0,0 });
		curr_viewport.minimapstate.item_2_img = curr_viewport.viewport_window->CreateImage(iPoint(items_back_image_pos.x + 26, items_back_image_pos.y + 100), { 0,0,0,0 });
		curr_viewport.minimapstate.item_3_img = curr_viewport.viewport_window->CreateImage(iPoint(items_back_image_pos.x + 26, items_back_image_pos.y + 150), { 0,0,0,0 });

		curr_viewport.minimapstate.item_1_img->blit_layer = MINIMAP_LAYER;
		curr_viewport.minimapstate.item_2_img->blit_layer = MINIMAP_LAYER;
		curr_viewport.minimapstate.item_3_img->blit_layer = MINIMAP_LAYER;

		curr_viewport.minimapstate.Disable(); 

		ui_viewports.push_back(curr_viewport);
		count++;
	}
	// ------------------

	//Load Minimap Button
	int button_it = 0;
	for (vector<MainSceneViewport>::iterator it = App->scene->main_scene->ui_viewports.begin(); it != App->scene->main_scene->ui_viewports.end(); it++)
	{
		//BUTTON REMAPPING
		key_mapping minimap_key = App->scene->players[button_it].mapping->GetMapping(m_k_minimap);
		SDL_Rect button_pos = { 703,2334,28,26 };
		switch (minimap_key.key_id)
		{
		case SDL_CONTROLLER_BUTTON_A:
			button_pos = { 533,762,36, 32 };
			break;
		case SDL_CONTROLLER_BUTTON_B:
			button_pos = { 569,762,36, 32 };
			break;
		case SDL_CONTROLLER_BUTTON_X:
			button_pos = { 497,762,36, 32 };
			break;
		case SDL_CONTROLLER_BUTTON_Y:
			button_pos = { 532, 587, 36, 32 };;
			break;
		}
		button_it++;
		it->minimap_icon = it->viewport_window->CreateImage(minimap_pos, button_pos);
	}
	// ------------------

	// Creating pause UI
	uint w, h;
	App->win->GetWindowSize(w, h);
	main_scene_window = App->gui->UI_CreateWin({ 0,0 }, w, h, 10, false);

	SDL_Rect win_size = { 0,0, w,h };

	// Common UI
	progress_bar = main_scene_window->CreateImage(iPoint(w / 2 - 192, h / 2 - 12), { 0, 28, 385, 24 });
	princess = main_scene_window->CreateImage(iPoint(progress_bar->rect.x + (progress_bar->rect.w / 2) - 15, progress_bar->rect.y - 2), { 0,0,32,28 });
	princess_timer = main_scene_window->CreateText(iPoint(progress_bar->rect.x + (progress_bar->rect.w / 2) - 15, progress_bar->rect.y - 2), App->font->game_font_25, 0, false, 0, 0, 0);

	progress_bar->enabled = true; 

	princess->enabled = false; 

	SDL_Rect back_button_rect = { 128, 52, 217, 55 };

	iPoint resume_button_pos = { win_size.w / 2 - back_button_rect.w / 2, win_size.h / 2 - back_button_rect.h / 2 - 35 };
	iPoint quit_button_pos = { win_size.w / 2 - back_button_rect.w / 2, win_size.h / 2 - back_button_rect.h / 2 + 35 };

	pause_ui.resume_background = main_scene_window->CreateImage(resume_button_pos, back_button_rect);
	pause_ui.quit_background = main_scene_window->CreateImage(quit_button_pos, back_button_rect);

	pause_ui.resume_text = main_scene_window->CreateText({ resume_button_pos.x + 57, resume_button_pos.y + 7 }, App->font->game_font_40);
	pause_ui.quit_text = main_scene_window->CreateText({ quit_button_pos.x + 75, quit_button_pos.y + 7 }, App->font->game_font_40);

	pause_ui.resume_text->SetText("RESUME");
	pause_ui.quit_text->SetText("QUIT");

	pause_ui.cursor_1 = main_scene_window->CreateImage({ 0,0 }, { 80, 52, 48, 47 });
	pause_ui.cursor_2 = main_scene_window->CreateImage({ 0,0 }, { 80, 52, 48, 47 });

	pause_ui.cursor_state = p_e_resume;

	pause_ui.SetPauseUI(false);

	App->render->DrawQuad(win_size, 0, 0, 0, 0, 1, 60, true);
	// -------------------

	App->console->AddText("viewports.set 4", Input);

	//Load Map
	LOG("Loading map");
	if (App->map->Load("zelda_moba.tmx"))
	{
		LOG("Creating walkability map");
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
		{
			LOG("Setting pathfinding map");
			App->pathfinding->SetMap(w, h, data);
		}

		RELEASE_ARRAY(data);
	}

	// Map collisions
	LOG("Loading collisions");
	CreateMapCollisions();

	// Shop Manager
	LOG("Loading shop");
	shop_manager = new ShopManager();
	shop_manager->Start();

	// Loading Players
	LOG("Loading Players");
	player_manager = new PlayerManager();
	player_manager->Start();

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
		Player* p1 = player_manager->AddPlayer(App->scene->players[0].character, iPoint(300, 700), 1, 1, 1, 1);
		Player* p2 = player_manager->AddPlayer(App->scene->players[1].character, iPoint(300, 700), 2, 2, 2, 1);
		Player* p3 = player_manager->AddPlayer(App->scene->players[2].character, iPoint(300, 700), 3, 3, 1, 2);
		Player* p4 = player_manager->AddPlayer(App->scene->players[3].character, iPoint(300, 700), 4, 4, 2, 2);
	}
	else
	{
		Player* p1 = player_manager->AddPlayer(entity_name::link, iPoint(300, 700), 1, 1, 1, 1);
		Player* p2 = player_manager->AddPlayer(entity_name::link, iPoint(300, 700), 2, 2, 2, 1);
		Player* p3 = player_manager->AddPlayer(entity_name::link, iPoint(300, 700), 3, 3, 1, 2);
		Player* p4 = player_manager->AddPlayer(entity_name::link, iPoint(300, 700), 4, 4, 2, 2);

	}

	// Disable player input until level is loaded
	player_manager->DisableInput(0);
	// ----

	//Jungle Camp manager
	jungleCamp_manager = new JungleCampManager();
	jungleCamp_manager->Start();

	//Minion manager
	LOG("Creating minion manager");
	minion_manager = new MinionManager();

	//Tower manager
	LOG("Creating tower manager");
	tower_manager = new TowerManager();
  
	// Zelda manager
	LOG("Creating zelda manager");
	zelda_manager = new ZeldaManager();
  
	// Aesth manager
	LOG("Creating aesthetics manager");
	aest_manager = new AestheticsManager(); 
	aest_manager->Start(); 

	// Base manager
	LOG("Creating base manager");
	base_manager = new BaseManager();

	// Quest manager
	LOG("Creating quest manager");
	quest_manager = new QuestManager();
	quest_manager->quests_enabled = App->scene->menu_scene->quests_enabled;

	// Minimap manager
	LOG("Creating minimap manager");
	minimap_manager = new MinimapManager();
	minimap_manager->Start();

	
	//Load Victory/Defeat Animations
	pugi::xml_document gs;

	App->xml->LoadXML("GameSettings.xml", gs);

	defeat = new Animator();
	victory = new Animator();
	zelda_appearing = new Animator(); 

	defeat->LoadAnimationsFromXML(gs, "defeat_animations");
	victory->LoadAnimationsFromXML(gs, "victory_animations");
	zelda_appearing->LoadAnimationsFromXML(gs, "zelda_face"); 

	defeat->SetAnimation("idle");
	victory->SetAnimation("idle");
	zelda_appearing->SetAnimation("idle"); 

	// Allow player input once the level is loaded
	player_manager->AllowInput(0);
	// ----

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
	if (minion_manager != nullptr)
		minion_manager->Update();
	if (shop_manager != nullptr)
		shop_manager->Update();
	if (player_manager != nullptr)
		player_manager->Update(dt);
	if (jungleCamp_manager != nullptr)
		jungleCamp_manager->Update(dt);
	if (quest_manager != nullptr)
	{
		quest_manager->Update();
		quest_manager->UpdateWindows();
	}
	if (minimap_manager != nullptr)
		minimap_manager->Update(dt);
	// ------

	// Update progress bar
	if (zelda_manager != nullptr && winner == 0)
		UpdateProgressBar();
	// ------

	// End Game
	if (winner != 0 && game_timer->ReadSec() > end_delay)
	{
		App->scene->ChangeScene((Scene*)App->scene->final_screen);
		App->view->SetViews(1);
	}

	//Update Victory/Defeat animation
	if (winner != 0)
	{
		if (!App->GetGamePause())
			UpdateWinnerAnim(winner, dt);
		else
			UpdateWinnerAnim(winner, 0);
	}

	for (int i = 0; i < 4; i++)
	{
		if (App->input->GetControllerButton(i, SDL_CONTROLLER_BUTTON_Y) == KEY_DOWN)
		{
			ui_viewports.at(i).minimapstate.SetPlayerStats(i);
		}

	}

	// Test
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		EndGame(1);
	}

	if (App->input->GetControllerButton(0, SDL_CONTROLLER_BUTTON_START) == KEY_DOWN ||
		App->input->GetControllerButton(1, SDL_CONTROLLER_BUTTON_START) == KEY_DOWN ||
		App->input->GetControllerButton(2, SDL_CONTROLLER_BUTTON_START) == KEY_DOWN ||
		App->input->GetControllerButton(3, SDL_CONTROLLER_BUTTON_START) == KEY_DOWN)
	{
		App->SetGamePause(!App->GetGamePause());
		pause_ui.SetPauseUI(true);
	}
	else if (App->GetGamePause() == false)
	{
		pause_ui.SetPauseUI(false);
	}
	else
		pause_ui.UpdatePause();
	// ------

	if (App->scene->main_scene->zelda_manager != nullptr)
	{
		int count_down = App->scene->main_scene->zelda_manager->GetSpawnTime() - game_timer->ReadSec();
		princess_timer->SetText(SecToMin(count_down));

		if (princess_timer->enabled == true && count_down <= 0)
		{
			princess_timer->enabled = false;

			princess->enabled = true;
		}
	}

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
	minimap_manager->CleanUp();    RELEASE(minimap_manager);
	App->map->CleanUp();
	App->entity->ClearEntities();
	App->spell->ClearSpells();

	//Release Animators
	victory->CleanUp();
	defeat->CleanUp();
	RELEASE(victory);
	RELEASE(defeat);

	App->DeleteGameplayTimer(game_timer);
	App->DeleteGameplayTimer(quest_timer);
	App->ClearGameplayTimers();

	// Free UI
	if (App->scene->GetCurrentScene() != App->scene->main_scene)	
	{
		for (vector<MainSceneViewport>::iterator it = ui_viewports.begin(); it != ui_viewports.end(); it++) 
		{
			App->gui->DeleteElement(it->viewport_window);
		}	
		App->gui->DeleteElement(main_scene_window); 
	}
	ui_viewports.clear();
	// -------

	// Delete Map Collisions
	for (std::vector<PhysBody*>::iterator it = map_collisions.begin(); it != map_collisions.end(); it++)
	{
		App->physics->DeleteBody(*it);
	}
	map_collisions.clear();
	// -------

	//Stop Music
	App->audio->StopMusic();

	//Reset cameras position
	App->view->ResetCameras();

	// Reset vars
	first_quest_completed = false;
	winner = 0;

	return ret;
}

j1Timer * MainScene::GetGameTimer()
{
	return game_timer;
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

	game_timer->Start();

	minion_manager->StunMinions();

	GetPlayerItemsRects();
}

void MainScene::UpdateProgressBar()
{
	iPoint zelda_pos = App->map->WorldToMap(zelda_manager->GetZeldaPos().x, zelda_manager->GetZeldaPos().y);

	float percentage = (zelda_pos.x-35) * 100 / 97;
	percentage /= 100;

	int delta = (progress_bar->rect.w * percentage);
	
	princess->SetPos({ progress_bar->GetPos().x + delta - princess->image.w/2, progress_bar->GetPos().y - 4});
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

		ui_viewports.at(1).win_text->image = lose_rect;

		ui_viewports.at(2).win_text->image = win_rect;

		ui_viewports.at(3).win_text->image = lose_rect;
		break;
	case 2:
		ui_viewports.at(0).win_text->image = lose_rect;

		ui_viewports.at(1).win_text->image = win_rect;

		ui_viewports.at(2).win_text->image = lose_rect;

		ui_viewports.at(3).win_text->image = win_rect;
		break;
	default:
		break;
	}
}

void MainScene::GetPlayerItemsRects()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; ++j)
		{
			if(player_manager->players[i]->items[j] != nullptr)
				App->scene->players[i].items_rects[j] = player_manager->players[i]->items[j]->image_rect;
		}
	}
}

void MinimapState::Enable()
{
	stats_back_image->enabled = true;
	hp_text->enabled = true;	
	hp_num->enabled = true; 
	power_text->enabled = true;
	power_num->enabled = true;
	speed_text->enabled = true;
	speed_num->enabled = true;
	kills_text->enabled = true;
	kills_num->enabled = true;
	minions_text->enabled = true;
	minions_num->enabled = true;
	items_text->enabled = true; 
	items_background->enabled = true; 
	item_1_img->enabled = true; 
	item_2_img->enabled = true;
	item_3_img->enabled = true;
}

void MinimapState::Disable()
{
	stats_back_image->enabled = false;
	hp_text->enabled = false;
	hp_num->enabled = false;
	power_text->enabled = false;
	power_num->enabled = false;
	speed_text->enabled = false;
	speed_num->enabled = false;
	kills_text->enabled = false;
	kills_num->enabled = false;
	minions_text->enabled = false;
	minions_num->enabled = false;
	items_text->enabled = false;
	items_background->enabled = false;
	item_1_img->enabled = false;
	item_2_img->enabled = false;
	item_3_img->enabled = false;
}

void PauseUI::SetPauseUI(bool ui_state)
{
	resume_background->enabled = ui_state; 
	resume_text->enabled = ui_state;

	quit_background->enabled = ui_state;
	quit_text->enabled = ui_state;

	cursor_1->enabled = ui_state;
	cursor_2->enabled = ui_state;

	cursor_state = p_e_resume; 
}

void PauseUI::MoveCursor()
{
	switch (cursor_state)
	{
	case p_e_resume:
		cursor_1->SetPos({resume_background->GetPos().x - 60, resume_background->GetPos().y});
		cursor_2->SetPos({ resume_background->GetPos().x + resume_background->image.w + 10, resume_background->GetPos().y});
		break; 

	case p_e_quit:
		cursor_1->SetPos({ quit_background->GetPos().x - 60, quit_background->GetPos().y });
		cursor_2->SetPos({ quit_background->GetPos().x + quit_background->image.w + 10, quit_background->GetPos().y });
		break; 

	case p_e_null:
		cursor_1->SetPos({0,0});
		cursor_2->SetPos({0,0});
		break; 
	}
}

void PauseUI::UpdatePause()
{
	if (App->input->GetControllerButton(0, SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_DOWN && cursor_state == p_e_quit)
	{
		cursor_state = p_e_resume;
	}

	else if (App->input->GetControllerButton(0, SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_DOWN  && cursor_state == p_e_resume)
	{
		cursor_state = p_e_quit;
	}

	else if (App->input->GetControllerButton(0, SDL_CONTROLLER_BUTTON_A) == KEY_DOWN)
	{
		switch (cursor_state)
		{
		case p_e_resume:
			App->SetGamePause(!App->GetGamePause());
			break;

		case p_e_quit:
			App->scene->ChangeScene((Scene*)App->scene->logo_scene); 
			break;

		case p_e_null:
			cursor_1->SetPos({ 0,0 });
			cursor_2->SetPos({ 0,0 });
			break;

		}
	}

	MoveCursor(); 
}

void MinimapState::SetPlayerStats(int player)
{
	Entity* curr_player = nullptr; 
	curr_player = App->scene->main_scene->player_manager->GetPlayer(player);

	hp_num->SetText(to_string(curr_player->stats.base_hp));
	power_num->SetText(to_string(curr_player->stats.power));
	speed_num->SetText(to_string(curr_player->stats.speed));
	kills_num->SetText(to_string(App->scene->players[player].kills));
	minions_num->SetText(to_string(App->scene->players[player].minions));

	item_1_img->image = App->scene->main_scene->shop_manager->GetPlayerItem(player, 0);
	item_2_img->image = App->scene->main_scene->shop_manager->GetPlayerItem(player, 1);
	item_3_img->image = App->scene->main_scene->shop_manager->GetPlayerItem(player, 2); 
}
