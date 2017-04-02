#include "Zelda.h"
#include "GameObject.h"
#include "j1Viewports.h"
#include "j1Map.h"
#include "p2Log.h"
#include "PlayerManager.h"
#include <algorithm>

Zelda::Zelda(iPoint pos)
{
	pugi::xml_document doc;
	App->LoadXML("zelda.xml", doc);

	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(30, 40), App->cf->CATEGORY_PAYLOAD_AREA, App->cf->MASK_PAYLOAD_AREA, pbody_type::p_t_npc, 0);

	radius = doc.child("file").child("radius").attribute("value").as_int(0);
	activation_time = doc.child("file").child("activation_time").attribute("sec").as_uint(0);

	game_object->CreateCollisionSensor(iPoint(0, 0), radius, fixture_type::f_t_zelda_area);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetFixedRotation(true);

	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));
	game_object->SetAnimation("idle_down");

	area_image = { doc.child("file").child("area").attribute("x").as_int(0),doc.child("file").child("area").attribute("y").as_int(0),doc.child("file").child("area").attribute("w").as_int(0),doc.child("file").child("area").attribute("h").as_int(0) };

	stats.speed = doc.child("file").child("speed").attribute("value").as_int(10);

	state = z_s_wait;

	App->UnloadXML(doc);
}

Zelda::~Zelda()
{
}

bool Zelda::Start()
{
	return true;
}

bool Zelda::PreUpdate()
{
	return true;
}

bool Zelda::Update(float dt)
{
	CheckState();

	speed = stats.speed*dt;

	switch (state)
	{
	case z_s_wait:
		break;
	case z_s_move_to_path:
		MoveToPathState();
		break;
	case z_s_idle:
		SetIdle();
		break;
	case z_s_move:
		MoveState();
		break;
	case z_s_end_game:
		SetIdle();
		App->scene->main_scene->EndGame((path_pos == 0) ? 1 : 2);
		state = z_s_null;
		break;
	case z_s_null:
		break;
	default:
		break;
	}

	return true;
}

bool Zelda::Draw(float dt)
{
	App->view->LayerBlit(game_object->GetPos().y, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x - 22, game_object->GetPos().y - draw_offset.y - 24 }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_NONE);
	
	if(active)
		App->view->LayerBlit(4, game_object->GetTexture(), { game_object->GetPos().x - radius, game_object->GetPos().y - radius}, area_image, 0, -1.0f, true, SDL_FLIP_NONE);

	return true;
}

bool Zelda::PostUpdate()
{
	return true;
}

bool Zelda::CleanUp()
{
	return true;
}

bool Zelda::SetTimer(j1Timer * timer)
{
	if (game_timer == nullptr)
	{
		game_timer = timer;
		return true;
	}
	return false;
}

void Zelda::SetInitialPath(std::vector<iPoint>& path)
{
	initial_path = path;
}

void Zelda::SetPath(std::vector<iPoint>& path)
{
	main_path = path;
}

void Zelda::OnCollEnter(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
	if (bodyA == game_object->pbody && fixtureA->type == fixture_type::f_t_zelda_area)
	{
		if (bodyB->type == pbody_type::p_t_player && fixtureB->type == fixture_type::f_t_hit_box)
		{
			int team = App->scene->main_scene->player_manager->GetPlayerTeamFromBody(bodyB);

			Player* contact_palyer = App->scene->main_scene->player_manager->GetPlayerFromBody(bodyB);

			bool found = false;

			for (std::vector<Player*>::iterator it = counted_players.begin(); it != counted_players.end(); it++)
			{
				if (*it == contact_palyer)
				{
					found = true;
					break;
				}

			}
			if (!found)
			{
				switch (team)
				{
				case 1:
					team1_players++;
					break;
				case 2:
					team2_players++;
					break;
				default:
					break;
				}
				counted_players.push_back(contact_palyer);
			}
		}
	}
}

void Zelda::OnCollOut(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
	if (bodyA == game_object->pbody && fixtureA->type == fixture_type::f_t_zelda_area)
	{
		if (bodyB->type == pbody_type::p_t_player && fixtureB->type == fixture_type::f_t_hit_box)
		{
			int team = App->scene->main_scene->player_manager->GetPlayerTeamFromBody(bodyB);

			Player* contact_palyer = App->scene->main_scene->player_manager->GetPlayerFromBody(bodyB);

			bool found = false;

			for (std::vector<Player*>::iterator it = counted_players.begin(); it != counted_players.end(); it++)
			{
				if (*it == contact_palyer)
				{
					found = true;
					counted_players.erase(it);
					break;
				}

			}
			if (found)
			{
				switch (team)
				{
				case 1:
					team1_players--;
					break;
				case 2:
					team2_players--;
					break;
				default:
					break;
				}
			}
		}
	}
}

iPoint Zelda::GetPos() const
{
	return game_object->GetPos();
}

void Zelda::MoveUp(float speed)
{
	game_object->SetPos({ game_object->fGetPos().x, game_object->fGetPos().y - speed });
}

void Zelda::MoveDown(float speed)
{
	game_object->SetPos({ game_object->fGetPos().x, game_object->fGetPos().y + speed });
}

void Zelda::MoveLeft(float speed)
{
	game_object->SetPos({ game_object->fGetPos().x - speed, game_object->fGetPos().y });
}

void Zelda::MoveRight(float speed)
{
	game_object->SetPos({ game_object->fGetPos().x + speed, game_object->fGetPos().y });
}

void Zelda::MoveUpRight(float speed)
{
	fPoint s(speed * cos(DEGTORAD * 45), speed * sin(DEGTORAD * 45));
	game_object->SetPos({ game_object->fGetPos().x + s.x, game_object->fGetPos().y - s.y });
}

void Zelda::MoveDownRight(float speed)
{
	fPoint s(speed * cos(DEGTORAD * 45), speed * sin(DEGTORAD * 45));
	game_object->SetPos({ game_object->fGetPos().x + s.x, game_object->fGetPos().y + s.y });
}

void Zelda::MoveUpLeft(float speed)
{
	fPoint s(speed * cos(DEGTORAD * 45), speed * sin(DEGTORAD * 45));
	game_object->SetPos({ game_object->fGetPos().x - s.x, game_object->fGetPos().y - s.y });
}

void Zelda::MoveDownLeft(float speed)
{
	fPoint s(speed * cos(DEGTORAD * 45), speed * sin(DEGTORAD * 45));
	game_object->SetPos({ game_object->fGetPos().x - s.x, game_object->fGetPos().y + s.y });
}

void Zelda::WalkUp()
{
	game_object->SetAnimation("walk_up");
	anim_state = walk_up;
}

void Zelda::WalkRight()
{
	game_object->SetAnimation("walk_right");
	anim_state = walk_right;
}

void Zelda::WalkLeft()
{
	game_object->SetAnimation("walk_left");
	anim_state = walk_left;
}

void Zelda::WalkDown()
{
	game_object->SetAnimation("walk_down");
	anim_state = walk_down;
}

void Zelda::CheckState()
{
	switch (state)
	{
	case z_s_wait:
		if (game_timer->ReadSec() > activation_time)
			state = z_s_move_to_path;
		break;
	case z_s_move_to_path:
		if (initial_path_pos == initial_path.size())
		{
			state = z_s_idle;
			active = true;
			path_pos = main_path.size() / 2;
		}
		break;
	case z_s_idle:
		if ((team1_players != 0 && team2_players == 0) || (team2_players != 0 && team1_players == 0))
		{
			state = z_s_move;

			bool change_dir = false;

			if (team1_players != 0)
			{
				if (direction != -1)
				{
					direction = -1;
					change_dir = true;
				}
			}
			else
			{
				if (direction != 1)
				{
					direction = 11;
					change_dir = true;
				}
			}

			if(change_dir)
				path_pos += direction;
		}
		break;
	case z_s_move:
		if ((team1_players == 0 && team2_players == 0) || (team1_players != 0 && team2_players != 0))
		{
			state = z_s_idle;
		}
		else if (path_pos == -1 || path_pos == main_path.size())
		{
			state = z_s_end_game;
			active = false;
		}
		break;
	case z_s_null:
		break;
	default:
		break;
	}
}

void Zelda::MoveState()
{
	iPoint map_pos = App->map->WorldToMap(GetPos().x, GetPos().y);

	if (map_pos == main_path[path_pos] && path_pos < main_path.size() && path_pos >= 0)
	{
		path_pos += direction;
	}

	if (path_pos < main_path.size() && path_pos >= 0)
	{
		iPoint target_pos = App->map->MapToWorld(main_path[path_pos].x, main_path[path_pos].y);
		target_pos.x += 16;
		target_pos.y += 16;

		Move(target_pos.x - GetPos().x, target_pos.y - GetPos().y);
	}
}

void Zelda::MoveToPathState()
{
	iPoint map_pos = App->map->WorldToMap(GetPos().x, GetPos().y);

	if (map_pos == initial_path[initial_path_pos] && initial_path_pos < initial_path.size())
	{
		initial_path_pos++;
	}

	if (initial_path_pos < initial_path.size())
	{
		iPoint target_pos = App->map->MapToWorld(initial_path[initial_path_pos].x, initial_path[initial_path_pos].y);
		target_pos.x += 16;
		target_pos.y += 16;

		Move(target_pos.x - GetPos().x, target_pos.y - GetPos().y);
	}

}

void Zelda::SetIdle()
{
	switch (anim_state)
	{
	case walk_up:
		game_object->SetAnimation("idle_up");
		anim_state = idle_up;
		break;
	case walk_left:
		game_object->SetAnimation("idle_left");
		anim_state = idle_left;
		break;
	case walk_down:
		game_object->SetAnimation("idle_down");
		anim_state = idle_down;
		break;
	case walk_right:
		game_object->SetAnimation("idle_right");
		anim_state = idle_right;
		break;
	default:
		break;
	}
}

void Zelda::Move(int delta_x, int delta_y)
{
	if (delta_x > 0)
	{
		if (delta_y > 0)
		{
			MoveDownRight(speed);
			WalkRight();
		}
		else if (delta_y < 0)
		{
			MoveUpRight(speed);
			WalkRight();
		}
		else {
			MoveRight(speed);
			WalkRight();
		}
	}
	else if (delta_x < 0)
	{
		if (delta_y > 0)
		{
			MoveDownLeft(speed);
			WalkLeft();
		}
		else if (delta_y < 0)
		{
			MoveUpLeft(speed);
			WalkLeft();
		}
		else {
			MoveLeft(speed);
			WalkLeft();
		}
	}
	else
	{
		if (delta_y > 0)
		{
			MoveDown(speed);
			WalkDown();
		}
		else if (delta_y < 0)
		{
			MoveUp(speed);
			WalkUp();
		}
	}
}
