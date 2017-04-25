#include "Cuco.h"
#include "GameObject.h"
#include "j1Viewports.h"
#include "CollisionFilters.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Pathfinding.h"
#include "PlayerManager.h"
#include "j1Entity.h"
#include "Entity.h"
#include "p2Log.h"
#include "MainScene.h"
#include "Tower.h"
#include "TowerManager.h"
#include "Quest_Manager.h"
#include "j1XMLLoader.h"

#define Half_Tile 16

Cuco::Cuco(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(30, 40), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_npc, 0);

	game_object->CreateCollisionSensor(iPoint(0, 0), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_hit_box);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetFixedRotation(true);

	pugi::xml_document doc;
	App->xml->LoadXML("cucco.xml", doc);
	pugi::xml_node stats_node = doc.child("file").child("stats");

	stats.base_speed = stats.speed = stats.restore_speed = stats_node.attribute("speed").as_int();

	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));

	event_thrower = new EventThrower();

	int cuco_type_int = GetRandomValue(1, 6);
	switch (cuco_type_int)
	{
	case 1: 
		cuco_type = "basic_chicken";
		break;
	case 2:
		cuco_type = "cap_chicken";
		break;
	case 3:
		cuco_type = "black_chicken";
		break;
	case 4:
		cuco_type = "pink_chicken";
		break;
	case 5:
		cuco_type = "cook_chicken";
		break;
	case 6:
		cuco_type = "stone_chicken";
		break;
	default:
		break;
	}
	GetNewPath();
	name = "Cuco";
}

Cuco::~Cuco()
{

}

bool Cuco::Start()
{
	bool ret = true;

	game_object->SetAnimation("idle_down");

	show_life_bar = true;

	return ret;
}

bool Cuco::PreUpdate()
{
	bool ret = true;



	return ret;
}

bool Cuco::Update(float dt)
{
	bool ret = true;

	if (to_delete)
		return true;

	speed = stats.speed*dt;

	if (!stuned)
	{
		switch (state)
		{
		case Cuco_Idle:
			CucoIdle();
			break;
		case Cuco_Move:
			CucoMove();
			break;
		default:
			break;
		}
	}
	else
		SetIdleAnim();

	return ret;
}

bool Cuco::Draw(float dt)
{
	bool ret = true;

	if (flip)
		App->view->LayerBlit(GetPos().y, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x , game_object->GetPos().y - draw_offset.y - 19 }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_HORIZONTAL);
	else
		App->view->LayerBlit(GetPos().y, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x -20, game_object->GetPos().y - draw_offset.y - 19 }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_NONE);

	return ret;
}

bool Cuco::PostUpdate()
{
	bool ret = true;



	return ret;
}

bool Cuco::CleanUp()
{
	bool ret = true;

	RELEASE(event_thrower);

	return ret;
}

void Cuco::MoveUp(float speed)
{
	game_object->SetPos({ game_object->fGetPos().x, game_object->fGetPos().y - speed });
}

void Cuco::MoveDown(float speed)
{
	game_object->SetPos({ game_object->fGetPos().x, game_object->fGetPos().y + speed });
}

void Cuco::MoveLeft(float speed)
{
	game_object->SetPos({ game_object->fGetPos().x - speed, game_object->fGetPos().y });
}

void Cuco::MoveRight(float speed)
{
	game_object->SetPos({ game_object->fGetPos().x + speed, game_object->fGetPos().y });
}

void Cuco::MoveUpRight(float speed)
{
	fPoint s(speed * cos(DEGTORAD * 45), speed * sin(DEGTORAD * 45));
	game_object->SetPos({ game_object->fGetPos().x + s.x, game_object->fGetPos().y - s.y });
}

void Cuco::MoveDownRight(float speed)
{
	fPoint s(speed * cos(DEGTORAD * 45), speed * sin(DEGTORAD * 45));
	game_object->SetPos({ game_object->fGetPos().x + s.x, game_object->fGetPos().y + s.y });
}

void Cuco::MoveUpLeft(float speed)
{
	fPoint s(speed * cos(DEGTORAD * 45), speed * sin(DEGTORAD * 45));
	game_object->SetPos({ game_object->fGetPos().x - s.x, game_object->fGetPos().y - s.y });
}

void Cuco::MoveDownLeft(float speed)
{
	fPoint s(speed * cos(DEGTORAD * 45), speed * sin(DEGTORAD * 45));
	game_object->SetPos({ game_object->fGetPos().x - s.x, game_object->fGetPos().y + s.y });
}

void Cuco::RunUp()
{
	game_object->SetAnimation(cuco_type.c_str());
	flip = false;
	anim_state = run_up;
}

void Cuco::RunDown()
{
	game_object->SetAnimation(cuco_type.c_str());
	flip = false;
	anim_state = run_down;
}

void Cuco::RunLeft()
{
	game_object->SetAnimation(cuco_type.c_str());
	flip = true;
	anim_state = run_left;
	draw_offset.x = 8;
}

void Cuco::RunRight()
{
	game_object->SetAnimation(cuco_type.c_str());
	flip = false;
	anim_state = run_right;
	draw_offset.x = -8;
}

void Cuco::IdleUp()
{
	game_object->SetAnimation(cuco_type.c_str());
	flip = false;
	anim_state = idle_up;
}

void Cuco::IdleDown()
{
	game_object->SetAnimation(cuco_type.c_str());
	flip = false;
	anim_state = idle_down;
}

void Cuco::IdleLeft()
{
	game_object->SetAnimation(cuco_type.c_str());
	flip = true;
	anim_state = idle_left;
	draw_offset.x = 8;
}

void Cuco::IdleRight()
{
	game_object->SetAnimation(cuco_type.c_str());;
	flip = false;
	anim_state = idle_right;
	draw_offset.x = -8;
}

void Cuco::OnColl(PhysBody* bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
	switch (bodyA->type)
	{
	case pbody_type::p_t_npc:
		break;
	case pbody_type::p_t_player:
		if (bodyB == this->game_object->pbody && dead == false)
		{
			if (App->scene->main_scene->quest_manager->vquest[1]->state == active)
			{
				Entity* entity = App->entity->FindEntityByBody(bodyA);
				if (entity->is_player)
				{
					App->entity->DeleteEntity(this);
					App->scene->main_scene->quest_manager->add_progress(2, entity->GetTeam());
					dead = true;
				}
			}
		}
		//Delete
		break;
	default:
		break;
	}
}

iPoint Cuco::GetPos() const
{
	return game_object->GetPos();
}

void Cuco::SetBasePath(const std::list<iPoint>* path)
{
	for (std::list<iPoint>::const_iterator it = path->begin(); it != path->end(); it++)
	{
		base_path.push_back(*it);
	}
}

void Cuco::CucoIdle()
{
	CheckState();

	SetIdleAnim();
}

void Cuco::CucoMove()
{
	CheckState();
	draw_offset.SetToZero();

	iPoint map_pos = App->map->WorldToMap(GetPos().x, GetPos().y);

	switch (move_state)
	{
	case cMove_FollowBasePath:
	{
		if (base_path_index < base_path.size() - 1)
		{
			while(base_path.size() == 0)
			{
				GetNewPath();
			}
 			if (map_pos == base_path.at(base_path_index))
				base_path_index++;
		}
		else
		{
			state = Cuco_Idle;
			break;
		}

		iPoint target_pos = App->map->MapToWorld(base_path.at(base_path_index).x, base_path.at(base_path_index).y);
		target_pos.y += Half_Tile;
		target_pos.x += Half_Tile;

		Move(target_pos.x - GetPos().x, target_pos.y - GetPos().y);

		break;
	}
	default:
		break;
	}

}


void Cuco::CheckState()
{
	switch (state)
	{
	case Cuco_Idle:
		if (base_path_index < base_path.size() - 1)
		{
			state = Cuco_Move;
			move_state = cMove_FollowBasePath;
		}
		else {
			GetNewPath();
		}
		break;
	case Cuco_Move:
	{
		
		break;
	}
	default:
		break;
	}
}

void Cuco::GetNewPath()
{
	target = App->map->WorldToMap(GetRandomValue(GetPos().x - 320, GetPos().x + 320), GetRandomValue(GetPos().y - 320, GetPos().y + 320));
	while (!App->pathfinding->IsWalkable(target) && target!=GetPos())
	{
		target = App->map->WorldToMap(GetRandomValue(GetPos().x - 320, GetPos().x + 320), GetRandomValue(GetPos().y - 320, GetPos().y + 320));
	}
	if (App->pathfinding->CreatePath(App->map->WorldToMap(GetPos().x, GetPos().y), target) > 0)
	{
		base_path.clear();
		SetBasePath(App->pathfinding->GetLastPath());
		base_path_index = 0;
		move_state = cMove_FollowBasePath;
	}

}

void Cuco::Move(int delta_x, int delta_y)
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


void Cuco::SetIdleAnim()
{
	switch (anim_state)
	{
	case run_up:
	case idle_up:
	case basic_atack_up:
		IdleUp();
		break;
	case run_left:
	case idle_left:
	case basic_atack_left:
		IdleLeft();
		break;
	case run_down:
	case idle_down:
	case basic_atack_down:
		IdleDown();
		break;
	case run_right:
	case idle_right:
	case basic_atack_right:
		IdleRight();
		break;
	default:
		break;
	}
}