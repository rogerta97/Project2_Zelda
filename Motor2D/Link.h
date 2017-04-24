#ifndef _LINK_
#define _LINK_

#include "Entity.h"

class GameObject;

enum link_ability3_dir
{
	a3_up,
	a3_down,
	a3_left,
	a3_right,
	a3_direction_null
};


class Link : public Entity
{
public:
	Link(iPoint pos);
	~Link();

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

	void MoveAngle(float speed, float angle);

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

	void BasicAttackUp();
	void BasicAttackDown();
	void BasicAttackLeft();
	void BasicAttackRight();

	void ShowBasicAttackUp();
	void ShowBasicAttackDown();
	void ShowBasicAttackLeft();
	void ShowBasicAttackRight();

	void Ability1Up();
	void Ability1Down();
	void Ability1Left();
	void Ability1Right();

	void ShowAbility1Up();
	void ShowAbility1Down();
	void ShowAbility1Left();
	void ShowAbility1Right();

	void Ability2Up();
	void Ability2Down();
	void Ability2Left();
	void Ability2Right();

	void ShowAbility2Up();
	void ShowAbility2Down();
	void ShowAbility2Left();
	void ShowAbility2Right();

	void Ability3Up();
	void Ability3Down();
	void Ability3Left();
	void Ability3Right();

	void ShowAbility3Up();
	void ShowAbility3Down();
	void ShowAbility3Left();
	void ShowAbility3Right();

	// On Collision
	void OnColl(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);
	void OnCollEnter(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);
	void OnCollOut(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);

	//Set Camera to this player. 1<=id<=4
	void SetCamera(int id);

	iPoint GetPos() const;

private:
	void CreateAbility3Test();
	void DeleteAbility3Test();
public:

private:
	int		      camera = 1;
	bool	      flip = false;

	// Ability 3
	float        ability3_range = 0.0f;
	iPoint       ability3_point_up = NULLPOINT;
	iPoint       ability3_point_down = NULLPOINT;
	iPoint       ability3_point_left = NULLPOINT;
	iPoint       ability3_point_right = NULLPOINT;
	iPoint       ability3_end_up = NULLPOINT;
	iPoint       ability3_end_down = NULLPOINT;
	iPoint       ability3_end_left = NULLPOINT;
	iPoint       ability3_end_right = NULLPOINT;
	bool		 get_up = true;
	bool		 get_down = true;
	bool		 get_left = true;
	bool		 get_right = true;
	link_ability3_dir ab3_dir = link_ability3_dir::a3_direction_null;
	bool		 find = false;
	b2Fixture*   link_collision = nullptr;

	int			 rupee_reward = 0;
};

#endif