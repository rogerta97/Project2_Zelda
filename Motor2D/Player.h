#ifndef _PLAYER_
#define _PLAYER_

#include "Entity.h"

class GameObject;
class Player : public Entity
{
public:
	Player();
	~Player();

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

	// On Collision
	void OnColl(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);

public:
	GameObject* player_go = nullptr;

	bool flip = false;
	bool on_ground = false;
	bool going_up = false;

	float last_height = 0.0f;
};

#endif