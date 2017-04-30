#ifndef _CUCO_H_
#define _CUCO_H_

#include "Entity.h"
#include <list>
#include "j1Timer.h"

class GameObject;

enum CUCO_STATE
{
	Cuco_Idle,
	Cuco_Move,
};

class Cuco :public Entity
{
public:

	Cuco(iPoint pos);

	~Cuco();

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

	void SetBasePath(const std::list<iPoint>* path);

private:
	void CucoMove();

	void GetNewPath();

	void Move(int delta_x, int delta_y);

	void SetIdleAnim();

public:
	p2Point<int>        target;

private:
	bool			   flip = false;

	string             cuco_type;
	CUCO_STATE		   state = Cuco_Idle;

	std::queue<iPoint> base_path;
	int				   base_path_index = 0;

	float			   speed = 0;
	bool			   dead = false;

	states			   anim_state = states_null;

	EventThrower*      event_thrower = nullptr;
};

#endif // !CUCO_H_