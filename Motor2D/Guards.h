#ifndef _GUARDS_H_
#define _GUARDS_H_

#include "Entity.h"
#include <vector>
#include "j1Timer.h"

class GameObject;

enum GUARDS_STATE
{
	g_s_null,
	
};

class Guards :public Entity
{
public:

	Guards(iPoint pos);

	~Guards();

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

	void Idle();

	void DoAttack();

	void AttackLeft();
	void AttackRight();
	void AttackUp();
	void AttackDown();

	bool LookForTarget();

public:
	Entity*					target = nullptr;
private:

	j1Timer					cd_timer;

	bool					flip = false;

	bool					is_attacked = false;

	int						rel_angle = 0;

	GUARDS_STATE			state = g_s_null;

	states					anim_state = states_null;

	int						rupee_reward = 0;

};
#endif // !_GUARDS_H_

