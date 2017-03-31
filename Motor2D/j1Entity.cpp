#include "j1Entity.h"
#include "Link.h"
#include "p2Log.h"
#include "PlayerManager.h"
#include "Minion.h"
#include "GameObject.h"
#include "Entity.h"
#include "Bushes.h"
#include "Spell.h"
#include "Trunk.h"
#include "Trees.h"
#include "j1Spell.h"
#include "Tower.h"
#include "Functions.h"
#include "j1Timer.h"
#include "Zelda.h"
#include "Base.h"
#include "Eyes.h"
#include "Snakes.h"
#include "Skeleton.h"


j1Entity::j1Entity()
{
	name = "entity";
}

j1Entity::~j1Entity()
{
}

bool j1Entity::Awake(pugi::xml_node &)
{
	bool ret = true;


	return ret;
}

bool j1Entity::Start()
{
	bool ret = true;

	return ret;
}

bool j1Entity::PreUpdate()
{
	bool ret = true;

	RemoveEntities();

	for(list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
		ret = (*it)->PreUpdate();

	return ret;
}

bool j1Entity::Update(float dt)
{
	bool ret = true;

	for (list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
	{
		ret = (*it)->Update(dt);
		(*it)->Draw(dt);
	}

	SlowEntities();
	StunEntities();

	return ret;
}

bool j1Entity::PostUpdate()
{
	bool ret = true;

	for (list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
		ret = (*it)->PostUpdate();

	return ret;
}

bool j1Entity::CleanUp()
{
	bool ret = true;

	ClearEntities();

	return ret;
}

void j1Entity::OnCollision(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
	for (list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
		(*it)->OnColl(bodyA, bodyB, fixtureA, fixtureB);
}

void j1Entity::OnCollisionEnter(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
	for (list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
		(*it)->OnCollEnter(bodyA, bodyB, fixtureA, fixtureB);

	// Returns GotHit to the entity --------
	if (fixtureA->type == fixture_type::f_t_attack && fixtureB->type == fixture_type::f_t_hit_box)
	{
		// Find the entity that got hit
		Entity* entity = FindEntityByBody(bodyB);

		if (entity != nullptr)
			entity->hit = false;

		// Find the entity that hits
		if (entity != nullptr)
		{
			entity->hit_by = FindEntityByBody(bodyA);

			// Find the ability that hits with
			if (entity->hit_by != nullptr)
			{
				entity->hit_ability = FindAbilityByFixture(entity->hit_by, fixtureA);

				// If ability found return true
				if (entity->hit_ability != nullptr)
				{
					entity->hit = true;
				}

				// If not ability found, check it by Spell and ability names
				else
				{
					entity->hit_spell = FindSpellByBody(bodyA);
					entity->hit_ability = FindAbilityBySpellBody(bodyA);

					// If ability found return true
					if (entity->hit_ability != nullptr)
					{
						// Check if it has target
						if (entity->hit_spell->target == nullptr)
							entity->hit = true;
						else
						{
							// Check that this hits the target
							if(entity->hit_spell->target == entity)
								entity->hit = true;
						}
					}
				}
			}
		}

		if (entity != nullptr && !entity->hit_by)
		{
			entity->hit_by = nullptr;
			entity->hit_ability = nullptr;
			entity->hit_spell = nullptr;
		}
	}
	// ------------------------------------
}

void j1Entity::OnCollisionOut(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
	if (!entity_list.empty())
	{
		for (list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
			(*it)->OnCollOut(bodyA, bodyB, fixtureA, fixtureB);
	}
}

Entity* j1Entity::CreateEntity(entity_name entity, iPoint pos)
{
	Entity* ret = nullptr;

	switch (entity)
	{
	case link:
		ret = new Link(pos);
		break;
	case minion:
		ret = new Minion(pos);
		break;
	case tower:
		ret = new Tower(pos);
		break;
	case zelda:
		ret = new Zelda(pos);
        break;
	case trunk:
		ret = new Trunk(pos);
		break;
	case base:
		ret = new Base(pos);
		break;
	case tree:
		ret = new Tree(pos);
		break;
	case eyes:
		ret = new Eyes(pos);
		break;
	case snake:
		ret = new Snakes(pos);
	case bush:
		ret = new Bush(pos);
		break;
	case skeleton:
		ret = new Skeleton(pos);
	default:
		break;
	}

	if (ret != nullptr)
	{
		ret->Start();
		ret->type = entity;
		entity_list.push_back(ret);
	}
	else
		LOG("Entity creation returned nullptr");

	return ret;
}

void j1Entity::ClearEntities()
{
	if (!entity_list.empty())
	{
		for (list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
			(*it)->to_delete = true;
	}
}

int j1Entity::GetEntitiesNumber()
{
	return entity_list.size();
}

Entity * j1Entity::FindEntityByBody(PhysBody* body)
{
	Entity* ret = nullptr;

	// Look on entities
	if (!entity_list.empty())
	{
		for (list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
		{
			if ((*it)->game_object != nullptr && body == (*it)->game_object->pbody)
			{
				ret = *it;
				break;
			}
		}
	}

	// Look on Spells
	if (!App->spell->spell_list.empty())
	{
		for (list<Spell*>::iterator it = App->spell->spell_list.begin(); it != App->spell->spell_list.end(); it++)
		{
			if ((*it)->game_object != nullptr && body == (*it)->game_object->pbody)
			{
				ret = (*it)->owner;
				break;
			}
		}
	}

	return ret;
}

Ability* j1Entity::FindAbilityByFixture(Entity* entity, b2Fixture * fixture)
{
	Ability* ret = nullptr;
	for (int i = 0; i < entity->abilities.size(); i++)
	{
		if (entity->abilities.at(i)->fixture == fixture)
		{
			ret = entity->abilities.at(i);
			break;
		}
	}

	return ret;
}

Ability * j1Entity::FindAbilityBySpellBody(PhysBody * spell)
{
	Spell* sp = nullptr;

	sp = FindSpellByBody(spell);

	if (sp != nullptr)
	{
		for (vector<Ability*>::iterator it = sp->owner->abilities.begin(); it != sp->owner->abilities.end(); it++)
		{
			if (TextCmp((*it)->name.c_str(), sp->name.c_str()))
			{
				return *it;
			}
		}
	}
}

Spell * j1Entity::FindSpellByBody(PhysBody * spell)
{
	Spell* ret = nullptr;

	for (list<Spell*>::iterator it = App->spell->spell_list.begin(); it != App->spell->spell_list.end(); it++)
	{
		if ((*it)->game_object != nullptr && spell == (*it)->game_object->pbody)
		{
			ret = (*it);
			break;
		}
	}

	return ret;
}

void j1Entity::DeleteEntity(Entity* entity)
{
	entity->to_delete = true;
}

void j1Entity::RemoveEntities()
{
	if (!entity_list.empty())
	{
		for (list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end();)
		{
			if ((*it)->to_delete == true)
			{
				(*it)->CleanUp();
				(*it)->CleanEntity();
				RELEASE(*it);
				it = entity_list.erase(it);
			}
			else
				++it;
		}
	}
}

void j1Entity::SlowEntities()
{
	if (!slowed_entities.empty())
	{
		for (list<slow>::iterator it = slowed_entities.begin(); it != slowed_entities.end();)
		{
			if ((*it).entity != nullptr)
			{
				if ((*it).time <= (*it).timer.ReadSec())
				{
					(*it).entity->stats.speed = (*it).entity->stats.restore_speed;
					it = slowed_entities.erase(it);
				}
				else
					++it;
			}
			else
				it = (slowed_entities.erase(it));
		}
	}
}

void j1Entity::StunEntities()
{
	if (!stuned_entities.empty())
	{
		for (list<stun>::iterator it = stuned_entities.begin(); it != stuned_entities.end();)
		{
			if ((*it).entity != nullptr)
			{
				if ((*it).time <= (*it).timer.ReadSec())
				{
					(*it).entity->stuned = false;
					it = stuned_entities.erase(it);
				}
				else
					++it;
			}
			else
				it = (stuned_entities.erase(it));
		}
	}
}



