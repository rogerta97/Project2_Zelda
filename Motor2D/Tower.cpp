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
#include "TowerManager.h"
#include "MinionManager.h"
#include "MainScene.h"
#include "j1Scene.h"
#include "Minion.h"
#include "j1Spell.h"
#include "Spell.h"
#include "TowerAttack.h"

#define TOWER_H 64
#define TOWER_W 64

Tower::Tower(iPoint pos) 
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(TOWER_H, TOWER_W), App->cf->CATEGORY_SCENERY, App->cf->MASK_SCENERY, pbody_type::p_t_tower, 0);
	
	game_object->CreateCollision(iPoint(0, 0), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_hit_box);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetFixedRotation(true);
	game_object->SetKinematic();

	AddAbility(0, 50, 2.5f, 2, "t_attack");

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

	game_object->SetAnimation("tower_idle");

	stats.max_life = stats.life = 400;

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
		Idle();
		target = nullptr;
		if (LookForTarget())
		{
			state = Tower_Attack;
		}
		break;
	case Tower_Attack:
		if (target != nullptr && !target->to_delete)
		{
			if (GetPos().DistanceTo(target->GetPos()) > attack_range)
			{
				state = Tower_Idle;
			}
			else
			{
				DoAttack();
			}
		}
		else
		{
			state = Tower_Idle;
		}
		break;
	default:
		break;
	}

	LifeBar(iPoint(64, 4), iPoint(-32, -92));

	Entity* entity = nullptr;
	Ability* ability = nullptr;
	Spell* spell = nullptr;
	if (GotHit(entity, ability, spell))
	{
		if (entity->GetTeam() != GetTeam())
		{
			stats.life -= ability->damage;
			if (stats.life <= 0)
			{
				App->scene->main_scene->tower_manager->KillTower(this);
			}
		}
	}

	return ret;
}

bool Tower::Draw(float dt)
{
	bool ret = true;

	App->view->LayerBlit(2, game_object->GetTexture(), { game_object->GetPos().x -32, game_object->GetPos().y -96}, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_NONE);

	if (App->debug_mode)
		App->view->LayerDrawCircle(game_object->GetPos().x, game_object->GetPos().y, attack_range, 255, 0, 0);

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

iPoint Tower::GetPos() const
{
	return game_object->GetPos();
}

void Tower::Idle()
{	
	game_object->SetAnimation("tower_idle");
}

void Tower::OnColl(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
}

void Tower::DoAttack()
{
	if (abilities.at(0)->CdCompleted())
	{
		game_object->SetAnimation("tower_attack");
		anim_state = tower_attack;

		TowerAttack* ta = (TowerAttack*)App->spell->CreateSpell(t_attack, { game_object->GetPos().x, game_object->GetPos().y - 70 }, this);
		ta->SetTarget(target);
		abilities.at(0)->cd_timer.Start();
	}
}

bool Tower::LookForTarget()
{
	bool ret = false;

	//check for minions
	std::list<Minion*> minions;

	if (GetTeam() == 1)
		minions = App->scene->main_scene->minion_manager->GetMinionList(2);
	else
		minions = App->scene->main_scene->minion_manager->GetMinionList(1);

	for (std::list<Minion*>::iterator it = minions.begin(); it != minions.end(); it++)
	{
		if (GetPos().DistanceTo((*it)->GetPos()) < attack_range)
		{
			target = *it;
			ret = true;
			break;
		}
	}

	//check for players
	if (target == nullptr)
	{
		std::vector<Entity*> players;

		if (GetTeam() == 1)
			players = App->entity->player_manager->GetTeamPlayers(2);
		else
			players = App->entity->player_manager->GetTeamPlayers(1);

		for (std::vector<Entity*>::iterator it = players.begin(); it != players.end(); it++)
		{
			if (GetPos().DistanceTo((*it)->GetPos()) < attack_range)
			{
				target = *it;
				ret = true;
				break;
			}
		}
	}
	
	return ret;
}

