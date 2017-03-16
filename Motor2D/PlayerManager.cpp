#include "PlayerManager.h"
#include "j1Viewports.h"
#include "j1Scene.h"
#include "j1Input.h"
#include "p2Log.h"
#include "j1Map.h"

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

		// Input

		// Left Joystick -------

			// Diagonal moves
		if (curr_player->entity->stuned)
		{
			curr_player->move = stop;
		}
		else if (App->input->GetControllerJoystickMove(curr_player->index, LEFTJOY_LEFT) > 12000 && App->input->GetControllerJoystickMove(curr_player->index, LEFTJOY_UP) > 12000)
		{
			curr_player->move = move_upleft;
		}
		else if (App->input->GetControllerJoystickMove(curr_player->index, LEFTJOY_RIGHT) > 12000 && App->input->GetControllerJoystickMove(curr_player->index, LEFTJOY_UP) > 12000)
		{
			curr_player->move = move_upright;
		}
		else if (App->input->GetControllerJoystickMove(curr_player->index, LEFTJOY_LEFT) > 12000 && App->input->GetControllerJoystickMove(curr_player->index, LEFTJOY_DOWN) > 12000)
		{
			curr_player->move = move_downleft;
		}
		else if (App->input->GetControllerJoystickMove(curr_player->index, LEFTJOY_RIGHT) > 12000 && App->input->GetControllerJoystickMove(curr_player->index, LEFTJOY_DOWN) > 12000)
		{
			curr_player->move = move_downright;
		}
			// Normal moves
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || App->input->GetControllerJoystickMove(curr_player->index, LEFTJOY_LEFT) > 12000)
		{
			curr_player->move = move_left;
		}
		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetControllerJoystickMove(curr_player->index, LEFTJOY_RIGHT) > 12000)
		{
			curr_player->move = move_right;
		}
		else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || App->input->GetControllerJoystickMove(curr_player->index, LEFTJOY_UP) > 6000)
		{
			curr_player->move = move_up;
		}
		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || App->input->GetControllerJoystickMove(curr_player->index, LEFTJOY_DOWN) > 6000)
		{
			curr_player->move = move_down;
		}
		else
		{
			curr_player->move = stop;
		}
		// --------------------

		// Right Joystick -----
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || App->input->GetControllerButton(curr_player->index, SDL_CONTROLLER_BUTTON_DPAD_LEFT) == KEY_REPEAT || App->input->GetControllerJoystickMove(curr_player->index, RIGHTJOY_LEFT) > 22000)
		{
			if (curr_player->move != stop)
				curr_player->state = run_left;
			else
				curr_player->state = idle_left;
		}
		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetControllerButton(curr_player->index, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == KEY_REPEAT || App->input->GetControllerJoystickMove(curr_player->index, RIGHTJOY_RIGHT) > 22000)
		{
			if (curr_player->move != stop)
				curr_player->state = run_right;
			else
				curr_player->state = idle_right;
		}
		else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || App->input->GetControllerButton(curr_player->index, SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_REPEAT || App->input->GetControllerJoystickMove(curr_player->index, RIGHTJOY_UP) > 12000)
		{
			if (curr_player->move != stop)
				curr_player->state = run_up;
			else
				curr_player->state = idle_up;
		}
		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || App->input->GetControllerButton(curr_player->index, SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_REPEAT || App->input->GetControllerJoystickMove(curr_player->index, RIGHTJOY_DOWN) > 12000)
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
		if (App->input->GetControllerButton(curr_player->index, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == KEY_REPEAT)
		{
			if(curr_player->state == idle_down || curr_player->state == run_down)
				curr_player->show = shows::show_basic_atack_down;
			else if (curr_player->state == idle_up || curr_player->state == run_up)
				curr_player->show = shows::show_basic_atack_up;
			else if (curr_player->state == idle_left || curr_player->state == run_left)
				curr_player->show = shows::show_basic_atack_left;
			else if (curr_player->state == idle_right || curr_player->state == run_right)
				curr_player->show = shows::show_basic_atack_right;
		}
		else if (App->input->GetControllerButton(curr_player->index, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == KEY_REPEAT)
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
		else if (App->input->GetControllerJoystickMove(curr_player->index, RIGHT_TRIGGER) > 22000)
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
		else if (App->input->GetControllerJoystickMove(curr_player->index, LEFT_TRIGGER) > 22000)
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

		// Abilities RELEASE
		if (App->input->GetControllerButton(curr_player->index, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == KEY_IDLE && !curr_player->entity->stuned)
		{
			if (curr_player->show != shows::show_null)
			{
				if (curr_player->show == shows::show_basic_atack_down)
				{
					curr_player->state = basic_atack_down;
					curr_player->show = shows::show_null;
				}
				else if (curr_player->show == shows::show_basic_atack_up)
				{
					curr_player->state = basic_atack_up;
					curr_player->show = shows::show_null;
				}
				else if (curr_player->show == shows::show_basic_atack_left)
				{
					curr_player->state = basic_atack_left;
					curr_player->show = shows::show_null;
				}
				else if (curr_player->show == shows::show_basic_atack_right)
				{
					curr_player->state = basic_atack_right;
					curr_player->show = shows::show_null;
				}
			}
		}

		if (App->input->GetControllerButton(curr_player->index, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == KEY_IDLE)
		{
			if (curr_player->show != shows::show_null)
			{
				if (curr_player->show == shows::show_ability1_down)
				{
					curr_player->state = ability1_down;
					curr_player->show = shows::show_null;
				}
				else if (curr_player->show == shows::show_ability1_up)
				{
					curr_player->state = ability1_up;
					curr_player->show = shows::show_null;
				}
				else if (curr_player->show == shows::show_ability1_left)
				{
					curr_player->state = ability1_left;
					curr_player->show = shows::show_null;
				}
				else if (curr_player->show == shows::show_ability1_right)
				{
					curr_player->state = ability1_right;
					curr_player->show = shows::show_null;
				}
			}
		}

		if (App->input->GetControllerJoystickMove(curr_player->index, RIGHT_TRIGGER) < 22000)
		{
			if (curr_player->show != shows::show_null)
			{
				if (curr_player->show == shows::show_ability2_down)
				{
					curr_player->state = ability2_down;
					curr_player->show = shows::show_null;
				}
				else if (curr_player->show == shows::show_ability2_up)
				{
					curr_player->state = ability2_up;
					curr_player->show = shows::show_null;
				}
				else if (curr_player->show == shows::show_ability2_left)
				{
					curr_player->state = ability2_left;
					curr_player->show = shows::show_null;
				}
				else if (curr_player->show == shows::show_ability2_right)
				{
					curr_player->state = ability2_right;
					curr_player->show = shows::show_null;
				}
			}
		}

		if (App->input->GetControllerJoystickMove(curr_player->index, LEFT_TRIGGER) < 22000)
		{
			if (curr_player->show != shows::show_null)
			{
				if (curr_player->show == shows::show_ability3_down)
				{
					curr_player->state = ability3_down;
					curr_player->show = shows::show_null;
				}
				else if (curr_player->show == shows::show_ability3_up)
				{
					curr_player->state = ability3_up;
					curr_player->show = shows::show_null;
				}
				else if (curr_player->show == shows::show_ability3_left)
				{
					curr_player->state = ability3_left;
					curr_player->show = shows::show_null;
				}
				else if (curr_player->show == shows::show_ability3_right)
				{
					curr_player->state = ability3_right;
					curr_player->show = shows::show_null;
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
			curr_player->entity->Ability3Down();
			curr_player->state = idle_left;
			break;
		case ability3_down:
			curr_player->entity->Ability3Left();
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
		float speed = (curr_player->entity->stats.speed * dt);

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
			curr_player->entity->ShowAbility3Down();
			break;
		case show_ability3_down:
			curr_player->entity->ShowAbility3Left();
			break;
		case show_ability3_right:
			curr_player->entity->ShowAbility3Right();
			break;
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
	spawn_points_used_team1.clear();
	spawn_points_used_team2.clear();

	return ret;
}

Player* PlayerManager::AddPlayer(entity_name name, int index, iPoint pos, int team, bool on_spawn, int show_life_bar)
{
	Player* ret = nullptr;

	if (players.size() <= 3)
	{
		iPoint position = pos;

		// Searching for avaliable spawn points
		if(on_spawn)
		{
			vector<iPoint> spawn_points;
			App->map->GetPlayerSpawnPoints(team, spawn_points);
			for (int i = 0; i < spawn_points.size(); i++)
			{
				if (!CheckIfSpawnPointIsUsed(team, spawn_points.at(i)))
				{
					position = spawn_points.at(i);
					break;
				}
			}
		}

		// Create player
		Player* p = new Player(App->entity->CreateEntity(name, position), index - 1);
		p->entity->SetCamera(p->index + 1);
		p->entity->SetTeam(team);
		p->entity->show_life_bar = show_life_bar;
		p->entity->is_player = true;
		players.push_back(p);
		ret = p;
	}

	return ret;
}

void PlayerManager::ChangePlayer(entity_name name, int index)
{
	iPoint pos;
	for(vector<Player*>::iterator it = players.begin(); it != players.end(); it++)
	{
		if ((*it)->index == index - 1)
		{
			pos = (*it)->entity->GetPos();
			App->entity->DeleteEntity((*it)->entity);
			players.erase(it);
			RELEASE(*it);
			break;
		}
	}

	Player* p = new Player(App->entity->CreateEntity(name, pos), index - 1);
	p->entity->SetCamera(p->index + 1);
	players.push_back(p);
}

void PlayerManager::DeletePlayer(int index)
{
	for (vector<Player*>::iterator it = players.begin(); it != players.end(); it++)
	{
		if ((*it)->index == index - 1)
		{
			players.erase(it);
			App->entity->DeleteEntity((*it)->entity);
			RELEASE(*it);
			break;
		}
	}
}

void PlayerManager::ClearPlayers()
{
	for (int i = 0; i < players.size(); i++)
	{
		RELEASE(players.at(i));
	}
	players.clear();
}

std::vector<Entity*> PlayerManager::GetTeamPlayers(int team)
{
	std::vector<Entity*> ret;

	for (std::vector<Player*>::iterator it = players.begin(); it != players.end(); it++)
	{
		if ((*it)->entity->GetTeam() == team)
		{
			ret.push_back((*it)->entity);
		}
	}

	return ret;
}

std::vector<int> PlayerManager::GetTeamViewports(int team)
{
	std::vector<int> ret;

	for (std::vector<Player*>::iterator it = players.begin(); it != players.end(); it++)
	{
		if ((*it)->entity->GetTeam() == team)
		{
			ret.push_back((*it)->index+1);
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
			return players.at(i)->index + 1;
		}
	}
}

bool PlayerManager::CheckIfSpawnPointIsUsed(int team, iPoint pos)
{
	switch (team)
	{
	case 1:
		for (int i = 0; i < spawn_points_used_team1.size(); i++)
		{
			if (spawn_points_used_team1.at(i) == pos)
			{
				spawn_points_used_team1.push_back(pos);
				return true;
			}
		}
		return false;
		break;
	case 2:
		for (int i = 0; i < spawn_points_used_team2.size(); i++)
		{
			if (spawn_points_used_team2.at(i) == pos)
			{
				spawn_points_used_team2.push_back(pos);
				return true;
			}
		}
		return false;
		break;
	default:
		break;
	}
}

