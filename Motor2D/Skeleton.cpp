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
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(SKELETON_H, SKELETON_W), App->cf->CATEGORY_SCENERY, App->cf->MASK_SCENERY, pbody_type::p_t_npc, 0);

	game_object->CreateCollision(iPoint(0, 0), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_hit_box);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetFixedRotation(true);
	game_object->SetKinematic();


	/*pugi::xml_document doc;
	App->LoadXML("snakes.xml", doc);
	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));*/
}

Skeleton::~Skeleton()
{
}

bool Skeleton::Start()
{
	bool ret = true;

	stats.max_life = stats.life = 40;

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

	//LifeBar(iPoint(32, 4), iPoint(-20, -32));

	//Entity* entity = nullptr;
	//Ability* ability = nullptr;
	//Spell* spell = nullptr;
	//if (GotHit(entity, ability, spell))
	//{
	//	// Enemy attacks
	//	if (entity != nullptr && ability != nullptr)
	//	{
	//		DealDamage(ability->damage * ability->damage_multiplicator);

	//		if (spell != nullptr && TextCmp(spell->name.c_str(), "boomerang"))
	//		{
	//			DealDamage(ability->damage * (spell->stats.damage_multiplicator - 1)); // Spells control their own damage mutiplicator

	//			if (spell->stats.slow_duration > 0)
	//				Slow(spell->stats.slow_multiplicator, spell->stats.slow_duration);
	//			if (spell->stats.stun_duration > 0)
	//				Stun(spell->stats.stun_duration);
	//		}
	//		if (state == Snk_S_Idle)
	//		{
	//			is_attacked = true;
	//			state = Snk_S_Attack;
	//			target = entity;
	//		}

	//	}
	//	if (stats.life <= 0)
	//		App->scene->main_scene->jungleCamp_manager->KillJungleCamp(this);
	//}


	//switch (state)
	//{
	//case Snk_S_Null:
	//	Idle();
	//	break;
	//case Snk_S_Idle:
	//	Idle();
	//	break;
	//case Snk_S_Attack:
	//	if (abs(DistanceFromTwoPoints(target->GetPos().x, target->GetPos().y, game_object->fGetPos().x, game_object->fGetPos().y)) < ATTACK_RANGE)
	//	{
	//		rel_angle = AngleFromTwoPoints(game_object->fGetPos().x, game_object->fGetPos().y, target->GetPos().x, target->GetPos().y) + 180;

	//		if (rel_angle >= -45 && rel_angle <= 45)
	//		{
	//			AttackLeft();
	//		}
	//		else if (rel_angle > 45 && rel_angle <= 135)
	//		{
	//			AttackUp();
	//		}
	//		else if (rel_angle > 135 && rel_angle <= 225)
	//		{
	//			AttackRight();
	//		}
	//		else
	//		{
	//			AttackDown();
	//		}
	//	}
	//	else
	//	{
	//		if (!LookForTarget())
	//			Idle();

	//	}
	//	break;
	//default:
	//	break;
	//}

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

void Skeleton::OnCollEnter(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
}



//bool Skeleton::LookForTarget()
//{
//	bool ret = false;
//
//	int shortest_distance = ATTACK_RANGE;
//
//	std::vector<Entity*> players1;
//	std::vector<Entity*> players2;
//
//
//	players1 = App->scene->main_scene->player_manager->GetTeamPlayers(1);
//	players2 = App->scene->main_scene->player_manager->GetTeamPlayers(2);
//
//	for (std::vector<Entity*>::iterator it = players1.begin(); it != players1.end(); it++)
//	{
//		if (GetPos().DistanceTo((*it)->GetPos()) < ATTACK_RANGE && GetPos().DistanceTo((*it)->GetPos()) < shortest_distance)
//		{
//			shortest_distance = GetPos().DistanceTo((*it)->GetPos());
//			target = *it;
//			ret = true;
//			break;
//		}
//	}
//	for (std::vector<Entity*>::iterator it = players2.begin(); it != players2.end(); it++)
//	{
//		if (GetPos().DistanceTo((*it)->GetPos()) < ATTACK_RANGE && GetPos().DistanceTo((*it)->GetPos()) < shortest_distance)
//		{
//			shortest_distance = GetPos().DistanceTo((*it)->GetPos());
//			target = *it;
//			ret = true;
//			break;
//		}
//	}
//
//	return ret;
//}

