#include "j1Spell.h"
#include "Spell.h"
#include "Boomerang.h"
#include "TowerAttack.h"

j1Spell::j1Spell()
{
}

j1Spell::~j1Spell()
{
}

bool j1Spell::Awake(pugi::xml_node &)
{
	bool ret = true;



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

	RemoveSpells();

	for (list<Spell*>::iterator it = spell_list.begin(); it != spell_list.end(); it++)
		ret = (*it)->PreUpdate();

	return ret;
}

bool j1Spell::Update(float dt)
{
	bool ret = true;


	for (list<Spell*>::iterator it = spell_list.begin(); it != spell_list.end(); it++)
	{
		ret = (*it)->Update(dt);
		(*it)->Draw(dt);
	}

	return ret;
}

bool j1Spell::PostUpdate()
{
	bool ret = true;

	for (list<Spell*>::iterator it = spell_list.begin(); it != spell_list.end(); it++)
		ret = (*it)->PostUpdate();

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
	for (list<Spell*>::iterator it = spell_list.begin(); it != spell_list.end(); it++)
	{
		if ((*it)->to_delete == true)
		{
			(*it)->CleanUp();
			(*it)->CleanSpell();
			RELEASE(*it);
			spell_list.remove(*it);
		}
	}
}

