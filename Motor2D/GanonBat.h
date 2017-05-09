#ifndef _GANON_BAT_
#define _GANON_BAT_

#include "j1App.h"
#include "j1Render.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1Physics.h"
#include "Animation.h"
#include "Spell.h"

class b2Fixture;
class PhysBody;
class GameObject;
class Spell;
class GameObject;

enum g_b_direction
{
	g_b_null,
	g_b_up,
	g_b_down,
	g_b_left,
	g_b_right,
};

struct fire
{
	void CleanUp();

	GameObject* game_object = nullptr;
	j1Timer*	death_timer = nullptr;
	bool		to_die = false;
};

struct entity_hit
{
	void CleanUp();

	Entity*		entity = nullptr;
	j1Timer*	hit_timer = nullptr;
};

class GanonBat : public Spell
{
public:
	GanonBat(iPoint pos);

	virtual ~GanonBat();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool Draw(float dt);
	bool PostUpdate();
	bool CleanUp();

	void CleanSpell();

	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node&) const
	{
		return true;
	}

	void OnColl(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);

	void Set(g_b_direction dir);

	void Effects(Entity* entity, Ability * ability);

	void DeleteEntityFromHitList(Entity* entity);

private:
	void CreateFire(iPoint pos);
	bool CanDealDamage(Entity* entity);

private:
	g_b_direction   dir = g_b_direction::g_b_null;
	float			initial_speed = 0;
	bool			bat_print = true;

	j1Timer*		timer = nullptr;

	vector<fire>    fires;
	vector<entity_hit> entities_hit;

	iPoint			last_spawn_pos = NULLPOINT;

	int				fire_bd = 0;
	float			fire_mult = 0;
};

#endif // _NAVI_BASIC_ATTACK_