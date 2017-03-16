#include "j1Entity.h"
#include "Link.h"
#include "p2Log.h"
#include "PlayerManager.h"
#include "Minion.h"
#include "GameObject.h"
#include "Entity.h"
#include "Spell.h"
#include "j1Spell.h"
#include "Tower.h"
#include "Functions.h"
#include "j1Timer.h"


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

	player_manager = new PlayerManager();
	player_manager->Start();

	return ret;
}

bool j1Entity::PreUpdate()
{
	bool ret = true;

	RemoveEntities();

	for(list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
		ret = (*it)->PreUpdate();

	player_manager->PreUpdate();

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

	player_manager->Update(dt);

	SlowEntities();

	return ret;
}

bool j1Entity::PostUpdate()
{
	bool ret = true;

	for (list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
		ret = (*it)->PostUpdate();

	player_manager->PostUpdate();

	return ret;
}

bool j1Entity::CleanUp()
{
	bool ret = true;

	ClearEntities();

	player_manager->CleanUp();

	return ret;
}

void j1Entity::OnCollision(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
	for (list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
		(*it)->OnColl(bodyA, bodyB, fixtureA, fixtureB);

	// Returns GotHit to the entity --------
	if (fixtureA->type == fixture_type::f_t_attack && fixtureB->type == fixture_type::f_t_hit_box)
	{
		// Find the entity that got hit
		Entity* entity = FindEntityByBody(bodyB);
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
						entity->hit = true;
				}
			}
		}

		if (!entity->hit_by)
		{
			entity->hit_by = nullptr;
			entity->hit_ability = nullptr;
			entity->hit_spell = nullptr;
		}
	}
	// ------------------------------------
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
	default:
		break;
	}

	if (ret != nullptr)
	{
		ret->Start();
		entity_list.push_back(ret);
	}
	else
		LOG("Entity creation returned nullptr");

	return ret;
}

void j1Entity::ClearEntities()
{
	for (list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
		(*it)->to_delete = true;
	
	entity_list.clear();
}

Entity * j1Entity::FindEntityByBody(PhysBody* body)
{
	Entity* ret = nullptr;

	// Look on entities
	for(list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
	{
		if ((*it)->game_object != nullptr && body == (*it)->game_object->pbody)
		{
			ret = *it;
			break;
		}
	}

	// Look on Spells
	for (list<Spell*>::iterator it = App->spell->spell_list.begin(); it != App->spell->spell_list.end(); it++)
	{
		if ((*it)->game_object != nullptr && body == (*it)->game_object->pbody)
		{
			ret = (*it)->owner;
			break;
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
	for (list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
	{
		if ((*it)->to_delete == true)
		{
			(*it)->CleanUp();
			(*it)->CleanEntity();
			entity_list.remove(*it);
			RELEASE(*it);
		}
	}
}

void j1Entity::SlowEntities()
{
	for(list<slow>::iterator it = slowed_entities.begin(); it != slowed_entities.end(); it++)
	{
		if ((*it).time <= (*it).timer.ReadSec())
		{
			(*it).entity->stats.speed = (*it).entity->stats.restore_speed;
			slowed_entities.remove(*it);
		}
	}
}

void j1Entity::StunEntities()
{
}



