#include "PlayerManager.h"
#include "j1Viewports.h"
#include "j1Input.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1XMLLoader.h"
#include "GameObject.h"
#include "j1Audio.h"
#include "Quest_Manager.h"

#include "MinimapManager.h"
#include "Mapping.h"

#define DEATH_CAMERA_SPEED 500
#define BASE_TRAVEL_TIME 4
#define BASE_TRAVEL_RECT_W 240
#define BASE_TRAVEL_RECT_H 15

#define EXPLOSION_OFFSET 54


PlayerManager::PlayerManager()
{
}

PlayerManager::~PlayerManager()
{
}

bool PlayerManager::Awake(pugi::xml_node &)
{
	bool ret = true;


	return ret;
}

bool PlayerManager::Start()
{
	// Abilities UI ------
	SDL_Rect screen = App->view->GetViewportRect(1);
	iPoint ability1_pos = { screen.w - 120 , screen.h - 135 };
	iPoint ability2_pos = { 13, screen.h - 135 };
	iPoint ability3_pos = { screen.w - 90, screen.h - 85 };
	iPoint ability4_pos = { 13, screen.h - 85 };
	
	_TTF_Font* text_font = App->font->game_font;
	iPoint text1_pos = { screen.w - 85 , screen.h - 128 };
	iPoint text2_pos = { (50), screen.h - 128 };
	iPoint text3_pos = { screen.w - 75, screen.h - 65 };
	iPoint text4_pos = { 27, screen.h - 65 };

	// Death timer
	iPoint death_timer_pos = { (screen.w / 2) - 5, (screen.h / 2) + 15};

	// Death quad
	death_quad_rect = {0, 1400, 1125, 561};
	death_quad_pos = { -(death_quad_rect.w /2 - screen.w/2), -(death_quad_rect.h / 2 - screen.h / 2) };

	// Death text animation
	death_text_pos = { int(screen.w * 0.5f) - 131, int(screen.h * 0.5f) - 13 };
	death_text_anim = new Animator(); 

	pugi::xml_document doc;
	App->xml->LoadXML("GameSettings.xml", doc);

	death_text_anim->LoadAnimationsFromXML(doc, "death_text");
	death_text_texture = App->gui->atlas;

	death_text_anim->SetAnimation("idle"); 
	// ---------------
    
	for (vector<MainSceneViewport>::iterator it = App->scene->main_scene->ui_viewports.begin(); it != App->scene->main_scene->ui_viewports.end(); it++)
	{
		PlayerManagerUI ui_elements;

		ui_elements.abilities_button.push_back(it->viewport_window->CreateImage(ability1_pos, { 0,0,0,0 })); 
		ui_elements.abilities_button.push_back(it->viewport_window->CreateImage(ability2_pos, { 0,0,0,0 }));
		ui_elements.abilities_button.push_back(it->viewport_window->CreateImage(ability3_pos, { 0,0,0,0 }));
		ui_elements.abilities_button.push_back(it->viewport_window->CreateImage(ability4_pos, { 0,0,0,0 }));

		ui_elements.abilities_icon.push_back(it->viewport_window->CreateImage({ ability1_pos.x + 20, ability1_pos.y + 7 }, { 0,0,0,0 }));
		ui_elements.abilities_icon.push_back(it->viewport_window->CreateImage({ ability2_pos.x + 21, ability2_pos.y + 6 }, { 0,0,0,0 }));
		ui_elements.abilities_icon.push_back(it->viewport_window->CreateImage({ ability3_pos.x + 9, ability3_pos.y + 21 }, { 0,0,0,0 }));
		ui_elements.abilities_icon.push_back(it->viewport_window->CreateImage({ ability4_pos.x + 7, ability4_pos.y + 21 }, { 0,0,0,0 }));
				   
		ui_elements.abilities_cd.push_back(it->viewport_window->CreateText(text1_pos, text_font));
		ui_elements.abilities_cd.push_back(it->viewport_window->CreateText(text2_pos, text_font));
		ui_elements.abilities_cd.push_back(it->viewport_window->CreateText(text3_pos, text_font));
		ui_elements.abilities_cd.push_back(it->viewport_window->CreateText(text4_pos, text_font));

		ui_elements.death_time = it->viewport_window->CreateText(death_timer_pos, App->font->game_font_60);
		ui_elements.death_time->enabled = false; ui_elements.death_time->blit_layer = 9999;
			   
		p_manager_ui_elements.push_back(ui_elements);
	}
	// ---------------------

	// Event
	event_thrower = new EventThrower();

	death_sound_effect = App->audio->LoadFx("Audio/FX/Entities/Link/LTTP_Link_Dying.wav");

	return true;
}

bool PlayerManager::PreUpdate()
{
	return true;
}

bool PlayerManager::Update(float dt)
{
	if (players.empty())
		return true;

	for (int i = 0; i < players.size(); i++)
	{
		Player* curr_player = players.at(i);

		// If player is not dead
		if (!curr_player->is_dead)
		{
			// Take player input
			if (!curr_player->disable_controller && !App->GetGamePause())
				PlayerInput(curr_player, i);

			// Update pasive heal
			PasiveHP(curr_player);

			//Reward pasive Rupee
			PasiveRupee(curr_player);

			// Update ui
			UpdateUI(curr_player);

			// Check if is dead
			CheckIfDeath(curr_player);

			// Check if is on base travel
			if (curr_player->base_travel)
				curr_player->BaseTravel();
		}
		// If player is dead
		else
		{
			// Enable camera movement
			if (!curr_player->disable_controller && !App->GetGamePause())
				MoveCamera(curr_player);

			// Check if it has to respawn
			CheckIfRespawn(curr_player);

			// Update death text
			UpdateDeathUI(curr_player, dt);
		}
		if (curr_player->play_exp)
		{
			Explode(curr_player);
		}
	}

	return true;
}

bool PlayerManager::PostUpdate()
{
	return true;
}

bool PlayerManager::CleanUp()
{
	bool ret = true;

	LOG("Unloading PlayerManager");

	ClearPlayers();

	for (vector<PlayerManagerUI>::iterator it = p_manager_ui_elements.begin(); it != p_manager_ui_elements.end(); it++)
	{
		(*it).abilities_button.clear(); 
		(*it).abilities_cd.clear(); 
		(*it).abilities_icon.clear(); 
	}

	RELEASE(event_thrower);

	// Release animator
	death_text_anim->CleanUp();
	RELEASE(death_text_anim);

	return ret;
}

Player* PlayerManager::AddPlayer(entity_name name, iPoint pos, int controller_index, int viewport, int team, int respawn, int show_life_bar)
{
	Player* ret = nullptr;

	if (players.size() <= 3)
	{
		iPoint position = pos;
		
		position = GetFreePlayerSpawn(team, respawn);
		
		position.x += 16;
		position.y += 9;
		
		// Create player
		Player* p = new Player(App->entity->CreateEntity(name, position), controller_index - 1, viewport, position);
		p->entity->SetCamera(p->viewport);
		p->entity->SetTeam(team);
		p->entity->show_life_bar = show_life_bar;
		p->entity->is_player = true;
		p->type = p->entity->type;
		players.push_back(p);
		ret = p;
		p->team = team;

		SetAbilitiesRemaping(p);

		pugi::xml_document explo_doc;
		App->xml->LoadXML("explosion.xml", explo_doc);

		p->explosion = new Animator();
		p->explosion_tex = p->explosion->LoadAnimationsFromXML(explo_doc, "animations");
		p->explosion->SetAnimation("explosion");
	}

	return ret;
}

void PlayerManager::DeletePlayer(int controller_index)
{
	if (!players.empty())
	{
		for (vector<Player*>::iterator it = players.begin(); it != players.end(); it++)
		{
			if ((*it)->controller_index == controller_index - 1)
			{
				(*it)->CleanUp();
				(*it)->explosion->CleanUp();
				RELEASE((*it)->explosion)
				RELEASE(*it);
				players.erase(it);
				break;
			}
		}
	}
}

void PlayerManager::ClearPlayers()
{
	if (!players.empty())
	{
		for (vector<Player*>::iterator it = players.begin(); it != players.end();)
		{
			(*it)->CleanUp();
			RELEASE(*it);
			it = players.erase(it);
		}
		players.clear();
	}
}

std::vector<Entity*> PlayerManager::GetTeamPlayers(int team)
{
	std::vector<Entity*> ret;

	if (!players.empty())
	{
		for (std::vector<Player*>::iterator it = players.begin(); it != players.end(); it++)
		{
			if ((*it)->team == team && !(*it)->is_dead)
			{
				ret.push_back((*it)->entity);
			}
		}
	}

	return ret;
}

std::vector<int> PlayerManager::GetTeamViewports(int team)
{
	std::vector<int> ret;

	if (!players.empty())
	{
		for (std::vector<Player*>::iterator it = players.begin(); it != players.end(); it++)
		{
			if ((*it)->entity->GetTeam() == team)
			{
				ret.push_back((*it)->controller_index + 1);
			}
		}
	}

	return ret;
}

int PlayerManager::GetEntityViewportIfIsPlayer(Entity * entity)
{
	int ret = 0;

	for (int i = 0; i < players.size(); i++)
	{
		if (!players.at(i)->is_dead && players.at(i)->entity != nullptr && players.at(i)->entity == entity)
		{
			ret = players.at(i)->viewport;
			break;
		}
	}

	return ret;
}

iPoint PlayerManager::GetFreePlayerSpawn(int team, int respawn)
{
	iPoint ret = NULLPOINT;

	vector<iPoint> spawn_points;
	App->map->GetPlayerSpawnPoints(team, spawn_points);

	for (int i = 0; i < spawn_points.size(); i++)
	{
		if (i == respawn - 1)
			ret = spawn_points.at(i);
	}
	
	return ret;
}

bool PlayerManager::IsAbilityCdCompleted(Player* player, int ability)
{
	bool ret = false;

	if(player->entity->GetAbility(ability - 1) != nullptr)
		ret = player->entity->GetAbility(ability - 1)->CdCompleted();

	return ret;
}

void PlayerManager::ResetAbilityTimer(Player* player, int ability)
{
	if (player->entity->GetAbility(ability - 1) != nullptr)
		player->entity->GetAbility(ability - 1)->cd_timer->Start();
}

int PlayerManager::GetPlayerTeamFromBody(PhysBody * body)
{
	int ret = 0;

	if (!players.empty())
	{
		for (std::vector<Player*>::iterator it = players.begin(); it != players.end(); it++)
		{
			if ((*it)->entity->game_object->pbody == body)
			{
				ret = (*it)->entity->GetTeam();
			}
		}
	}

	return ret;
}

Player * PlayerManager::GetPlayerFromBody(PhysBody * body)
{
	Player* ret = nullptr;

	if (!players.empty())
	{
		for (std::vector<Player*>::iterator it = players.begin(); it != players.end(); it++)
		{
			if (!(*it)->is_dead && (*it)->entity != nullptr && (*it)->entity->game_object->pbody == body)
			{
				ret = (*it);
				break;
			}
		}
	}

	return ret;
}

void PlayerManager::DisableInput(int player)
{
	switch (player)
	{
	case 0:
		players[0]->disable_controller = true;
		players[1]->disable_controller = true;
		players[2]->disable_controller = true;
		players[3]->disable_controller = true;
		break;
	case 1:
		players[0]->disable_controller = true;
		break;
	case 2:
		players[1]->disable_controller = true;
		break;
	case 3:
		players[2]->disable_controller = true;
		break;
	case 4:
		players[3]->disable_controller = true;
		break;
	}
}

void PlayerManager::AllowInput(int player)
{
	switch (player)
	{
	case 0:
		players[0]->disable_controller = false;
		players[1]->disable_controller = false;
		players[2]->disable_controller = false;
		players[3]->disable_controller = false;
		break;
	case 1:
		players[0]->disable_controller = false;
		break;
	case 2:
		players[1]->disable_controller = false;
		break;
	case 3:
		players[2]->disable_controller = false;
		break;
	case 4:
		players[3]->disable_controller = false;
		break;
	}
}

Entity* PlayerManager::GetPlayer(int index)
{
	return players.at(index)->entity; 
}

void PlayerManager::PlayerInput(Player * curr_player, int index)
{
	// Get abilities mapping
	key_mapping a1, a2, a3, a4;
	a1 = App->scene->players[index].mapping->GetMapping(m_k_ability1);
	a2 = App->scene->players[index].mapping->GetMapping(m_k_ability2);
	a3 = App->scene->players[index].mapping->GetMapping(m_k_ability3);
	a4 = App->scene->players[index].mapping->GetMapping(m_k_ability4);

	// Left Joystick -------

	if (curr_player->entity == nullptr)
		return;

	// Base travel
	key_mapping back_key = App->scene->players[index].mapping->GetMapping(m_k_back);
	if (App->input->GetControllerButton(curr_player->controller_index, back_key.key_id) == KEY_DOWN && !App->scene->main_scene->shop_manager->IsActive(curr_player->viewport))
	{
		curr_player->BaseTravel();
		curr_player->move = stop;
	}

	// Minimap
	int minimap_key;
	App->scene->players[(curr_player)->controller_index].mapping->GetKey(m_k_minimap, &minimap_key);
	if (App->input->GetControllerButton(curr_player->controller_index, minimap_key) == KEY_DOWN)
	{
		App->scene->main_scene->minimap_manager->SetActive(true, curr_player->viewport);
		App->scene->main_scene->ui_viewports[curr_player->controller_index].minimapstate.Enable();
	}
	if (App->input->GetControllerButton(curr_player->controller_index, minimap_key) == KEY_UP)
	{
		App->scene->main_scene->minimap_manager->SetActive(false, curr_player->viewport);
		App->scene->main_scene->ui_viewports[curr_player->controller_index].minimapstate.Disable();
	}

	// Shop
	int shop_key;
	App->scene->players[(curr_player)->controller_index].mapping->GetKey(m_k_shop, &shop_key);
	if (App->input->GetControllerButton(curr_player->controller_index, shop_key) == KEY_UP && App->scene->main_scene->quest_manager->quests_enabled && App->scene->main_scene->quest_manager->active_quest != -1)
	{
		App->scene->main_scene->quest_manager->SwitchWindowState(curr_player->viewport);
	}

	// Cancel ability
	key_mapping back_mapping;
	back_mapping = App->scene->players[curr_player->viewport - 1].mapping->GetMapping(m_k_back);

	if (App->input->GetControllerButton(curr_player->viewport - 1, back_mapping.key_id) == KEY_DOWN)
		curr_player->cancel_hability = true;

	// Diagonal moves
	if (curr_player->entity->stuned || curr_player->disable_controller)
	{
		curr_player->move = stop;
	}
	else if (App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_LEFT) > 12000 && App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_UP) > 12000)
	{
		if (curr_player->invert_controls)
		{
			curr_player->move = move_downright;
		}
		else
		{
			curr_player->move = move_upleft;
		}
	}
	else if (App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_RIGHT) > 12000 && App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_UP) > 12000)
	{
		if (curr_player->invert_controls)
		{
			curr_player->move = move_downleft;
		}
		else
		{
			curr_player->move = move_upright;
		}
	}
	else if (App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_LEFT) > 12000 && App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_DOWN) > 12000)
	{
		if (curr_player->invert_controls)
		{
			curr_player->move = move_upright;
		}
		else
		{
			curr_player->move = move_downleft;
		}
	}
	else if (App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_RIGHT) > 12000 && App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_DOWN) > 12000)
	{
		if (curr_player->invert_controls)
		{
			curr_player->move = move_upleft;
		}
		else
		{
			curr_player->move = move_downright;
		}
	}
	// Normal moves
	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_LEFT) > 12000)
	{
		if (curr_player->invert_controls)
		{
			curr_player->move = move_right;
			curr_player->state = run_right;
		}
		else
		{
			curr_player->move = move_left;
			curr_player->state = run_left;
		}
		
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_RIGHT) > 12000)
	{
		if (curr_player->invert_controls)
		{
			curr_player->move = move_left;
			curr_player->state = run_left;
		}
		else
		{
			curr_player->move = move_right;
			curr_player->state = run_right;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_UP) > 6000)
	{
		if (curr_player->invert_controls)
		{
			curr_player->move = move_down;
			curr_player->state = run_down;
		}
		else
		{
			curr_player->move = move_up;
			curr_player->state = run_up;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_DOWN) > 6000)
	{
		if (curr_player->invert_controls)
		{
			curr_player->move = move_up;
			curr_player->state = run_up;
		}
		else
		{
			curr_player->move = move_down;
			curr_player->state = run_down;
		}
	}
	else
	{
		curr_player->move = stop;
	}
	// --------------------

	// Right Joystick -----
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || App->input->GetControllerButton(curr_player->controller_index, SDL_CONTROLLER_BUTTON_DPAD_LEFT) == KEY_REPEAT || App->input->GetControllerJoystickMove(curr_player->controller_index, RIGHTJOY_LEFT) > 22000)
	{
		if (curr_player->move != stop)
			curr_player->state = run_left;
		else
			curr_player->state = idle_left;
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetControllerButton(curr_player->controller_index, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == KEY_REPEAT || App->input->GetControllerJoystickMove(curr_player->controller_index, RIGHTJOY_RIGHT) > 22000)
	{
		if (curr_player->move != stop)
			curr_player->state = run_right;
		else
			curr_player->state = idle_right;
	}
	else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || App->input->GetControllerButton(curr_player->controller_index, SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_REPEAT || App->input->GetControllerJoystickMove(curr_player->controller_index, RIGHTJOY_UP) > 12000)
	{
		if (curr_player->move != stop)
			curr_player->state = run_up;
		else
			curr_player->state = idle_up;
	}
	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || App->input->GetControllerButton(curr_player->controller_index, SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_REPEAT || App->input->GetControllerJoystickMove(curr_player->controller_index, RIGHTJOY_DOWN) > 12000)
	{
		if (curr_player->move != stop)
			curr_player->state = run_down;
		else
			curr_player->state = idle_down;
	}
	// --------------------

	// Run to idle when not moving
	else if (curr_player->state == run_left)
	{
		curr_player->state = idle_left;
	}
	else if (curr_player->state == run_right)
	{
		curr_player->state = idle_right;
	}
	else if (curr_player->state == run_up)
	{
		curr_player->state = idle_up;
	}
	else if (curr_player->state == run_down)
	{
		curr_player->state = idle_down;
	}

	// Idle to run when moving
	if (curr_player->move != stop)
	{
		if (curr_player->state == idle_left)
			curr_player->state = run_left;

		else if (curr_player->state == idle_right)
			curr_player->state = run_right;

		else if (curr_player->state == idle_up)
			curr_player->state = run_up;

		else if (curr_player->state == idle_down)
			curr_player->state = run_down;
	}

	// Abilities PRESS
	if (!curr_player->entity->stuned && !curr_player->disable_controller)
	{
		bool a1_press = false, a2_press = false, a3_press = false, a4_press = false;

		a1_press = (a1.is_button == true) ? App->input->GetControllerButton(curr_player->controller_index, a1.key_id) == KEY_REPEAT : App->input->GetControllerJoystickMove(curr_player->controller_index, a1.key_id) > 22000;
		a2_press = (a2.is_button == true) ? App->input->GetControllerButton(curr_player->controller_index, a2.key_id) == KEY_REPEAT : App->input->GetControllerJoystickMove(curr_player->controller_index, a2.key_id) > 22000;
		a3_press = (a3.is_button == true) ? App->input->GetControllerButton(curr_player->controller_index, a3.key_id) == KEY_REPEAT : App->input->GetControllerJoystickMove(curr_player->controller_index, a3.key_id) > 22000;
		a4_press = (a4.is_button == true) ? App->input->GetControllerButton(curr_player->controller_index, a4.key_id) == KEY_REPEAT : App->input->GetControllerJoystickMove(curr_player->controller_index, a4.key_id) > 22000;

		if (a1_press)
		{
			if (IsAbilityCdCompleted(curr_player, 1))
			{
				if (curr_player->state == idle_down || curr_player->state == run_down)
					curr_player->show = shows::show_basic_atack_down;
				else if (curr_player->state == idle_up || curr_player->state == run_up)
					curr_player->show = shows::show_basic_atack_up;
				else if (curr_player->state == idle_left || curr_player->state == run_left)
					curr_player->show = shows::show_basic_atack_left;
				else if (curr_player->state == idle_right || curr_player->state == run_right)
					curr_player->show = shows::show_basic_atack_right;
			}
		}
		else if (a2_press)
		{
			if (IsAbilityCdCompleted(curr_player, 2))
			{
				if (curr_player->state == idle_down || curr_player->state == run_down)
					curr_player->show = shows::show_ability1_down;
				else if (curr_player->state == idle_up || curr_player->state == run_up)
					curr_player->show = shows::show_ability1_up;
				else if (curr_player->state == idle_left || curr_player->state == run_left)
					curr_player->show = shows::show_ability1_left;
				else if (curr_player->state == idle_right || curr_player->state == run_right)
					curr_player->show = shows::show_ability1_right;
			}
		}
		else if (a3_press)
		{
			if (IsAbilityCdCompleted(curr_player, 3))
			{
				if (curr_player->state == idle_down || curr_player->state == run_down)
					curr_player->show = shows::show_ability2_down;
				else if (curr_player->state == idle_up || curr_player->state == run_up)
					curr_player->show = shows::show_ability2_up;
				else if (curr_player->state == idle_left || curr_player->state == run_left)
					curr_player->show = shows::show_ability2_left;
				else if (curr_player->state == idle_right || curr_player->state == run_right)
					curr_player->show = shows::show_ability2_right;
			}
		}
		else if (a4_press)
		{
			if (IsAbilityCdCompleted(curr_player, 4))
			{
				if (curr_player->state == idle_down || curr_player->state == run_down)
					curr_player->show = shows::show_ability3_down;
				else if (curr_player->state == idle_up || curr_player->state == run_up)
					curr_player->show = shows::show_ability3_up;
				else if (curr_player->state == idle_left || curr_player->state == run_left)
					curr_player->show = shows::show_ability3_left;
				else if (curr_player->state == idle_right || curr_player->state == run_right)
					curr_player->show = shows::show_ability3_right;
			}
		}
	}

	// Abilities RELEASE
	bool a1_release = false, a2_release = false, a3_release = false, a4_release = false;

	a1_release = (a1.is_button == true) ? App->input->GetControllerButton(curr_player->controller_index, a1.key_id) == KEY_IDLE : App->input->GetControllerJoystickMove(curr_player->controller_index, a1.key_id) < 22000;
	a2_release = (a2.is_button == true) ? App->input->GetControllerButton(curr_player->controller_index, a2.key_id) == KEY_IDLE : App->input->GetControllerJoystickMove(curr_player->controller_index, a2.key_id) < 22000;
	a3_release = (a3.is_button == true) ? App->input->GetControllerButton(curr_player->controller_index, a3.key_id) == KEY_IDLE : App->input->GetControllerJoystickMove(curr_player->controller_index, a3.key_id) < 22000;
	a4_release = (a4.is_button == true) ? App->input->GetControllerButton(curr_player->controller_index, a4.key_id) == KEY_IDLE : App->input->GetControllerJoystickMove(curr_player->controller_index, a4.key_id) < 22000;

	bool ability_use = false;
	
	if (a1_release)
	{
		if (curr_player->show != shows::show_null)
		{
			if (curr_player->show == shows::show_basic_atack_down)
			{
				curr_player->state = basic_atack_down;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 1);
				ability_use = true;
			}
			else if (curr_player->show == shows::show_basic_atack_up)
			{
				curr_player->state = basic_atack_up;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 1);
				ability_use = true;
			}
			else if (curr_player->show == shows::show_basic_atack_left)
			{
				curr_player->state = basic_atack_left;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 1);
				ability_use = true;
			}
			else if (curr_player->show == shows::show_basic_atack_right)
			{
				curr_player->state = basic_atack_right;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 1);
				ability_use = true;
			}
		}
	}

	if (a2_release)
	{
		if (curr_player->show != shows::show_null)
		{
			if (curr_player->show == shows::show_ability1_down)
			{
				curr_player->state = ability1_down;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 2);
				ability_use = true;
			}
			else if (curr_player->show == shows::show_ability1_up)
			{
				curr_player->state = ability1_up;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 2);
				ability_use = true;
			}
			else if (curr_player->show == shows::show_ability1_left)
			{
				curr_player->state = ability1_left;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 2);
				ability_use = true;
			}
			else if (curr_player->show == shows::show_ability1_right)
			{
				curr_player->state = ability1_right;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 2);
				ability_use = true;
			}
		}
	}

	if (a3_release)
	{
		if (curr_player->show != shows::show_null)
		{
			if (curr_player->show == shows::show_ability2_down)
			{
				curr_player->state = ability2_down;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 3);
				ability_use = true;
			}
			else if (curr_player->show == shows::show_ability2_up)
			{
				curr_player->state = ability2_up;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 3);
				ability_use = true;
			}
			else if (curr_player->show == shows::show_ability2_left)
			{
				curr_player->state = ability2_left;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 3);
				ability_use = true;
			}
			else if (curr_player->show == shows::show_ability2_right)
			{
				curr_player->state = ability2_right;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 3);
				ability_use = true;
			}
		}
	}

	if (a4_release)
	{
		if (curr_player->show != shows::show_null)
		{
			if (curr_player->show == shows::show_ability3_down)
			{
				curr_player->state = ability3_down;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 4);
				ability_use = true;
			}
			else if (curr_player->show == shows::show_ability3_up)
			{
				curr_player->state = ability3_up;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 4);
				ability_use = true;
			}
			else if (curr_player->show == shows::show_ability3_left)
			{
				curr_player->state = ability3_left;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 4);
				ability_use = true;
			}
			else if (curr_player->show == shows::show_ability3_right)
			{
				curr_player->state = ability3_right;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 4);
				ability_use = true;
			}
		}

	}


	//check if bomb explode
	if (ability_use)
		CheckBomb(curr_player);

	if (curr_player->move != stop || curr_player->show != show_null)
		curr_player->base_travel = false;

	// State machines

	// Animations
	switch (curr_player->state)
	{
	case run_up:
		curr_player->entity->RunUp();
		break;
	case run_left:
		curr_player->entity->RunLeft();
		break;
	case run_down:
		curr_player->entity->RunDown();
		break;
	case run_right:
		curr_player->entity->RunRight();
		break;
	case walk_up:
		break;
	case walk_left:
		break;
	case walk_down:
		break;
	case walk_right:
		break;
	case idle_up:
		curr_player->entity->IdleUp();
		break;
	case idle_left:
		curr_player->entity->IdleLeft();
		break;
	case idle_down:
		curr_player->entity->IdleDown();
		break;
	case idle_right:
		curr_player->entity->IdleRight();
		break;
	case basic_atack_up:
		curr_player->entity->BasicAttackUp();
		curr_player->state = idle_up;
		break;
	case basic_atack_left:
		curr_player->entity->BasicAttackLeft();
		curr_player->state = idle_left;
		break;
	case basic_atack_down:
		curr_player->entity->BasicAttackDown();
		curr_player->state = idle_down;
		break;
	case basic_atack_right:
		curr_player->entity->BasicAttackRight();
		curr_player->state = idle_right;
		break;
	case ability1_up:
		curr_player->entity->Ability1Up();
		curr_player->state = idle_up;
		break;
	case ability1_left:
		curr_player->entity->Ability1Left();
		curr_player->state = idle_left;
		break;
	case ability1_down:
		curr_player->entity->Ability1Down();
		curr_player->state = idle_down;
		break;
	case ability1_right:
		curr_player->entity->Ability1Right();
		curr_player->state = idle_right;
		break;
	case ability2_up:
		curr_player->entity->Ability2Up();
		curr_player->state = idle_up;
		break;
	case ability2_left:
		curr_player->entity->Ability2Left();
		curr_player->state = idle_left;
		break;
	case ability2_down:
		curr_player->entity->Ability2Down();
		curr_player->state = idle_down;
		break;
	case ability2_right:
		curr_player->entity->Ability2Right();
		curr_player->state = idle_right;
		break;
	case ability3_up:
		curr_player->entity->Ability3Up();
		curr_player->state = idle_up;
		break;
	case ability3_left:
		curr_player->entity->Ability3Left();
		curr_player->state = idle_left;
		break;
	case ability3_down:
		curr_player->entity->Ability3Down();
		curr_player->state = idle_down;
		break;
	case ability3_right:
		curr_player->entity->Ability3Right();
		curr_player->state = idle_right;
		break;
	case states_null:
		break;
	default:
		break;
	}

	// Movement
	float speed = (curr_player->entity->stats.speed * App->GetDT());

	switch (curr_player->move)
	{
	case move_up:
		curr_player->entity->MoveUp(speed);
		break;
	case move_down:
		curr_player->entity->MoveDown(speed);
		break;
	case move_left:
		curr_player->entity->MoveLeft(speed);
		break;
	case move_right:
		curr_player->entity->MoveRight(speed);
		break;
	case move_upleft:
		curr_player->entity->MoveUpLeft(speed);
		break;
	case move_upright:
		curr_player->entity->MoveUpRight(speed);
		break;
	case move_downleft:
		curr_player->entity->MoveDownLeft(speed);
		break;
	case move_downright:
		curr_player->entity->MoveDownRight(speed);
		break;
	case stop:
		break;
	}

	// Show
	switch (curr_player->show)
	{
	case show_basic_atack_up:
		curr_player->entity->ShowBasicAttackUp();
		break;
	case show_basic_atack_left:
		curr_player->entity->ShowBasicAttackLeft();
		break;
	case show_basic_atack_down:
		curr_player->entity->ShowBasicAttackDown();
		break;
	case show_basic_atack_right:
		curr_player->entity->ShowBasicAttackRight();
		break;
	case show_ability1_up:
		curr_player->entity->ShowAbility1Up();
		break;
	case show_ability1_left:
		curr_player->entity->ShowAbility1Left();
		break;
	case show_ability1_down:
		curr_player->entity->ShowAbility1Down();
		break;
	case show_ability1_right:
		curr_player->entity->ShowAbility1Right();
		break;
	case show_ability2_up:
		curr_player->entity->ShowAbility2Up();
		break;
	case show_ability2_left:
		curr_player->entity->ShowAbility2Left();
		break;
	case show_ability2_down:
		curr_player->entity->ShowAbility2Down();
		break;
	case show_ability2_right:
		curr_player->entity->ShowAbility2Right();
		break;
	case show_ability3_up:
		curr_player->entity->ShowAbility3Up();
		break;
	case show_ability3_left:
		curr_player->entity->ShowAbility3Left();
		break;
	case show_ability3_down:
		curr_player->entity->ShowAbility3Down();
		break;
	case show_ability3_right:
		curr_player->entity->ShowAbility3Right();
		break;
	}
}

void PlayerManager::MoveCamera(Player * curr_player)
{
	float speed = DEATH_CAMERA_SPEED*App->GetDT();

	if (App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_LEFT) > 12000 && App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_UP) > 12000)
	{
		App->view->MoveCamera(curr_player->viewport, speed*cos(45 * DEGTORAD), speed*sin(45*DEGTORAD));
	}
	else if (App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_RIGHT) > 12000 && App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_UP) > 12000)
	{
		App->view->MoveCamera(curr_player->viewport, -speed*cos(45 * DEGTORAD), speed*sin(45 * DEGTORAD));
	}
	else if (App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_LEFT) > 12000 && App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_DOWN) > 12000)
	{
		App->view->MoveCamera(curr_player->viewport, speed*cos(45 * DEGTORAD), -speed*sin(45 * DEGTORAD));
	}
	else if (App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_RIGHT) > 12000 && App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_DOWN) > 12000)
	{
		App->view->MoveCamera(curr_player->viewport, -speed*cos(45 * DEGTORAD), -speed*sin(45 * DEGTORAD));
	}

	// Normal moves
	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_LEFT) > 12000)
	{
		App->view->MoveCamera(curr_player->viewport, speed, 0);
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_RIGHT) > 12000)
	{
		App->view->MoveCamera(curr_player->viewport, -speed, 0);
	}
	else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_UP) > 6000)
	{
		App->view->MoveCamera(curr_player->viewport, 0, speed);
	}
	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_DOWN) > 6000)
	{
		App->view->MoveCamera(curr_player->viewport, 0, -speed);
	}
}

void PlayerManager::CheckIfRespawn(Player * player)
{
	if (player->is_dead)
	{
		if (player->death_timer->ReadSec() > player->death_time)
		{
			player->Respawn();
			player->ApplyItemStats();
			p_manager_ui_elements.at(player->viewport - 1).death_time->enabled = false;
		}
	}
}

void PlayerManager::CheckIfDeath(Player * player)
{
	if (player->entity->stats.life <= 0)
	{
    	player->Kill();
		player->show = shows::show_null;

		App->audio->PlayFx(death_sound_effect, 0);

		p_manager_ui_elements.at(player->viewport - 1).death_time->enabled = true;
	}
}

void PlayerManager::UpdateUI(Player* curr_player)
{
	// UI Control -----------

	bool showing_attack = false; 

	int index = curr_player->viewport - 1;

	for (int i = 0; i < curr_player->entity->abilities.size(); i++)
	{
		Ability* ui_ability = nullptr;
		int ability_index = GetUiAbilityByAbility(curr_player, curr_player->entity->abilities.at(i));

		if (i == 0 && (curr_player->show == shows::show_basic_atack_down || curr_player->show == shows::show_basic_atack_up || curr_player->show == shows::show_basic_atack_left || curr_player->show == shows::show_basic_atack_right))
		{
			p_manager_ui_elements.at(index).abilities_button.at(ability_index)->ChangeImage(curr_player->entity->abilities.at(ability_index)->ability_avaliable_pressed);
			showing_attack = true;
		}

		else if (i == 1 && (curr_player->show == shows::show_ability1_down || curr_player->show == shows::show_ability1_up || curr_player->show == shows::show_ability1_left || curr_player->show == shows::show_ability1_right))
		{
			p_manager_ui_elements.at(index).abilities_button.at(ability_index)->ChangeImage(curr_player->entity->abilities.at(ability_index)->ability_avaliable_pressed);
			showing_attack = true;
		}


		else if (i == 2 && (curr_player->show == shows::show_ability2_down || curr_player->show == shows::show_ability2_up || curr_player->show == shows::show_ability2_left || curr_player->show == shows::show_ability2_right))
		{
			p_manager_ui_elements.at(index).abilities_button.at(ability_index)->ChangeImage(curr_player->entity->abilities.at(ability_index)->ability_avaliable_pressed);
			showing_attack = true;
		}


		else if (i == 3 && (curr_player->show == shows::show_ability3_down || curr_player->show == shows::show_ability3_up || curr_player->show == shows::show_ability3_left || curr_player->show == shows::show_ability3_right))
		{
			p_manager_ui_elements.at(index).abilities_button.at(ability_index)->ChangeImage(curr_player->entity->abilities.at(ability_index)->ability_avaliable_pressed);
			showing_attack = true;
		}


		if (curr_player->entity->GetAbility(ability_index)->CdCompleted() && showing_attack == false)
		{
			p_manager_ui_elements.at(index).abilities_button.at(i)->ChangeImage(curr_player->entity->abilities.at(i)->ablility_avaliable);
			p_manager_ui_elements.at(index).abilities_icon.at(i)->ChangeImage(curr_player->entity->abilities.at(ability_index)->ability_icon);
			p_manager_ui_elements.at(index).abilities_cd.at(i)->enabled = false;
			p_manager_ui_elements.at(index).abilities_icon.at(i)->enabled = true;
		}

		else if (showing_attack != true)
		{
			p_manager_ui_elements.at(index).abilities_button.at(i)->ChangeImage(curr_player->entity->abilities.at(i)->ability_in_cd);
			string str("");
			str += std::to_string((int)curr_player->entity->abilities.at(ability_index)->GetCdTimeLeft() + 1);
			p_manager_ui_elements.at(index).abilities_cd.at(i)->enabled = true;
			p_manager_ui_elements.at(index).abilities_icon.at(i)->enabled = false;
			p_manager_ui_elements.at(index).abilities_cd.at(i)->SetText(str);
		}
	}



	// --------------------


	/*switch (curr_player->viewport)
	{
	case 1:

		

		for (int i = 0; i < curr_player->entity->abilities.size(); i++)
		{

			if (curr_player->show == shows::show_basic_atack_down || curr_player->show == shows::show_basic_atack_up || curr_player->show == shows::show_basic_atack_left || curr_player->show == shows::show_basic_atack_right)
			{
				p_manager_ui_elements.at(0).abilities_button.at(0)->ChangeImage(curr_player->entity->GetAbility(0)->ability_avaliable_pressed);
				showing_attack = true; 
			}
			

			else if (curr_player->show == shows::show_ability1_down || curr_player->show == shows::show_ability1_up || curr_player->show == shows::show_ability1_left || curr_player->show == shows::show_ability1_right)
			{
				p_manager_ui_elements.at(0).abilities_button.at(1)->ChangeImage(curr_player->entity->GetAbility(1)->ability_avaliable_pressed);
				showing_attack = true;
			}
			

			else if ((curr_player->show == shows::show_ability2_down || curr_player->show == shows::show_ability2_up || curr_player->show == shows::show_ability2_left || curr_player->show == shows::show_ability2_right) && i == 2)
			{
				p_manager_ui_elements.at(0).abilities_button.at(2)->ChangeImage(curr_player->entity->GetAbility(2)->ability_avaliable_pressed);
				showing_attack = true;
			}
			

			else if ((curr_player->show == shows::show_ability3_down || curr_player->show == shows::show_ability3_up || curr_player->show == shows::show_ability3_left || curr_player->show == shows::show_ability3_right) && i == 3)
			{
				p_manager_ui_elements.at(0).abilities_button.at(3)->ChangeImage(curr_player->entity->GetAbility(3)->ability_avaliable_pressed);
				showing_attack = true;
			}
			

			if (curr_player->entity->GetAbility(i)->CdCompleted() && showing_attack == false)
			{				
				p_manager_ui_elements.at(0).abilities_button.at(i)->ChangeImage(curr_player->entity->GetAbility(i)->ablility_avaliable);
				p_manager_ui_elements.at(0).abilities_cd.at(i)->enabled = false;
				p_manager_ui_elements.at(0).abilities_icon.at(i)->enabled = true;			
			}

			else if(showing_attack != true)
			{
				p_manager_ui_elements.at(0).abilities_button.at(i)->ChangeImage(curr_player->entity->GetAbility(i)->ability_in_cd);
				string str("");
				str += std::to_string((int)curr_player->entity->GetAbility(i)->GetCdTimeLeft() + 1);
				p_manager_ui_elements.at(0).abilities_cd.at(i)->enabled = true;
				p_manager_ui_elements.at(0).abilities_icon.at(i)->enabled = false;
				p_manager_ui_elements.at(0).abilities_cd.at(i)->SetText(str);
			}
		}

		break;
	case 2:
		if (curr_player->show == shows::show_basic_atack_down || curr_player->show == shows::show_basic_atack_up || curr_player->show == shows::show_basic_atack_left || curr_player->show == shows::show_basic_atack_right)
		{
			p_manager_ui_elements.at(1).abilities_button.at(0)->ChangeImage(curr_player->entity->GetAbility(0)->ability_avaliable_pressed);
			showing_attack = true;
		}


		else if (curr_player->show == shows::show_ability1_down || curr_player->show == shows::show_ability1_up || curr_player->show == shows::show_ability1_left || curr_player->show == shows::show_ability1_right)
		{
			p_manager_ui_elements.at(1).abilities_button.at(1)->ChangeImage(curr_player->entity->GetAbility(1)->ability_avaliable_pressed);
			showing_attack = true;
		}


		else if ((curr_player->show == shows::show_ability2_down || curr_player->show == shows::show_ability2_up || curr_player->show == shows::show_ability2_left || curr_player->show == shows::show_ability2_right))
		{
			p_manager_ui_elements.at(1).abilities_button.at(2)->ChangeImage(curr_player->entity->GetAbility(2)->ability_avaliable_pressed);
			showing_attack = true;
		}


		else if ((curr_player->show == shows::show_ability3_down || curr_player->show == shows::show_ability3_up || curr_player->show == shows::show_ability3_left || curr_player->show == shows::show_ability3_right))
		{
			p_manager_ui_elements.at(1).abilities_button.at(3)->ChangeImage(curr_player->entity->GetAbility(3)->ability_avaliable_pressed);
			showing_attack = true;
		}

		for (int i = 0; i < curr_player->entity->abilities.size(); i++)
		{
			if (curr_player->entity->GetAbility(i)->CdCompleted() && showing_attack == false)
			{
				p_manager_ui_elements.at(1).abilities_button.at(i)->ChangeImage(curr_player->entity->GetAbility(i)->ablility_avaliable);
				p_manager_ui_elements.at(1).abilities_cd.at(i)->enabled = false;
				p_manager_ui_elements.at(1).abilities_icon.at(i)->enabled = true;
			}
			else if (showing_attack != true)
			{
				p_manager_ui_elements.at(1).abilities_button.at(i)->ChangeImage(curr_player->entity->GetAbility(i)->ability_in_cd);
				string str("");
				str += std::to_string((int)curr_player->entity->GetAbility(i)->GetCdTimeLeft() + 1);
				p_manager_ui_elements.at(1).abilities_cd.at(i)->enabled = true;
				p_manager_ui_elements.at(1).abilities_icon.at(i)->enabled = false;
				p_manager_ui_elements.at(1).abilities_cd.at(i)->SetText(str);
			}
		}
		break;
	case 3:
		if (curr_player->show == shows::show_basic_atack_down || curr_player->show == shows::show_basic_atack_up || curr_player->show == shows::show_basic_atack_left || curr_player->show == shows::show_basic_atack_right)
		{
			p_manager_ui_elements.at(2).abilities_button.at(0)->ChangeImage(curr_player->entity->GetAbility(0)->ability_avaliable_pressed);
			showing_attack = true;
		}


		else if (curr_player->show == shows::show_ability1_down || curr_player->show == shows::show_ability1_up || curr_player->show == shows::show_ability1_left || curr_player->show == shows::show_ability1_right)
		{
			p_manager_ui_elements.at(2).abilities_button.at(1)->ChangeImage(curr_player->entity->GetAbility(1)->ability_avaliable_pressed);
			showing_attack = true;
		}


		else if ((curr_player->show == shows::show_ability2_down || curr_player->show == shows::show_ability2_up || curr_player->show == shows::show_ability2_left || curr_player->show == shows::show_ability2_right))
		{
			p_manager_ui_elements.at(2).abilities_button.at(2)->ChangeImage(curr_player->entity->GetAbility(2)->ability_avaliable_pressed);
			showing_attack = true;
		}


		else if ((curr_player->show == shows::show_ability3_down || curr_player->show == shows::show_ability3_up || curr_player->show == shows::show_ability3_left || curr_player->show == shows::show_ability3_right))
		{
			p_manager_ui_elements.at(2).abilities_button.at(3)->ChangeImage(curr_player->entity->GetAbility(3)->ability_avaliable_pressed);
			showing_attack = true;
		}

		for (int i = 0; i < curr_player->entity->abilities.size(); i++)
		{
			if (curr_player->entity->GetAbility(i)->CdCompleted() && showing_attack == false)
			{
				p_manager_ui_elements.at(2).abilities_button.at(i)->ChangeImage(curr_player->entity->GetAbility(i)->ablility_avaliable);
				p_manager_ui_elements.at(2).abilities_cd.at(i)->enabled = false;
				p_manager_ui_elements.at(2).abilities_icon.at(i)->enabled = true;
			}
			else if (showing_attack != true)
			{
				p_manager_ui_elements.at(2).abilities_button.at(i)->ChangeImage(curr_player->entity->GetAbility(i)->ability_in_cd);
				string str("");
				str += std::to_string((int)curr_player->entity->GetAbility(i)->GetCdTimeLeft() + 1);
				p_manager_ui_elements.at(2).abilities_cd.at(i)->enabled = true;
				p_manager_ui_elements.at(2).abilities_icon.at(i)->enabled = false;
				p_manager_ui_elements.at(2).abilities_cd.at(i)->SetText(str);
			}
		}
		break;
	case 4:
		if (curr_player->show == shows::show_basic_atack_down || curr_player->show == shows::show_basic_atack_up || curr_player->show == shows::show_basic_atack_left || curr_player->show == shows::show_basic_atack_right)
		{
			p_manager_ui_elements.at(3).abilities_button.at(0)->ChangeImage(curr_player->entity->GetAbility(0)->ability_avaliable_pressed);
			showing_attack = true;
		}


		else if (curr_player->show == shows::show_ability1_down || curr_player->show == shows::show_ability1_up || curr_player->show == shows::show_ability1_left || curr_player->show == shows::show_ability1_right)
		{
			p_manager_ui_elements.at(3).abilities_button.at(1)->ChangeImage(curr_player->entity->GetAbility(1)->ability_avaliable_pressed);
			showing_attack = true;
		}


		else if ((curr_player->show == shows::show_ability2_down || curr_player->show == shows::show_ability2_up || curr_player->show == shows::show_ability2_left || curr_player->show == shows::show_ability2_right))
		{
			p_manager_ui_elements.at(3).abilities_button.at(2)->ChangeImage(curr_player->entity->GetAbility(2)->ability_avaliable_pressed);
			showing_attack = true;
		}


		else if ((curr_player->show == shows::show_ability3_down || curr_player->show == shows::show_ability3_up || curr_player->show == shows::show_ability3_left || curr_player->show == shows::show_ability3_right))
		{
			p_manager_ui_elements.at(3).abilities_button.at(3)->ChangeImage(curr_player->entity->GetAbility(3)->ability_avaliable_pressed);
			showing_attack = true;
		}

		for (int i = 0; i < curr_player->entity->abilities.size(); i++)
		{
			if (curr_player->entity->GetAbility(i)->CdCompleted() && showing_attack == false)
			{
				p_manager_ui_elements.at(3).abilities_button.at(i)->ChangeImage(curr_player->entity->GetAbility(i)->ablility_avaliable);
				p_manager_ui_elements.at(3).abilities_cd.at(i)->enabled = false;
				p_manager_ui_elements.at(3).abilities_icon.at(i)->enabled = true;
			}
			else if (showing_attack != true)
			{
				p_manager_ui_elements.at(3).abilities_button.at(i)->ChangeImage(curr_player->entity->GetAbility(i)->ability_in_cd);
				string str("");
				str += std::to_string((int)curr_player->entity->GetAbility(i)->GetCdTimeLeft() + 1);
				p_manager_ui_elements.at(3).abilities_cd.at(i)->enabled = true;
				p_manager_ui_elements.at(3).abilities_icon.at(i)->enabled = false;
				p_manager_ui_elements.at(3).abilities_cd.at(i)->SetText(str);
			}
		}
		break;
	}*/

	// --------------+
}

void PlayerManager::UpdateDeathUI(Player* curr_player, float dt)
{
	App->view->LayerBlit(9999, death_text_texture, death_text_pos, death_text_anim->GetCurrentAnimation()->GetAnimationFrame(dt), curr_player->viewport, -1.0f, false);
	App->view->LayerBlit(9998, death_text_texture, death_quad_pos, death_quad_rect, curr_player->viewport, -1.0f, false);
	
	string time(""); time += std::to_string((int)curr_player->death_time - (int)curr_player->death_timer->ReadSec());
	p_manager_ui_elements.at(curr_player->viewport-1).death_time->SetText(time);
}

void PlayerManager::PasiveHP(Player * curr_player)
{
	if (curr_player->is_dead)
		return;

	int my_team = curr_player->entity->GetTeam();

	int enemy_team = 0;
	if (my_team == 1)
		enemy_team = 2;
	else
		enemy_team = 1;

	// Heal on my base
	if (curr_player->entity->GetPos().DistanceTo(App->scene->main_scene->shop_manager->team_shop[my_team - 1]) < 200)
	{
		curr_player->entity->Heal(1);
	}
	// Damage me on enemy base
	else if (curr_player->entity->GetPos().DistanceTo(App->scene->main_scene->shop_manager->team_shop[enemy_team - 1]) < 200)
	{
		curr_player->entity->DealDamage(2);
	}
	
	// Passive heal
	if (App->scene->main_scene->GetGameTimer()->ReadSec() - curr_player->last_heal_time > 2)
	{
		curr_player->last_heal_time = App->scene->main_scene->GetGameTimer()->ReadSec();
		curr_player->entity->Heal(1);
	}
}

void PlayerManager::PasiveRupee(Player * curr_player)
{
	if (curr_player->is_dead)
		return;

	if (App->scene->main_scene->GetGameTimer()->ReadSec() - curr_player->last_rupee_time > 3)
	{
		curr_player->last_rupee_time = App->scene->main_scene->GetGameTimer()->ReadSec();
		curr_player->AddRupees(1);
	}
}

void PlayerManager::SetAbilitiesRemaping(Player * curr_player)
{
	int index = curr_player->viewport - 1;

	for (int i = 0; i < 4; i++)
	{
		key_mapping key;
		Ability* curr_player_ability = curr_player->entity->abilities.at(i);

		switch (i)
		{
		case 0:
			key = App->scene->players[index].mapping->GetMapping(m_k_ability1);
			break;
		case 1:
			key = App->scene->players[index].mapping->GetMapping(m_k_ability2);
			break;
		case 2:
			key = App->scene->players[index].mapping->GetMapping(m_k_ability3);
			break;
		case 3:
			key = App->scene->players[index].mapping->GetMapping(m_k_ability4);
			break;
		}

		switch (key.key_id)
		{
		case 10:
			if (key.is_button)
				p_manager_ui_elements.at(index).ability1 = curr_player_ability;
			break;
		case 9:
			if(key.is_button)
				p_manager_ui_elements.at(index).ability2 = curr_player_ability;
			else
				p_manager_ui_elements.at(index).ability3 = curr_player_ability;
			break;
		case 8:
			p_manager_ui_elements.at(index).ability4 = curr_player_ability;
			break;
		}
	}
}

int PlayerManager::GetUiAbilityByAbility(Player* curr_player, Ability * ability)
{
	if (p_manager_ui_elements.at(curr_player->viewport - 1).ability1 == ability)
		return 0;

	if (p_manager_ui_elements.at(curr_player->viewport - 1).ability2 == ability)
		return 1;

	if (p_manager_ui_elements.at(curr_player->viewport - 1).ability3 == ability)
		return 2;

	if (p_manager_ui_elements.at(curr_player->viewport - 1).ability4 == ability)
		return 3;

	return 0;
}

void PlayerManager::CheckBomb(Player * player)
{
	int i = 0;
	for (; i < 3; ++i)
	{
		if (player->items[i] != nullptr && player->items[i]->name == "Bomb")
		{
			break;
		}
	}

	if (i != 3 && GetRandomValue(0, 99) < 20)
	{
		player->entity->stats.life -= player->entity->stats.max_life*0.7f;
		player->items[i] = nullptr;
		player->ApplyItemStats();
		App->scene->main_scene->shop_manager->UpdatePlayerItems(player->viewport - 1, player);
		player->play_exp = true;
	}
}

void PlayerManager::Explode(Player * player)
{
	if (player->explosion->GetCurrentAnimation()->Finished() && player->explo_times == 2)
	{
		player->play_exp = false;
		player->explosion->GetCurrentAnimation()->Reset();
		player->explo_times = 0;
		return;
	}
	else if (player->explosion->GetCurrentAnimation()->Finished())
	{
		player->explosion->GetCurrentAnimation()->Reset();
		player->explo_times++;
	}

	App->view->LayerBlit(player->entity->GetPos().y + 1, player->explosion_tex, { player->entity->GetPos().x-(EXPLOSION_OFFSET*((player->explo_times %2 == 0)? 1:0)), player->entity->GetPos().y-EXPLOSION_OFFSET+27*player->explo_times }, player->explosion->GetCurrentAnimation()->GetAnimationFrame(App->GetDT()));
}


void Player::BuyItem(Item * item, int price)
{
	if (is_dead)
		return;

	for (int i = 0; i < 3; i++)
	{
		if (items[i] != nullptr)
		{
			if (item->upgrade_from == items[i])
			{
				items[i] = item;
				break;
			}
		}
		else
		{
			items[i] = item;
			break;
		}
	}

	rupees -= price;

	int extra_power = 0, extra_hp = 0, extra_speed = 0;

	for (int i = 0; i < 3; i++)
	{
		if (items[i] == nullptr)
		{
			break;
		}
		extra_hp += items[i]->hp;
		extra_power += items[i]->power;
		extra_speed += items[i]->speed;
	}

	entity->UpdateStats(extra_power, extra_hp, extra_speed);

	UpdateRupees();
}

void Player::Kill()
{
	if (entity != nullptr)
	{
		base_travel = false;
		App->entity->DeleteEntity(entity);
		is_dead = true;
		death_timer->Start();
	}
}

void Player::Respawn()
{
	if (is_dead)
	{
		entity = App->entity->CreateEntity(type, respawn);
		entity->SetCamera(viewport);
		entity->SetTeam(team);
		entity->show_life_bar = true;
		entity->is_player = true;
		base_travel = false; 
		is_dead = false;
		state = idle_down;
		invert_controls = false;
	}
}

void Player::BaseTravel()
{
	if (!base_travel)
	{
		base_travel_timer->Start();
		base_travel = true;
	}
	else if(!is_dead)
	{
		SDL_Rect win = App->view->GetViewportRect(1);

		SDL_Rect base_rect = { (win.w / 2 - (BASE_TRAVEL_RECT_W / 2)), win.h - 20, BASE_TRAVEL_RECT_W, BASE_TRAVEL_RECT_H };

		float width = (base_rect.w * base_travel_timer->ReadSec()) / BASE_TRAVEL_TIME;
		SDL_Rect time_rect = { base_rect.x, base_rect.y, width, base_rect.h };

		App->view->LayerDrawQuad(base_rect, 32, 32, 32, 200, true, 1, viewport, false);
		App->view->LayerDrawQuad(time_rect, 208, 240, 208, 200, true, 1, viewport, false);

		if (base_travel_timer->ReadSec() > BASE_TRAVEL_TIME)
		{
			fPoint r; r.x = respawn.x; r.y = respawn.y;
			entity->game_object->SetPos(r);
			base_travel = false;
		}
	}
	else
	{
		base_travel = false;
	}
}

void Player::ApplyItemStats()
{
	if (is_dead)
		return;

	int extra_hp = 0;
	int extra_power = 0;
	int extra_speed = 0;

	for (int i = 0; i < 3; i++)
	{
		if (items[i] == nullptr)
		{
			continue;
		}
		extra_hp += items[i]->hp;
		extra_power += items[i]->power;
		extra_speed += items[i]->speed;
	}

	entity->UpdateStats(extra_power, extra_hp, extra_speed);
}

void Player::AddRupees(int add)
{
	int curr_num = rupees; 
	rupees += add;
	int next_num = rupees; 

	if (curr_num <= 9 && next_num >= 10)
		rupees_num->SetPos({ rupees_num->GetPos().x - 2, rupees_num->GetPos().y });

	if (curr_num <= 99 && next_num >= 100)
		rupees_num->SetPos({ rupees_num->GetPos().x - 4, rupees_num->GetPos().y });

	if (curr_num <= 999 && next_num >= 1000)
		rupees_num->SetPos({ rupees_num->GetPos().x - 6, rupees_num->GetPos().y });

	UpdateRupees();
}

void Player::CleanUp()
{
	App->entity->DeleteEntity(entity);

	App->DeleteGameplayTimer(death_timer);
	App->DeleteGameplayTimer(base_travel_timer);

	explosion->CleanUp();
	RELEASE(explosion);
}

void Player::UpdateQuestsStats()
{
	int extra_power = 0, extra_hp = 0, extra_speed = 0;

	for (int i = 0; i < 3; i++)
	{
		if (items[i] == nullptr)
		{
			break;
		}
		extra_hp += items[i]->hp;
		extra_power += items[i]->power;
		extra_speed += items[i]->speed;
	}

	entity->UpdateStats(extra_power, extra_hp, extra_speed);
}

void Player::UpdateRupees()
{
	string r;
	r = std::to_string(rupees);
	rupees_num->SetText(r);
}
