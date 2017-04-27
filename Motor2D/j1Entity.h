#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "j1Module.h"
#include "p2Defs.h"
#include "p2List.h"
#include "p2Point.h"
#include "j1Timer.h"
#include "Animation.h"

class PlayerManager;
class b2Fixture;
class PhysBody;
class Entity;
class Ability;
class Spell;
class j1Timer;

enum class pbody_type;

enum entity_name
{
	e_n_null, link, zelda, navi, minion, tower, ganon, trunk, base, tree, eyes, bush, snake, skeleton, waterfall, mskeleton,cuco
};

class slow
{
public:
	slow() {};
	slow(float _time, Entity* _entity) 
	{
		time = _time; entity = _entity;
		timer = App->AddGameplayTimer();
	};
	~slow() {};

	void CleanUp()
	{
		App->DeleteGameplayTimer(timer);
	}

	bool operator==(slow s)
	{
		if (time == s.time && entity == s.entity)
			return true;
		return false;
	}

	float    time = 0.0f;
	Entity*  entity = nullptr;
	j1Timer* timer = nullptr;
};

class stun
{
public:
	stun() {};
	stun(float _time, Entity* _entity, Animation* ani)
	{

		time = _time; entity = _entity;
		timer = App->AddGameplayTimer();
		Animation* a = new Animation(*ani);
		animator = new Animator();
		animator->AddAnimation(a);
		animator->SetAnimation("stun");
	};
	~stun() {};

	void CleanUp()
	{
		animator->CleanUp();
		RELEASE(animator);
		App->DeleteGameplayTimer(timer);
	};

	bool operator==(stun s)
	{
		if (time == s.time && entity == s.entity)
			return true;
		return false;
	}

	float     time = 0.0f;
	Entity*   entity = nullptr;
	j1Timer*  timer = nullptr;
	Animator* animator = nullptr;
};

class die
{
public:
	die() {};
	die(iPoint _pos, Animation* ani)
	{
		pos = _pos;
		Animation* a = new Animation(*ani);
		animator = new Animator();
		animator->AddAnimation(a);
		animator->SetAnimation("dead");
	};
	~die() {};

	void CleanUp()
	{
		animator->CleanUp();
		RELEASE(animator);
	};

	iPoint     pos = NULLPOINT;
	Animator*  animator = nullptr;
};

class Entity;
class j1Entity : public j1Module
{
public:

	j1Entity();

	// Destructor
	virtual ~j1Entity();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);
	void OnCollisionEnter(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);
	void OnCollisionOut(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);
	void ListenEvent(int type, EventThrower* origin, int id);

	Entity* CreateEntity(entity_name entity, iPoint pos);
	void DeleteEntity(Entity* entity);
	void ClearEntities();
	int GetEntitiesNumber();

	Entity* FindEntityByBody(PhysBody* type);
	Ability* FindAbilityByFixture(Entity* entity, b2Fixture* fixture);
	Ability* FindAbilityBySpellBody(PhysBody* spell);
	Spell* FindSpellByBody(PhysBody* spell);
	vector<Entity*> FindEntitiesByName(char* name);
	vector<Entity*> FindEntitiesByBodyType(pbody_type type);
	void DeathAnimation(Entity* die);
	void AddRupeesIfPlayer(Entity* entity, int amount);
	Animator* GetEntityEffectsAnimator();

private:
	void RemoveEntities();
	void SlowEntities();
	void DeleteFromSlow(Entity* entity);
	void StunEntities();
	void DeleteFromStun(Entity* entity);
	void DieEntities();

public:
	list<slow>     slowed_entities;
	list<stun>     stuned_entities;
	list<die>	   dying_entities;

private:
	// List with all entities
	list<Entity*>  entity_list;

	// Texture with effects animations
	SDL_Texture*   entity_effects_texture = nullptr;
	Animator*	   entity_effects_animator = nullptr;
};

#endif // __j1ENTITY_H__