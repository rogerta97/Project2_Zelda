#include "GanonBat.h"
#include "GameObject.h"
#include "j1Viewports.h"
#include "p2Log.h"
#include "Entity.h"
#include "j1XMLLoader.h"
#include "Functions.h"
#include "p2Log.h"

#define BAT_DESTRUCTION_TIME 0.5f
#define SPEED 400

#define DROP_DISTANCE 35
#define FIRE_DESTRUCTION_TIME 3.0f
#define HIT_TIME 0.7

GanonBat::GanonBat(iPoint pos)
{
	game_object = new GameObject(iPoint(pos.x, pos.y), iPoint(20, 20), App->cf->CATEGORY_ABILITIES, App->cf->MASK_ABILITIES, pbody_type::p_t_ganon_bat, 0);
	hit_box = game_object->CreateCollisionSensor(iPoint(5, 10), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_attack);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetListener((j1Module*)App->spell);
	game_object->SetFixedRotation(true);
	game_object->pbody->body->SetBullet(true);

	pugi::xml_document doc;
	App->xml->LoadXML("ganon_bat.xml", doc);
	game_object->SetTexture(game_object->LoadAnimationsFromXML(doc, "animations"));

	pugi::xml_document doc2;
	App->xml->LoadXML("ganon.xml", doc2);
	pugi::xml_node stats_node = doc2.child("file").child("stats").child("bat_spell");
	stats.damage_multiplicator = stats_node.attribute("mult").as_float();
	fire_bd = stats_node.attribute("bat_fire_bd").as_int();
	fire_mult = stats_node.attribute("bat_fire_dm").as_float();

	//stats.damage_multiplicator = stats_node.attribute("mult").as_float(0.0f);

	draw_offset = restore_draw_offset = { 25, 10 };

	name = "ganon_bat";

	last_spawn_pos = pos;
}

GanonBat::~GanonBat()
{
}

bool GanonBat::Start()
{
	bool ret = true;

	timer = App->AddGameplayTimer();

	if (owner->GetTeam() == ANIMATIONS_TEAM)
		game_object->SetAnimation("bat_spawn");
	else
		game_object->SetAnimation("bat_spawn_2");

	return ret;
}

bool GanonBat::PreUpdate()
{
	bool ret = true;



	return ret;
}

bool GanonBat::Update(float dt)
{
	bool ret = true;

	if (bat_print)
	{
		float speed = dt * SPEED;

		// Move
		switch (dir)
		{
		case g_b_direction::g_b_up:
			game_object->SetPos({ game_object->fGetPos().x, game_object->fGetPos().y - speed });
			break;
		case g_b_direction::g_b_down:
			game_object->SetPos({ game_object->fGetPos().x, game_object->fGetPos().y + speed });
			break;
		case g_b_direction::g_b_left:
			game_object->SetPos({ game_object->fGetPos().x - speed, game_object->fGetPos().y });
			break;
		case g_b_direction::g_b_right:
			game_object->SetPos({ game_object->fGetPos().x + speed, game_object->fGetPos().y });
			break;
		default:
			break;
		}

		// Spawn to fly
		if ((game_object->animator->IsCurrentAnimation("bat_spawn") || game_object->animator->IsCurrentAnimation("bat_spawn_2")) && game_object->animator->GetCurrentAnimation()->Finished())
		{
			if (owner->GetTeam() == ANIMATIONS_TEAM)
				game_object->SetAnimation("bat_fly");
			else
				game_object->SetAnimation("bat_fly_2");
		}

		// Spawn fire
		if (abs(DistanceFromTwoPoints(last_spawn_pos.x, last_spawn_pos.y, game_object->GetPos().x, game_object->GetPos().y)) > DROP_DISTANCE)
		{
			CreateFire(last_spawn_pos);
			last_spawn_pos = game_object->GetPos();
		}

		if (timer->ReadSec() > BAT_DESTRUCTION_TIME)
		{
			if (owner->GetTeam() == ANIMATIONS_TEAM)
				game_object->SetAnimation("bat_die");
			else
				game_object->SetAnimation("bat_die_2");
		}

		if ((game_object->animator->IsCurrentAnimation("bat_die") || game_object->animator->IsCurrentAnimation("bat_die_2")) && game_object->animator->GetCurrentAnimation()->Finished())
		{
			bat_print = false;
			game_object->SetCatMask(App->cf->CATEGORY_NONCOLLISIONABLE, App->cf->MASK_NONCOLLISIONABLE);
		}
	}

	// Fire control
	if (!fires.empty())
	{
		for (vector<fire>::iterator it = fires.begin(); it != fires.end();)
		{
			fire curr_fire = (*it);

			if (!curr_fire.to_die)
			{
				if ((curr_fire.game_object->animator->IsCurrentAnimation("fire_spawn") || curr_fire.game_object->animator->IsCurrentAnimation("fire_spawn_2")) && curr_fire.game_object->animator->GetCurrentAnimation()->Finished())
				{
					if (owner->GetTeam() == ANIMATIONS_TEAM)
						curr_fire.game_object->SetAnimation("fire_burn");
					else
						curr_fire.game_object->SetAnimation("fire_burn_2");
				}

				if (curr_fire.death_timer->ReadSec() > FIRE_DESTRUCTION_TIME)
				{
					curr_fire.to_die = true;
					if (owner->GetTeam() == ANIMATIONS_TEAM)
						curr_fire.game_object->SetAnimation("fire_die");
					else
						curr_fire.game_object->SetAnimation("fire_die_2");
				}
			}

			if (curr_fire.to_die && curr_fire.game_object->animator->GetCurrentAnimation()->Finished())
			{
				curr_fire.CleanUp();
				it = fires.erase(it);
			}
			else
				++it;
		}

		if (!bat_print && fires.empty())
		{
			App->spell->DeleteSpell(this);
		}
	}

	return ret;
}

bool GanonBat::Draw(float dt)
{
	bool ret = true;

	if(bat_print)
		App->view->LayerBlit(game_object->GetPos().y, game_object->GetTexture(), { game_object->GetPos().x - draw_offset.x - 3, game_object->GetPos().y - draw_offset.y }, game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_HORIZONTAL);

	for (int i = 0; i < fires.size(); i++)
	{
		fire curr_fire = fires.at(i);
		App->view->LayerBlit(curr_fire.game_object->GetPos().y, game_object->GetTexture(), { curr_fire.game_object->GetPos().x - draw_offset.x - 3, curr_fire.game_object->GetPos().y - draw_offset.y }, curr_fire.game_object->GetCurrentAnimationRect(dt), 0, -1.0f, true, SDL_FLIP_HORIZONTAL);
	}

	return ret;
}

bool GanonBat::PostUpdate()
{
	bool ret = true;



	return ret;
}

bool GanonBat::CleanUp()
{
	bool ret = true;

	App->DeleteGameplayTimer(timer);

	if (!fires.empty())
	{
		for (vector<fire>::iterator it = fires.begin(); it != fires.end();)
		{
			(*it).CleanUp();
			it = fires.erase(it);
		}
	}

	if (!entities_hit.empty())
	{
		for (vector<entity_hit>::iterator it = entities_hit.begin(); it != entities_hit.end();)
		{
			(*it).CleanUp();
			it = entities_hit.erase(it);
		}
	}

	return ret;
}

void GanonBat::CleanSpell()
{
}

void GanonBat::OnColl(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
	for (int i = 0; i < fires.size(); i++)
	{
		if (bodyA == fires.at(i).game_object->pbody)
		{
			Entity* e = App->entity->FindEntityByBody(bodyB);

			if (e != nullptr && owner != nullptr &&  e != owner && e->GetTeam() != owner->GetTeam() && !owner->to_delete)
			{
				if (CanDealDamage(e))
				{
					e->DealDamage((owner->stats.power * fire_mult) + (fire_bd));
					e->Die(owner);
				}
			}

			break;
		}
	}
}

void GanonBat::Set(g_b_direction _dir)
{
	dir = _dir;
}

void GanonBat::Effects(Entity * entity, Ability * ability)
{

}

void GanonBat::DeleteEntityFromHitList(Entity * entity)
{
	if (entity == nullptr)
		return;

	if (!entities_hit.empty())
	{
		for (vector<entity_hit>::iterator it = entities_hit.begin(); it != entities_hit.end();)
		{
			if ((*it).entity == entity)
			{
				(*it).CleanUp();
				it = entities_hit.erase(it);
				break;
			}
			else
				++it;
		}
	}
}

void GanonBat::CreateFire(iPoint pos)
{
	fire f;

	GameObject* g_o = new GameObject(iPoint(pos.x, pos.y), iPoint(20, 20), App->cf->CATEGORY_ABILITIES, App->cf->MASK_ABILITIES, pbody_type::p_t_ganon_bat, 0);
	hit_box = g_o->CreateCollisionSensor(iPoint(0, +10), game_object->GetHitBoxSize().x, game_object->GetHitBoxSize().y, fixture_type::f_t_attack);
	g_o->SetListener((j1Module*)App->entity);
	g_o->SetListener((j1Module*)App->spell);
	g_o->SetFixedRotation(true);
	g_o->pbody->body->SetBullet(true);

	pugi::xml_document doc;
	App->xml->LoadXML("ganon_bat.xml", doc);
	g_o->SetTexture(g_o->LoadAnimationsFromXML(doc, "animations"));

	if (owner->GetTeam() == ANIMATIONS_TEAM)
		g_o->SetAnimation("fire_spawn");
	else
		g_o->SetAnimation("fire_spawn_2");

	f.game_object = g_o;
	f.death_timer = App->AddGameplayTimer();

	fires.push_back(f);
}

bool GanonBat::CanDealDamage(Entity * entity)
{
	bool ret = true;

	if (entity == nullptr)
		return false;

	for (int i = 0; i < entities_hit.size(); i++)
	{
		if (entity == entities_hit.at(i).entity)
		{
			if (entities_hit.at(i).hit_timer->ReadSec() < HIT_TIME)
			{
				return false;
			}
			else
			{
				entities_hit.at(i).hit_timer->Start();
				return true;
			}
		}
	}

	entity_hit eh;
	eh.entity = entity;
	eh.hit_timer = App->AddGameplayTimer();
	entities_hit.push_back(eh);

	return ret;
}

void fire::CleanUp()
{
	game_object->CleanUp();
	RELEASE(game_object);

	App->DeleteGameplayTimer(death_timer);
}

void entity_hit::CleanUp()
{
	App->DeleteGameplayTimer(hit_timer);
}
