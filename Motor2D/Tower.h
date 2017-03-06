#ifndef _TOWER_H_
#define _TOWER_H_

#include "Entity.h"
#include <list>

class GameObject;

enum TOWER_STATE
{
	Tower_Idle,
	Tower_Attack,
};


class Tower :public Entity
{
public:

	Tower(iPoint pos);

	~Tower();

	// Start variables
	bool Start();

	// PreUpdate
	bool PreUpdate();

	// Update
	bool Update(float dt);

	// Draw and change animations
	bool Draw(float dt);

	// Post Update
	bool PostUpdate();

	// CleanUp
	bool CleanUp();

	


	// On Collision
	void OnColl(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);

	iPoint GetPos() const;



private:

	void TowerIdle();
	void TowerAttack();

	void Idle();
	void Attack();

	void CheckTowerState();

	bool LookForTarget();

private:

	bool				flip = false;

	uint				attack_range = 150;

	TOWER_STATE			state = Tower_Idle;

	Entity*				target = nullptr;

	states				anim_state = states_null;

};

#endif // !TOWER_H_