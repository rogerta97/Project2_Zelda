#include "Minion.h"
#include "GameObject.h"
#include "j1Viewports.h"
#include "CollisionFilters.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Pathfinding.h"

Minion::Minion(iPoint pos)
{
	minion_go = new GameObject(iPoint(pos.x, pos.y), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_npc, 0);

	minion_go->CreateCollision(iPoint(0, 0), 30, 40, fixture_type::f_t_null);
	minion_go->SetListener((j1Module*)App->entity);
	minion_go->SetFixedRotation(true);

	pugi::xml_document doc;
	App->LoadXML("minion.xml", doc);
	minion_go->SetTexture(minion_go->LoadAnimationsFromXML(doc, "animations"));
}

Minion::~Minion()
{
	RELEASE(minion_go);
}

bool Minion::Start()
{
	bool ret = true;

	minion_go->SetAnimation("idle_down");

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

	switch (state)
	{
	case Minion_Idle:
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

	return ret;
}

bool Minion::Draw(float dt)
{
	bool ret = true;

	if (flip)
		App->view->LayerBlit(2, minion_go->GetTexture(), { minion_go->GetPos().x - 20, minion_go->GetPos().y - 23 }, minion_go->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_HORIZONTAL);
	else
		App->view->LayerBlit(2, minion_go->GetTexture(), { minion_go->GetPos().x - 17, minion_go->GetPos().y - 23 }, minion_go->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_NONE);

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
	minion_go->SetPos({ minion_go->fGetPos().x, minion_go->fGetPos().y - speed });
}

void Minion::MoveDown(float speed)
{
	minion_go->SetPos({ minion_go->fGetPos().x, minion_go->fGetPos().y + speed });
}

void Minion::MoveLeft(float speed)
{
	minion_go->SetPos({ minion_go->fGetPos().x - speed, minion_go->fGetPos().y });
}

void Minion::MoveRight(float speed)
{
	minion_go->SetPos({ minion_go->fGetPos().x + speed, minion_go->fGetPos().y });
}

void Minion::MoveUpRight(float speed)
{
	fPoint s(speed * cos(45), speed * sin(45));
	minion_go->SetPos({ minion_go->fGetPos().x + s.x, minion_go->fGetPos().y - s.y });
}

void Minion::MoveDownRight(float speed)
{
	fPoint s(speed * cos(45), speed * sin(45));
	minion_go->SetPos({ minion_go->fGetPos().x + s.x, minion_go->fGetPos().y + s.y });
}

void Minion::MoveUpLeft(float speed)
{
	fPoint s(speed * cos(45), speed * sin(45));
	minion_go->SetPos({ minion_go->fGetPos().x - s.x, minion_go->fGetPos().y - s.y });
}

void Minion::MoveDownLeft(float speed)
{
	fPoint s(speed * cos(45), speed * sin(45));
	minion_go->SetPos({ minion_go->fGetPos().x - s.x, minion_go->fGetPos().y + s.y });
}

void Minion::RunUp()
{
	minion_go->SetAnimation("run_up");
	flip = false;
}

void Minion::RunDown()
{
	minion_go->SetAnimation("run_down");
	flip = false;
}

void Minion::RunLeft()
{
	minion_go->SetAnimation("run_lateral");
	flip = true;
}

void Minion::RunRight()
{
	minion_go->SetAnimation("run_lateral");
	flip = false;
}

void Minion::IdleUp()
{
	minion_go->SetAnimation("idle_up");
	flip = false;
}

void Minion::IdleDown()
{
	minion_go->SetAnimation("idle_down");
	flip = false;
}

void Minion::IdleLeft()
{
	minion_go->SetAnimation("idle_lateral");
	flip = true;
}

void Minion::IdleRight()
{
	minion_go->SetAnimation("idle_lateral");
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
	return minion_go->GetPos();
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

	switch (move_state)
	{
	case Move_FollowBasePath:
		break;
	case Move_AproachTarget:
		break;
	case Move_ReturnToPath:
		break;
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
	if (target == nullptr)
	{
		if (LookForTarget())
		{

		}
		else
		{
			move_state = Move_FollowBasePath;
		}
	}
	else
	{
		if (false) //check if player is under attack
		{
			//set target to player agressor
			PathToTarget();
		}
		else
		{
			if (minion_go->GetPos().DistanceTo(target->GetPos()) < vision_range)
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
				move_state = Move_ReturnToPath;
			}
		}
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

bool Minion::LookForTarget()
{
	return false;
}
