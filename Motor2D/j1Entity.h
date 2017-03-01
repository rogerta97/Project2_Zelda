#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "j1Module.h"
#include "p2Defs.h"
#include "p2List.h"
#include "p2Point.h"

class PlayerManager;
class b2Fixture;
class PhysBody;
class Entity;
class Ability;

enum class pbody_type;

enum entity_name
{
	link, minion, turret, castle
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

	Entity* CreateEntity(entity_name entity, iPoint pos);
	void DeleteEntity(Entity* entity);
	void ClearEntities();

	Entity* FindEntityByBody(PhysBody* type);
	Ability* FindAbilityByFixture(Entity* entity, b2Fixture* fixture);

public:
	PlayerManager* player_manager = nullptr;

private:
	// List with all entities
	list<Entity*> entity_list;


};

#endif // __j1ENTITY_H__