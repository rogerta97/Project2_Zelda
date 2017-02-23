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

	bool MinionMove();
	bool MinionAttack();

	void CheckState();

	void SetTargetPath(const std::list<iPoint>* path);

	void PathToTarget();
	bool LookForTarget();

public:

private:
	GameObject*			minion_go = nullptr;

	bool				flip = false;

	uint				vision_range = 0;
	uint				attack_range = 0;

	MINION_STATE		state = Minion_Idle;
	MINION_MOVE_STATE	move_state = Move_FollowBasePath;

	Entity*				target = nullptr;

	std::list<iPoint>	base_path;
	std::list<iPoint>	target_path;
	uint				base_path_index = 0;
	uint				target_path_index = 0;

};

#endif // !MINION_H_

