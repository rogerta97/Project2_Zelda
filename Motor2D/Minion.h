#ifndef _MINION_H_
#define _MINION_H_

#include "Entity.h"
#include <list>

class GameObject;

enum MINION_STATE
{
	Minion_Idle,
	Minion_Move,
	Minion_Attack,
};

enum MINION_MOVE_STATE
{
	Move_FollowBasePath,
	Move_AproachTarget,
	Move_ReturnToPath,
	Move_Idle,
};

class Minion :public Entity 
{
public:

	Minion(iPoint pos);

	~Minion();

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

	// On Collision
	void OnColl(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);

	iPoint GetPos() const;

	void SetBasePath(std::list<iPoint>& path);

private:

	void MinionIdle();
	void MinionMove();
	void MinionAttack();

	void CheckState();

	void SetTargetPath(const std::list<iPoint>* path);

	void PathToTarget();
	void PathToBasePath();

	bool LookForTarget();

	void Move(int delta_x, int delta_y);

public:

private:

	bool				flip = false;

	uint				vision_range = 150;
	uint				attack_range = 60;

	MINION_STATE		state = Minion_Idle;
	MINION_MOVE_STATE	move_state = Move_FollowBasePath;

	Entity*				target = nullptr;

	std::vector<iPoint>	base_path;
	std::vector<iPoint>	target_path;
	int					base_path_index = 0;
	int					target_path_index = 0;

	float				speed = 0;

	states				anim_state = states_null;

};

#endif // !MINION_H_

