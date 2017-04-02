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
#include "BoneAttack.h"


#define SKELETON_W 78
#define SKELETON_H 48

#define STUN 2.0f

#define HALFMAP 81*32


Skeleton::Skeleton(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(SKELETON_H, SKELETON_W), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_tower, 0);

	game_object->CreateCollision(iPoint(0, 0), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_hit_box);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetFixedRotation(true);
	game_object->SetKinematic();

	AddAbility(0, 50, 2, 2, "spin"); //times are ms.
	AddAbility(1, 30, 2, 2, "bone");

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

	LifeBar(iPoint(50, 4), iPoint(-23, -52));

	Entity* entity = nullptr;
	Ability* ability = nullptr;
	Spell* spell = nullptr;
	if (GotHit(entity, ability, spell))
	{
		// Enemy attacks
		if (entity != nullptr && ability != nullptr && entity->GetTeam() != GetTeam())
		{
			DealDamage(ability->damage * ability->damage_multiplicator);

			if (spell != nullptr && TextCmp(spell->name.c_str(), "boomerang"))
			{
				BoomerangEffects(ability, spell);
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
		Attack();
		break;
	case s_s_stunned:
		Stunned();
		if (stun_timer.ReadSec() > STUN)
		{
			stun_timer.Stop();
			state = s_s_attack;
		}
	default:
		break;
	}
	return ret;
}

bool Skeleton::Draw(float dt)
{
	bool ret = true;

	App->view->LayerBlit(2, game_object->GetTexture(), { game_object->GetPos().x - 24 - draw_offset.x , game_object->GetPos().y - 39 - draw_offset.y}, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_NONE);
	
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

	draw_offset = NULLPOINT;
}

void Skeleton::Stunned()
{
	state = s_s_stunned;
	game_object->SetAnimation("stunned");
	flip = false;
	anim_state = skeleton_stunned;

	draw_offset = NULLPOINT;

	if (!stun_timer.IsActive())
		stun_timer.Start();

}

void Skeleton::Attack()
{
	if (abilities.at(1)->CdCompleted() && abilities.at(0)->CdCompleted())
	{
		if (!game_object->animator->IsCurrentAnimation("spin"))
		{
			if (!game_object->animator->IsCurrentAnimation("bone"))
			{
				Bonemerang();

			}
			if (game_object->animator->IsCurrentAnimation("bone") && game_object->animator->GetCurrentAnimation()->Finished())
			{
				game_object->animator->GetCurrentAnimation()->Reset();
				SpinAttack();
			}
		}
		if (game_object->animator->IsCurrentAnimation("spin") && game_object->animator->GetCurrentAnimation()->Finished())
		{
			game_object->animator->GetCurrentAnimation()->Reset();
			game_object->DeleteFixture(abilities.at(0)->fixture);
			state = s_s_stunned;
		}
	}
}

void Skeleton::SpinAttack()
{
	game_object->SetAnimation("spin");
	flip = false;
	anim_state = skeleton_spin;

	GetAbility(0)->fixture = game_object->CreateCollisionSensor(iPoint(0, 0), 70, fixture_type::f_t_attack);

	draw_offset.x = 40;
	draw_offset.y = 22;
	
}

void Skeleton::Bonemerang()
{
	game_object->SetAnimation("bone");
	flip = false;
	anim_state = skeleton_bone;
	int angle = 0;
	
	if(game_object->GetPos().x < HALFMAP)
		angle = GetRandomValue(100, 170);
	else
		angle = GetRandomValue(80, -80);

	BoneAttack* ba = (BoneAttack*)App->spell->CreateSpell(bone_attack, { game_object->GetPos().x, game_object->GetPos().y - 30 }, this);
	ba->SetAngle(angle);
}

void Skeleton::OnCollEnter(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
}
