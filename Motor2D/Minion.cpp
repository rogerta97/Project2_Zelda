#include "Minion.h"
#include "GameObject.h"
#include "j1Viewports.h"
#include "CollisionFilters.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Pathfinding.h"
#include "PlayerManager.h"
#include "j1Entity.h"
#include <vector>
#include "p2Log.h"

#define Half_Tile 16

Minion::Minion(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_npc, 0);

	game_object->CreateCollision(iPoint(0, 0), 30, 40, fixture_type::f_t_null);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetFixedRotation(true);

	pugi::xml_document doc;
	App->LoadXML("minion.xml", doc);
	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));
}

Minion::~Minion()
{
	RELEASE(game_object);
}

bool Minion::Start()
{
	bool ret = true;

	game_object->SetAnimation("idle_down");

	stats.speed = 150;

	return ret;
}

bool Minion::PreUpdate()
{
	bool ret = true;



	return ret;
}

bool Minion::Update(float dt)
{
	bool ret = true;

	speed = stats.speed*dt;

	switch (state)
	{
	case Minion_Idle:
		CheckState();
		break;
	case Minion_Move:
		MinionMove();
		break;
	case Minion_Attack:
		MinionAttack();
		break;
	default:
		break;
	}

	iPoint pos = App->map->WorldToMap(GetPos().x, GetPos().y);

	//LOG("min pos: %d %d", pos.x, pos.y);

	return ret;
}

bool Minion::Draw(float dt)
{
	bool ret = true;

	if (flip)
		App->view->LayerBlit(2, game_object->GetTexture(), { game_object->GetPos().x - 20, game_object->GetPos().y - 23 }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_HORIZONTAL);
	else
		App->view->LayerBlit(2, game_object->GetTexture(), { game_object->GetPos().x - 17, game_object->GetPos().y - 23 }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_NONE);

	return ret;
}

bool Minion::PostUpdate()
{
	bool ret = true;



	return ret;
}

bool Minion::CleanUp()
{
	bool ret = true;



	return ret;
}

void Minion::MoveUp(float speed)
{
	game_object->SetPos({ game_object->fGetPos().x, game_object->fGetPos().y - speed });
}

void Minion::MoveDown(float speed)
{
	game_object->SetPos({ game_object->fGetPos().x, game_object->fGetPos().y + speed });
}

void Minion::MoveLeft(float speed)
{
	game_object->SetPos({ game_object->fGetPos().x - speed, game_object->fGetPos().y });
}

void Minion::MoveRight(float speed)
{
	game_object->SetPos({ game_object->fGetPos().x + speed, game_object->fGetPos().y });
}

void Minion::MoveUpRight(float speed)
{
	fPoint s(speed * cos(45), speed * sin(45));
	game_object->SetPos({ game_object->fGetPos().x + s.x, game_object->fGetPos().y - s.y });
}

void Minion::MoveDownRight(float speed)
{
	fPoint s(speed * cos(45), speed * sin(45));
	game_object->SetPos({ game_object->fGetPos().x + s.x, game_object->fGetPos().y + s.y });
}

void Minion::MoveUpLeft(float speed)
{
	fPoint s(speed * cos(45), speed * sin(45));
	game_object->SetPos({ game_object->fGetPos().x - s.x, game_object->fGetPos().y - s.y });
}

void Minion::MoveDownLeft(float speed)
{
	fPoint s(speed * cos(45), speed * sin(45));
	game_object->SetPos({ game_object->fGetPos().x - s.x, game_object->fGetPos().y + s.y });
}

void Minion::RunUp()
{
	game_object->SetAnimation("run_up");
	flip = false;
}

void Minion::RunDown()
{
	game_object->SetAnimation("run_down");
	flip = false;
}

void Minion::RunLeft()
{
	game_object->SetAnimation("run_lateral");
	flip = true;
}

void Minion::RunRight()
{
	game_object->SetAnimation("run_lateral");
	flip = false;
}

void Minion::IdleUp()
{
	game_object->SetAnimation("idle_up");
	flip = false;
}

void Minion::IdleDown()
{
	game_object->SetAnimation("idle_down");
	flip = false;
}

void Minion::IdleLeft()
{
	game_object->SetAnimation("idle_lateral");
	flip = true;
}

void Minion::IdleRight()
{
	game_object->SetAnimation("idle_lateral");
	flip = false;
}

void Minion::OnColl(PhysBody* bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
	switch (bodyA->type)
	{
	case pbody_type::p_t_npc:
		if (bodyB->type == pbody_type::p_t_world)
		{
		}
		break;

	}
}

iPoint Minion::GetPos() const
{
	return game_object->GetPos();
}

void Minion::SetBasePath(std::list<iPoint>& path)
{
	for (std::list<iPoint>::iterator it = path.begin(); it != path.end(); it++)
	{
		base_path.push_back(*it);
	}
}

bool Minion::MinionMove()
{
	bool ret = true;

	CheckState();

	iPoint map_pos = App->map->WorldToMap(GetPos().x + Half_Tile, GetPos().y + Half_Tile);

	switch (move_state)
	{
	case Move_FollowBasePath:
	{
		if (base_path_index < base_path.size()-1)
		{
			if (GetPos().DistanceTo(App->map->MapToWorld(base_path.at(base_path_index).x, base_path.at(base_path_index).y)) < 18)
				base_path_index++;
		}
		else
			move_state = Move_Idle;

		iPoint target_pos = App->map->MapToWorld(base_path.at(base_path_index).x, base_path.at(base_path_index).y);
		target_pos.y += Half_Tile;
		target_pos.x += Half_Tile;

		Move(target_pos.x - GetPos().x, target_pos.y - GetPos().y);

		break; 
	}
	case Move_AproachTarget:
	{
		if (target_path_index < target_path.size()-1)
			if (map_pos.DistanceTo(target_path.at(target_path_index)) < 18)
				target_path_index++;

		iPoint target_pos = App->map->MapToWorld(target_path.at(target_path_index).x, target_path.at(target_path_index).y);
		target_pos.y += Half_Tile;
		target_pos.x += Half_Tile;

		Move(target_pos.x - GetPos().x, target_pos.y - GetPos().y);

		break;
	}
	case Move_ReturnToPath:
	{
		if (target_path_index < target_path.size())
			if (map_pos.DistanceTo(target_path.at(target_path_index)) < 5)
				target_path_index++;

		iPoint target_pos = App->map->MapToWorld(target_path.at(target_path_index).x, target_path.at(target_path_index).y);
		target_pos.y += Half_Tile;
		target_pos.x += Half_Tile;

		Move(target_pos.x - GetPos().x, target_pos.y - GetPos().y);

		break;
	}
	default:
		break;
	}

	return ret;
}

bool Minion::MinionAttack()
{
	return false;
}

void Minion::CheckState()
{
	switch (state)
	{
	case Minion_Idle:
		state = Minion_Move;
		break;
	case Minion_Move:
	{
		switch (move_state)
		{
		case Move_FollowBasePath:
			if (LookForTarget())
				PathToTarget();
			break;
		case Move_AproachTarget:
			if (GetPos().DistanceTo(target->GetPos()) < attack_range - attack_range / 3)
				state = Minion_Attack;
			else
			{
				if (game_object->GetPos().DistanceTo(target->GetPos()) < vision_range && GetPos().DistanceTo(base_path.at(base_path_index))<vision_range)
				{
					if (App->map->WorldToMap(target->GetPos().x, target->GetPos().y) != *target_path.end())
					{
						PathToTarget();
					}
				}
				else
				{
					target_path_index = 0;
					target_path.clear();
					target = nullptr;
					move_state = Move_ReturnToPath;
					PathToBasePath();
				}
			}
			break;
		case Move_ReturnToPath:
			if (LookForTarget())
				PathToTarget();
			if (base_path_index < base_path.size()) {
				iPoint map_pos = App->map->WorldToMap(GetPos().x - Half_Tile, GetPos().y - Half_Tile);
				if (map_pos.DistanceTo(base_path.at(base_path_index)) < 5)
					move_state = Move_FollowBasePath;
			}
			break;
		default:
			break;
		}
		break; 
	}
	case Minion_Attack:
		move_state = Move_ReturnToPath;
		state = Minion_Move;
		PathToBasePath();
		break;
	default:
		break;
	}
}

void Minion::SetTargetPath(const std::list<iPoint>* path)
{
	for (std::list<iPoint>::const_iterator it = path->begin(); it != path->end(); it++)
	{
		target_path.push_back(*it);
	}
}

void Minion::PathToTarget()
{
	App->pathfinding->CreatePath(GetPos(), target->GetPos());
	SetTargetPath(App->pathfinding->GetLastPath());
	target_path_index = 0;
	move_state = Move_AproachTarget;
}

void Minion::PathToBasePath()
{
	App->pathfinding->CreatePath(GetPos(), base_path.at(base_path_index));
	SetTargetPath(App->pathfinding->GetLastPath());
	target_path_index = 0;
	move_state = Move_ReturnToPath;
}

bool Minion::LookForTarget()
{
	bool ret = false;

	//now only check for players
	std::vector<Entity*> players;
	if (GetTeam() == 1)
		players = App->entity->player_manager->GetTeamPlayers(2);
	else
		players = App->entity->player_manager->GetTeamPlayers(1);

	for (std::vector<Entity*>::iterator it = players.begin(); it != players.end(); it++)
	{
		if (GetPos().DistanceTo((*it)->GetPos()) < vision_range)
		{
			target = *it;
			ret = true;
		}
	}

	return ret;
}

void Minion::Move(int delta_x, int delta_y)
{
	if (delta_x > 0)
	{
		if (delta_y > 0)
		{
			MoveDownRight(speed);
			RunRight();
		}
		else if (delta_y < 0)
		{
			MoveUpRight(speed);
			RunRight();
		}
		else {
			MoveRight(speed);
			RunRight();
		}
	}
	else if (delta_x < 0)
	{
		if (delta_y > 0)
		{
			MoveDownLeft(speed);
			RunLeft();
		}
		else if (delta_y < 0)
		{
			MoveUpLeft(speed);
			RunLeft();
		}
		else {
			MoveLeft(speed);
			RunLeft();
		}
	}
	else
	{
		if (delta_y > 0)
		{
			MoveDown(speed);
			RunDown();
		}
		else if (delta_y < 0)
		{
			MoveUp(speed);
			RunUp();
		}
	}
}
