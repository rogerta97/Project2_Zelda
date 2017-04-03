#include "PlayerManager.h"
#include "j1Viewports.h"
#include "j1Scene.h"
#include "j1Input.h"
#include "p2Log.h"
#include "j1Map.h"
#include "GameObject.h"

#define DEATH_CAMERA_SPEED 500

PlayerManager::PlayerManager()
{
}

PlayerManager::~PlayerManager()
{
}

bool PlayerManager::Awake(pugi::xml_node &)
{
	return true;
}

bool PlayerManager::Start()
{
	// Abilities UI
	SDL_Rect screen = App->view->GetViewportRect(1);
	iPoint ability1_pos = { screen.w - 120 , screen.h - 126 };
	iPoint ability2_pos = { 13, screen.h - 126 };
	iPoint ability3_pos = { screen.w - 90, screen.h - 76 };
	iPoint ability4_pos = { 13, screen.h - 76 };

	_TTF_Font* text_font = App->font->game_font;
	iPoint text1_pos = { screen.w - 85 , screen.h - 119 };
	iPoint text2_pos = { (55), screen.h - 119 };
	iPoint text3_pos = { screen.w - 70, screen.h - 56 };
	iPoint text4_pos = { 35, screen.h - 56 };

	SDL_Color death_rect_color = { 32, 32, 32, 100 };
	iPoint death_text_pos = { int(screen.w*0.5f) - 185, int(screen.h*0.5f) - 50 };

	// p1
	abilities_1.push_back(App->scene->main_scene->main_window_1->CreateImage(ability1_pos, { 182, 78, 35, 35 }));
	abilities_1.push_back(App->scene->main_scene->main_window_1->CreateImage(ability2_pos, { 182, 78, 35, 35 }));
	abilities_1.push_back(App->scene->main_scene->main_window_1->CreateImage(ability3_pos, { 182, 78, 35, 35 }));
	abilities_1.push_back(App->scene->main_scene->main_window_1->CreateImage(ability4_pos, { 182, 78, 35, 35 }));

	abilities_cd_1.push_back(App->scene->main_scene->main_window_1->CreateText(text1_pos, text_font));
	abilities_cd_1.push_back(App->scene->main_scene->main_window_1->CreateText(text2_pos, text_font));
	abilities_cd_1.push_back(App->scene->main_scene->main_window_1->CreateText(text3_pos, text_font));
	abilities_cd_1.push_back(App->scene->main_scene->main_window_1->CreateText(text4_pos, text_font));

	death_rect_1 = App->scene->main_scene->main_window_1->CreateColoredRect(iPoint(0, 0), screen.w, screen.h, death_rect_color, true);
	death_rect_1->enabled = false; death_rect_1->blit_layer += 1;
	death_text_1 = App->scene->main_scene->main_window_1->CreateText(death_text_pos, App->font->game_font_20, 0);
	death_text_1->enabled = false; death_text_1->blit_layer += 1;

	// p2
	abilities_2.push_back(App->scene->main_scene->main_window_2->CreateImage(ability1_pos, { 182, 78, 35, 35 }));
	abilities_2.push_back(App->scene->main_scene->main_window_2->CreateImage(ability2_pos, { 182, 78, 35, 35 }));
	abilities_2.push_back(App->scene->main_scene->main_window_2->CreateImage(ability3_pos, { 182, 78, 35, 35 }));
	abilities_2.push_back(App->scene->main_scene->main_window_2->CreateImage(ability4_pos, { 182, 78, 35, 35 }));

	abilities_cd_2.push_back(App->scene->main_scene->main_window_2->CreateText(text1_pos, text_font));
	abilities_cd_2.push_back(App->scene->main_scene->main_window_2->CreateText(text2_pos, text_font));
	abilities_cd_2.push_back(App->scene->main_scene->main_window_2->CreateText(text3_pos, text_font));
	abilities_cd_2.push_back(App->scene->main_scene->main_window_2->CreateText(text4_pos, text_font));

	death_rect_2 = App->scene->main_scene->main_window_2->CreateColoredRect(iPoint(0, 0), screen.w, screen.h, death_rect_color, true);
	death_rect_2->enabled = false; death_rect_2->blit_layer += 1;
	death_text_2 = App->scene->main_scene->main_window_2->CreateText(death_text_pos, App->font->game_font_20, 0);
	death_text_2->enabled = false; death_text_2->blit_layer += 1;

	// p3
	abilities_3.push_back(App->scene->main_scene->main_window_3->CreateImage(ability1_pos, { 182, 78, 35, 35 }));
	abilities_3.push_back(App->scene->main_scene->main_window_3->CreateImage(ability2_pos, { 182, 78, 35, 35 }));
	abilities_3.push_back(App->scene->main_scene->main_window_3->CreateImage(ability3_pos, { 182, 78, 35, 35 }));
	abilities_3.push_back(App->scene->main_scene->main_window_3->CreateImage(ability4_pos, { 182, 78, 35, 35 }));

	abilities_cd_3.push_back(App->scene->main_scene->main_window_3->CreateText(text1_pos, text_font));
	abilities_cd_3.push_back(App->scene->main_scene->main_window_3->CreateText(text2_pos, text_font));
	abilities_cd_3.push_back(App->scene->main_scene->main_window_3->CreateText(text3_pos, text_font));
	abilities_cd_3.push_back(App->scene->main_scene->main_window_3->CreateText(text4_pos, text_font));

	death_rect_3 = App->scene->main_scene->main_window_3->CreateColoredRect(iPoint(0, 0), screen.w, screen.h, death_rect_color, true);
	death_rect_3->enabled = false; death_rect_3->blit_layer += 1;
	death_text_3 = App->scene->main_scene->main_window_3->CreateText(death_text_pos, App->font->game_font_20, 0);
	death_text_3->enabled = false; death_text_3->blit_layer += 1;

	// p4
	abilities_4.push_back(App->scene->main_scene->main_window_4->CreateImage(ability1_pos, { 182, 78, 35, 35 }));
	abilities_4.push_back(App->scene->main_scene->main_window_4->CreateImage(ability2_pos, { 182, 78, 35, 35 }));
	abilities_4.push_back(App->scene->main_scene->main_window_4->CreateImage(ability3_pos, { 182, 78, 35, 35 }));
	abilities_4.push_back(App->scene->main_scene->main_window_4->CreateImage(ability4_pos, { 182, 78, 35, 35 }));

	abilities_cd_4.push_back(App->scene->main_scene->main_window_4->CreateText(text1_pos, text_font));
	abilities_cd_4.push_back(App->scene->main_scene->main_window_4->CreateText(text2_pos, text_font));
	abilities_cd_4.push_back(App->scene->main_scene->main_window_4->CreateText(text3_pos, text_font));
	abilities_cd_4.push_back(App->scene->main_scene->main_window_4->CreateText(text4_pos, text_font));

	death_rect_4 = App->scene->main_scene->main_window_4->CreateColoredRect(iPoint(0, 0), screen.w, screen.h, death_rect_color, true);
	death_rect_4->enabled = false; death_rect_4->blit_layer += 1;
	death_text_4 = App->scene->main_scene->main_window_4->CreateText(death_text_pos, App->font->game_font_20, 0);
	death_text_4->enabled = false; death_text_4->blit_layer += 1;

	// Event
	event_thrower = new EventThrower();

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

		if (!curr_player->is_dead)
		{
			if (curr_player->entity->stats.life < curr_player->entity->stats.max_life)
			{
				if (curr_player->entity->GetPos().DistanceTo(App->scene->main_scene->shop_manager->team_shop[curr_player->entity->GetTeam() - 1]) < 200)
				{
					curr_player->entity->stats.life += 1;
				}
				else
				{
					if (App->scene->main_scene->GetGameTimer()->ReadSec() - last_heal_time > 2)
					{
						last_heal_time = App->scene->main_scene->GetGameTimer()->ReadSec();
						curr_player->entity->stats.life += 1;
					}
				}
			}

			PlayerInput(curr_player);

			UpdateUI(curr_player);
			CheckIfDeath(curr_player);
		}
		else
		{
			if (!curr_player->disable_controller)
				MoveCamera(curr_player);

			CheckIfRespawn(curr_player);
			UpdateDeathUI(curr_player);
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

	ClearPlayers();

	abilities_1.clear();
	abilities_2.clear();
	abilities_3.clear();
	abilities_4.clear();

	RELEASE(event_thrower);

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
				App->entity->DeleteEntity((*it)->entity);
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
			App->entity->DeleteEntity((*it)->entity);
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
	for (int i = 0; i < players.size(); i++)
	{
		if (players.at(i)->entity == entity)
		{
			return players.at(i)->viewport;
		}
	}
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
		player->entity->GetAbility(ability - 1)->cd_timer.Start();
}

int PlayerManager::GetPlayerTeamFromBody(PhysBody * body)
{
	if (!players.empty())
	{
		for (std::vector<Player*>::iterator it = players.begin(); it != players.end(); it++)
		{
			if ((*it)->entity->game_object->pbody == body)
			{
				return (*it)->entity->GetTeam();
			}
		}
	}
	return 0;
}

Player * PlayerManager::GetPlayerFromBody(PhysBody * body)
{
	if (!players.empty())
	{
		for (std::vector<Player*>::iterator it = players.begin(); it != players.end(); it++)
		{
			if ((*it)->entity->game_object->pbody == body)
			{
				return (*it);
			}
		}
	}
	return nullptr;
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

void PlayerManager::PlayerInput(Player * curr_player)
{
	// Left Joystick -------

	if (curr_player->entity == nullptr)
		return;

	// Diagonal moves
	if (curr_player->entity->stuned || curr_player->disable_controller)
	{
		curr_player->move = stop;
	}
	else if (App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_LEFT) > 12000 && App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_UP) > 12000)
	{
		curr_player->move = move_upleft;
	}
	else if (App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_RIGHT) > 12000 && App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_UP) > 12000)
	{
		curr_player->move = move_upright;
	}
	else if (App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_LEFT) > 12000 && App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_DOWN) > 12000)
	{
		curr_player->move = move_downleft;
	}
	else if (App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_RIGHT) > 12000 && App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_DOWN) > 12000)
	{
		curr_player->move = move_downright;
	}
	// Normal moves
	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_LEFT) > 12000)
	{
		curr_player->move = move_left;
		curr_player->state = run_left;
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_RIGHT) > 12000)
	{
		curr_player->move = move_right;
		curr_player->state = run_right;
	}
	else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_UP) > 6000)
	{
		curr_player->move = move_up;
		curr_player->state = run_up;
	}
	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || App->input->GetControllerJoystickMove(curr_player->controller_index, LEFTJOY_DOWN) > 6000)
	{
		curr_player->move = move_down;
		curr_player->state = run_down;
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
		if (App->input->GetControllerButton(curr_player->controller_index, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == KEY_REPEAT)
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
		else if (App->input->GetControllerButton(curr_player->controller_index, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == KEY_REPEAT)
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
		else if (App->input->GetControllerJoystickMove(curr_player->controller_index, RIGHT_TRIGGER) > 22000)
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
		else if (App->input->GetControllerJoystickMove(curr_player->controller_index, LEFT_TRIGGER) > 22000)
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
	if (App->input->GetControllerButton(curr_player->controller_index, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == KEY_IDLE)
	{
		if (curr_player->show != shows::show_null)
		{
			if (curr_player->show == shows::show_basic_atack_down)
			{
				curr_player->state = basic_atack_down;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 1);
			}
			else if (curr_player->show == shows::show_basic_atack_up)
			{
				curr_player->state = basic_atack_up;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 1);
			}
			else if (curr_player->show == shows::show_basic_atack_left)
			{
				curr_player->state = basic_atack_left;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 1);
			}
			else if (curr_player->show == shows::show_basic_atack_right)
			{
				curr_player->state = basic_atack_right;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 1);
			}
		}
	}

	if (App->input->GetControllerButton(curr_player->controller_index, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == KEY_IDLE)
	{
		if (curr_player->show != shows::show_null)
		{
			if (curr_player->show == shows::show_ability1_down)
			{
				curr_player->state = ability1_down;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 2);
			}
			else if (curr_player->show == shows::show_ability1_up)
			{
				curr_player->state = ability1_up;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 2);
			}
			else if (curr_player->show == shows::show_ability1_left)
			{
				curr_player->state = ability1_left;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 2);
			}
			else if (curr_player->show == shows::show_ability1_right)
			{
				curr_player->state = ability1_right;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 2);
			}
		}
	}

	if (App->input->GetControllerJoystickMove(curr_player->controller_index, RIGHT_TRIGGER) < 22000)
	{
		if (curr_player->show != shows::show_null)
		{
			if (curr_player->show == shows::show_ability2_down)
			{
				curr_player->state = ability2_down;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 3);
			}
			else if (curr_player->show == shows::show_ability2_up)
			{
				curr_player->state = ability2_up;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 3);
			}
			else if (curr_player->show == shows::show_ability2_left)
			{
				curr_player->state = ability2_left;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 3);
			}
			else if (curr_player->show == shows::show_ability2_right)
			{
				curr_player->state = ability2_right;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 3);
			}
		}
	}

	if (App->input->GetControllerJoystickMove(curr_player->controller_index, LEFT_TRIGGER) < 22000)
	{
		if (curr_player->show != shows::show_null)
		{
			if (curr_player->show == shows::show_ability3_down)
			{
				curr_player->state = ability3_down;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 4);
			}
			else if (curr_player->show == shows::show_ability3_up)
			{
				curr_player->state = ability3_up;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 4);
			}
			else if (curr_player->show == shows::show_ability3_left)
			{
				curr_player->state = ability3_left;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 4);
			}
			else if (curr_player->show == shows::show_ability3_right)
			{
				curr_player->state = ability3_right;
				curr_player->show = shows::show_null;
				ResetAbilityTimer(curr_player, 4);
			}
		}
	}

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
		curr_player->entity->Ability1Down();
		curr_player->state = idle_left;
		break;
	case ability1_down:
		curr_player->entity->Ability1Left();
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
		curr_player->entity->ShowAbility1Down();
		break;
	case show_ability1_down:
		curr_player->entity->ShowAbility1Left();
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
		if (player->death_timer.ReadSec() > player->death_time)
		{
			switch (player->viewport)
			{
			case 1:
				death_rect_1->SetEnabled(false);
				death_text_1->SetEnabled(false);
				break;
			case 2:
				death_rect_2->SetEnabled(false);
				death_text_2->SetEnabled(false);
				break;
			case 3:
				death_rect_3->SetEnabled(false);
				death_text_3->SetEnabled(false);
				break;
			case 4:
				death_rect_4->SetEnabled(false);
				death_text_4->SetEnabled(false);
				break;
			}

			player->Respawn();
			player->ApplyItemStats();
		}
	}
}

void PlayerManager::CheckIfDeath(Player * player)
{
	if (player->entity->stats.life <= 0)
	{
		Event* event_die = new Event();
		event_die->type = e_t_death;
		event_die->event_data.entity = player->entity;
		event_thrower->AddEvent(event_die);

		switch (player->viewport)
		{
		case 1:
			death_rect_1->SetEnabled(true);
			death_text_1->SetEnabled(true);
			break;
		case 2:
			death_rect_2->SetEnabled(true);
			death_text_2->SetEnabled(true);
			break;
		case 3:
			death_rect_3->SetEnabled(true);
			death_text_3->SetEnabled(true);
			break;
		case 4:
			death_rect_4->SetEnabled(true);
			death_text_4->SetEnabled(true);
			break;
		}

		player->Kill();
		player->show = shows::show_null;
	}
}

void PlayerManager::UpdateUI(Player* curr_player)
{
	// UI Control -----------

	switch (curr_player->viewport)
	{
	case 1:
		for (int i = 0; i < curr_player->entity->abilities.size(); i++)
		{
			if (curr_player->entity->GetAbility(i)->CdCompleted())
			{
				abilities_1.at(i)->ChangeImage(curr_player->entity->GetAbility(i)->ablility_avaliable);
				abilities_cd_1.at(i)->enabled = false;
			}
			else
			{
				abilities_1.at(i)->ChangeImage(curr_player->entity->GetAbility(i)->ability_in_cd);
				string str("");
				str += std::to_string((int)curr_player->entity->GetAbility(i)->GetCdTimeLeft() + 1);
				abilities_cd_1.at(i)->enabled = true; abilities_cd_1.at(i)->SetText(str);
			}
		}
		break;
	case 2:
		for (int i = 0; i < curr_player->entity->abilities.size(); i++)
		{
			if (curr_player->entity->GetAbility(i)->CdCompleted())
			{
				abilities_2.at(i)->ChangeImage(curr_player->entity->GetAbility(i)->ablility_avaliable);
				abilities_cd_2.at(i)->enabled = false;
			}
			else
			{
				abilities_2.at(i)->ChangeImage(curr_player->entity->GetAbility(i)->ability_in_cd);
				string str("");
				str += std::to_string((int)curr_player->entity->GetAbility(i)->GetCdTimeLeft() + 1);
				abilities_cd_2.at(i)->enabled = true; abilities_cd_2.at(i)->SetText(str);
			}
		}
		break;
	case 3:
		for (int i = 0; i < curr_player->entity->abilities.size(); i++)
		{
			if (curr_player->entity->GetAbility(i)->CdCompleted())
			{
				abilities_3.at(i)->ChangeImage(curr_player->entity->GetAbility(i)->ablility_avaliable);
				abilities_cd_3.at(i)->enabled = false;
			}
			else
			{
				abilities_3.at(i)->ChangeImage(curr_player->entity->GetAbility(i)->ability_in_cd);
				string str("");
				str += std::to_string((int)curr_player->entity->GetAbility(i)->GetCdTimeLeft() + 1);
				abilities_cd_3.at(i)->enabled = true; abilities_cd_3.at(i)->SetText(str);
			}
		}
		break;
	case 4:
		for (int i = 0; i < curr_player->entity->abilities.size(); i++)
		{
			if (curr_player->entity->GetAbility(i)->CdCompleted())
			{
				abilities_4.at(i)->ChangeImage(curr_player->entity->GetAbility(i)->ablility_avaliable);
				abilities_cd_4.at(i)->enabled = false;
			}
			else
			{
				abilities_4.at(i)->ChangeImage(curr_player->entity->GetAbility(i)->ability_in_cd);
				string str("");
				str += std::to_string((int)curr_player->entity->GetAbility(i)->GetCdTimeLeft() + 1);
				abilities_cd_4.at(i)->enabled = true; abilities_cd_4.at(i)->SetText(str);
			}
		}
		break;
	}

	// --------------
}

void PlayerManager::UpdateDeathUI(Player * player)
{
	string str("You have been slain, respawn time ");
	int time = player->death_time + 1 - player->death_timer.ReadSec();
	 
	str += std::to_string(time);

	switch (player->viewport)
	{
	case 1:
	{	
		death_text_1->SetText(str);
	}
	break;
	case 2:
	{
		death_text_2->SetText(str);
	}
	break;
	case 3:
	{
		death_text_3->SetText(str);
	}
	break;
	case 4:
	{
		death_text_4->SetText(str);
	}
	break;
	}
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
		App->entity->DeleteEntity(entity);
		is_dead = true;
		death_timer.Start();
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
		is_dead = false;
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

void Player::UpdateRupees()
{
	string r;
	r = std::to_string(rupees);
	rupees_num->SetText(r);
}
