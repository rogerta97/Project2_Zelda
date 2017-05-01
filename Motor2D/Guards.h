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
	g_s_reset,
	
};

enum GUARDS_MOVE_STATE
{
	gMove_AproachTarget,
	gMove_Idle,
	gMove_ReturnToPath,
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


private:

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

public:
	Entity*					target = nullptr;
private:
	j1Timer					cd_timer;

	bool					flip = false;

	bool					is_attacked = false;

	int						rel_angle = 0;
	float					speed = 0;

	GUARDS_STATE			state = g_s_null;
	GUARDS_MOVE_STATE		move_state = gMove_Idle;

	states					anim_state = states_null;

	int						rupee_reward = 0;

	std::vector<iPoint>		target_path;
	int						target_path_index = 0;

	iPoint					initialPos = { 3000,3000 };

	EventThrower*       event_thrower = nullptr;

};
#endif // !_GUARDS_H_

