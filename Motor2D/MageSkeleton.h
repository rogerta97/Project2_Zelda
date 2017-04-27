#ifndef _MAGESKELETON_H_
#define _MAGESKELETON_H_

#include "Entity.h"
#include <vector>
#include "j1Timer.h"

class GameObject;

enum MSKL_STATE
{
	MSkl_S_Null,
	MSkl_S_Idle,
	MSkl_S_Attack,
};


class MageSkeleton :public Entity
{
public:

	MageSkeleton(iPoint pos);

	~MageSkeleton();

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

	bool					flip = false;

	bool					is_attacked = false;

	int						rel_angle = 0;

	MSKL_STATE				state = MSkl_S_Null;

	states					anim_state = states_null;

	int						rupee_reward = 0;

};
#endif //_MAGESKELETON_H_