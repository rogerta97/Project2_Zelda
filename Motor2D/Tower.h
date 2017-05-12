#ifndef _TOWER_H_
#define _TOWER_H_

#include "Entity.h"
#include <list>
#include "j1Timer.h"

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

	void Die(Entity* killed_by);

	void SetInvulnerableAnimation(uint team);

private:
	void DoAttack();
	void Idle();
	bool LookForTarget();

public:
	Entity*				target = nullptr;

private:
	bool				flip = false;

	uint				attack_range = 175;

	TOWER_STATE			state = Tower_Idle;
	states				anim_state = states_null;

	int					rupee_reward = 0;

	bool				wait_first = true;
	j1Timer*			wait_firest_timer = nullptr;

	SDL_Texture*		shield = nullptr;
	SDL_Rect			shield_rect = NULLRECT;
};

#endif // !TOWER_H_