#include "Snakes.h"
#include "GameObject.h"
#include "j1Viewports.h"
#include "CollisionFilters.h"
#include "j1App.h"
#include "j1Map.h"
#include "PlayerManager.h"
#include "JungleCampManager.h"
#include "Entity.h"
#include "j1Entity.h"
#include <vector>
#include "p2Log.h"
#include "MainScene.h"
#include "j1Scene.h"
#include "j1Spell.h"
#include "Spell.h"
#include "SnakePoison.h"

#define SNAKE_H 32
#define SNAKE_W 32

Snakes::Snakes(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(SNAKE_H, SNAKE_W), App->cf->CATEGORY_SCENERY, App->cf->MASK_SCENERY, pbody_type::p_t_npc, 0);

	game_object->CreateCollision(iPoint(0, 0), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_hit_box);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetFixedRotation(true);
	game_object->SetKinematic();

	AddAbility(0, 10, 2.5f, 2, "s_attack");

	pugi::xml_document doc;
	App->LoadXML("snakes.xml", doc);
	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));
}

Snakes::~Snakes()
{
	RELEASE(game_object);
}

bool Snakes::Start()
{
	bool ret = true;

	stats.max_life = stats.life = 40;

	show_life_bar = true;

	return ret;
}

bool Snakes::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool Snakes::Update(float dt)
{
	bool ret = true;

	LifeBar(iPoint(64, 4), iPoint(-32, -32));


	CheckState();

	switch (state)
	{
	case Snk_S_Null:
		Idle();
		break;
	case Snk_S_Idle:
		Idle();
		break;
	case Snk_S_Attack:
		if (rel_angle >= 315 && rel_angle <= 45)
		{
			AttackRight();
			break;
		}
		else if (rel_angle < 45 && rel_angle >= 135)
		{
			AttackUp();
			break;
		}
		else if (rel_angle > 135 && rel_angle <= 225)
		{
			AttackLeft();
			break;
		}
		else
		{
			AttackDown();
			break;
		}
			
	default:
		break;
	}

	return ret;
}

bool Snakes::Draw(float dt)
{
	bool ret = true;

	App->view->LayerBlit(2, game_object->GetTexture(), { game_object->GetPos().x - 14 , game_object->GetPos().y - 20 }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_NONE);

	if (App->debug_mode)
		App->view->LayerDrawCircle(game_object->GetPos().x, game_object->GetPos().y, attack_range, 255, 0, 0);

	return ret;
}

bool Snakes::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool Snakes::CleanUp()
{
	bool ret = true;

	return ret;
}

iPoint Snakes::GetPos() const
{
	return game_object->GetPos();
}

void Snakes::CheckState()
{
	switch (state)
	{
	case Snk_S_Null:
		state = Snk_S_Idle;
		break;
	case Snk_S_Idle:
	{
		Entity* entity = nullptr;
		Ability* ability = nullptr;
		Spell* spell = nullptr;
		if (GotHit(entity, ability, spell))
		{
			// Enemy attacks
			if (entity != nullptr && ability != nullptr)
			{
				DealDamage(ability->damage * ability->damage_multiplicator);

				if (spell != nullptr && TextCmp(spell->name.c_str(), "boomerang"))
				{
					DealDamage(ability->damage * (spell->stats.damage_multiplicator - 1)); // Spells control their own damage mutiplicator

					if (spell->stats.slow_duration > 0)
						Slow(spell->stats.slow_multiplicator, spell->stats.slow_duration);
					if (spell->stats.stun_duration > 0)
						Stun(spell->stats.stun_duration);
				}

				is_attacked = true;
				state = Snk_S_Attack;
				targets.push_back(entity);

				rel_angle = AngleFromTwoPoints(game_object->fGetPos().x, game_object->fGetPos().y, targets.at(0)->GetPos().x, targets.at(0)->GetPos().y);

			}
			if (stats.life <= 0)
					App->scene->main_scene->jungleCamp_manager->KillJungleCamp(this);
			break;
		}
	}
	case Snk_S_Attack:
		for (std::vector<Entity*>::iterator it = targets.begin(); it != targets.end();)
		{
			if ((*it)->to_delete == true || GetPos().DistanceTo((*it)->GetPos()) < 150)
			{
				it = targets.erase(targets.begin());
			}
		}

		break;
	default:
		break;
	}
}

void Snakes::OnCollEnter(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
}

void Snakes::Idle()
{
	game_object->SetAnimation("snake_down");
	flip = false;
	anim_state = snake_down;
}

void Snakes::DoAttack()
{
	if (abilities.at(0)->CdCompleted())
	{
		SnakePoison* sp = (SnakePoison*)App->spell->CreateSpell(s_attack, { game_object->GetPos().x, game_object->GetPos().y - 30 }, this);
		sp->SetTarget(targets[0]);
		abilities.at(0)->cd_timer.Start();
	}
}

void Snakes::AttackLeft()
{
	
	game_object->SetAnimation("snake_lateral");
	flip = false;
	anim_state = snake_attack_lateral;
	DoAttack();
}

void Snakes::AttackRight()
{
	game_object->SetAnimation("snake_lateral");
	flip = true;
	anim_state = snake_attack_lateral;
	DoAttack();
}

void Snakes::AttackUp()
{
	game_object->SetAnimation("snake_up");
	flip = false;
	anim_state = snake_attack_up;
	DoAttack();
}

void Snakes::AttackDown()
{
	game_object->SetAnimation("snake_attack_down");
	flip = false;
	anim_state = snake_attack_down;
	DoAttack();
}

/*bool Snakes::LookForTarget()
{
	bool ret = false;

	std::vector<Entity*> players;

	players = App->entity->player_manager->GetAllPlayers();

	for (std::vector<Entity*>::iterator it = players.begin(); it != players.end(); it++)
	{
		if (GetPos().DistanceTo((*it)->GetPos()) < attack_range)
		{
			target = *it;
			ret = true;
			break;
		}
	}

	return ret;
}*/

