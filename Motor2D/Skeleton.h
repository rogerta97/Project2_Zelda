#ifndef _SKELETON_H_
#define _SKELETON_H_

#include "Entity.h"
#include <list>
#include "j1Timer.h"

class GameObject;


enum SKELETON_STATE
{
	s_s_null,
	s_s_idle,
	s_s_attack,
	s_s_stunned,
};


class Skeleton :public Entity
{
public:

	Skeleton(iPoint pos);

	~Skeleton();

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

	iPoint GetPos() const;

private:

	void Idle();
	void Stunned();
	void Attack();

	void SpinAttack();
	void Bonemerang();

	bool LookForTarget();


private:
	j1Timer*				stun_timer = nullptr;;

	bool					flip = false;

	SKELETON_STATE			state = s_s_null;

	states					anim_state = states_null;

	iPoint					draw_offset = NULLPOINT;

	Entity*					target = nullptr;

	int						rupee_reward = 0;

};

#endif // !SKELETON_H_