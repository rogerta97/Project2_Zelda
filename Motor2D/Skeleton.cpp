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
#include "p2Log.h"
#include "j1Spell.h"
#include "Spell.h"
#include "BoneAttack.h"
#include "j1XMLLoader.h"
#include "Quest_Manager.h"

#define SKELETON_W 85
#define SKELETON_H 75

#define STUN 1.0f

#define HALFMAP 81*32

#define RANGE 150


Skeleton::Skeleton(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(SKELETON_W, SKELETON_H), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_tower, 0);

	game_object->CreateCollision(iPoint(0, 0), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_hit_box);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetFixedRotation(true);
	game_object->SetKinematic();

	pugi::xml_document doc;
	App->xml->LoadXML("skeleton.xml", doc);
	pugi::xml_node stats_node = doc.child("file").child("stats");

	stats.life = stats.base_hp = stats.max_life = stats_node.attribute("hp").as_int();
	stats.base_power = stats.power = stats_node.attribute("power").as_int();
	rupee_reward = stats_node.attribute("rupees").as_int();

	float dmg_mult = stats_node.child("ability1").attribute("mult").as_float();
	float cd = stats_node.child("ability1").attribute("cd").as_float();
	int bd = stats_node.child("ability1").attribute("bd").as_int();
	AddAbility(0, cd, bd, dmg_mult, "spin");

	dmg_mult = stats_node.child("ability2").attribute("mult").as_float();
	cd = stats_node.child("ability2").attribute("cd").as_float();
	bd = stats_node.child("ability2").attribute("bd").as_int();
	AddAbility(1, cd, bd, dmg_mult, "bone");

	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));

	name = "skeleton";
}

Skeleton::~Skeleton()
{
}

bool Skeleton::Start()
{
	bool ret = true;

	stun_timer = App->AddGameplayTimer();

	Idle();

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

	if (to_delete)
		return true;

	Entity* entity = nullptr;
	Ability* ability = nullptr;
	Spell* spell = nullptr;
	if (GotHit(entity, ability, spell))
	{
		// Enemy attacks
		if (entity != nullptr && ability != nullptr && entity->GetTeam() != GetTeam())
		{
			if (spell != nullptr)
			{
				DealDamage((entity->stats.power * spell->stats.damage_multiplicator) + ability->damage); // Spells control their own damage mutiplicator

				spell->Effects(entity, this, ability);
			}
			else
				DealDamage((entity->stats.power * ability->damage_multiplicator) + ability->damage);

			if (state == s_s_idle)
			{
				state = s_s_attack;
			}

		}
		if (stats.life <= 0)
		{
			App->entity->AddRupeesIfPlayer(entity, rupee_reward);
			App->scene->main_scene->jungleCamp_manager->KillJungleCamp(this);
			if (App->scene->main_scene->quest_manager->vquest[2]->state == active)
			{
				App->scene->main_scene->quest_manager->add_progress(3, entity->GetTeam());
			}
		}
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
		if (!LookForTarget())
		{
			if (!game_object->animator->GetCurrentAnimation()->Finished())
			{
				if(game_object->animator->IsCurrentAnimation("spin"))
					game_object->DeleteFixture(abilities.at(0)->fixture);
			}
			Idle();
		}
		break;
	case s_s_stunned:
		Stunned();
		if (stun_timer->ReadSec() > STUN)
		{
			stun_timer->Stop();
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

	LifeBar(iPoint(50, 4), iPoint(-27, -52));

	App->view->LayerBlit(2, game_object->GetTexture(), { game_object->GetPos().x - 42 - draw_offset.x , game_object->GetPos().y - 39 - draw_offset.y}, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_NONE);
	if (App->debug_mode)
		App->view->LayerDrawCircle(game_object->GetPos().x, game_object->GetPos().y, RANGE, 255, 0, 0);
	
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

	App->DeleteGameplayTimer(stun_timer);

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

	if (!stun_timer->IsActive())
		stun_timer->Start();

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

	draw_offset.x = 36;
	draw_offset.y = 44;
	
}

void Skeleton::Bonemerang()
{
	game_object->SetAnimation("bone");
	flip = false;
	anim_state = skeleton_bone;
	int angle = 0;
	
	angle = GetRandomValue(0, 360);

	BoneAttack* ba = (BoneAttack*)App->spell->CreateSpell(bone_attack, { game_object->GetPos().x, game_object->GetPos().y - 30 }, this);
	ba->SetAngle(angle);
}

bool Skeleton::LookForTarget()
{
	bool ret = false;

	int shortest_distance = RANGE;

	std::vector<Entity*> players1;
	std::vector<Entity*> players2;

	players1 = App->scene->main_scene->player_manager->GetTeamPlayers(1);
	players2 = App->scene->main_scene->player_manager->GetTeamPlayers(2);

	for (std::vector<Entity*>::iterator it = players1.begin(); it != players1.end(); it++)
	{
		if (GetPos().DistanceTo((*it)->GetPos()) < RANGE && GetPos().DistanceTo((*it)->GetPos()) < shortest_distance)
		{
			shortest_distance = GetPos().DistanceTo((*it)->GetPos());
			target = *it;
			ret = true;
			break;
		}
	}

	for (std::vector<Entity*>::iterator it = players2.begin(); it != players2.end(); it++)
	{
		if (GetPos().DistanceTo((*it)->GetPos()) < RANGE && GetPos().DistanceTo((*it)->GetPos()) < shortest_distance)
		{
			shortest_distance = GetPos().DistanceTo((*it)->GetPos());
			target = *it;
			ret = true;
			break;
		}
	}

	return ret;
}

void Skeleton::OnCollEnter(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
}
