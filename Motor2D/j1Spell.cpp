#include "j1Spell.h"
#include "Spell.h"
#include "Boomerang.h"
#include "TowerAttack.h"
#include "SnakePoison.h"
#include "EventThrower.h"
#include "GameObject.h"

j1Spell::j1Spell()
{
}

j1Spell::~j1Spell()
{
}

bool j1Spell::Awake(pugi::xml_node &)
{
	bool ret = true;

	name = "spell";

	return ret;
}

bool j1Spell::Start()
{
	bool ret = true;



	return ret;
}

bool j1Spell::PreUpdate()
{
	bool ret = true;

	if (!spell_list.empty())
	{
		for (list<Spell*>::iterator it = spell_list.begin(); it != spell_list.end(); it++)
			ret = (*it)->PreUpdate();
	}

	return ret;
}

bool j1Spell::Update(float dt)
{
	bool ret = true;

	if (!spell_list.empty())
	{
		for (list<Spell*>::iterator it = spell_list.begin(); it != spell_list.end(); it++)
		{
			ret = (*it)->Update(dt);
			(*it)->Draw(dt);
		}
	}

	return ret;
}

bool j1Spell::PostUpdate()
{
	bool ret = true;

	if (!spell_list.empty())
	{
		for (list<Spell*>::iterator it = spell_list.begin(); it != spell_list.end(); it++)
			ret = (*it)->PostUpdate();
	}

	RemoveSpells();

	return ret;
}

bool j1Spell::CleanUp()
{
	bool ret = true;

	ClearSpells();

	return ret;
}

void j1Spell::OnCollision(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
	for (list<Spell*>::iterator it = spell_list.begin(); it != spell_list.end(); it++)
		(*it)->OnColl(bodyA, bodyB, fixtureA, fixtureB);
}

void j1Spell::OnCollisionEnter(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
	for (list<Spell*>::iterator it = spell_list.begin(); it != spell_list.end(); it++)
		(*it)->OnCollEnter(bodyA, bodyB, fixtureA, fixtureB);
}

void j1Spell::OnCollisionOut(PhysBody * bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{
	for (list<Spell*>::iterator it = spell_list.begin(); it != spell_list.end(); it++)
		(*it)->OnCollOut(bodyA, bodyB, fixtureA, fixtureB);
}

void j1Spell::ListenEvent(int type, EventThrower * origin, int id)
{
	Event* curr_event = nullptr;

	if (type = static_cast<int>(event_type::e_t_death))
	{
		curr_event = origin->GetEvent(id);

		if (curr_event->event_data.entity != nullptr)
		{
			DeleteSpellIfTarget(curr_event->event_data.entity);
		}
	}
}

Spell * j1Spell::CreateSpell(spell_name spell, iPoint pos, Entity * owner)
{
	Spell* ret = nullptr;

	switch (spell)
	{
	case boomerang:
		ret = new Boomerang(pos);
		break;
	case t_attack:
		ret = new TowerAttack(pos);
		break;
	case s_attack:
		ret = new SnakePoison(pos);
		break;
	}
	
	ret->owner = owner;
	ret->Start();
	spell_list.push_back(ret);
	return ret;
}

void j1Spell::DeleteSpell(Spell * spell)
{
	spell->to_delete = true;
}

void j1Spell::DeleteSpellIfTarget(Entity * target)
{
	if (!spell_list.empty())
	{
		for (list<Spell*>::iterator it = spell_list.begin(); it != spell_list.end(); it++)
		{
			if ((*it)->target == target)
			{
				DeleteSpell(*it);
			}
		}
	}
}

void j1Spell::ClearSpells()
{
	for (list<Spell*>::iterator it = spell_list.begin(); it != spell_list.end(); it++)
	{
		(*it)->CleanUp();
		(*it)->CleanSpell();
		RELEASE(*it);
	}
	spell_list.clear();
}

void j1Spell::RemoveSpells()
{
	if (!spell_list.empty())
	{
		for (list<Spell*>::iterator it = spell_list.begin(); it != spell_list.end();)
		{
			if ((*it)->to_delete == true)
			{
				(*it)->CleanUp();
				(*it)->CleanSpell();
				RELEASE(*it);
				it = spell_list.erase(it);
			}
			else
				++it;
		}
	}
}

