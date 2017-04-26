#include "MageSkeleton.h"
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
#include "Spell.h"
#include "TacoAttack.h"
#include "j1XMLLoader.h"
#include "Quest_Manager.h"

#define MSKL_H 32
#define MSKL_W 32

#define ATTACK_RANGE 150
#define HALFMAP 81*32

MageSkeleton::MageSkeleton(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(MSKL_H, MSKL_W), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_npc, 0);

	game_object->CreateCollision(iPoint(0, 0), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_hit_box);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetFixedRotation(true);
	game_object->SetKinematic();

	pugi::xml_document doc;
	App->xml->LoadXML("mageskeleton.xml", doc);
	pugi::xml_node stats_node = doc.child("file").child("stats");

	stats.life = stats.base_hp = stats.max_life = stats_node.attribute("hp").as_int();
	stats.base_power = stats.power = stats_node.attribute("power").as_int();
	rupee_reward = stats_node.attribute("rupees").as_int();

	float dmg_mult = stats_node.child("ability1").attribute("mult").as_float();
	float cd = stats_node.child("ability1").attribute("cd").as_float();
	int bd = stats_node.child("ability1").attribute("bd").as_int();
	AddAbility(0, cd, bd, dmg_mult, "taco_attack");

	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));

	name = "mageskeleton";
}

MageSkeleton::~MageSkeleton()
{
}

bool MageSkeleton::Start()
{
	bool ret = true;

	Idle();

	show_life_bar = true;

	return ret;
}

bool MageSkeleton::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool MageSkeleton::Update(float dt)
{
	bool ret = true;

	if (to_delete)
		return true;

	LifeBar(iPoint(32, 4), iPoint(-20, -32));

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

			if (state == MSkl_S_Idle)
			{
				is_attacked = true;
				state = MSkl_S_Attack;
				target = entity;
				if (this->GetPos().x < HALFMAP)
				{
					for (int i = 0; i < App->scene->main_scene->jungleCamp_manager->mageskeleton_camp1.size(); i++)
					{
						MageSkeleton* m = static_cast<MageSkeleton*>(App->scene->main_scene->jungleCamp_manager->mageskeleton_camp1[i]);
						m->target = target;
						m->state = state;
					}
				}
				else
				{
					for (int i = 0; i < App->scene->main_scene->jungleCamp_manager->mageskeleton_camp2.size(); i++)
					{
						MageSkeleton* m = static_cast<MageSkeleton*>(App->scene->main_scene->jungleCamp_manager->mageskeleton_camp2[i]);
						m->target = target;
						m->state = state;
					}
				}
			}

		}
		if (stats.life <= 0)
		{
			App->entity->AddRupeesIfPlayer(entity, rupee_reward);
			App->scene->main_scene->jungleCamp_manager->KillJungleCamp(this);

			if (App->scene->main_scene->quest_manager->vquest[2]->state == active)
			{
				if (this->GetPos().x < HALFMAP)
				{
					if (App->scene->main_scene->jungleCamp_manager->mageskeleton_camp1.empty())
						if (entity->is_player)
							App->scene->main_scene->quest_manager->add_progress(3, entity->GetTeam());
				}
				else
				{
					if (App->scene->main_scene->jungleCamp_manager->mageskeleton_camp2.empty())
						if (entity->is_player)
							App->scene->main_scene->quest_manager->add_progress(3, entity->GetTeam());
				}
			}
		}
	}


	switch (state)
	{
	case MSkl_S_Null:
		Idle();
		break;
	case MSkl_S_Idle:
		Idle();
		break;
	case MSkl_S_Attack:
		if (target != nullptr)
		{
			if (abs(DistanceFromTwoPoints(target->GetPos().x, target->GetPos().y, game_object->fGetPos().x, game_object->fGetPos().y)) < ATTACK_RANGE)
			{
				rel_angle = AngleFromTwoPoints(game_object->fGetPos().x, game_object->fGetPos().y, target->GetPos().x, target->GetPos().y) + 180;

				if (rel_angle >= -45 && rel_angle <= 45)
				{
					AttackLeft();
				}
				else if (rel_angle > 45 && rel_angle <= 135)
				{
					AttackUp();
				}
				else if (rel_angle > 135 && rel_angle <= 225)
				{
					AttackRight();
				}
				else
				{
					AttackDown();
				}
			}
			else
			{
				if (!LookForTarget())
					Idle();
			}
		}
		else
			if (!LookForTarget())
				Idle();
		break;
	default:
		break;
	}

	return ret;
}

bool MageSkeleton::Draw(float dt)
{
	bool ret = true;

	if (!flip)
		App->view->LayerBlit(GetPos().y, game_object->GetTexture(), { game_object->GetPos().x - 14 , game_object->GetPos().y - 20 }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_NONE);
	else
		App->view->LayerBlit(GetPos().y, game_object->GetTexture(), { game_object->GetPos().x - 14 , game_object->GetPos().y - 20 }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_HORIZONTAL);

	if (App->debug_mode)
		App->view->LayerDrawCircle(game_object->GetPos().x, game_object->GetPos().y, ATTACK_RANGE, 255, 0, 0);

	return ret;
}

bool MageSkeleton::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool MageSkeleton::CleanUp()
{
	bool ret = true;

	return ret;
}

iPoint MageSkeleton::GetPos() const
{
	return game_object->GetPos();
}

void MageSkeleton::OnCollEnter(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
}

void MageSkeleton::Idle()
{
	if (game_object->GetPos().x < HALFMAP)
	{
		game_object->SetAnimation("mskeleton_lateral");
		anim_state = mskeleton_lateral;
	}
	else
	{
		game_object->SetAnimation("mskeleton_down");
		anim_state = mskeleton_down;
	}

	state = MSkl_S_Idle;
	flip = false;

}

void MageSkeleton::DoAttack()
{
	if (abilities.at(0)->CdCompleted())
	{
		if (target != nullptr)
		{
			TacoAttack* sp = (TacoAttack*)App->spell->CreateSpell(taco_attack, { game_object->GetPos().x, game_object->GetPos().y - 30 }, this);
			sp->SetTarget(target);
		}
		else
			if (!LookForTarget())
				Idle();


		abilities.at(0)->cd_timer.Start();
	}
}

void MageSkeleton::AttackLeft()
{
	game_object->SetAnimation("mskeleton_lateral");
	flip = false;
	DoAttack();
}

void MageSkeleton::AttackRight()
{
	game_object->SetAnimation("mskeleton_lateral");
	flip = true;
	DoAttack();
}

void MageSkeleton::AttackUp()
{
	game_object->SetAnimation("mskeleton_up");
	flip = false;
	DoAttack();
}

void MageSkeleton::AttackDown()
{
	game_object->SetAnimation("mskeleton_attack_down");
	flip = false;
	DoAttack();
}

bool MageSkeleton::LookForTarget()
{
	bool ret = false;

	int shortest_distance = ATTACK_RANGE;

	std::vector<Entity*> players1;
	std::vector<Entity*> players2;

	players1 = App->scene->main_scene->player_manager->GetTeamPlayers(1);
	players2 = App->scene->main_scene->player_manager->GetTeamPlayers(2);

	for (std::vector<Entity*>::iterator it = players1.begin(); it != players1.end(); it++)
	{
		if (GetPos().DistanceTo((*it)->GetPos()) < ATTACK_RANGE && GetPos().DistanceTo((*it)->GetPos()) < shortest_distance)
		{
			shortest_distance = GetPos().DistanceTo((*it)->GetPos());
			target = *it;
			for (int i = 0; App->scene->main_scene->jungleCamp_manager->mageskeleton_camp1.size(); i++)
			{
				
				MageSkeleton* test = (MageSkeleton*)App->scene->main_scene->jungleCamp_manager->mageskeleton_camp1[i];
				test->target = *it;
			}
			ret = true;
			break;
		}
	}

	for (std::vector<Entity*>::iterator it = players2.begin(); it != players2.end(); it++)
	{
		if (GetPos().DistanceTo((*it)->GetPos()) < ATTACK_RANGE && GetPos().DistanceTo((*it)->GetPos()) < shortest_distance)
		{
			shortest_distance = GetPos().DistanceTo((*it)->GetPos());
			target = *it;
			for (int i = 0; App->scene->main_scene->jungleCamp_manager->mageskeleton_camp1.size(); i++)
			{

				MageSkeleton* test = (MageSkeleton*)App->scene->main_scene->jungleCamp_manager->mageskeleton_camp1[i];
				test->target = *it;
			}
			ret = true;
			break;
		}
	}

	return ret;
}