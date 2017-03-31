#include "Skeleton.h"
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


#define SKELETON_H 32
#define SKELETON_W 32



Skeleton::Skeleton(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(SKELETON_H, SKELETON_W), App->cf->CATEGORY_SCENERY, App->cf->MASK_SCENERY, pbody_type::p_t_tower, 0);

	game_object->CreateCollision(iPoint(0, 0), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_hit_box);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetFixedRotation(true);
	game_object->SetKinematic();

	//AddAbility(0, 50, 500, 500, "spin"); //times are ms.
	//AddAbility(1, 30, 500, 3000, "bone");

	pugi::xml_document doc;
	App->LoadXML("skeleton.xml", doc);
	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));
}

Skeleton::~Skeleton()
{
}

bool Skeleton::Start()
{
	bool ret = true;

	Idle();

	stats.max_life = stats.life = 250;

	show_life_bar = true;

	return ret;
}

bool Skeleton::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool Skeleton::Update(float dt)
{
	bool ret = true;

	LifeBar(iPoint(50, 4), iPoint(-20, -32));

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
			if (state == s_s_idle)
			{
				state = s_s_attack;
			}

		}
		if (stats.life <= 0)
			App->scene->main_scene->jungleCamp_manager->KillJungleCamp(this);
	}


	switch (state)
	{
	case s_s_null:
		Idle();
		break;
	case s_s_idle:
		Idle();
		break;
	case s_s_attack:
		break;
	default:
		break;
	}

	return ret;
}

bool Skeleton::Draw(float dt)
{
	bool ret = true;

	App->view->LayerBlit(2, game_object->GetTexture(), { game_object->GetPos().x - 14 , game_object->GetPos().y - 20 }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_NONE);

	if (App->debug_mode)
		App->view->LayerDrawCircle(game_object->GetPos().x, game_object->GetPos().y, ATTACK_RANGE, 255, 0, 0);

	return ret;
}

bool Skeleton::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool Skeleton::CleanUp()
{
	bool ret = true;

	return ret;
}

iPoint Skeleton::GetPos() const
{
	return game_object->GetPos();
}

void Skeleton::Idle()
{
	state = s_s_idle;
	game_object->SetAnimation("skeleton_idle");
	flip = false;
	anim_state = skeleton_idle;
}

void Skeleton::Stunned()
{
	state = s_s_stunned;
	game_object->SetAnimation("stunned");
	flip = false;
	anim_state = skeleton_stunned;
}

void Skeleton::Attack()
{
	

}

void Skeleton::SpinAttack()
{
	game_object->SetAnimation("spin");
	flip = false;
	anim_state = skeleton_spin;

	/*if (abilities.at(0)->CdCompleted())
	{
		SnakePoison* sp = (SnakePoison*)App->spell->CreateSpell(s_attack, { game_object->GetPos().x, game_object->GetPos().y - 30 }, this);
		sp->SetTarget(target);
		abilities.at(0)->cd_timer.Start();
	}*/
}

void Skeleton::BoneAttack()
{
	game_object->SetAnimation("bone");
	flip = false;
	anim_state = skeleton_bone;

	/*if (abilities.at(0)->CdCompleted())
	{
		SnakePoison* sp = (SnakePoison*)App->spell->CreateSpell(s_attack, { game_object->GetPos().x, game_object->GetPos().y - 30 }, this);
		sp->SetTarget(target);
		abilities.at(0)->cd_timer.Start();
	}*/
}

void Skeleton::OnCollEnter(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
}
