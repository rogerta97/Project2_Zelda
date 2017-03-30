#ifndef _SNAKES_H_
#define _SNAKES_H_

#include "Entity.h"
#include <list>
#include "j1Timer.h"

class GameObject;

enum SNAKE_STATE
{
	Snk_S_Null,
	Snk_S_Idle,
	Snk_S_Attack,
};


class Snakes :public Entity
{
public:

	Snakes(iPoint pos);

	~Snakes();

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
	void OnCollEnter(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);
	//void OnColl(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);

	iPoint GetPos() const;

private:
	
	void CheckState();

	void Idle();
	void DoAttack();
	void AttackLeft();
	void AttackRight();
	void AttackUp();
	void AttackDown();

	//void DoAttack();
	//bool LookForTarget();

private:

	j1Timer					cd_timer;

	bool					flip = false;

	bool					is_attacked = false;

	uint					attack_range = 150;

	SNAKE_STATE				state = Snk_S_Null;

	states					anim_state = states_null;

	std::vector<Entity*>	targets;

};

#endif // !TOWER_H_