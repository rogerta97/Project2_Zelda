#ifndef _LINK_
#define _LINK_

#include "Entity.h"

class GameObject;
class Link : public Entity
{
public:
	Link();
	~Link();

	// Load animations
	bool LoadEntity();

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

	void RunUp(float speed);
	void RunDown(float speed);
	void RunLeft(float speed);
	void RunRight(float speed);

	void IdleUp(float speed);
	void IdleDown(float speed);
	void IdleLeft(float speed);
	void IdleRight(float speed);

	// On Collision
	void OnColl(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);

	//Set Gamepad number to this player. id>0
	void SetGamePad(int id);

	//Set Camera to this player. 1<=id<=4
	void SetCamera(int id);

public:
	GameObject* player_go = nullptr;

	bool flip = false;

private:
	uint gamepad_num = 20;
	int camera = 1;

};

#endif