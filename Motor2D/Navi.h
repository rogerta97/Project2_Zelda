#ifndef _NAVI_
#define _NAVI_

#include "Entity.h"

class GameObject;
class Animator;

enum navi_ability2_dir
{
	a2_up,
	a2_down,
	a2_left,
	a2_right,
	a2_direction_null
};

struct heal_element
{
	void CleanUp()
	{
		App->DeleteGameplayTimer(timer);
	}

	Entity* entity = nullptr;
	j1Timer* timer = nullptr;
};

class Navi : public Entity
{
public:
	Navi(iPoint pos);
	~Navi();

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
	void ListenEv(int type, EventThrower* origin, int id);

	//Set Camera to this player. 1<=id<=4
	void SetCamera(int id);

	iPoint GetPos() const;

	void Die(Entity* killed_by);
public:

private:
	int	      camera = 1;
	bool 	  flip = false;

	int		  rupee_reward = 0;

	// Ability 1
	j1Timer*  ability1_timer = nullptr;
	bool	  ability1 = false;
	vector<heal_element> to_heal;
	bool	  look_for_entities = true;
	float	  heal = 0.0f;
	float	  ability1_duration = 0.0f;
	float	  ability1_ticks_time = 0.0f;

	// Ability 2
	bool	  ability2 = false;
	navi_ability2_dir ability2_dir = navi_ability2_dir::a2_direction_null;
	iPoint    ability2_point = NULLPOINT;
	bool	  find = false;
	bool      point_found = false;

	// Ability 3
	j1Timer* ability3_timer = nullptr;
	bool	 ability3 = false;

	// 
};

#endif