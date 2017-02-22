#include "PlayerManager.h"
#include "j1Viewports.h"
#include "j1Input.h"

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

	float speed = (200 * dt); // Temporal

	for (int i = 0; i < players.size(); i++)
	{
		// Input
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || App->input->GetControllerButton(players.at(i).index, SDL_CONTROLLER_BUTTON_DPAD_LEFT) == KEY_REPEAT || App->input->GetControllerJoystickMove(players.at(i).index, LEFTJOY_LEFT) > 12000)
		{
			players.at(i).state = run_left;
		}
		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetControllerButton(players.at(i).index, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == KEY_REPEAT || App->input->GetControllerJoystickMove(players.at(i).index, LEFTJOY_RIGHT) > 12000)
		{
			players.at(i).state = run_right;
		}
		else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || App->input->GetControllerButton(players.at(i).index, SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_REPEAT || App->input->GetControllerJoystickMove(players.at(i).index, LEFTJOY_UP) > 12000)
		{
			players.at(i).state = run_up;
		}
		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || App->input->GetControllerButton(players.at(i).index, SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_REPEAT || App->input->GetControllerJoystickMove(players.at(i).index, LEFTJOY_DOWN) > 12000)
		{
			players.at(i).state = run_down;
		}
		else if (players.at(i).state == run_left)
		{
			players.at(i).state = idle_left;
		}
		else if (players.at(i).state == run_right)
		{
			players.at(i).state = idle_right;
		}
		else if (players.at(i).state == run_up)
		{
			players.at(i).state = idle_up;
		}
		else if (players.at(i).state == run_down)
		{
			players.at(i).state = idle_down;
		}


		// State machine
		switch (players.at(i).state)
		{
		case run_up:
			players.at(i).player->RunUp(speed);
			break;
		case run_left:
			players.at(i).player->RunLeft(speed);
			break;
		case run_down:
			players.at(i).player->RunDown(speed);
			break;
		case run_right:
			players.at(i).player->RunRight(speed);
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
			players.at(i).player->IdleUp(speed);
			break;
		case idle_left:
			players.at(i).player->IdleLeft(speed);
			break;
		case idle_down:
			players.at(i).player->IdleDown(speed);
			break;
		case idle_right:
			players.at(i).player->IdleRight(speed);
			break;
		case basic_atack_up:
			break;
		case basic_atack_left:
			break;
		case basic_atack_down:
			break;
		case basic_atack_right:
			break;
		case states_null:
			break;
		default:
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
	return true;
}

void PlayerManager::AddPlayer(entity_name name, int index)
{
	Player p(App->entity->CreateEntity(name), index-1);
	p.player->SetCamera(p.index+1);
	players.push_back(p);
}

void PlayerManager::ChangePlayer(entity_name name, int index)
{
	for(vector<Player>::iterator it = players.begin(); it != players.end(); it++)
	{
		if ((*it).index == index - 1)
		{
			App->entity->DeleteEntity((*it).player);
			players.erase(it);
			break;
		}
	}

	Player p(App->entity->CreateEntity(name), index - 1);
	p.player->SetCamera(p.index + 1);
	players.push_back(p);
}
