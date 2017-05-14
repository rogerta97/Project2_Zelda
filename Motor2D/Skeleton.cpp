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
#include "j1Input.h"

#define SKELETON_W 85
#define SKELETON_H 75

#define STUN 1.0f

#define HALFMAP 81*32

#define RANGE 200


Skeleton::Skeleton(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(SKELETON_H, SKELETON_W), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_npc, 0);

	game_object->CreateCollision(iPoint(0, 0), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_hit_box);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetListener((j1Module*)App->spell);
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

	last_life = stats.life;

	name = "skeleton";
}

Skeleton::~Skeleton()
{
}

bool Skeleton::Start()
{
	bool ret = true;

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
				DealDamage(((float)entity->stats.power * (float)spell->stats.damage_multiplicator) + (float)ability->damage); // Spells control their own damage mutiplicator

				spell->Effects(entity, this, ability);
			}
			else
				DealDamage(((float)entity->stats.power * (float)ability->damage_multiplicator) + (float)ability->damage);

			Die(entity);
		}
	}

	if (stats.life < last_life)
	{
		if (LookForTarget())
		{
			state = s_s_attack;
		}
	}
	last_life = stats.life;

	if (target != nullptr && target->to_delete)
		target = nullptr;

	switch (state)
	{
	case s_s_null:
		Idle();
		break;
	case s_s_idle:
		Idle();
		break;
	case s_s_attack:
		if (target == nullptr)
		{
			LookForTarget();
		}

		if (target == nullptr)
		{
			state = s_s_idle;
		}
		else
		{
			if (GetAbility(0)->CdCompleted())
			{
				SpinAttack();
				GetAbility(0)->cd_timer->Start();
			}
			if (GetAbility(1)->CdCompleted())
			{
				Bonemerang();
				GetAbility(1)->cd_timer->Start();
			}
		}

		if (target != nullptr && abs(DistanceFromTwoPoints(game_object->GetPos().x, game_object->GetPos().y, target->GetPos().x, target->GetPos().y)) > RANGE)
		{
			target = nullptr;
		}
		break;
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

	if (game_object->animator->IsCurrentAnimation("spin"))
	{
		if (game_object->animator->GetCurrentAnimation()->Finished())
		{
			game_object->DeleteFixture(abilities.at(0)->fixture);
			game_object->animator->GetCurrentAnimation()->Reset();
			Idle();
		}
	}
	
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

void Skeleton::Die(Entity * killed_by)
{
	if (stats.life <= 0 && !to_delete && killed_by != nullptr)
	{
		App->entity->AddRupeesIfPlayer(killed_by, rupee_reward);
		App->scene->main_scene->jungleCamp_manager->KillJungleCamp(this);
		if (App->scene->main_scene->quest_manager->vquest[2]->state == active)
		{
			App->scene->main_scene->quest_manager->add_progress(3, killed_by->GetTeam());
		}
	}
}

void Skeleton::Idle()
{
	game_object->SetAnimation("skeleton_idle");
	flip = false;

	draw_offset = NULLPOINT;
}

void Skeleton::SpinAttack()
{
	game_object->SetAnimation("spin");
	flip = false;

	GetAbility(0)->fixture = game_object->CreateCollisionSensor(iPoint(0, 0), 70, fixture_type::f_t_attack);

	draw_offset.x = 36;
	draw_offset.y = 44;
}

void Skeleton::Bonemerang()
{
	flip = false;
	int angle = GetRandomValue(0, 360);

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
