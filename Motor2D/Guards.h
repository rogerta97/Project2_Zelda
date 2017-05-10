#ifndef _GUARDS_H_
#define _GUARDS_H_

#include "Entity.h"
#include <vector>
#include "j1Timer.h"

class GameObject;

enum GUARDS_STATE
{
	g_s_null,
	g_s_idle,
	g_s_follow,
	g_s_attack,
};

enum GUARDS_MOVE_STATE
{
	gMove_AproachTarget,
	gMove_ReturnToPath,
	gMove_null,
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

	void MoveUp(float speed);
	void MoveDown(float speed);
	void MoveLeft(float speed);
	void MoveRight(float speed);

	void MoveUpRight(float speed);
	void MoveDownRight(float speed);
	void MoveUpLeft(float speed);
	void MoveDownLeft(float speed);

	void RunUp();
	void RunDown();
	void RunLeft();
	void RunRight();

	void IdleUp();
	void IdleDown();
	void IdleLeft();
	void IdleRight();

	void Die(Entity* killed_by);

private:
	bool OnRangeFollow(iPoint pos);
	bool OnRangeAttack(iPoint pos);
	void CheckState();

	void SetTargetPath(const std::list<iPoint>* path);
	void PathToTarget();
	void PathToInitialPos();

	void GuardIdle();
	void GuardMove();
	void GuardAttack();

	bool LookForTarget();

	void Move(int delta_x, int delta_y);

	void BasicAttackUp();
	void BasicAttackDown();
	void BasicAttackLeft();
	void BasicAttackRight();

	void Attack();

	void SetIdleAnim();

	void ClearTargetPath();

public:
	Entity*					target = nullptr;

private:
	j1Timer*				cd_timer = nullptr;

	bool					flip = false;

	bool					is_attacked = false;

	int						rel_angle = 0;
	float					speed = 0;

	GUARDS_STATE			state = g_s_null;
	GUARDS_MOVE_STATE		move_state = gMove_null;

	states					anim_state = states_null;

	int						rupee_reward = 0;

	std::queue<iPoint>		target_path;

	iPoint					initial_pos = NULLPOINT;

	int						attacking = false;

};
#endif // !_GUARDS_H_

