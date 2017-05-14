#ifndef _GONON_H_
#define _GONON_H_

#include "Entity.h"

class GameObject;

struct ball
{
	GameObject* game_object = nullptr;
	float		angle = 0;
	bool		to_delete = false;
};

class Ganon : public Entity
{
public:
	Ganon(iPoint pos);
	~Ganon();

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
	//void OnColl(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);
	void OnCollEnter(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);
	//void OnCollOut(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);

	//Set Camera to this player. 1<=id<=4
	void SetCamera(int id);

	iPoint GetPos() const;

	void Die(Entity* killed_by);

private:
	void CreateAbility2Balls();
	void DeleteAbility2BallByPbody(PhysBody* body);
	void ClearAbility2Balls();
	void MoveCamera();
	iPoint DrawTarget();
	void DestroyBalls();
public:

private:
	int		      camera = 1;
	bool	      flip = false;

	int			  rupee_reward = 0;

	// Ability2
	int			  shield = 0;
	vector<ball>  balls;
	j1Timer*	  ability2_timer = nullptr;
	bool		  ability2 = false;
	
	// Ability3
	bool		  ability3 = false;
	bool		  look_for_target = false;
	j1Timer*	  look_for_target_timer = nullptr;
	iPoint		  target = NULLPOINT;
	bool		  target_found = false;
	float		  stun_time = 0.0f;
	int			  angle = 0;
};

#endif
