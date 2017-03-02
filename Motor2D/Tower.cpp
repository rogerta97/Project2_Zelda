#include "Tower.h"
#include "GameObject.h"
#include "j1Viewports.h"
#include "CollisionFilters.h"
#include "j1App.h"
#include "j1Map.h"
#include "PlayerManager.h"
#include "Entity.h"
#include "j1Entity.h"
#include <vector>
#include "p2Log.h"

#define TOWER_H 100
#define TOWER_W 100

Tower::Tower(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(TOWER_H, TOWER_W), App->cf->CATEGORY_SCENERY, App->cf->MASK_SCENERY, pbody_type::p_t_tower, 0);

	game_object->CreateCollision(iPoint(0, 0), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_hit_box);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetFixedRotation(true);

	pugi::xml_document doc;
	App->LoadXML("tower.xml", doc);
	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));
}

Tower::~Tower()
{
	RELEASE(game_object);
}

bool Tower::Start()
{
	bool ret = true;

	game_object->SetAnimation("idle");

	stats.max_life = stats.life = 300;

	show_life_bar = true;

	return ret;
}

bool Tower::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool Tower::Update(float dt)
{
	bool ret = true;


	switch (state)
	{
	case Tower_Idle:
		TowerIdle();
		break;
	case Tower_Attack:
		TowerAttack();
		break;
	default:
		break;
	}

	LifeBar(iPoint(20, 3), iPoint(0, 0));

	Entity* entity = nullptr;
	Ability* ability = nullptr;
	if (GotHit(entity, ability))
	{
		if (entity->GetTeam() != GetTeam())
		{
			stats.life -= ability->damage;
			if (stats.life < 0)
			{
				//tower destroyed
			}
		}
	}

	return ret;
}

bool Tower::Draw(float dt)
{
	bool ret = true;

	App->view->LayerBlit(2, game_object->GetTexture(), { game_object->GetPos().x - 17, game_object->GetPos().y - 23 }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_NONE);

	return ret;
}

bool Tower::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool Tower::CleanUp()
{
	bool ret = true;

	return ret;
}

void Tower::Idle()
{
	game_object->SetAnimation("idle");
	flip = false;
	anim_state = tower_idle;
}

void Tower::OnColl(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
	switch (bodyA->type)
	{
	case pbody_type::p_t_tower:
		if (fixtureB->type == fixture_type::f_t_hit_box)
		{
			if (bodyB->type == pbody_type::p_t_link)
			{
			}
			else if (bodyB->type == pbody_type::p_t_npc)
			{
			}
		}
		break;

	}
}

iPoint Tower::GetPos() const
{
	return game_object->GetPos();
}

void Tower::TowerIdle()
{
	CheckTowerState();

	switch (anim_state)
	{
	case tower_idle:
		Idle();
		break;
	case tower_attack:
		Idle();
		break;
	default:
		break;
	}
}

void Tower::TowerAttack()
{
	CheckTowerState();
}

void Tower::CheckTowerState()
{
	switch (state)
	{
	case Tower_Idle:
		if (false)
		{

		}
		break;

	case Tower_Attack:
		if (game_object->animator->IsCurrentAnimation("tower_attack"))
		{
			if (game_object->animator->GetCurrentAnimation()->Finished())
			{
				if (GetPos().DistanceTo(target->GetPos()) > attack_range)
				{
					state = Tower_Idle;
				}
			}
		}
		else if (target->to_delete == true)
		{
			state = Tower_Idle;
		}
		break;
	default:
		break;
	}
}

