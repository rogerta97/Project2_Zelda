#include "Tower.h"
#include "GameObject.h"
#include "j1Viewports.h"
#include "CollisionFilters.h"
#include "j1App.h"
#include "j1Map.h"
#include "PlayerManager.h"
#include "Entity.h"
#include "j1Entity.h"
#include "p2Log.h"
#include "TowerManager.h"
#include "MinionManager.h"
#include "Minion.h"
#include "j1Spell.h"
#include "Spell.h"
#include "TowerAttack.h"
#include "Quest_Manager.h"
#include "j1XMLLoader.h"

#define TOWER_H 38
#define TOWER_W 64

#define FIRST_TARGET_WAIT 0.4

#define HALFMAP 81*32

Tower::Tower(iPoint pos) 
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(TOWER_W, TOWER_H), App->cf->CATEGORY_TOWER, App->cf->MASK_TOWER, pbody_type::p_t_tower, 0);
	
	game_object->CreateCollision(iPoint(0, 10), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_hit_box);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetListener((j1Module*)App->spell);
	game_object->SetFixedRotation(true);
	game_object->SetKinematic();

	pugi::xml_document doc;
	App->xml->LoadXML("tower.xml", doc);
	pugi::xml_node stats_node = doc.child("file").child("stats");

	stats.life = stats.base_hp = stats.max_life = stats_node.attribute("hp").as_int();;
	stats.base_power = stats.power = stats_node.attribute("power").as_int();
	rupee_reward = stats_node.attribute("rupees").as_int();

	float dmg_mult = stats_node.child("ability1").attribute("mult").as_float();
	float cd = stats_node.child("ability1").attribute("cd").as_float();
	int bd = stats_node.child("ability1").attribute("bd").as_int();
	AddAbility(0, cd, bd, dmg_mult, "t_attack");

	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));

	name = "tower";
}

Tower::~Tower()
{
	
}

bool Tower::Start()
{
	bool ret = true;

	show_life_bar = true;

	if (game_object->GetPos().x < HALFMAP)
	{
		game_object->SetAnimation("tower_idle");
	}
	else
	{
		game_object->SetAnimation("tower2_idle");
	}

	wait_firest_timer = App->AddGameplayTimer();

	shield_rect = { 192, 401, 16, 20 };
	shield = App->tex->LoadTexture("textures/towersminions_sheet.png");

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

	if (to_delete)
		return true;

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

	Entity* entity = nullptr;
	Ability* ability = nullptr;
	Spell* spell = nullptr;
	if (GotHit(entity, ability, spell))
	{
		// Enemy attacks
		if (entity != nullptr && ability != nullptr && entity->GetTeam() != GetTeam())
		{
			if (!invulnerable)
			{
				if (spell != nullptr)
				{
					DealDamage(((float)entity->stats.power * (float)spell->stats.damage_multiplicator) + (float)ability->damage); // Spells control their own damage mutiplicator
				}
				else
					DealDamage(((float)entity->stats.power * (float)ability->damage_multiplicator) + (float)ability->damage);
			}	

			Die(entity);
		}
	}

	return ret;
}

bool Tower::Draw(float dt)
{
	bool ret = true;

	LifeBar(iPoint(75, 6), iPoint(-36, -88));

	App->view->LayerBlit(GetPos().y, game_object->GetTexture(), { game_object->GetPos().x -32, game_object->GetPos().y -96}, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_NONE);

	if (invulnerable)
	{
		App->view->LayerBlit(GetPos().y, shield, { game_object->GetPos().x - 53, game_object->GetPos().y - 94 }, shield_rect, 0, -1.0f, true, SDL_FLIP_NONE);
	}
	if (target != nullptr && !target->to_delete)
	{
		App->view->LayerDrawLine(game_object->GetPos().x, game_object->GetPos().y-72, target->game_object->GetPos().x, target->game_object->GetPos().y, 240, 10, 10, 255, 2);
	}

	if (App->debug_mode)
		App->view->LayerDrawCircle(game_object->GetPos().x, game_object->GetPos().y, attack_range, 190, 0, 0);

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

	App->DeleteGameplayTimer(wait_firest_timer);

	return ret;
}

iPoint Tower::GetPos() const
{
	return game_object->GetPos();
}

void Tower::Die(Entity * killed_by)
{
	if (stats.life <= 0 && !to_delete && killed_by != nullptr)
	{
		App->entity->AddRupeesIfPlayer(killed_by, rupee_reward);
		App->scene->main_scene->tower_manager->KillTower(this);

		if (killed_by->is_player)
		{
			//Add kill to killer
			App->scene->players[App->scene->main_scene->player_manager->GetEntityViewportIfIsPlayer(killed_by) - 1].towers++;
		}
	}
}

void Tower::SetInvulnerableAnimation(uint team)
{
	switch (team)
	{
	case 1:
		game_object->SetAnimation("tower1_invulnerable");
		break;
	case 2:
		game_object->SetAnimation("tower2_invulnerable");
		break;
	default:
		break;
	}
}

void Tower::Idle()
{	
	if (game_object->GetPos().x < HALFMAP)
	{
		game_object->SetAnimation("tower_idle");

		if(invulnerable)
			game_object->SetAnimation("tower1_invulnerable");
	}
	else
	{
		game_object->SetAnimation("tower2_idle");
		if (invulnerable)
			game_object->SetAnimation("tower2_invulnerable");
	}
}

void Tower::OnColl(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
}

void Tower::DoAttack()
{
	if (abilities.at(0)->CdCompleted() && wait_firest_timer->ReadSec() > FIRST_TARGET_WAIT)
	{
		if (game_object->GetPos().x < HALFMAP)
		{
			game_object->SetAnimation("tower_attack");
		}
		else
		{
			game_object->SetAnimation("tower2_attack");
		}
		
		anim_state = tower_attack;

		TowerAttack* ta = (TowerAttack*)App->spell->CreateSpell(t_attack, { game_object->GetPos().x, game_object->GetPos().y - 70 }, this);
		ta->SetTarget(target);
		abilities.at(0)->cd_timer->Start();
		wait_first = false;
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
			players = App->scene->main_scene->player_manager->GetTeamPlayers(2);
		else
			players = App->scene->main_scene->player_manager->GetTeamPlayers(1);

		for (std::vector<Entity*>::iterator it = players.begin(); it != players.end(); it++)
		{
			if (GetPos().DistanceTo((*it)->GetPos()) < attack_range)
			{
				target = *it;
				ret = true;
				wait_first = true;
				wait_firest_timer->Start();
				break;
			}
		}
	}
	
	return ret;
}

